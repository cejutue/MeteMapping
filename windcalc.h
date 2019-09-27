#pragma once
class Windcalc
{
public:
	float * m_Databuff;
	float * m_AngleDatabuff;
	long long m_buffSize = 0;
	int w = 1426;
	int h = 2231;
	std::string m_strgfs;

	int m_stepw = 50;
	//�����ֱ���,��
	double m_griddel = 0.01;
	//���ű���, ��Ϊ����ʵ�ʵ���Χ�ȷ��ٴ�Ķ�, ��ͼЧ���ϲ�
	double m_scale = 500;
	//����뾶
	double m_dblEarthRadius = 6378137.0;
	//PI
	double m_dblPI = 3.14159265358979323846;

	//����糡���˶�ʱ��
	int m_time= 15;

	double D2M(double d)
	{
		return  m_dblEarthRadius * d * m_dblPI / 180.0;
	}
public:
	Windcalc();
	~Windcalc();
	void OutputJson(const char* OutJson);
	bool Calc(const char* UFile);
	bool Calc(const char* UFile, const char* VFile);
	bool Convert2GFS(float* UBuf, float* Vbuf);
	//���
	void RandomFieldFluid(byte * pColor);
};

