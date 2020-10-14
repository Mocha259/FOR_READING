/**********************************************************
* Copyright (C) 2018 TYUT_TRoMaC 690208412@qq.com	      *
*                                                         *
*  	preidict next coordinates   						  *
*                                                         *
*  @file     Predictor.h                                  *
*  @brief                                                 *
*  Details.                                               *
*                                                         *
*  @author   Shuyuan.Yu                                   *
*  @email    690208412@qq.com                             *
*  @date     2019/3/19                                    *
*  @license  GNU General Public License (GPL)             *
*---------------------------------------------------------*
*  Remark:                                                *
*---------------------------------------------------------*
*  Change History:                                        *
*  <Date>    | <Version> |  <Author>  | <Description>     *
*  2018/3/19|    1.0.   | Shuyuan.Yu | Create file        *
***********************************************************/

#pragma once
#include "iostream"
#include "math.h"
#include "opencv2/opencv.hpp"
#include "KalmanFilter.h"
#include "thread"
#include "atomic"

using namespace std;
using namespace cv;


class Predictor{
public:
	/**
	* @����: Predictor
	* @����: ��ʼ��Ԥ�к���
	* @����: float pC ��ʾpitchˮƽ����ֵ
	 */
	Predictor(float pC);
	~Predictor();
public:
	/**
	* @����: predict
    * @����: �Ծ����������Ԥ��
	* @����: float yaw װ�װ����ڵ�yaw�Ƕ�ֵ
			 float pitch װ�װ����ڵ�pitch����ֵ
			 float yaw_now �˿���̨��yaw
			 float pitch_now �˿���̨��pitch
			 float distance , int bulletSpeed
	* @���أ�pair<cv::Point2f,bool> Point2fԤ�����̨��(yaw��pitch) bool�Ƿ��ܹ����
	 */
	pair<cv::Point2f,bool> predict(float yaw, float pitch, float yaw_now, float pitch_now , float distance , int bulletSpeed);

private:
	/**
	* @����: kalmanPredict
	* @����: �Ծ����������Ԥ��
	* @����: float yaw װ�װ����ڵ�yaw�Ƕ�ֵ
			 float pitch װ�װ����ڵ�pitch����ֵ
			 float time Ԥ��ʱ��
	* @���أ�Point2f Ԥ�����̨��yaw��pitch
	*/
	cv::Point2f kalmanPredict(float yaw, float pitch, float time);
	/**
	* @����: compensaBallisticDrop
	* @����: �Ե�����׹�Ĳ�����δ���ǿ���������
	* @����: float input �����pitch����ֵ
			 float distance װ�װ����
			 float bulletSpeed �ӵ��ٶ�
	* @���أ�float Ԥ�����̨��pitch
	*/
	float compensaBallisticDrop(float input, float distance, int bulletSpeed);
private:
	EigenKalman::KalmanFilter KF;
	Eigen::VectorXd x;
	float pitchCenter;//��ʼ��pitchˮƽ�ı���ֵ
public:
	atomic<int> shootSpeed;//�����ٶ� ��λ;��/��
};