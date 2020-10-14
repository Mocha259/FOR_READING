#pragma once

#include <stdio.h>
#include <string.h>

#include <fcntl.h> 
#include <unistd.h>

#include <termios.h>

#include <sys/ioctl.h>
#include <errno.h>

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

class Uart{
	public:
		Uart();
		~Uart();

		/**
		* @����: Open
		* @����: �򿪴���
		 */
		bool Open(const char * device, int _speed, int _parity, bool _should_block);

		/**
		* @����: restart
		* @����: ��������
		 */
		bool restart();
		
		/**
		* @����: ReadData
		* @����: ��ȡ����
		 */
		bool ReadData(unsigned char data[23], unsigned char & mode, float & yaw, short & pitch, float & timestamp, short & heat42, short & heat17, unsigned char & bulletSpeed17);
		
		/**
		* @����: SendData
		* @����: �����ֽ�����
		 */
		bool SendData(char* data,int len);

		/**
		* @����: send
		* @����: ���ͱ�������
		 */
		bool send(float yaw, short pitch, float timestamp,bool shoot42 ,bool shoot17);

		/**
		* @����: SendMiss();
		* @����: ������ʶ�����ָ��
		 */
		bool SendMiss();
		/**
		* @����: Close
		* @����: �رմ���
		 */
		void Close();
	private:
		const char *uart_path;
		int fd;
		int speed;
		int parity;
		bool should_block;		

};
