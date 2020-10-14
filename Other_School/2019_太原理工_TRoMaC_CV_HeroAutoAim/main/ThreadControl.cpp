#include "ThreadControl.h"

string cameraMatrixFileName("/home/tromac/program/hero1907/camera/intrinsics.yml");
string smallArmorModelFileName("/home/tromac/program/hero1907/detector/classifier.xml");
string bigArmorModelFileName("/home/tromac/program/hero1907/detector/big_armor.xml");

#define picWidth 640
#define picHeight 640

atomic<bool> Exit(false);

Uart Serial;

bool InitAccomplish = false;//��ʼ���ɹ���־
unsigned int needSynchronizedTime(3);//��Ҫ����ͬ��ʱ���־

atomic<bool> EnableSynchronizedTime(true);//����ͬ��ʱ���־
atomic<bool> InitUartAccomplish(false);//����ͨ�ų�ʼ���ɹ���־

atomic<double> startTime(0);//��ʼ��ʱ��ϵͳʱ��

CoordinatesFusion fusion(6342,6894,7289);
/*��ʼ��Ԥ��*/
Predictor predictor(6894);

/*��ʼ������*/
Decision decision;

MyCamera cam;

processer::processer(){
}

processer::~processer(){
}


void processer::InfoReadThreadLoop() {
	/*��ʼ������*/
	Serial.Open("/dev/ttyUSB0",B115200,0,true);
	
	InitUartAccomplish = true;
	/*���崮�ڶ�������*/
	unsigned char data[23];//��ȡ23���ֽ�
	unsigned char mode = 0,bulletSpeed = 20;
	startTime = cv::getTickCount();
	float yaw = 0, timestamp = 0 , lastTimestamp = -1 ;
	short pitch = 0, startYaw = 0, heat42 = 0, heat17 = 0;
	/*��ȡѭ��*/
	while (true) {
		if (Serial.ReadData(data, mode, yaw, pitch, timestamp, heat42 , heat17, bulletSpeed)) {
			//cout << " mode" << (int)mode; 
			//cout << " yaw" << yaw;
			//cout << " pitch" << pitch;
			//cout << " time" << timestamp;
			//cout << " heatA" << heat42; 
			//cout << " heatB" << heat17;
			//cout << " bulletSpeed" << (int)bulletSpeed ;
			//cout << endl;

			if (abs(timestamp - (cv::getTickCount() - startTime) / cv::getTickFrequency()) > 0.010)
				needSynchronizedTime++;
			else
				needSynchronizedTime = 0;
			if (timestamp != 0){
				if (lastTimestamp < timestamp && needSynchronizedTime != 3) {
					fusion.pushGimblaPose(yaw, pitch, timestamp);//������̨����ֵ
					decision.setState((int)mode, (int)heat42 , (int)heat17, (int)bulletSpeed);
					lastTimestamp == timestamp;
				}
				/*ͬ���߼�*/
				if (needSynchronizedTime >= 3 && EnableSynchronizedTime) {
					//ͬ������ʱ���ᣬ���ض�ȡʱ�䷢�أ����������ʼ��ϵͳʱ�䣬����ͨ�ż��Ͻ���ʱ��ԼΪ1.3ms
					startTime = cv::getTickCount() - ((double)timestamp)*cv::getTickFrequency() + 0.0013;
					fusion.clear();
					needSynchronizedTime = 0;
					cout << "init TimeStamp success" << endl;
				}
			}
		}
		else {
		//	cout << "fales" << endl;
		}
		if (Exit)
			break;
	}
	
}

void processer::ArmorDetectorThreadLoop(){
	while (!InitUartAccomplish);//�ȴ����ڳ�ʼ��
	/*��ʼ��װ��ʶ��*/
	ArmorDetector armorDetector(picHeight, picWidth);
	armorDetector.loadClassifier(smallArmorModelFileName, bigArmorModelFileName);
	/*��ʼ������ͷ*/
	cam.open();
	cam.loadMatrix(cameraMatrixFileName);

	double lastFindTimestamp = 0;

	cv::Mat frame;//�̶�ȡͼ�ĵ�ַ
	double timestamp = 0;

	pair<Point2f,bool> predictValue;
	Point2f targetValue;

	while (true) {
		if (cam.getFrame(frame)) {
			timestamp = (cv::getTickCount() - startTime) / cv::getTickFrequency() - 0.004;//��ȡͼƬ��ʱ���
			Armor armor;
			if (armorDetector.getArmor2(frame, armor, 200, 100, ArmorDetector::RED, 0)) {
			//if (armorDetector.getArmor2(frame, armor, 200, 100, ArmorDetector::BLUE, 0)) {
				tuple <cv::Mat, int, bool> modeState = decision.make();

				if (fusion.caculateTargetPose(cam.pix2angle(armor.center, get<2>(modeState)), get<0>(modeState), (float)armor.distance, timestamp, targetValue)) {//��һ�����ڵõ�Ŀ��ı�������

					pair<float, short> gimblaState = fusion.getLatestData();
					predictValue = predictor.predict(targetValue.x, targetValue.y, gimblaState.first, gimblaState.second, (float)armor.distance, get<1>(modeState));//��һ������Ԥ�к���׹����

					if (get<2>(modeState)){
						if(Serial.send(predictValue.first.x, (short)predictValue.first.y, (float)timestamp, predictValue.second, false))
							Serial.restart();
					}
					else{
						if(Serial.send(predictValue.first.x, (short)predictValue.first.y, (float)timestamp, false, predictValue.second))
							Serial.restart();
					}
				}
				lastFindTimestamp = timestamp;
				armorDetector.drawArmors(frame, armor);
			}
			else if (timestamp - lastFindTimestamp < 0.3 && timestamp - lastFindTimestamp > 0) {
				Serial.Send(targetValue.x, (short)targetValue.y, (float)timestamp, false, false);
			}
			else {
				if(!Serial.SendMiss()){
					Serial.restart();
				}
			}
			circle(frame, cam.picCenter(), 10, Scalar(0, 255, 0));
			cv::imshow("frame", frame);
			char key = waitKey(1);
			if (key == 27) {
				Exit = true;
				break;
			}
		}
	}
}