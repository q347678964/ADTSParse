#include "stdafx.h"
#include "ADTSParseHandler.h"

#include "resource.h"		// 主符号,need to push before dlg.h
#include "ADTSParseDlg.h"

CADTSParseDlg *DlgOperate;


void ADTSHandler::DebugUpdate(void)
{
	DebugInfoString+="\r\n";
	DlgOperate = (CADTSParseDlg *)AfxGetMainWnd();//AfxGetMainWnd();返回CWnd主窗口
	DlgOperate->DebugInfo(DebugInfoString);
}

unsigned long long  ADTSHandler::FindSynvPoint(CFile *pFile,unsigned long long FileSize,unsigned long long StartPoint)
{
	unsigned char buff[2];
	while(1){
		pFile->Seek(StartPoint,CFile::begin);
		pFile->Read(buff,1);
		if(buff[0] == 0xff){		//just first byte to be 0xff
			if(StartPoint++>FileSize) //Over file size 
				return 0xffffffff;
			pFile->Seek(StartPoint,CFile::begin);
			pFile->Read(buff,1);
			if((buff[0]&0xf0) == 0xf0)	//just next byte to be 0xf
			{
				break;
			}
		}else{
			if(StartPoint++>FileSize) //Over file size 
				return 0xffffffff;
		}
	}	

	return StartPoint-1;
}
/*
#define SYNCWORD_MARK(x)			((x & 0xFFF0000000000000)>>52)
#define ID_MARK(x)					((x & 0x0008000000000000)>>51)
#define LAYER_MARK(x)				((x & 0x0006000000000000)>>49)
#define PROTECTION_MARK(x)			((x & 0x0001000000000000)>>48)
#define PROFILE_MARK(x)				((x & 0x0000C00000000000)>>46)
#define SAMPLING_MARK(x)			((x & 0x00003C0000000000)>>42)
#define PRIVATE_MARK(x)				((x & 0x0000020000000000)>>41)
#define CHANNEL_MARK(x)				((x & 0x000001C000000000)>>38)
#define ORIGINAL_MARK(x)			((x & 0x0000002000000000)>>37)
#define HOME_MARK(x)				((x & 0x0000001000000000)>>36)
#define COPYID_MARK(x)				((x & 0x0000000800000000)>>35)
#define COPYSTART_MARK(x)			((x & 0x0000000400000000)>>34)
#define FRAMELENGTH_MARK(x)			((x & 0x00000003FFD00000)>>21)
#define ADTSBUFFERFULLNESS_MARK		((x & 0x00000000001FFC00)>>10)
#define NUMBERBLOCKS_MARK			((x & 0x0000000000000300)>>8)
*/
void ADTSHandler::Init(void)
{
	CString  strPathName;
	CFileStatus Fstatus;

	GetModuleFileName(NULL,strPathName.GetBuffer(256),256);
	strPathName += "ADTSInformation.txt";

	if(CFile::GetStatus(strPathName,Fstatus,NULL)){		//Remove exit file
		CFile::Remove(strPathName);
		CFile pOutputFile(strPathName,CFile::modeCreate);//
		pOutputFile.Close();
	}else{
		CFile pOutputFile(strPathName,CFile::modeCreate);
		pOutputFile.Close();
	}

	g_FrameCounter = 0; //Clear Counter;
}
void ADTSHandler::PrintfHeaderToFile(void)
{
	CString  strPathName;
	CFileStatus Fstatus;
	CString OutPutString;
	unsigned int OutputStringLength = 0;
	unsigned long long File_Length = 0,File_CurRP = 0;
	GetModuleFileName(NULL,strPathName.GetBuffer(256),256);
	strPathName += "ADTSInformation.txt";

	CFile pOutputFile(strPathName,CFile::modeReadWrite);//
	File_Length = pOutputFile.GetLength();	//Length
	File_CurRP = File_Length;
	pOutputFile.Seek(File_CurRP,CFile::begin);
	OutPutString = (CString)("Id = ")+(CString)ADTSHandler::UITCS(g_AtdsHeader.ID)+(CString)(",");
	OutPutString += (CString)("Layer = ")+(CString)ADTSHandler::UITCS(g_AtdsHeader.layer)+(CString)(",");
	OutPutString += (CString)("ProtectionAbsent = ")+(CString)ADTSHandler::UITCS(g_AtdsHeader.protection_absent)+(CString)(",");
	OutPutString += (CString)("Profile = ")+(CString)ADTSHandler::UITCS(g_AtdsHeader.profile)+(CString)(",");
	OutPutString += (CString)("Samplling = ")+(CString)ADTSHandler::UITCS(g_AtdsHeader.sampling_frequency)+(CString)(",");
	OutPutString += (CString)("PrivateBit = ")+(CString)ADTSHandler::UITCS(g_AtdsHeader.private_bit)+(CString)(",");
	OutPutString += (CString)("Channel = ")+(CString)ADTSHandler::UITCS(g_AtdsHeader.channel_config)+(CString)(",");
	OutPutString += (CString)("OriginalCopy = ")+(CString)ADTSHandler::UITCS(g_AtdsHeader.original_copy)+(CString)(",");
	OutPutString += (CString)("Home = ")+(CString)ADTSHandler::UITCS(g_AtdsHeader.home)+(CString)(",");
	OutPutString += (CString)("CopyRightID = ")+(CString)ADTSHandler::UITCS(g_AtdsHeader.copyright_id_bit)+(CString)(",");
	OutPutString += (CString)("CopyRightIDStart = ")+(CString)ADTSHandler::UITCS(g_AtdsHeader.copyright_id_start)+(CString)(",");
	OutPutString += (CString)("AACFrameLength = ")+(CString)ADTSHandler::UITCS(g_AtdsHeader.aac_frame_length)+(CString)(",");
	OutPutString += (CString)("ADTSBufferFull = ")+(CString)ADTSHandler::UITCS(g_AtdsHeader.adts_buffer_fullness)+(CString)(",");
	OutPutString += (CString)("NumberBlocks = ")+(CString)ADTSHandler::UITCS(g_AtdsHeader.number_of_rawdata_blocks_in_frame)+(CString)("\r\n");
	pOutputFile.Write(OutPutString,OutPutString.GetLength()*sizeof(wchar_t));//一定要乘sizeof
	pOutputFile.Close();

}

