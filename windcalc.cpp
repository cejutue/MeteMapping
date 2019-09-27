#include "stdafx.h"
#include "windcalc.h"
#include <string>
#include <fstream>
#include <ostream>
#include <sstream>
//
//private static BigDecimal[] getDirAndSpeed(BigDecimal u, BigDecimal v) {
//	double ud = u.doubleValue();
//	double vd = v.doubleValue();
//	//计算风速
//	double speedd = Math.sqrt(Math.pow(ud, 2) + Math.pow(vd, 2));
//	BigDecimal speed = new BigDecimal(speedd).setScale(5, BigDecimal.ROUND_HALF_UP);
//	//计算角度
//	double dird = Math.atan(ud / vd) * 180 / Math.PI;
//	if (ud != 0 && vd < 0) {
//		dird = dird + 180;
//	}
//	else if (ud < 0 && vd > 0) {
//		dird = dird + 360;
//	}
//	else if (ud == 0 && vd > 0) {
//		dird = 0;
//	}
//	else if (ud == 0 && vd < 0) {
//		dird = 180;
//	}
//	else if (ud > 0 && vd == 0) {
//		dird = 90;
//	}
//	else if (ud < 0 && vd == 0) {
//		dird = 270;
//	}
//	else if (ud == 0 && vd == 0) {
//		dird = -999;
//	}
//	BigDecimal dir = new BigDecimal(dird).setScale(5, BigDecimal.ROUND_HALF_UP);
//	return new BigDecimal[]{ dir, speed };
//}
//
///**
//* 计算UV向风
//*
//* @param dir   风的角度
//* @param speed 风的速度
//* @return BigDecimal[]
//*/
//private static BigDecimal[] getUV(BigDecimal dir, BigDecimal speed) {
//	double dird = dir.doubleValue();
//	//获得弧度
//	double radian = 2 * Math.PI / 360 * dird;
//	double ud = speed.doubleValue() * Math.sin(radian);
//	double vd = speed.doubleValue() * Math.cos(radian);
//
//	BigDecimal u = new BigDecimal(ud).setScale(5, BigDecimal.ROUND_HALF_UP);
//	BigDecimal v = new BigDecimal(vd).setScale(5, BigDecimal.ROUND_HALF_UP);
//	return new BigDecimal[]{ u, v };
//}

float dird(float ud, float vd) {
	float dird = (float)(atan(ud / vd) * 180 / 3.1415926535898);
	if (ud != 0 && vd < 0) {
		dird = dird + 180;
	}
	else if (ud < 0 && vd > 0) {
		dird = dird + 360;
	}
	else if (ud == 0 && vd > 0) {
		dird = 0;
	}
	else if (ud == 0 && vd < 0) {
		dird = 180;
	}
	else if (ud > 0 && vd == 0) {
		dird = 90;
	}
	else if (ud < 0 && vd == 0) {
		dird = 270;
	}
	else if (ud == 0 && vd == 0) {
		dird = -999;
	}
	return dird;
}

Windcalc::Windcalc()
{
}


Windcalc::~Windcalc()
{
	delete m_Databuff;
	delete m_AngleDatabuff;
}
void Windcalc::OutputJson(const char * OutJson)
{

	m_strgfs = OutJson;
	if (m_strgfs.find(".png") != m_strgfs.npos)
		m_strgfs.replace(m_strgfs.find(".png"), 5, ".json");
}
bool Windcalc::Calc(const char* UorVFile)
{
	std::string inFile = UorVFile;
	std::string name = inFile.substr(inFile.find_last_of('\\') + 1, inFile.size() - (inFile.find_last_of('\\') + 1));
	string subhead = name.substr(0, 1);
	std::string parentPath = inFile.substr(0, inFile.find_last_of('\\'));
	std::string uFile = parentPath + "\\" + "U" + name.substr(1);
	std::string vFile = parentPath + "\\" + "V" + name.substr(1);

	return Calc(uFile.c_str(), vFile.c_str());
}
bool Windcalc::Calc(const char * UFile, const char * VFile)
{
	if (0 != _access(UFile, 0) || 0 != _access(VFile, 0))
		return false;

	struct stat s1;
	stat(UFile, &s1);
	float* bufU = new float[s1.st_size / sizeof(float)];
	long long fsize1 = s1.st_size / sizeof(float);
	memset(bufU, sizeof(float), fsize1);
	FILE *fin_bin = fopen(UFile, "rb");
	fread(bufU, sizeof(float), fsize1, fin_bin);

	struct stat s2;
	stat(VFile, &s2);
	float* bufV = new float[s2.st_size / sizeof(float)];
	long long fsize2 = s2.st_size / sizeof(float);
	memset(bufV, sizeof(float), fsize2);
	FILE *fin_binV = fopen(VFile, "rb");
	fread(bufV, sizeof(float), fsize2, fin_binV);
	if (fsize1 != fsize2)
	{
		delete bufU; delete bufV; fclose(fin_bin); fclose(fin_binV);
		return false;
	}
	m_buffSize = fsize2;
	m_Databuff = new float[s2.st_size / sizeof(float)];
	m_AngleDatabuff = new float[fsize2];

	for (int i = 0; i < fsize2; i++) {
		m_Databuff[i] = sqrt(bufU[i] * bufU[i] + bufV[i] * bufV[i]);
		m_AngleDatabuff[i] = dird(bufU[i], bufV[i]);
	}
	Convert2GFS(bufU, bufV);
	delete bufU; delete bufV; fclose(fin_bin); fclose(fin_binV);
	return true;
}

