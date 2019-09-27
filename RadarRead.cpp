#include "stdafx.h"
#include "RadarRead.h"


RadarRead::RadarRead()
{
}


RadarRead::~RadarRead()
{
	delete m_Databuff;
}

bool IsRadar(const char *InPath, int& Width, int &Height, int &type)
{
	return false;

}


//矩阵行列转换
void ColumnTransposition(unsigned char * pData, int w, int h)
{
	int rowcount = w;
	int ttt = sizeof(unsigned char)*w;
	unsigned char * pdbltmp = (unsigned char*)malloc(sizeof(unsigned char)*w);
	
	for (int i = 0; i < h/2; i++)
	{
		memcpy(pdbltmp, (void*)(pData+(i*w)), w);
		memcpy((void*)(pData+(i*w)), (void*)(pData+(w*h-(i+1)*w)), w);
		memcpy((void*)(pData + (w*h - (i + 1)*w)), pdbltmp, w);
	}
	free(pdbltmp);
}

bool RadarRead::Read(const char * strfile)
{
	FILE *fin_bin = fopen(strfile, "rb");
	int rc;

	int size = 0;

	fseek(fin_bin, 50, 0);

	
	//读数据
	if ((rc = fread(Flag, sizeof(unsigned char), 8, fin_bin)) != 0)
	{
		std::string tmp(Flag);
		if (tmp == "SWAN")
			isRadarData = true;
	}
	if (!isRadarData)
		return false;
	fseek(fin_bin, 0, 0);
	rc = fread(ZonName, sizeof(unsigned char), 12, fin_bin); 
	rc = fread(DataName, sizeof(unsigned char), 38, fin_bin);
	rc = fread(Flag, sizeof(unsigned char), 8, fin_bin);
	rc = fread(Version, sizeof(unsigned char), 8, fin_bin);
	rc = fread(&year, sizeof(unsigned char), 2, fin_bin);
	rc = fread(&month, sizeof(unsigned char), 2, fin_bin);
	rc = fread(&day, sizeof(unsigned char), 2, fin_bin);
	rc = fread(&hour, sizeof(unsigned char), 2, fin_bin);
	rc = fread(&minute, sizeof(unsigned char), 2, fin_bin);
	rc = fread(&interval, sizeof(unsigned char), 2, fin_bin);
	rc = fread(&XNumGrids, sizeof(unsigned char), 2, fin_bin);
	rc = fread(&YNumGrids, sizeof(unsigned char), 2, fin_bin);
	rc = fread(&ZNumGrids, sizeof(unsigned char), 2, fin_bin);
	rc = fread(&RadarCount, sizeof(unsigned char), 4, fin_bin);
	rc = fread(&StartLon, sizeof(unsigned char), 4, fin_bin);
	rc = fread(&StartLat, sizeof(unsigned char), 4, fin_bin);
	rc = fread(&CenterLon, sizeof(unsigned char), 4, fin_bin);
	rc = fread(&CenterLat, sizeof(unsigned char), 4, fin_bin);
	rc = fread(&XReso, sizeof(unsigned char), 4, fin_bin);
	rc = fread(&YReso, sizeof(unsigned char), 4, fin_bin);
	rc = fread(ZhighGrids, sizeof(unsigned char), 160, fin_bin);
	rc = fread(RadarStationName, sizeof(unsigned char), 20 * 16, fin_bin);
	rc = fread(RadarLongitude, sizeof(unsigned char), 4 * 20, fin_bin);
	rc = fread(RadarLatitude, sizeof(unsigned char), 4 * 20, fin_bin);
	rc = fread(RadarAltitude, sizeof(unsigned char), 4 * 20, fin_bin);
	rc = fread(MosaicFlag, sizeof(unsigned char),  20, fin_bin);
	rc = fread(&m_iDataType, sizeof(unsigned char), 2, fin_bin);
	rc = fread(&m_iLevelDimension, sizeof(unsigned char), 2, fin_bin);
	rc = fread(Reserved, sizeof(unsigned char), 168, fin_bin);


	int datatypesize = 1;
	if (m_iDataType >= 3 && m_iDataType <= 1)			datatypesize = 1;
	else if (m_iDataType > 1 && m_iDataType <= 3)		datatypesize = 2;
	else if (m_iDataType > 3 && m_iDataType <= 5)		datatypesize = 4;
	else												datatypesize = 1;
	m_Databuff = new unsigned char[XNumGrids*YNumGrids*ZNumGrids*datatypesize];
	fread(m_Databuff, sizeof(unsigned char), XNumGrids*YNumGrids*ZNumGrids*datatypesize, fin_bin);
	fclose(fin_bin);
	//列变换
	int w = XNumGrids;
	int h = YNumGrids;
	ColumnTransposition(m_Databuff, w, h);

	m_buffSize = XNumGrids * YNumGrids*ZNumGrids*datatypesize;
	return true;
}