void ADTSHandler::PrintfHeader(void)
{
	
#if 0
	DebugInfoString += ADTSHandler::UITXCS(g_AtdsHeader.syncword);DebugInfoString += ',';
	DebugInfoString += ADTSHandler::UITCS(g_AtdsHeader.ID);	DebugInfoString += ',';
	DebugInfoString += ADTSHandler::UITCS(g_AtdsHeader.layer);DebugInfoString += ',';
	DebugInfoString += ADTSHandler::UITCS(g_AtdsHeader.protection_absent);	DebugInfoString += ',';
	DebugInfoString += ADTSHandler::UITCS(g_AtdsHeader.profile);DebugInfoString += ',';
	DebugInfoString += ADTSHandler::UITCS(g_AtdsHeader.sampling_frequency);	DebugInfoString += ',';
	DebugInfoString += ADTSHandler::UITCS(g_AtdsHeader.private_bit);DebugInfoString += ',';
	DebugInfoString += ADTSHandler::UITCS(g_AtdsHeader.channel_config);	DebugInfoString += ',';
	DebugInfoString += ADTSHandler::UITCS(g_AtdsHeader.original_copy);DebugInfoString += ',';
	DebugInfoString += ADTSHandler::UITCS(g_AtdsHeader.home);	DebugInfoString += ',';
	DebugInfoString += ADTSHandler::UITCS(g_AtdsHeader.copyright_id_bit);DebugInfoString += ',';
	DebugInfoString += ADTSHandler::UITCS(g_AtdsHeader.copyright_id_start);DebugInfoString += ',';
	DebugInfoString += ADTSHandler::UITCS(g_AtdsHeader.aac_frame_length);DebugInfoString += ',';
	DebugInfoString += ADTSHandler::UITCS(g_AtdsHeader.adts_buffer_fullness);DebugInfoString += ',';
	DebugInfoString += ADTSHandler::UITCS(g_AtdsHeader.number_of_rawdata_blocks_in_frame);
#endif
	g_FrameCounter++;
	DebugInfoString = ADTSHandler::UITCS(g_FrameCounter);

	ADTSHandler::DebugUpdate();
	ADTSHandler::PrintfHeaderToFile();
}

