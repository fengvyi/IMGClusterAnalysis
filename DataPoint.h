#include<vector>
using namespace std;

const int DIME_NUM = 2;

class DataPoint
{
private:
	unsigned long dpID;                         //���ݵ�ID
	double dimension[DIME_NUM];                 //ά������
    long clusterID;                             //��������ID
	bool visited;                               //�Ƿ��ѷ���
	
	
public:
	int* arrivalPoints=new int[1000];         //�������ݵ�id�б�
	bool isKey;                                 //�Ƿ����
	int pixel[5];
	unsigned int count = 0;
	DataPoint(){}

		DataPoint(unsigned long dpID, double *dimension, bool isKey)
	{
		this->dpID = dpID;
		for (int i = 0; i<DIME_NUM; i++)
		{
			this->dimension[i] = dimension[i];
		}
		this->isKey = isKey;
	}

	unsigned long getDpID()
	{
		return dpID;
	}

	void setDpID(unsigned long dpID)
	{
		this->dpID = dpID;
	}

	void setDimension(double* dimension)
	{
		for (int i = 0; i<DIME_NUM; i++)
		{
			this->dimension[i] = dimension[i];
		}
	}

	double * getDimension()
	{
		return this->dimension;
	}

	
	void setKey(bool isKey)
	{
		this->isKey = isKey;
	}

	bool isVisited()
	{
		return this->visited;
	}

	void setVisited(bool visited)
	{
		this->visited = visited;
	}

	long getClusterID()
	{
		return this->clusterID;
	}

	void setClusterID(long classID)
	{
		clusterID = classID;
	}

	int* getArrivalPoints()
	{
		return arrivalPoints;
	}

	void setPixel(int B, int G, int R)
	{
		pixel[0] = B;
		pixel[1] = G;
		pixel[2] = R;
	
	}

	void setPixel(int B, int G, int R, int x, int y)
	{
		pixel[0] = B;
		pixel[1] = G;
		pixel[2] = R;
		pixel[3] = x;
		pixel[4] = y;
	}
};


