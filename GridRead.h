#pragma once
class GridRead
{
public:
	float * m_Databuff;
	long long m_buffSize = 0;
	int w = 1426;
	int h = 2231;
public:
	GridRead();
	~GridRead();
	bool Read(const char* file);
};

