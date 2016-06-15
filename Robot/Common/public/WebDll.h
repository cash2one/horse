/*****************************************************************************/
/**
*	\file ExportMain.h
*	\brief  
*
*	<pre>
*
*	Copyright (c) 2006,�Ϻ�ʢ�������з�����
*	All rights reserved.
*
*	�ļ����ƣ�ExportMain.h
*	ժ    Ҫ��ͷ�ļ�
*
*	��ǰ�汾��1.0
*	��    �ߣ�xmd
*	�����ʼ���
*   �������ڣ�2009-1-16
*	������ڣ�
*
*	</pre>
*/
/*****************************************************************************/


/*****************************************************************************/
// Pragma
/*****************************************************************************/
#pragma once

/*****************************************************************************/
// Includes
/*****************************************************************************/
#pragma pack(push, 1)


/*****************************************************************************/
// Defines
/*****************************************************************************/
#define WEB_DLL_SYNTHESIZER_VERSION		4

enum SYNTHESIZER_ROOM_STATE
{
	SYNTH_ROOM_IDLE = 0,
	SYNTH_ROOM_SINGING,
	SYNTH_ROOM_COMMENT,

	SYNTH_ROOM_MAX
};


enum SynthAudioType
{
	AudioType_Singing = 0x02,  //�ݳ���Ƶ����
	AudioType_Speaking= 0x20,  //��������Ƶ����
	AudioType_Comment = 0x40,  //������������
};


struct SynthAudioInfo
{
	WORD	wAudioType;
	WORD	wHeadLen;
	WORD	wAudioLen;
	WORD	wMusicLen;
	BYTE*	pHead;
	BYTE*	pAudio;
	BYTE*	pMusic;
};


/*****************************************************************************/
// Declaration
/*****************************************************************************/
class CStreamSynthesizer
{
public:
	CStreamSynthesizer() {}
	~CStreamSynthesizer() {}

	virtual int __stdcall GetClassVersion() = 0;
	virtual void __stdcall WarmInitStream(HWND hwnd, const char* _debugfile_ = NULL) = 0;
	virtual bool __stdcall StartStream() = 0;
	virtual bool __stdcall FinishStream() = 0;
	virtual void __stdcall UpdateStream() = 0;
	virtual unsigned short __stdcall GetAudioStreamFreq() = 0;
	virtual unsigned char __stdcall GetAudioStreamChan() = 0;
	virtual bool __stdcall SetRoomState(char state) = 0;
	virtual void __stdcall ReceiveAudioUDP(unsigned long nSrcPlayerID, const SynthAudioInfo* pAudioDesc) = 0;
	virtual unsigned long __stdcall GetAudioStreamBuffer(void** address) = 0;
};


extern "C" CStreamSynthesizer* __stdcall CreateStreamSynthesizer();
extern "C" bool __stdcall ReleaseStreamSynthesizer(CStreamSynthesizer** synth);

typedef CStreamSynthesizer* (__stdcall *pfn_CreateStreamSynthesizer)();
typedef bool (__stdcall *pfn_ReleaseStreamSynthesizer)(CStreamSynthesizer** synth);

#pragma pack(pop)
/*****************************************************************************/
// ExportMain.h
/*****************************************************************************/
