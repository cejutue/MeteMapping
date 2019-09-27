#include "stdafx.h"
#include "GridRead.h"


GridRead::GridRead()
{
}


GridRead::~GridRead()
{
	delete m_Databuff;
}

bool GridRead::Read(const char * InPath)
{
	FILE *fin_bin = fopen(InPath, "rb");
	int rc = 0;
	struct stat s;
	stat(InPath, &s);
	m_Databuff = new float[s.st_size / sizeof(float)];
	m_buffSize = s.st_size / sizeof(float);
	memset(m_Databuff, sizeof(float), m_buffSize);
	if ((rc = fread(m_Databuff, sizeof(float), m_buffSize, fin_bin)) == 0)
	{
		fclose(fin_bin);
		return false;
	}
	fclose(fin_bin);
	return true;
}
