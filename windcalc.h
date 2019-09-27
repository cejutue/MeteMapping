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
	//格网分辨率,度
	double m_griddel = 0.01;
	//缩放比例, 因为格网实际地理范围比风速大的多, 成图效果较差
	double m_scale = 500;
	//地球半径
	double m_dblEarthRadius = 6378137.0;
	//PI
	double m_dblPI = 3.14159265358979323846;

	//随机风场的运动时间
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
	//随机
	void RandomFieldFluid(byte * pColor);
};

