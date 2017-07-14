#ifndef ADTS_HANDLER
#define ADTS_HANDLER

#include "FormatTransfer.h"

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
#define ADTSBUFFERFULLNESS_MARK(x)	((x & 0x00000000001FFC00)>>10)
#define NUMBERBLOCKS_MARK(x)		((x & 0x0000000000000300)>>8)

typedef struct ADTS_HEADER
{
	unsigned int syncword;//12
	unsigned int ID;//1
	unsigned int layer;//2
	unsigned int protection_absent;//1
	unsigned int profile;//2
	unsigned int sampling_frequency;//4
	unsigned int private_bit;//1
	unsigned int channel_config;//3
	unsigned int original_copy;//1
	unsigned int home;	//1
	/*adts_variable_header 28bits*/
	unsigned int copyright_id_bit;//1
	unsigned int copyright_id_start;//1
	unsigned int aac_frame_length;//13		//a frame length = header+frame data
	unsigned int adts_buffer_fullness;//11
	unsigned int number_of_rawdata_blocks_in_frame;//2
}ADTSHEADER;

class ADTSHandler:public Format_Trans
{
	public:
		void ADTSHandler::Init(void);
		unsigned long long  ADTSHandler::FindSynvPoint(CFile *pFile,unsigned long long FileSize,unsigned long long StartPoint);
		unsigned int ADTSHandler::SampleIndexToSample(unsigned int index);
		void ADTSParselHandler(CString Path);
		void DebugUpdate(void);
		CString DebugInfoString;
		void PrintfHeader(void);
		void PrintfHeaderToFile(void);
		ADTSHEADER g_AtdsHeader;
		void ParselHeaderNumber(unsigned long long HeaderNumber);
		unsigned int g_FrameCounter;
};

#endif