unsigned int ADTSHandler::SampleIndexToSample(unsigned int index)
{
	unsigned int SamplingSize[100] = {96000,88200,64000,48000,44100,32000,24000,\
									  22050,16000,12000,11025,8000,7350,0,0,0};

	return SamplingSize[index];
}

void ADTSHandler::ParselHeaderNumber(unsigned long long HeaderNumber)
{
	g_AtdsHeader.syncword =								(unsigned int)SYNCWORD_MARK(HeaderNumber);
	g_AtdsHeader.ID =									(unsigned int)ID_MARK(HeaderNumber);
	g_AtdsHeader.layer =								(unsigned int)LAYER_MARK(HeaderNumber);
	g_AtdsHeader.protection_absent =					(unsigned int)PROTECTION_MARK(HeaderNumber);
	g_AtdsHeader.profile =								(unsigned int)PROFILE_MARK(HeaderNumber);
	g_AtdsHeader.sampling_frequency =					ADTSHandler::SampleIndexToSample((unsigned int)SAMPLING_MARK(HeaderNumber));
	g_AtdsHeader.private_bit =							(unsigned int)PRIVATE_MARK(HeaderNumber);
	g_AtdsHeader.channel_config =						(unsigned int)CHANNEL_MARK(HeaderNumber);
	g_AtdsHeader.original_copy =						(unsigned int)ORIGINAL_MARK(HeaderNumber);
	g_AtdsHeader.home =									(unsigned int)HOME_MARK(HeaderNumber);
	g_AtdsHeader.copyright_id_bit =						(unsigned int)COPYID_MARK(HeaderNumber);
	g_AtdsHeader.copyright_id_start =					(unsigned int)COPYSTART_MARK(HeaderNumber);
	g_AtdsHeader.aac_frame_length =						(unsigned int)FRAMELENGTH_MARK(HeaderNumber);
	g_AtdsHeader.adts_buffer_fullness =					(unsigned int)ADTSBUFFERFULLNESS_MARK(HeaderNumber);
	g_AtdsHeader.number_of_rawdata_blocks_in_frame =	(unsigned int)NUMBERBLOCKS_MARK(HeaderNumber);
}

void ADTSHandler::ADTSParselHandler(CString Path)
{
	unsigned long long File_Length = 0,File_CurRP = 0;
	unsigned char buff[100];
	unsigned long long HeaderDataNumber = 0;

	ADTSHandler::Init();

	CFile pADTSFile(Path,CFile::modeRead);
	File_Length = pADTSFile.GetLength();	//Length
	pADTSFile.Seek(File_CurRP,CFile::begin);

	while(File_CurRP < File_Length){
		File_CurRP = ADTSHandler::FindSynvPoint(&pADTSFile,File_Length,File_CurRP);
		if(File_CurRP > File_Length)
			break;
		pADTSFile.Seek(File_CurRP,CFile::begin);

		pADTSFile.Read(buff,8);

		HeaderDataNumber = Format_Trans::GetU64FromAddr(buff);
		//DebugInfoString = ADTSHandler::ULLTXCS(HeaderDataNumber);//printf in Hex Mode
		//ADTSHandler::DebugUpdate();
		ADTSHandler::ParselHeaderNumber(HeaderDataNumber);
		ADTSHandler::PrintfHeader();
		File_CurRP += g_AtdsHeader.aac_frame_length;
	}

	if(g_FrameCounter < 2)
		AfxMessageBox(_T("未找到ADTS同步标志"));

	pADTSFile.Close();
}