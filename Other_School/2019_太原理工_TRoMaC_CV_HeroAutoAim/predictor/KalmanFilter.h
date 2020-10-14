#pragma once

#include "opencv2/opencv.hpp"
#include "Eigen/Dense"

namespace EigenKalman{
	class KalmanFilter
	{
	private:
		int stateSize;     //״̬����ά��
		int measSize;      //������������
		Eigen::MatrixXd A; //״̬ת�ƾ���
		Eigen::MatrixXd P; //Э�������
		Eigen::MatrixXd H; //��������
		Eigen::MatrixXd R; //�������
		Eigen::MatrixXd Q; //Ԥ�����
	public:
		KalmanFilter();
		~KalmanFilter(){}
		/**
		* @����: init
		* @����: ��ʼ��kalmanԤ�в���
		 */
		void init(int state, int means, Eigen::MatrixXd& _A, Eigen::MatrixXd& _P, Eigen::MatrixXd& _R, Eigen::MatrixXd& _Q, Eigen::MatrixXd& _H);
		/**
		* @����: predict
		* @����: kalmanԤ��
		* @�������:Eigen::VectorXd &x ״̬����
		*/
		void predict(Eigen::VectorXd &x);
		/**
		* @����: update
		* @����: kalman����
		* @����: Eigen::VectorXd z_meas ������������
		* @�������:Eigen::VectorXd &x ״̬����
		 */
		void update(Eigen::VectorXd &x, Eigen::VectorXd z_meas);
	};
}