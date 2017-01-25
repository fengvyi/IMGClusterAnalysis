#include<iostream>
#include"ClusterAnalysis.h"
using namespace std;
using namespace cv;
//cv��Opencv�����ֿռ�


int main(){
	srand((int)time(0));
	ClusterAnalysis myClusterAnalysis;
	
    Mat img = imread("test3.jpg");
	
	if (img.empty())
	{
		cout << "��ͼ��ʧ�ܣ�" << endl;
		return -1;
	}
	myClusterAnalysis.Init(img, 10, 10,10);
	myClusterAnalysis.DoDBSCAN();
	myClusterAnalysis.showImage(img);
	namedWindow("image", CV_WINDOW_AUTOSIZE);

	imshow("image", img);

	

	waitKey();
	return 0;
}