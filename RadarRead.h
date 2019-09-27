#pragma once
class Read
{

};
//char ZonName[12];	// diamond 131 12���ֽ�
//char DataName[38];//����˵��(���� 2008��5��19���״���άƴͼ)38���ֽ�
//char	Flag[8];		// �ļ���־��"swan"
//char	Version[8];		// ���ݰ汾�ţ�"1.0" ĿǰΪ2.0
//unsigned short int year;//2008 �����ֽ�
//unsigned short int month;//05  �����ֽ�
//unsigned short int day;//19    �����ֽ�
//unsigned short int hour;//14   �����ֽ�
//unsigned short int minute;//31 �����ֽ�
//unsigned short int interval;  //�����ֽ�        
//unsigned short int XNumGrids;//1300 �����ֽ�
//unsigned short int YNumGrids;//800 �����ֽ�
//unsigned short int ZNumGrids;//20  �����ֽ�
//int RadarCount; //ƴͼ�״��� �ĸ��ֽ�
//float StartLon; //����ʼ���ȣ����Ͻǣ� �ĸ��ֽ�
//float StartLat; //����ʼγ�ȣ����Ͻǣ� �ĸ��ֽ�
//float CenterLon;//�������ľ��� �ĸ��ֽ�
//float CenterLat;//��������γ�� �ĸ��ֽ�
//float XReso;	//���ȷ���ֱ��� �ĸ��ֽ�
//float YReso;	//γ�ȷ���ֱ��� �ĸ��ֽ�
//float ZhighGrids[40];//��ֱ����ĸ߶ȣ���λkm����Ŀ����ZnumGrids���ã����40�㣩 160���ֽڡ�
//char RadarStationName[20][16];    //���վ������,	20*16�ֽ�
//float  RadarLongitude[20];      //���վ�����ھ��ȣ���λ���ȣ� 4*20�ֽ�
//float  RadarLatitude[20];       //���վ������γ�ȣ���λ���ȣ� 4*20�ֽ�
//float  RadarAltitude[20];      //���վ�����ں��θ߶ȣ���λ���ף� 4*20�ֽ�
//unsigned char    MosaicFlag[20];    //�����վ�������Ƿ�����ڱ���ƴͼ�У�δ����:0������:1, 20�ֽ�
//									//�������Ͷ��壬�汾��Ϊ.=1.5
//short  m_iDataType;   //0-unsigned char 1-char 2-unsigned short 3-short 4
//					  //ÿһ������������汾=2.0
//short  m_iLevelDimension;
//char	Reserved[168];
//�����������ݿ飬�ӵײ㵽�߲�������й�ZnumGrids�㡣һ���ֽڴ洢һ�����ݣ�ֵ�ķ�Χ0 - 255��2*dBZ + 66���ڸ��ֽڵ�ֵ��ÿ������ݴ���ʼ�㣨���Ͻǣ���ʼ����ά��γ��y����Сдÿ�еľ��򣨾���x�������ݡ�
class RadarRead
{
public:
	char ZonName[12];	// diamond 131 12���ֽ�
	char DataName[38];//����˵��(���� 2008��5��19���״���άƴͼ)38���ֽ�
	char	Flag[8];		// �ļ���־��"swan"
	char	Version[8];		// ���ݰ汾�ţ�"1.0" ĿǰΪ2.0
	unsigned short int year;//2008 �����ֽ�
	unsigned short int month;//05  �����ֽ�
	unsigned short int day;//19    �����ֽ�
	unsigned short int hour;//14   �����ֽ�
	unsigned short int minute;//31 �����ֽ�
	unsigned short int interval;  //�����ֽ�        
	unsigned short int XNumGrids;//1300 �����ֽ�
	unsigned short int YNumGrids;//800 �����ֽ�
	unsigned short int ZNumGrids;//20  �����ֽ�
	int RadarCount; //ƴͼ�״��� �ĸ��ֽ�
	float StartLon; //����ʼ���ȣ����Ͻǣ� �ĸ��ֽ�
	float StartLat; //����ʼγ�ȣ����Ͻǣ� �ĸ��ֽ�
	float CenterLon;//�������ľ��� �ĸ��ֽ�
	float CenterLat;//��������γ�� �ĸ��ֽ�
	float XReso;	//���ȷ���ֱ��� �ĸ��ֽ�
	float YReso;	//γ�ȷ���ֱ��� �ĸ��ֽ�
	float ZhighGrids[40];//��ֱ����ĸ߶ȣ���λkm����Ŀ����ZnumGrids���ã����40�㣩 160���ֽڡ�
	char RadarStationName[20][16];    //���վ������,	20*16�ֽ�
	float  RadarLongitude[20];      //���վ�����ھ��ȣ���λ���ȣ� 4*20�ֽ�
	float  RadarLatitude[20];       //���վ������γ�ȣ���λ���ȣ� 4*20�ֽ�
	float  RadarAltitude[20];      //���վ�����ں��θ߶ȣ���λ���ף� 4*20�ֽ�
	unsigned char    MosaicFlag[20];    //�����վ�������Ƿ�����ڱ���ƴͼ�У�δ����:0������:1, 20�ֽ�
										//�������Ͷ��壬�汾��Ϊ.=1.5
	short   m_iDataType;   //0-unsigned char 1-char 2-unsigned short 3-short 4
	//ÿһ������������汾=2.0
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

