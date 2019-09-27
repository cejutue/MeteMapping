// SatelliteImage.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "RadarRead.h"
#include "windcalc.h"
#include "GridRead.h"
#ifdef _MSC_VER
#pragma warning(disable : 4018)

#pragma warning(disable : 4100)
#pragma warning(disable : 4101)
#pragma warning(disable : 4189)
#pragma warning(disable : 4192)

#pragma warning(disable : 4238)
#pragma warning(disable : 4239)
#pragma warning(disable : 4244)
#pragma warning(disable : 4245)
#pragma warning(disable : 4251)
#pragma warning(disable : 4267)


#pragma warning(disable : 4305)
#pragma warning(disable : 4311)
#pragma warning(disable : 4389)

#pragma warning(disable : 4482)
#pragma warning(disable : 4635) 
#pragma warning(disable : 4701)

#pragma warning(disable : 4800)
#pragma warning(disable : 4996)

#endif
bool g_gfs = false;
bool g_adduv = false;
bool IsValueInputFile(std::string &inFile);
enum DataSourceType
{
	Satellite,
	Radar,
	PRCP,//雨量
	TMP,//温度
	U10m,// 风
	V10m//风
};
/*
* Class:     org_shine_network_windows_command_WindowsCommand
* Method:    getScreenpixels
* Signature: (I)[B
*/
void SavePngTransparent(const char* file, byte *pColor, int Width, int Height)
{
	CImage imDest;
	imDest.Create(Width, Height, 32, CImage::createAlphaChannel);

	// Create the header info
	BITMAPINFOHEADER bmInfohdr;
	bmInfohdr.biSize = sizeof(BITMAPINFOHEADER);
	bmInfohdr.biWidth = Width;
	bmInfohdr.biHeight = Height;
	bmInfohdr.biPlanes = 1;
	bmInfohdr.biBitCount = 4 * 8;
	bmInfohdr.biCompression = BI_RGB;
	bmInfohdr.biSizeImage = Width * Height * 4;
	bmInfohdr.biXPelsPerMeter = 0;
	bmInfohdr.biYPelsPerMeter = 0;
	bmInfohdr.biClrUsed = 0;
	bmInfohdr.biClrImportant = 0;

	BITMAPINFO bmInfo;
	bmInfo.bmiHeader = bmInfohdr;
	bmInfo.bmiColors[0].rgbBlue = 255;

	// Allocate some memory and some pointers
	unsigned char * p32Img = new unsigned char[Width*Height * 4];
	BYTE *ptr = p32Img;
	// Convert image from RGB to BGR
	int size = 0;
	for (int y = 0; y < Height; y++)
	{
		for (int x = 0; x < Width; x++)
		{
			BYTE R = *(pColor + 4 * (Width*y + x));
			BYTE G = *(pColor + 4 * (Width*y + x) + 1);
			BYTE B = *(pColor + 4 * (Width*y + x) + 2);
			*(ptr++) = R;
			*(ptr++) = G;
			*(ptr++) = B;
			//透明度设置，这里取三个值的最大值
			*(ptr++) = max(max(R, G), B);
			size++;
		}
	}

	HDC dc = imDest.GetDC();
	SetDIBitsToDevice(dc, 0, 0, Width, Height, 0, 0, 0, Height, p32Img, &bmInfo, DIB_RGB_COLORS);

	//保存
	CString sPath = file;
	//sPath.Format(_T("D:\\12.png"));
	imDest.Save(sPath);
	imDest.ReleaseDC();
}
void SavePng(const char* file, byte *pColor, int Width, int Height)
{
	CImage imDest;
	imDest.Create(Width, Height, 32);

	byte *pRealData = (byte*)imDest.GetBits();//真实数据
	BYTE *ptr = pRealData;
	int pit = imDest.GetPitch();//图像每行字节数
	int bitCount = imDest.GetBPP() / 8;
	int size = 0;
	for (int y = 0; y < Height; y++)
	{
		for (int x = 0; x < Width; x++)
		{

			//*(pRealData + pit * (Height - 1 - y) + x * bitCount) = *(pColor + 3 * (Width*y + x) + 2);
			//*(pRealData + pit * (Height - 1 - y) + x * bitCount + 1) = *(pColor + 3 * (Width*y + x) + 1);
			//*(pRealData + pit * (Height - 1 - y) + x * bitCount + 2) = *(pColor + 3 * (Width*y + x));
				*(pRealData + pit * (Height - 1 - y) + x * bitCount) = *(pColor + 4 * (Width*y + x) + 2);
				*(pRealData + pit * (Height - 1 - y) + x * bitCount + 1) = *(pColor + 4 * (Width*y + x) + 1);
				*(pRealData + pit * (Height - 1 - y) + x * bitCount + 2) = *(pColor + 4 * (Width*y + x));
				*(pRealData + pit * (Height - 1 - y) + x * bitCount + 3) = *(pColor + 4 * (Width*y + x) + 3);
			

			//BYTE R = *(pColor + 3 * (Width*y + x));
			//BYTE G = *(pColor + 3 * (Width*y + x) + 1);
			//BYTE B = *(pColor + 3 * (Width*y + x) + 2);
			//*(ptr++) = R;
			//*(ptr++) = G;
			//*(ptr++) = B;
			//imDest.SetPixelRGB(x, Height - 1 - y, *(pColor + 3 * (Width*y + x)), *(pColor + 3 * (Width*y + x) + 1), *(pColor + 3 * (Width*y + x) + 2));
			//size++;
		}
	}

	//保存
	CString sPath = file;
	//sPath.Format(_T("D:\\12.png"));
	imDest.Save(sPath);
}