bool Windcalc::Convert2GFS(float * UBuf, float * Vbuf)
{
	if (m_strgfs.size() <= 0)
		return false;
	//m_buffSize = 100;
	ofstream ss(m_strgfs.c_str());
	//std::stringstream ss;
	ss << "[";
	//U方向的数据
	ss << "{ \
	\"header\": { \
		\"discipline\": 0,\
		\"disciplineName\" :\"Meteorological product\", \
		\"gribEdition\" : 2, \
		\"gribLength\" : 6362812, \
		\"center\" : 7, \
		\"centerName\" : \"US National Weather Service - NCEP(WMC)\", \
		\"subcenter\" : 0, \
		\"refTime\" : \"2014-11-30T06:00:00.000Z\", \
		\"significanceOfRT\" : 1, \
		\"significanceOfRTName\" : \"Start of forecast\", \
		\"productStatus\" : 0,\
		\"productStatusName\" : \"Operational products\",\
		\"productType\" : 1,\
		\"productTypeName\" : \"Forecast products\",\
		\"productDefinitionTemplate\" : 0,\
		\"productDefinitionTemplateName\" : \"Analysis/forecast at horizontal level/layer at a point in time\",\
		\"parameterCategory\" : 2, \
		\"parameterCategoryName\" : \"Momentum\", \
		\"parameterNumber\" : 2, \
		\"parameterNumberName\" : \"U-component_of_wind\", \
		\"parameterUnit\" : \"m.s-1\", \
		\"genProcessType\" : 2, \
		\"genProcessTypeName\" : \"Forecast\", \
		\"forecastTime\" : 6, \
		\"surface1Type\" : 100, \
		\"surface1TypeName\" : \"Isobaric surface\", \
		\"surface1Value\" : 85000, \
		\"surface2Type\" : 255, \
		\"surface2TypeName\" : \"Missing\",\
			\"surface2Value\" : 0,\
		\"gridDefinitionTemplate\" : 0,\
		\"gridDefinitionTemplateName\" : \"Latitude_Longitude\",\
		\"numberPoints\" : 3181406,\
		\"shape\" : 6,\
		\"shapeName\" : \"Earth spherical with radius of 6,371,229.0 m\",\
		\"gridUnits\" : \"degrees\",\
		\"resolution\" : 48,\
		\"winds\" : \"true\",\
		\"scanMode\" : 0,\
		\"nx\" : 1426,\
		\"ny\" : 2231 ,\
		\"basicAngle\" : 0,\
		\"subDivisions\" : 0,\
		\"lo1\" : 106.1,\
		\"la1\" : 23.61,\
		\"lo2\" : 120.36,\
		\"la2\" : 1.3,\
		\"dx\" : 0.01,\
		\"dy\" : 0.01 \
	}, ";
	//"Data":[],
	ss << "\"data\":[";

	for (int i = 0; i < m_buffSize; i++)
	{
		ss << UBuf[i];
		if (i != m_buffSize - 1)
			ss << ",";
	}
	ss << "],";
	ss << "\"meta\": {";
	ss << "\"date\": \"2014-11-30T12:00:00.000Z\"";
	ss << "}},";

	//V方向的数据
	ss << "{ \
	\"header\": { \
		\"discipline\": 0,\
		\"disciplineName\" :\"Meteorological product\", \
		\"gribEdition\" : 2, \
		\"gribLength\" : 6362812, \
		\"center\" : 7, \
		\"centerName\" : \"US National Weather Service - NCEP(WMC)\", \
		\"subcenter\" : 0, \
		\"refTime\" : \"2014-11-30T06:00:00.000Z\", \
		\"significanceOfRT\" : 1, \
		\"significanceOfRTName\" : \"Start of forecast\", \
		\"productStatus\" : 0,\
		\"productStatusName\" : \"Operational products\",\
		\"productType\" : 1,\
		\"productTypeName\" : \"Forecast products\",\
		\"productDefinitionTemplate\" : 0,\
		\"productDefinitionTemplateName\" : \"Analysis/forecast at horizontal level/layer at a point in time\",\
		\"parameterCategory\" : 2, \
		\"parameterCategoryName\" : \"Momentum\", \
		\"parameterNumber\" : 2, \
		\"parameterNumberName\" : \"U-component_of_wind\", \
		\"parameterUnit\" : \"m.s-1\", \
		\"genProcessType\" : 2, \
		\"genProcessTypeName\" : \"Forecast\", \
		\"forecastTime\" : 6, \
		\"surface1Type\" : 100, \
		\"surface1TypeName\" : \"Isobaric surface\", \
		\"surface1Value\" : 85000, \
		\"surface2Type\" : 255, \
		\"surface2TypeName\" : \"Missing\",\
		\"surface2Value\" : 0,\
		\"gridDefinitionTemplate\" : 0,\
		\"gridDefinitionTemplateName\" : \"Latitude_Longitude\",\
		\"numberPoints\" : 3181406,\
		\"shape\" : 6,\
		\"shapeName\" : \"Earth spherical with radius of 6,371,229.0 m\",\
		\"gridUnits\" : \"degrees\",\
		\"resolution\" : 48,\
		\"winds\" : \"true\",\
		\"scanMode\" : 0,\
		\"nx\" : 1426,\
		\"ny\" : 2231 ,\
		\"basicAngle\" : 0,\
		\"subDivisions\" : 0,\
		\"lo1\" : 106.1,\
		\"la1\" : 23.61,\
		\"lo2\" : 120.36,\
		\"la2\" : 1.3,\
		\"dx\" : 0.01,\
		\"dy\" : 0.01 \
	}, ";
	//"Data":[],
	ss << "\"data\":[";

	for (int i = 0; i < m_buffSize; i++)
	{
		ss << Vbuf[i];
		if (i != m_buffSize - 1)
			ss << ",";
	}
	ss << "],";
	ss << "\"meta\": {";
	ss << "\"date\": \"2014-11-30T12:00:00.000Z\"";
	ss << "}}";


	ss << "]";
	return false;
}

