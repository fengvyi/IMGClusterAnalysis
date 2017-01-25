#include<iostream>
#include<math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include<opencv2\opencv.hpp>//opencv2下是C++接口
#include"DataPoint.h"
#define random(x) (rand()%x)
using namespace cv;
using namespace std;

class ClusterAnalysis
{

private:
	DataPoint* dataSets=new DataPoint[100000];    //数据集合
	unsigned int dimNum;          //维度
	double radius;                //半径
	unsigned int dataNum;         //数据数量
	unsigned int minPTs;          //邻域最小数据个数
	double minDTs;
public:

	ClusterAnalysis(){};

	/*
	函数：获取两数据点之间距离
	参数：
	DataPoint& dp1;        //数据点1
	DataPoint& dp2;        //数据点2
	返回值： double;    //两点之间的距离        */

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
	函数：设置数据点的领域点列表
	说明：设置数据点的领域点列表
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
	  函数：计算与密度比dp大的点的距离
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
	  函数：对数据点领域内的点执行聚类操作
	  说明：采用递归的方法，深度优先聚类数据
	  参数：
	  unsigned long dpID;            //数据点id
	  unsigned long clusterId;    //数据点所属簇id
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
	函数：聚类初始化操作
	double radius;    //半径
	int minPTs;        //领域最小数据个数
	double minDTs;     //与密度比它高的点的距离
	返回值： true;    */

	bool Init(Mat img, double radius, int minPTs,double minDTS)
	{
		this->radius = radius;
		this->minPTs = minPTs;
		this->dimNum = DIME_NUM;
		this->minDTs = minDTs;


		//读入图像
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
	函数：执行聚类操作
	说明：执行聚类操作
	参数：
	返回值： true;    */
     
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

		cout << "聚集个数:" << clusterId << endl;
		
		return true;
	}

	/*
	处理聚集，修改图像并输出
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
		//输出图像
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
