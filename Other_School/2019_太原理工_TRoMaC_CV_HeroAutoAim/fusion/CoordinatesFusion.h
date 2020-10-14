/**********************************************************
* Copyright (C) 2019 TYUT_TRoMaC 690208412@qq.com	      *
*                                                         *
*  	Write functions for each thread						  *
*                                                         *
*  @file     CoordiantesFusion.h                          *
*  @brief                                                 *
*  Details.                                               *
*                                                         *
*  @author   Shuyuan.Yu                                   *
*  @email    690208412@qq.com                             *
*  @version  1.0                                          *
*  @date     2019/3/19                                    *
*  @license  GNU General Public License (GPL)             *
*---------------------------------------------------------*
*  Remark:                                                *
*---------------------------------------------------------*
*  Change History:                                        *
*  <Date>    | <Version> |  <Author>  | <Description>     *
*  2019/3/19|    1.0.   | Shuyuan.Yu | Create file        *
***********************************************************/
#pragma once
#define PostProcesser_H
#ifdef PostProcesser_H
#define Predict
//#define None_Pose

#include "iostream"
#include "thread"
#include "mutex"
#include "atomic"
#include "opencv2/opencv.hpp"
#include "../detector/ArmorDetector.h"

class GimblaPose {
public:
	float yaw;
	short pitch;
	float timestamp;
};

class CoordinatesFusion {
public:
	CoordinatesFusion();
	/**
	* @����: CoordinatesFusion
	* @����: ��ʼ��
	* @����: short PMIN ����ֵpitch������
			 short PC   ����ֵpitch������
			 short PMAX ����ֵpitch������
	 */
	CoordinatesFusion(short PMIN , short PC,short PMAX);
	
	~CoordinatesFusion();
	
	/**
	* @����: pushGimblaPose
	* @����: ����������ֵ̨
	* @����: float yaw	   ���ط���yaw����ֵ
			 short pitch   ���ط���pitch����ֵ
			 float timestamp ���ط��ص�ϵͳʱ��
	 */
	void pushGimblaPose(float yaw, short pitch, float timestamp);
	
	/**
	* @����: caculateTargetPose
	* @����: �õ�Ŀ�����������
	* @����: Point2f angle			�з�������ͷ����ƽ���д��ߵĽǶȣ�x��y��
			 cv::Mat tvecDeviation  ǹ�ܵ�����ͷ��ƽ�ƾ���
			 float distance			�з�����
			 double timestamp		��ʱ��ʱ��
	* @�������:cv::Point2f &targetPose �з���������
	* @����: bool �ɹ�ת��
	*/
	bool caculateTargetPose(cv::Point2f angle, cv::Mat tvecDeviation,  float distance, double timestamp, cv::Point2f &targetPose);
	
	/**
	* @����: getLatestData
	* @����: �õ����һ����̨����ֵ
	* @����: pair<float,short> �˿̵�yaw��pitch
	*/
	pair<float,short> getLatestData();
	
	/**
	* @����: getLatestData
	* @����: �����ʷ����
	*/
	void clear();

private:
	/**
	* @����: interpolateRobotPose
	* @����: ����ʱ�����ֵ̨���в�ֵ
	* @����: double timestamp �˿�ʱ��
			 GimblaPose &pose ��̨״̬
	*/
	bool interpolateRobotPose(double timestamp, GimblaPose &pose);

private:
	double PoseDataSaveTime;//��̨����ֵ�ı���ʱ��
	std::vector<GimblaPose> historyGimblaPose; //��ʷ��̨����
	std::atomic<double> LatestGimblaPoseTime; //���һ�η�����̨��ʱ��

	short pitchMin; //pitch��Сֵ
	short pitchCenter; //pitch����ֵ
	short pitchMax; //pitch���ֵ
};
#endif
