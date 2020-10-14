/**********************************************************
* Copyright (C) 2018 Shuyuan.Yu  690208412@qq.com	      *
*                                                         *
*	detecte RM_Armor			                          *
*                                                         *
*  @file     ArmorDetector.h                              *
*  @brief                                                 *
*  Details.                                               *
*                                                         *
*  @author   Shuyuan.Yu                                   *
*  @email    690208412@qq.com                             *
*  @version  1.0                                          *
*  @date     2018/10/26                                   *
*  @license  GNU General Public License (GPL)             *
*---------------------------------------------------------*
*  Remark:                                                *
*---------------------------------------------------------*
*  Change History:                                        *
*  <Date>    | <Version> |  <Author>  | <Description>     *
*  2018/10/26|    1.0.   | Shuyuan.Yu | Create file       *
***********************************************************/
#pragma once

#include "iostream"
#include "immintrin.h"
#include "opencv2/opencv.hpp"
#include "math.h"
#include "../tools/MathTools.h"
#include "../camera/camera.h"

using namespace std;

#define debug
#define USE_ID

class Lamp {
public:
	cv::Point2f top;//�Ϸ���
	cv::Point2f down;//�·���
	cv::Point2f corner[4];//��Χlamp���ε��ĸ��ǣ�˳��:[0]���£�[1]���ϣ�[2]����, [3]����
	float angle;//��б�Ƕȣ���ƽ��ֱ������ϵ�Ƕȱ���һ��
public:
	Lamp();
	Lamp(cv::Point2f t, cv::Point2f d, float a, cv::Point2f co[4]);
};

/*
 @����:sortFittedLamp
 @����:lamp��sort����
 */
bool sortFittedLamp(pair<vector<Lamp>::iterator, float> a, pair<vector<Lamp>::iterator, float> b);

class Armor {
public:
	Lamp lampL;//�����
	Lamp lampR;//�ҵ���
	pair<Lamp, bool> lampLL = make_pair(Lamp(), false);//�����(δʹ��)
	pair<Lamp, bool> lampRR = make_pair(Lamp(), false);//�����(δʹ��)
	cv::Point2f center;//װ������
	unsigned int id;//װ������
	enum ArmorType {//����
		BigArmor,
		SmallArmor,
		Unknow
	} type;
	cv::Mat rvec;//ƫת��(PNP�ó�,δʹ��PnP)
	cv::Mat tvec;//ƽ�ƾ���(����+�Ƕȼ���ó�)
	ushort distance;//����
public:
	Armor();
	Armor(Lamp L, Lamp R);
	Armor(Lamp L, Lamp R, ArmorType T);
};

class ArmorDetector
{
public:
	ArmorDetector(int rows, int cols);
	~ArmorDetector();
public:
	enum Enemy {
		BLUE,
		RED
	};
public:
	/**
	* @����: loadClassifier
	* @����: ���ط�����
	* @����: string file_name_small Сװ�׷�����·������
			 string file_name_big ��װ�׷�����·������
	* @����:�ҵ��ĵ�������
	*/
	void loadClassifier(string file_name_small, string file_name_big);
	
	/**
	* @����: getArmor2
	* @����: ����װ�װ�
	* @����: cv::Mat &src ԭ��ɫͼCV_8UC3
			 uchar bright_thr_value ������ֵ(��Ϊ0ֻ����ɫ���)
			 uchar color_thr_value ��ɫֵ
			 Enemy enemy �з���ɫ����
			 int useblur = 0 ��ʴ�����ں˴�С
	* @�������:Armor &OutputArmor ���װ��
	* @����:bool �Ƿ��ҵ�װ��
	*/
	bool getArmor2(cv::Mat & src, Armor &OutputArmor, uchar bright_thr_value, uchar color_thr_value, Enemy enemy, int useblur);

	/**
	* @����: drawArmors
	* @����: ��װ�װ�
	*/
	void drawArmors(cv::Mat & src, vector<Armor> armors);
	void drawArmors(cv::Mat & src, Armor a);
private:
	/**
	* @����: findLamps
	* @����: ͼ�����ҵ���
	* @����: cv::Mat &src ԭ��ɫͼCV_8UC3
			 uchar bright_thr_value ������ֵ(��Ϊ0ֻ����ɫ���)
			 uchar color_thr_value ��ɫֵ
			 Enemy enemy �з���ɫ����
			 int useblur = 0 ��ʴ�����ں˴�С
    * @���������vector<Lamp> &lamps ��⵽�ĵ���
	* @����:�ҵ��ĵ�������
	*/
	int findLamps(cv::Mat &src, vector<Lamp> &lamps, uchar bright_thr_value, uchar color_thr_value, Enemy enemy, int useblur = 0);

	/**
	* @����: fitArmor
	* @����: �������װ��
	* @����: cv::Mat &src ԭ��ɫͼCV_8UC3
			 vector<Lamp> lamps �ҵ��ĵ���
	* @�������:vector<Armor> &armors �ҵ���װ��
	* @����:�ҵ��ĵ�������
	*/
	int fitArmor(Mat &src, vector<Lamp> lamps, vector<Armor> &armors);
	
	/**
	* @����: getDistance
	* @����: ���õ����߶ȼ������
	* @�������:Armor & a װ��
	*/
	void getDistance(Armor & a);

	/**
	* @����: getID
	* @����: ����SVM�������
	* @�������:cv::Mat &src ԭͼ
				Armor &a װ��
	* @����:����ID
	*/
	unsigned int getID(cv::Mat &src, Armor &a);

	/**
	* @����: selectArmor
	* @����: �Ӽ�⵽�Ķ��ѡ��������װ��
	* @����: vector<Armor> armors ������װ��
	  @�������:  Armor &a ���װ��
	* @����:����ID
	*/
	void selectArmor(vector<Armor> armors, Armor &armor);
	
private:
	vector<cv::Point3f> BigArmorSize;//װ��ʵ�ʳߴ磬����PNP������
	vector<cv::Point3f> SmallArmorSize;//װ��ʵ�ʳߴ磬����PNP������
	vector<cv::Point2f> PointInBigArmorPic;//͸�ӱ任��װ��ͼƬ�ߴ�
	vector<cv::Point2f> PointInSmallArmorPic;//͸�ӱ任Сװ��ͼƬ�ߴ�

	cv::HOGDescriptor *hog_small,*hog_big;//��Сװ��HOG����
	cv::Ptr<cv::ml::SVM> IDclassifier_small, IDclassifier_big;//��Сװ��SVM������
	bool EnableClassifier;//����ʹ�÷������ı�־


	cv::Mat gray, color_thr ,bright_thr;//����Ԥ����ͼ��(�Ҷȡ���ɫ������)
};