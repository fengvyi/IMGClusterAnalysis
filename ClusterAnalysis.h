#include<iostream>
#include<math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include<opencv2\opencv.hpp>//opencv2 api for C++
#include"DataPoint.h"
#define random(x) (rand()%x)
#define INPUT_SIZE 100000
using namespace cv;
using namespace std;

class ClusterAnalysis
{

private:
	DataPoint* dataSets=new DataPoint[INPUT_SIZE];    //Data Sets
	unsigned int dimNum;          //Dimension
	double radius;                //Radius
	unsigned int dataNum;         //Data number
	unsigned int minPTs;          //Minimum number of neighborhood points
	double minDTs;                //Minimun distance to be recognized as a cluster center
public:

	ClusterAnalysis(){};

	/*
	Function: Get the distance between two data points
	Parameter：
	DataPoint& dp1;        //Data point 1
	DataPoint& dp2;        //Data point 2
	Return value： double; //Distance between two data points        
	*/

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
	Function: Sets the list of domain points for the data points
        Description: Sets the list of domain points for the data points
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
	  Function: Calculates the distance from the point where the density is greater than dp
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
	  Function: Performs clustering operations on points in the data point area
          Description: Recursive method, depth-first clustering data
	  Parameter：
	  unsigned long dpID;            //Data point id
	  unsigned long clusterId;       //Cluter id it belongs to
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
	Function: Clustering initialization operation
	double radius;     //Radius
	int minPTs;        //Minimum number of neighborhood points
	double minDTs;     //Minimun distance to be recognized as a cluster center
	Return value： true;    
	*/

	bool Init(Mat img, double radius, int minPTs,double minDTS)
	{
		this->radius = radius;
		this->minPTs = minPTs;
		this->dimNum = DIME_NUM;
		this->minDTs = minDTs;


		//Read image
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
	Function: Performs a clustering operation
        Description: Performs a clustering operation
        Parameters:
	Return value： true;    
	*/
     
	bool DoFSDP()
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

		cout << "Number of cluster recognized:" << clusterId << endl;
		
		return true;
	}

	/*
	Processing the clustering, re-coloring the image and outputting it
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
		//Output image
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
