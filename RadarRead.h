#pragma once
class Read
{

};
//char ZonName[12];	// diamond 131 12个字节
//char DataName[38];//数据说明(例如 2008年5月19日雷达三维拼图)38个字节
//char	Flag[8];		// 文件标志，"swan"
//char	Version[8];		// 数据版本号，"1.0" 目前为2.0
//unsigned short int year;//2008 两个字节
//unsigned short int month;//05  两个字节
//unsigned short int day;//19    两个字节
//unsigned short int hour;//14   两个字节
//unsigned short int minute;//31 两个字节
//unsigned short int interval;  //两个字节        
//unsigned short int XNumGrids;//1300 两个字节
//unsigned short int YNumGrids;//800 两个字节
//unsigned short int ZNumGrids;//20  两个字节
//int RadarCount; //拼图雷达数 四个字节
//float StartLon; //网格开始经度（左上角） 四个字节
//float StartLat; //网格开始纬度（左上角） 四个字节
//float CenterLon;//网格中心经度 四个字节
//float CenterLat;//网格中心纬度 四个字节
//float XReso;	//经度方向分辨率 四个字节
//float YReso;	//纬度方向分辨率 四个字节
//float ZhighGrids[40];//垂直方向的高度（单位km）数目根据ZnumGrids而得（最大40层） 160个字节。
//char RadarStationName[20][16];    //相关站点名称,	20*16字节
//float  RadarLongitude[20];      //相关站点所在经度，单位：度， 4*20字节
//float  RadarLatitude[20];       //相关站点所在纬度，单位：度， 4*20字节
//float  RadarAltitude[20];      //相关站点所在海拔高度，单位：米， 4*20字节
//unsigned char    MosaicFlag[20];    //该相关站点数据是否包含在本次拼图中，未包含:0，包含:1, 20字节
//									//数据类型定义，版本号为.=1.5
//short  m_iDataType;   //0-unsigned char 1-char 2-unsigned short 3-short 4
//					  //每一层的向量数，版本=2.0
//short  m_iLevelDimension;
//char	Reserved[168];
//接下来是数据块，从底层到高层进行排列共ZnumGrids层。一个字节存储一个数据，值的范围0 - 255，2*dBZ + 66等于该字节的值。每层的数据从起始点（左上角）开始，按维向（纬度y）减小写每行的经向（经度x增大）数据。
class RadarRead
{
public:
	char ZonName[12];	// diamond 131 12个字节
	char DataName[38];//数据说明(例如 2008年5月19日雷达三维拼图)38个字节
	char	Flag[8];		// 文件标志，"swan"
	char	Version[8];		// 数据版本号，"1.0" 目前为2.0
	unsigned short int year;//2008 两个字节
	unsigned short int month;//05  两个字节
	unsigned short int day;//19    两个字节
	unsigned short int hour;//14   两个字节
	unsigned short int minute;//31 两个字节
	unsigned short int interval;  //两个字节        
	unsigned short int XNumGrids;//1300 两个字节
	unsigned short int YNumGrids;//800 两个字节
	unsigned short int ZNumGrids;//20  两个字节
	int RadarCount; //拼图雷达数 四个字节
	float StartLon; //网格开始经度（左上角） 四个字节
	float StartLat; //网格开始纬度（左上角） 四个字节
	float CenterLon;//网格中心经度 四个字节
	float CenterLat;//网格中心纬度 四个字节
	float XReso;	//经度方向分辨率 四个字节
	float YReso;	//纬度方向分辨率 四个字节
	float ZhighGrids[40];//垂直方向的高度（单位km）数目根据ZnumGrids而得（最大40层） 160个字节。
	char RadarStationName[20][16];    //相关站点名称,	20*16字节
	float  RadarLongitude[20];      //相关站点所在经度，单位：度， 4*20字节
	float  RadarLatitude[20];       //相关站点所在纬度，单位：度， 4*20字节
	float  RadarAltitude[20];      //相关站点所在海拔高度，单位：米， 4*20字节
	unsigned char    MosaicFlag[20];    //该相关站点数据是否包含在本次拼图中，未包含:0，包含:1, 20字节
										//数据类型定义，版本号为.=1.5
	short   m_iDataType;   //0-unsigned char 1-char 2-unsigned short 3-short 4
	//每一层的向量数，版本=2.0
	short   m_iLevelDimension;
	char	Reserved[168];

	bool isRadarData = false;
	unsigned char * m_Databuff;
	long long m_buffSize = 0;
	
public:

	RadarRead();
	~RadarRead();
	bool Read(const char* strfile);
};