void Windcalc::RandomFieldFluid(byte * pColor)
{
	m_griddel = D2M(m_griddel) / m_scale;
	unsigned int* pIntColor = (unsigned int*)pColor;
	int step = 4;
	//风的原始颜色
	byte fR = 36;
	byte fG = 51;
	byte fB = 51;
	byte fA = 255;

	for (int i = m_stepw; i < h; i += m_stepw)
	{
		for (int j = m_stepw; j < w; j += m_stepw)
		{
			int fi = i, fj = j;
			for (int t = 0; t < m_time; t++) {
				if (fi < 0 || fj < 0 || fi >= h|| fj >= w)
					continue;
				float speed = m_Databuff[w*fi + fj];
				float der = m_AngleDatabuff[w*fi + fj];
				if (der < 0)
					continue;
				unsigned int srcColor = pIntColor[w*fi + fj];
				int step = (int)ceil(speed * 1 / m_griddel);
				for (int m = 0; m < step; m++)
				{
					if (der >= 337.5 || (der >= 0 && der < 22.5))
					{
						fi++;
					}
					else if (der >= 22.5 &&  der < 67.5)
					{
						fi++;
						fj++;
					}
					else if (der >= 67.5 &&  der < 112.5)
					{
						fj++;
					}
					else if (der >= 112.5 &&  der < 157.5)
					{
						fi--;
						fj++;
					}
					else if (der >= 157.5 &&  der < 202.5)
					{
						fi--;
					}
					else if (der >= 202.5 &&  der < 247.5)
					{
						fi--;
						fj--;
					}
					else if (der >= 247.5 &&  der < 292.5)
					{
						fj--;
					}
					else if (der >= 292.5 &&  der < 337.5)
					{
						fi++;
						fj--;
					}
					if (fi < 0) fi = 0;
					if (fj < 0) fj = 0;
					if (fi >= h) fi = h - 1;
					if (fj >= w) fi = w - 1;
					//先取原始颜色然后融合
					//byte R = srcColor & 0xff;
					//byte G = (srcColor & 0xff) >> 8;
					//byte B = (srcColor & 0xff) >> 16;
					//byte A = (srcColor & 0xff) >> 24;

					pIntColor[w*fi + fj] = (fR << 24) | (fG << 16) | (fB << 8) | fA;
				}

			}
		}
	}
}
