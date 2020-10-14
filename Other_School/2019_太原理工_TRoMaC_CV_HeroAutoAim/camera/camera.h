#pragma once
#include "iostream"
#include "opencv2/opencv.hpp"

using namespace std;

class MyCamera {
public:
	MyCamera();
	~MyCamera();
	/**
	* @����: open
	* @����: ������ͷ
	*/
	bool open();

	/**
	* @����: getFrame
	* @����: ȡһ��ͼƬ
	* @�������: cv::Mat & img ��ɫͼƬ
	*/
	bool getFrame(cv::Mat & img);

	void restart();

public:
	/**
	* @����: loadMatrix
	* @����: ��ȡ����ͷ����
	* @����: string filename �ļ�·������
	* @���أ��ɹ���ȡ��
	*/
	bool loadMatrix(string filename);

	/**
	* @����: loadMatrix
	* @����: ��ȡ����ͷ����
	* @���أ�ͼ���������
	*/
	cv::Point2f picCenter();

	/**
	* @����: pix2angle
	* @����: ������ת��Ϊ������ͷ�������Ĵ��ߵĽǶ�
	* @���룺cv::Point2f point����ֵ
			 bool shoot42 ǹ�ܽǶȽ�����42mm����17mm
	* @���: cv::Point2f &angle�Ƕ�
	*/
	void pix2angle(cv::Point2f point, cv::Point2f &angle, bool shoot42);
	cv::Point2f pix2angle(cv::Point2f point, bool shoot42);

	/**
	* @����: undistortPoints
	* @����: �Ե����ȥ����
	* @����: cv::Point2f inputPoint �����
	* @���������cv::Point2f & outputPoint ����ȥ��������ص�λ��
	*/
	void undistortPoints(cv::Point2f inputPoint, cv::Point2f & outputPoint);
	cv::Point2f undistortPoints(cv::Point2f point);

private:
	cv::VideoCapture cam;
private:
	bool loadedMatrix = false;//������������ı�־
	cv::Mat cameraMatrix;//����ڲ�
	cv::Mat distCoeffs;//����������
};