//从一个路径拿到所有文件
void GetFilesName(string path, vector<string>& files)
{
	//文件句柄
	intptr_t    hFile = 0;//32位下用long
	//文件信息
	struct __finddata64_t fileinfo;
	string p;
	if ((hFile = _findfirst64(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之
			//如果不是,加入列表
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					GetFilesName(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				if (IsValueInputFile(p.assign(path).append("\\").append(fileinfo.name)))
					files.push_back(p);
			}
		} while (_findnext64(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}


void SatelliteTrans(float cloud, float offset, byte& R, byte &G, byte &B, byte &A)
{
	float data = cloud - offset;
	if (data >= -100 && data < -90)
	{
		R = 55 + (data + 100) * 20;
		G = R;
		B = R;
		A = 255;
	}
	else if (data >= -90 && data < -80)
	{
		R = 55 + (data + 90) * 20;
		G = R;
		B = 100 - (data + 90) * 10;
		A = 255;
	}
	else if (data >= -80 && data < -70)
	{
		R = 255 - (data + 80) * 20;
		G = 40;
		B = 0;
		A = 255;
	}
	else if (data >= -70 && data < -60)
	{
		R = 0;
		G = 255 - (data + 80) * 20;
		B = 0;
		A = 255;
	}
	else if (data >= -60 && data < -50)
	{
		R = 0;
		G = 0;
		B = 255 - (data + 80) * 20;
		A = 255;
	}
	else if (data >= -50 && data < -30)
	{
		R = (data + 50) * 10;
		G = R + 30;
		B = R + 30;
		A = 255;
	}
	else if (data >= -30 && data < 20)
	{
		R = 255 - (data + 30) * 5;
		G = R;
		B = R;
		A = 255;
	}
	else
	{
		R = 0;
		G = 0;
		B = 0;
		A = 0;
	}
}

void RadarTrans(unsigned char cloud, float offset, byte& R, byte &G, byte &B, byte &A)
{
	float data = (cloud - 66) / 2;// 2.0 * cloud - 75;

	if (data >= 0 && data < 5)
	{
		R = 0;
		G = 0;
		B = 0;
		A = 0;
	}
	else if (data >= 5 && data < 10)
	{
		R = 122;
		G = 114;
		B = 239;
		A = 255;
	}
	else if (data >= 10 && data < 15)
	{
		R = 30;
		G = 38;
		B = 209;
		A = 255;
	}
	else if (data >= 15 && data < 20)
	{
		R = 165;
		G = 253;
		B = 167;
		A = 255;
	}
	else if (data >= 20 && data < 25)
	{
		R = 0;
		G = 235;
		B = 0;
		A = 255;
	}
	else if (data >= 25 && data < 30)
	{
		R = 16;
		G = 147;
		B = 26;
		A = 255;
	}
	else if (data >= 30 && data < 35)
	{
		R = 253;
		G = 245;
		B = 100;
		A = 255;
	}
	else if (data >= 35 && data < 40)
	{
		R = 199;
		G = 200;
		B = 2;
		A = 255;
	}
	else if (data >= 40 && data < 45)
	{
		R = 141;
		G = 141;
		B = 0;
		A = 255;
	}
	else if (data >= 45 && data < 50)
	{
		R = 255;
		G = 173;
		B = 173;
		A = 255;
	}
	else if (data >= 50 && data < 55)
	{
		R = 255;
		G = 100;
		B = 92;
		A = 255;
	}
	else if (data >= 55 && data < 60)
	{
		R = 239;
		G = 2;
		B = 48;
		A = 255;
	}
	else if (data >= 60 && data < 65)
	{
		R = 213;
		G = 143;
		B = 255;
		A = 255;
	}
	else if (data >= 65 && data < 70)
	{
		R = 171;
		G = 36;
		B = 251;
		A = 255;
	}
	else if (data >= 70 && data < 75)
	{
		R = 131;
		G = 20;
		B = 80;
		A = 255;
	}
	else
	{
		R = 0;
		G = 0;
		B = 0;
		A = 0;
	}
}

void DefaultTrans(float cloud, float offset, byte& R, byte &G, byte &B, byte &A)
{
	if (cloud >= 0 && cloud <= 255)
	{
		R = cloud;
		G = cloud;
		B = cloud;
		A = 255;
	}
	else
	{
		R = (int)cloud%255;
		G = (int)cloud % 255;;
		B = (int)cloud % 255;;
		A = 255;
	}
}


void HaiNanTMPTrans(float calc, float offset, byte& R, byte &G, byte &B, byte &A)
{
	//绝对 温度转摄氏温度
	float cloud = (calc) -273.15;
	if (cloud < 8) { R = 0; G = 0; B = 254; A = 255; }
	else if (cloud >= -20 && cloud < -15) { R = 0; G = 128; B = 255; A = 255; }
	else if (cloud >= -15 && cloud < -10) { R = 0; G = 225; B = 255; A = 255; }
	else if (cloud >= -10 && cloud < -5) { R = 0; G = 255; B = 208; A = 255; }
	else if (cloud >= -5 && cloud < 0) { R = 0; G = 255; B = 149; A = 255; }
	else if (cloud >= 0 && cloud < 5) { R = 0; G = 255; B = 109; A = 255; }
	else if (cloud >= 5 && cloud < 10) { R = 0; G = 255; B = 17; A = 255; }
	else if (cloud >= 10 && cloud < 15) { R = 118; G = 255; B = 0; A = 255; }
	else if (cloud >= 15 && cloud < 20) { R = 172; G =255; B = 0; A = 255; }
	else if (cloud >= 20 && cloud < 25) { R = 222; G = 251; B = 109; A = 255; }
	else if (cloud >= 25 && cloud < 30) { R = 255; G = 255; B = 129; A = 255; }
	else if (cloud >= 30 && cloud < 35) { R = 245; G = 222; B = 214; A = 255; }
	else if (cloud >= 35 && cloud < 37) { R = 255; G = 191; B = 191; A = 255; }
	else if (cloud >= 37 && cloud < 40) { R = 255; G = 85; B = 1; A = 255; }
	else if (cloud >= 40 ) { R = 255; G = 85; B = 1; A = 255; }
	else { R = 255; G = 255; B = 255; A = 0; }
}
void TMPTrans(float calc, float offset, byte& R, byte &G, byte &B, byte &A)
{
	//绝对 温度转摄氏温度
	float cloud = (calc)-273.15;
	if (cloud < -20) { R = 0; G = 0; B = 254; A = 255; }
	else if (cloud >= -20 && cloud < -15) { R = 0; G = 128; B = 255; A = 255; }
	else if (cloud >= -15 && cloud < -10) { R = 0; G = 225; B = 255; A = 255; }
	else if (cloud >= -10 && cloud < -5) { R = 0; G = 255; B = 208; A = 255; }
	else if (cloud >= -5 && cloud < 0) { R = 0; G = 255; B = 149; A = 255; }
	else if (cloud >= 0 && cloud < 5) { R = 0; G = 255; B = 109; A = 255; }
	else if (cloud >= 5 && cloud < 10) { R = 0; G = 255; B = 17; A = 255; }
	else if (cloud >= 10 && cloud < 15) { R = 118; G = 255; B = 0; A = 255; }
	else if (cloud >= 15 && cloud < 20) { R = 172; G = 255; B = 0; A = 255; }
	else if (cloud >= 20 && cloud < 25) { R = 222; G = 251; B = 109; A = 255; }
	else if (cloud >= 25 && cloud < 30) { R = 255; G = 255; B = 129; A = 255; }
	else if (cloud >= 30 && cloud < 35) { R = 245; G = 222; B = 214; A = 255; }
	else if (cloud >= 35 && cloud < 37) { R = 255; G = 191; B = 191; A = 255; }
	else if (cloud >= 37 && cloud < 40) { R = 255; G = 85; B = 1; A = 255; }
	else if (cloud >= 40) { R = 255; G = 85; B = 1; A = 255; }
	else { R = 255; G = 255; B = 255; A = 0; }
}
void PRCPTrans(float cloud, float offset, byte& R, byte &G, byte &B, byte &A)
{
	if (cloud >= 0 && cloud < 0.11) { R = 255; G = 255; B = 255; A = 0; }
	else if (cloud >= 0.11 && cloud < 10) { R = 170; G = 240; B = 144; A = 255; }
	else if (cloud >= 10 && cloud < 25) { R = 61; G = 183; B = 64; A = 255; }
	else if (cloud >= 25 && cloud < 50) { R = 93; G = 182; B = 255; A = 255; }
	else if (cloud >= 50 && cloud < 100) { R = 1; G = 1; B = 219; A = 255; }
	else if (cloud >= 100 && cloud < 250) { R = 253; G = 1; B = 246; A = 255; }
	else if (cloud >= 250 && cloud < 500) { R = 129; G = 1; B = 65; A = 255; }
	else if (cloud >= 500 && cloud < 500) { R = 255; G = 255; B = 255; A = 255; }
	else { R = 255; G = 255; B = 255; A = 0; }

}
void UVTrans(float cloud, float offset, byte& R, byte &G, byte &B, byte &A)
{
	 if (cloud >= 0 && cloud < 4) { R = 255; G = 255; B = 255; A = 0; }
	else if (cloud >= 4 && cloud < 6) { R = 232; G = 245; B = 155; A = 255; }
	else if (cloud >= 6 && cloud < 8) { R = 226; G = 239; B = 15; A = 255; }
	else if (cloud >= 8 && cloud < 12) { R = 255; G = 238; B = 124; A = 255; }
	else if (cloud >= 12 && cloud < 14) { R = 238; G = 166; B = 83; A = 255; }
	else if (cloud >= 14 && cloud < 18) { R = 222; G = 123; B = 55; A = 255; }
	else if (cloud >= 18 && cloud < 21) { R = 236; G = 83; B = 163; A = 255; }
	else if (cloud >= 21 && cloud < 25) { R = 155; G = 162; B = 118; A = 255; }
	else if (cloud >= 25 && cloud < 29) { R = 126; G = 150; B = 255; A = 255; }
	else if (cloud >= 29 && cloud < 33) { R = 141; G = 178; B = 255; A = 255; }
	else if (cloud >= 33  ) { R = 180; G = 201; B = 255; A = 255; }
	else { R = 255; G = 255; B = 255; A = 0; }
}
void VTrans(float cloud, float offset, byte& R, byte &G, byte &B, byte &A)
{
	if (cloud >= 0 && cloud <= 255)
	{
		R = cloud;
		G = cloud;
		B = cloud;
		A = 255;
	}
	else if (cloud <0)
	{
		R = 255 - cloud;
		G = 255 - cloud;
		B = 255 - cloud;
		A = 255;
	}
}
//传入一个路径，解析二进制后，再去存图片
void SaveCloudData(const char * OutPath, const char *InPath, int Width, int Height, float offset, int step, byte *pColor, bool bAlpha, DataSourceType eType)
{
	FILE *fin_bin = fopen(InPath, "rb");
	int rc;

	int size = 0;
	byte R =0;
	byte G =0;
	byte B =0 ;
	byte A = 0;
	RadarRead r;
	r.Read(InPath);

	if (r.isRadarData)
	{
		Width = r.XNumGrids;
		Height = r.YNumGrids;
		delete pColor;
		pColor = new byte[r.XNumGrids*r.YNumGrids*step];
		//二进制转RGB
		for (int i = 0; i < r.m_buffSize; i++) {
			RadarTrans(r.m_Databuff[i], offset, R, G, B, A);
			pColor[step * size] = R;
			pColor[step * size + 1] = G;
			pColor[step * size + 2] = B;
			pColor[step * size + 3] = A;
			size++;
		}

	}
	else
	{
		float* buf = 0;
		long long fsize = 0;
		Windcalc wind;
		GridRead grid;
		pColor = new byte[Width*Height*step];
		if (eType == U10m || eType == V10m)
		{
			if (g_gfs) 
			{
				wind.OutputJson(OutPath);
			}
			if (!wind.Calc(InPath))
				goto ret;
			buf = wind.m_Databuff;
			fsize = wind.m_buffSize;
		}	
		else 
		{
			if (!grid.Read(InPath))
				goto ret;

			buf = grid.m_Databuff;
			fsize = grid.m_buffSize;
		}

		for (int i = 0; i < fsize; i++) {
				switch (eType)
				{
				case Satellite:
					SatelliteTrans(buf[i], offset, R, G, B, A);
					break;
				case Radar:
					RadarTrans(buf[i], offset, R, G, B, A);
					break;
				case PRCP:
					PRCPTrans(buf[i], offset, R, G, B, A);
					break;
				case TMP:
					TMPTrans(buf[i], offset, R, G, B, A);
					break;
				case U10m:
				case V10m:
					UVTrans(buf[i], offset, R, G, B, A);
					break;
				default:
					DefaultTrans(buf[i], offset, R, G, B, A);
					break;
				}
				
				pColor[step * size] = R;
				pColor[step * size + 1] = G;
				pColor[step * size + 2] = B;
				pColor[step * size + 3] = A;
				size++;
			}
		if (g_adduv &&  (eType == U10m || eType == V10m))
			wind.RandomFieldFluid(pColor);
	}
	if (bAlpha)
	{
		//存png，带透明通道
		SavePngTransparent(OutPath, pColor, Width, Height);
	}
	else
	{
		//存png，不带透明通道
		SavePng(OutPath, pColor, Width, Height);
	}
ret:
	{
		fclose(fin_bin);
		delete pColor;
	}

}

bool isNumber(char uni)
{
	if (uni >= '0' && uni <= '9')
		return true;
	else
		return false;
}

bool isEnChar(char s)
{
	if ((s >= 'A' && s <= 'Z') || (s >= 'a' && s <= 'z'))
		return true;
	else
		return false;
}
// -outputdir:C:\sdfd   -head:REGC
std::string getoutputfile(std::string & binstrpath, std::string & outFolderPath, const char* g = "REGC")
{
	//std::string name = binstrpath.substr(binstrpath.find_last_of('/') + 1, binstrpath.size() - (binstrpath.find_last_of('/') + 1));

	if (outFolderPath.size() <= 0 && strlen(g) <= 0)
	{
		return binstrpath;
	}
	std::string path = "";
	std::string name = binstrpath.substr(binstrpath.find_last_of('\\') + 1, binstrpath.size() - (binstrpath.find_last_of('\\') + 1));
	std::string parPath = binstrpath.substr(0, binstrpath.find_last_of('\\'));

	int i;
	for (i = 0; i < name.size(); i++)
	{
		if (isNumber(name[i]))
			break;
	}
	std::string subhead = name.substr(0, i);
	std::string subend = name.substr(i, name.size() - i);
	std::string head = g;
	if (strlen(g) > 0)
	{
		name = head + subend;
	}
	if (outFolderPath.size() > 0)
	{
		parPath = outFolderPath;
	}



	return parPath + "\\" + name;
}

/// \brief 将字符串分割为若干子串
std::vector<std::string> Split(const char* ss, const char* strSep)
{
	if (NULL == ss || strlen(ss) <= 0)
		return std::vector <std::string>();

	std::vector <std::string> ret_;

	if (NULL == strSep || strlen(strSep) <= 0)
	{
		ret_.push_back((ss));
		return ret_;
	}

	std::string sep = strSep;
	std::string str = ss;
	std::string tmp;
	std::string::size_type pos_begin = str.find_first_not_of(sep);
	std::string::size_type comma_pos = 0;

	while (pos_begin != std::string::npos)
	{
		comma_pos = str.find(sep, pos_begin);
		if (comma_pos != std::string::npos)
		{
			tmp = str.substr(pos_begin, comma_pos - pos_begin);
			pos_begin = comma_pos + sep.length();
		}
		else
		{
			tmp = str.substr(pos_begin);
			pos_begin = comma_pos;
		}

		ret_.push_back(tmp);
		tmp.clear();
	}
	return ret_;
}
bool IsValueInputFile(std::string &inFile)
{
	std::string name = inFile.substr(inFile.find_last_of('\\') + 1, inFile.size() - (inFile.find_last_of('\\') + 1));
	string subhead = name.substr(0, 4);

	//if (subhead == "DISK" || subhead == "REGC" || subhead == "PRCP" || subhead == "U10m" || subhead == "V10m" || subhead.substr(0, 3) == "TMP")
	//{
	//	return true;
	//}
	if (name.find(".bin") != inFile.npos)
	{
		if (subhead == "DISK" || subhead == "REGC" || subhead == "Z_OT")
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	if (subhead == "PRCP" || subhead == "U10m" || subhead == "V10m" || subhead.substr(0, 3) == "TMP")
	{
		if (name.find(".") == inFile.npos)
			return true;
		else
			return false;
	}

	return false;
}


int main(int argc, char* argv[])
{
	int ggg = sizeof(float);
	if (argc < 1)
	{
		return 0;
	}

	//命令行输入参数
	bool bAlpha = false;
	vector<string> vFiles;
	std::string outputdir = "";
	string filehead = "";
	std::string wstr = "";
	std::string hstr = "";
	for (int i = 1; i < argc; i++)
	{
		string file = argv[i];
		if (IsValueInputFile(file))
		{
			vFiles.push_back(file);
		}
		else if (0 == file.compare("-a"))//-a是否存透明通道标签
		{
			bAlpha = true;
		}
		else if (file.npos != file.find("-outputdir:"))
		{
			outputdir = file.substr(file.find(":") + 1, file.size() - file.find(":") + 1);
			//std::vector<string> rrr = Split(file.c_str(), ":");
			//if(rrr.size() > 1)
			//outputdir = rrr[1];
			if (::_access(outputdir.c_str(), 00) != 0)
				_mkdir(outputdir.c_str());
		}
		else if (file.npos != file.find("-head:"))
		{
			filehead = file.substr(file.find(":") + 1, file.size() - file.find(":") + 1);
			/*std::vector<string> rrr = Split(file.c_str(), ":");
			if (rrr.size() > 1)
				outputdir = rrr[1];*/
		}
		else if (file.npos != file.find("-w:"))
		{
			wstr = file.substr(file.find(":") + 1, file.size() - file.find(":") + 1);
		}
		else if (file.npos != file.find("-h:"))
		{
			hstr = file.substr(file.find(":") + 1, file.size() - file.find(":") + 1);
		}
		else if (file.npos != file.find("-gfs"))
		{
			g_gfs = true;
		}
		else if (file.npos != file.find("-adduv"))
		{
			g_adduv = true;
		}
		else
		{
			vector<string> files;
			GetFilesName(file, files);
			vFiles.insert(vFiles.end(), files.begin(), files.end());
		}
	}



	int Width = 876;
	if (wstr.size() > 0)
		Width = atoi(wstr.c_str());
	int Height = 801;
	if (hstr.size() > 0)
		Height = atoi(hstr.c_str());
	float offset = 273.15;
	int step = 4;
	DataSourceType eType;
	byte *pColor = NULL;// new byte[step*Width*Height];
	//遍历文件，开始转换
	for (int i = 0; i < vFiles.size(); i++)
	{
		string inFile = vFiles[i];
		string outFile = getoutputfile(inFile, outputdir, filehead.c_str());
		std::string name = inFile.substr(inFile.find_last_of('\\') + 1, inFile.size() - (inFile.find_last_of('\\') + 1));
		string subhead = name.substr(0, 4);

		if (subhead == "DISK"  ) { Width = 2376; Height = 2001;	eType = Satellite; }
		else if (subhead == "REGC") { Width = 876; Height = 801; eType = Satellite;	}
		else if (subhead == "Z_OT") { Width = 876; Height = 801; eType = Radar; }
		else if (subhead == "PRCP") { Width = 1426; Height = 2231; eType = PRCP; }
		else if (subhead == "U10m") { Width = 1426; Height = 2231; eType = U10m; }
		else if (subhead == "V10m") { Width = 1426; Height = 2231; eType = V10m; }
		else if (subhead.substr(0, 3) == "TMP") { Width = 1426; Height = 2231; eType = TMP; }


		if (wstr.size() > 0)
			Width = atoi(wstr.c_str());

		if (hstr.size() > 0)
			Height = atoi(hstr.c_str());

		if (outFile.find(".bin") != outFile.npos)
			outFile.replace(outFile.find(".bin"), 4, ".png");
		else
			outFile = outFile.append(".png");

		std::cout << inFile.c_str() << std::endl;
		SaveCloudData(outFile.c_str(), inFile.c_str(), Width, Height, offset, step, pColor, bAlpha, eType);

	}

	//delete[]pColor;
	return 0;
}
