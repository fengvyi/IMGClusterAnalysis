#include<iostream>
#include"ClusterAnalysis.h"
using namespace std;
using namespace cv;
//cv is the namespace of Opencv


int main(){
	srand((int)time(0));
	ClusterAnalysis myClusterAnalysis;
	
        Mat img = imread("test.jpg");
	
	if (img.empty())
	{
		cout << "Load image failed!" << endl;
		return -1;
	}
	myClusterAnalysis.Init(img, 10, 10,10);
	myClusterAnalysis.DoFSDP();
	myClusterAnalysis.showImage(img);
	namedWindow("image", CV_WINDOW_AUTOSIZE);

	imshow("image", img);

	

	waitKey();
	return 0;
}
