/**********************************************************
* Copyright (C) 2019 TYUT_TRoMaC 690208412@qq.com	      *
*                                                         *
*  decide how to shoot 				                      *
*                                                         *
*  @file    Decision.h                                    *
*  @brief                                                 *
*  Details.                                               *
*                                                         *
*  @author   Shuyuan.Yu                                   *
*  @email    690208412@qq.com                             *
*  @date     2019/6/22                                    *
*  @license  GNU General Public License (GPL)             *
*---------------------------------------------------------*
*  Remark:                                                *
*---------------------------------------------------------*
*  Change History:                                        *
*  <Date>    | <Version> |  <Author>  | <Description>     *
*  2019/6/22|    1.0.   | Shuyuan.Yu | Create file        *
***********************************************************/
#pragma once
#include "iostream"
#include "opencv2/opencv.hpp"
#include "thread"
#include "atomic"


class Decision
{
public:
	Decision();
	~Decision();

public:
	/**
	* @����: setState
	* @����: ��������״̬
	* @����: int m ����ģʽ
			 int h42 42mmʣ������
			 int h17 17mmʣ������
			 int Speed 17mm����
	* @����:�ҵ��ĵ�������
	*/
	void setState(int m, int h42, int h17, int Speed);

	/**
	* @����: make
	* @����: �����������ӵ�
	* @����:tuple<cv::Mat, int, bool> Mat����ͷ����ǹ�ܵ�ƽ�ƾ���int���٣�bool���Сǹ��
	*/
	std::tuple<cv::Mat, int, bool> make();

private:
	std::atomic<int> mode;//mode 1:�Զ��л� 17��ֻ��С�ӵ� 42��ֻ����ӵ�
	std::atomic<int> heat42;//42mmʣ������
	std::atomic<int> heat17;//17mmʣ������
	std::atomic<int> speed17;//17mm����

	cv::Mat tvec42;//����ͷƫ�ƴ�ǹ�ܵľ���
	cv::Mat tvec17;//����ͷ����Сǿ�ܵľ���
};

