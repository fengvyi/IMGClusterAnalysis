#include<iostream>
#include<math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include<opencv2\opencv.hpp>//opencv2����C++�ӿ�
#include"DataPoint.h"
#define random(x) (rand()%x)
using namespace cv;
using namespace std;

class ClusterAnalysis
{

private:
	DataPoint* dataSets=new DataPoint[100000];    //���ݼ���
	unsigned int dimNum;          //ά��
	double radius;                //�뾶
	unsigned int dataNum;         //��������
	unsigned int minPTs;          //������С���ݸ���
	double minDTs;
public:

	ClusterAnalysis(){};

	/*
	��������ȡ�����ݵ�֮�����
	������
	DataPoint& dp1;        //���ݵ�1
	DataPoint& dp2;        //���ݵ�2
	����ֵ�� double;    //����֮��ľ���        */

	double getDistance(DataPoint& dp1, DataPoint& dp2)
	{

		double distance = 0;
		for (int i = 0; i < 5; i++)
		{
			distance += pow(dp1.pixel[i] - dp2.pixel[i], 2);
			
		}
		return sqrt(distance);
	}

	/*
	�������������ݵ��������б�
	˵�����������ݵ��������б�
	  */
      void setArrivalPoints(DataPoint& dp)
	{
		for (unsigned long i = 0; i < dataNum; i++)
		{
			double distance = getDistance(dataSets[i], dp);
			
			if (distance <= radius && i != dp.getDpID())
			{
				
				dp.arrivalPoints[dp.count] = i;
				dp.count++;
			}
		}
	
	}


	  /*
	  �������������ܶȱ�dp��ĵ�ľ���
	  */

	  void  setKey(DataPoint& dp)
	  {
		  double distance=1000;
		  double result = 0;
		  for (unsigned long i = 0; i < dataNum; i++)
		  {
			  
			  if (dataSets[i].count - dp.count>0)
			    result = getDistance(dataSets[i], dp);
			   if (result < distance && result>0)
				  distance = result;

		  }

		  if (dp.count >= minPTs && distance>minDTs)
		  {
			  dp.setKey(true);

			  return;
		  }
		  dp.setKey(false);
		  
		  
	  }



	  /*
	  �����������ݵ������ڵĵ�ִ�о������
	  ˵�������õݹ�ķ�����������Ⱦ�������
	  ������
	  unsigned long dpID;            //���ݵ�id
	  unsigned long clusterId;    //���ݵ�������id
	    */


	void KeyPointCluster(unsigned long dpID, unsigned long clusterId)
	{
		DataPoint& srcDp = dataSets[dpID];
		if (!srcDp.isKey)
			return;
		
		
		
		for (unsigned long i = 0; i < srcDp.count; i++)
		{
			DataPoint& desDp = dataSets[srcDp.arrivalPoints[i]];
			if (!desDp.isVisited())
			{
				desDp.setClusterID(clusterId);
				desDp.setVisited(true);

				if (desDp.isKey)
				{
					KeyPointCluster(desDp.getDpID(), clusterId);
				}
			}
		}
	}

	/*
	�����������ʼ������
	double radius;    //�뾶
	int minPTs;        //������С���ݸ���
	double minDTs;     //���ܶȱ����ߵĵ�ľ���
	����ֵ�� true;    */

	bool Init(Mat img, double radius, int minPTs,double minDTS)
	{
		this->radius = radius;
		this->minPTs = minPTs;
		this->dimNum = DIME_NUM;
		this->minDTs = minDTs;


		//����ͼ��
		unsigned long iCount = 0;
		for (int i = 0; i < img.rows; i++)
		{
			uchar* data = img.ptr<uchar>(i);
			for (int j = 0; j < img.cols; j++)
			{
				DataPoint tempDP;
				tempDP.setPixel(*data, *(data+1), *(data+2), i, j);
				tempDP.setDpID(iCount);
				tempDP.setVisited(false);
				tempDP.setClusterID(-1);
				dataSets[iCount]=tempDP;
				iCount++;
				
			}

		}
		dataNum = iCount;
		for (unsigned long i = 0; i < dataNum; i++)
		{
			setArrivalPoints(dataSets[i]);
			setKey(dataSets[i]);
		}

		return true;
	}

	/*
	������ִ�о������
	˵����ִ�о������
	������
	����ֵ�� true;    */
     
	bool DoDBSCAN()
	{
		unsigned long clusterId = 0;
		for (unsigned long i = 0; i < dataNum; i++)
		{
			DataPoint& dp = dataSets[i];

			if (!dp.isVisited() && dp.isKey)
			{
				dp.setClusterID(clusterId);
				dp.setVisited(true);
				KeyPointCluster(i, clusterId);
				clusterId++;
			}
		}

		cout << "�ۼ�����:" << clusterId << endl;
		
		return true;
	}

	/*
	����ۼ����޸�ͼ�����
	*/

	bool showImage(Mat& img)
	{
		DataPoint* it = dataSets;
		

		for (unsigned long i = 0; i < dataNum; i++)
		{
			if (dataSets[i].isKey)
			{
				int x = random(256);
				int y = random(256);
				int z = random(256);
				for (unsigned long j = 0; j < dataNum; j++)
					if (dataSets[j].getClusterID() == dataSets[i].getClusterID())
						dataSets[j].setPixel(x, y, z);
			}
		}
		//���ͼ��
		for (int i = 0; i < img.rows; i++)
		{
			
			uchar* data = img.ptr<uchar>(i);
			for (int j = 0; j < img.cols; j++)
			{
				
				*data= (*it).pixel[0];
				*(data+1) = (*it).pixel[1];
				*(data+2) = (*it).pixel[2];
				data += 3;
				it++;
			}

		}
	

		return true;

	}
	





};
