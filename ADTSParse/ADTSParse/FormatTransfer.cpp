#include "stdafx.h"
#include "FormatTransfer.h"

/*
params: 0~9,a~f,A~F
return:0~15
��0~9,a~f,A~F����ֵת���ɾ����0~15
*/
char Format_Trans::GetNumFromASCII(char data)
{
	if(data>='0' && data<='9')
		return (data-'0');
	else if(data>='a' && data<='f')
		return (data-'a'+10);
	else if(data>='A' && data<='F')
		return (data-'A'+10);
	else
		return 0;
}
/*
params: unsigned int data,0~0xffffffff
return: src = String,"ffffff"
��32λ����ת����ʮ�������ַ����ĸ�ʽ
*/
void Format_Trans::GetASCIIFromNum(unsigned int data,unsigned char *src)
{
	src[0] = Format_Trans::GetHexASCIIFromInt(((data&0xff000000)>>24)/16);
	src[1] = Format_Trans::GetHexASCIIFromInt(((data&0xff000000)>>24)%16);
	src[2] = Format_Trans::GetHexASCIIFromInt(((data&0x00ff0000)>>16)/16);
	src[3] = Format_Trans::GetHexASCIIFromInt(((data&0x00ff0000)>>16)%16);
	src[4] = Format_Trans::GetHexASCIIFromInt(((data&0x0000ff00)>>8)/16);
	src[5] = Format_Trans::GetHexASCIIFromInt(((data&0x0000ff00)>>8)%16);
	src[6] = Format_Trans::GetHexASCIIFromInt(((data&0x000000ff)>>0)/16);
	src[7] = Format_Trans::GetHexASCIIFromInt(((data&0x000000ff)>>0)%16);
}

/*
params: char data,0~9,a~f,A~F
return: 0: No Hex Format , 1:Hex Format
�ж��Ƿ���0~9,a~f,A~F��ʽ
*/
char Format_Trans::IsHexFormat(char data)
{
	if(data>='0' && data<='9')
		return 1;
	else if(data>='a' && data<='f')
		return 1;
	else if(data>='A' && data<='F')
		return 1;
	else
		return 0;
}
/*
params: 0~15
return: 0~9,A~F
��0~15ת����0~F
*/
char Format_Trans::GetHexASCIIFromInt(char data)
{
	char Index[] = {"0123456789ABCDEF"};
	return Index[data];
}
/*
params: src is hex,255
return: des is hex string "ff"
��0~255ת����ʮ�����Ƶ�0x??���ַ���
des�Ĵ�С������src��С������
*/
void Format_Trans::HexToASCII(unsigned char *src,unsigned char*des,unsigned int size)
{
	unsigned int i = 0;
	for(i=0;i<size;i++){
		des[2*i] = Format_Trans::GetHexASCIIFromInt(src[i]/16);
		des[2*i+1] = Format_Trans::GetHexASCIIFromInt(src[i]%16);
	}
}
/*
��ʮ�������ı���ʽ���ַ�����0x12345678
���ɾ����unsigned long long ����ֵ
*/
unsigned long long Format_Trans::GetAddFromAddchar(unsigned char *data)
{
	unsigned long long rts = 0;

	rts = GetNumFromASCII(data[0])*(1<<28) + GetNumFromASCII(data[1])*(1<<24) +\
		  GetNumFromASCII(data[2])*(1<<20) + GetNumFromASCII(data[3])*(1<<16) +\
		  GetNumFromASCII(data[4])*(1<<12) + GetNumFromASCII(data[5])*(1<<8) +\
		  GetNumFromASCII(data[6])*(1<<4) + GetNumFromASCII(data[7])*(1<<0);

	return rts;
}
/*
���ַ���1234׼����ʮ�����Ƶ�0x1234��ֵ
*/
unsigned long long Format_Trans::GetU32FromAddr(unsigned char *data)
{
	unsigned long long rts = 0;

	rts = (data[0])*(1<<24) + (data[1])*(1<<16) +\
		  (data[2])*(1<<8) + (data[3])*(1<<0);

	return rts;
}
/*
���ַ���1234׼����ʮ�����Ƶ�0x1234��ֵ\
sizeof(unsigned long) = 4;
sizeof(unsigned long long) = 8;
*/
unsigned long long Format_Trans::GetU64FromAddr(unsigned char *data)
{
	unsigned long long rts = 0;

	rts = (data[0])*((unsigned long long)1<<56) + (data[1])*((unsigned long long)1<<48) +\
		  (data[2])*((unsigned long long)1<<40) + (data[3])*((unsigned long long)1<<32)+\
		  (data[4])*((unsigned long long)1<<24) + (data[5])*((unsigned long long)1<<16) +\
		  (data[6])*((unsigned long long)1<<8) + (data[7])*((unsigned long long)1<<0);

	return rts;
}
/*
��Сдa~fת���ɴ�дA~F
*/
char Format_Trans::GotBigWriteFromLittle(char data)
{
	if(data>='a'&&data<='f')
		return 'A'+data-'a';
	else 
		return data;
}

/*
�ַ����ȽϺ���
*/
unsigned char Format_Trans::StringCmp(unsigned char *a,char*b,unsigned int length)
{
	unsigned int i = 0;
	for(i=0;i<length;i++)
	{
		if(a[i]!=b[i])
			return 0;
	}
	return 1;
}
/*
int ת���� Cstring
һ������������ݵ��ı�
*/
CString Format_Trans::ITCS(int i)
{
	CString str;
	str.Format(_T("%d"),i);
	return str;
}
/*
unsigned int ת���� CString
һ������������ݵ��ı�
*/
CString Format_Trans::UITXCS(unsigned int i)
{
	CString str;
	str.Format(_T("%x"),i);
	return str;
}

CString Format_Trans::UITCS(unsigned int i)
{
	CString str;
	str.Format(_T("%d"),i);
	return str;
}
/*
int ת���� Cstring
һ������������ݵ��ı�
unsigned long long to %x CString
*/
CString Format_Trans::ULLTXCS(unsigned long long i)
{
	CString str;
	str.Format(_T("%llx"),i);
	return str;
}
/*
double ת���� Cstring
һ���������double���ı�
*/
CString Format_Trans::DTCS(double i)
{
	CString str;
	str.Format(_T("%.2f"),i);
	return str;
}
/*
	��ȡ�������ַ���
*/
CString Format_Trans::GetDateString(void)
{
	SYSTEMTIME st;
	CString strDate;
	GetLocalTime(&st);
	strDate.Format(_T("%4d-%02d-%2d"),st.wYear,st.wMonth,st.wDay);
	return strDate;
}
/*
	��ȡ��ǰ���·��ַ���
*/
CString Format_Trans::GetCurMonthString(void)
{
	SYSTEMTIME st;
	CString strCurMonth;
	GetLocalTime(&st);
	strCurMonth.Format(_T("%2d �·�"),st.wMonth);
	return strCurMonth;
}
/*
	��ȡ�ϸ��µ��·��ַ���
*/
CString Format_Trans::GetLastMonthString(void)
{
	SYSTEMTIME st;
	CString strLastMonth;
	GetLocalTime(&st);
	strLastMonth.Format(_T("%2d �·�"),st.wMonth-1);
	return strLastMonth;
}
/*
	��ȡʱ�����ַ���
*/
CString Format_Trans::GetTimeString(void)
{
	SYSTEMTIME st;
	CString strTime;
	GetLocalTime(&st);
	strTime.Format(_T("%2d:%2d:%2d"),st.wHour,st.wMinute,st.wSecond);
	return strTime;
}