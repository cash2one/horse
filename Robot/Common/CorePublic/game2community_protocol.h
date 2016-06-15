#pragma once

#pragma pack(push ,1)

///��Center Serverͨ�ŵ�Э����Ϣͷ�ṹ
typedef struct tagGM2COMProtHeader
{
	WORD	length;			//���ݰ�����(head����+���ݳ���)
	WORD	type;			//��������
	DWORD	reserved;		//�����ֶ�
}GM2COMProtHeader;


//notice֪ͨ����󳤶���2M
#define COMMUTY_NOTICE_MAXSIZE	2*1024*1024


//------------------------------------------------------------------------------------------------
//game server��community server֮�����Ϣ���ݸ�ʽ�������µĸ�ʽ����
//<key> <bytes> <value>\r\n<key> <bytes> <value>\r\n
//��Ϣ���ݰ���һ�����key-value�����
//key:Ԫ��Ϣ������,Ϊ������Ϊ�ո���ַ���
//bytes:Ԫ��Ϣ�ĳ���
//value:Ԫ��Ϣ������
//ʾ����һ��������ߵ�֪ͨ
//charid 8 10789222\r\n
//servertime 17 2009:7:4:18:30:20\r\n
//action 6 online\r\n


//////////////////////////////////////////////////////////////////////////
//һ������£�һ��valueֻ��һ�������ֶΣ����Ҳ���Ǽ򵥵��ַ���
//
//��Щ����£�һ��value�ж������ʱ������һ��������ܰ���������ԣ��緿����������ID���������ͣ�
//#<AttribName>=<AttribValue>\r\n<AttribName>=<AttribValue>\r\n
//
//���Լ�֮ǰ��Ҫ��#�����б�ʶ
//����value ʾ����   "#roomname=���Է���\r\nroomid=100045\r\nroomtype=systemroom"
//
//////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//event����:�����ݳ���¼
//Э������:
//key=charid		value=<int32>		*��ɫID*
//key=digitalid		value=<int64>		*��ɫ����ID*
//key=charname		value=<string>		*��ɫ�ǳ�*
//key=songid		value=<int32>		*����ID*
//key=songname		value=<string>		*������*
//key=totalscore	value=<int32>		*�ܵ÷�*
//key=singscore		value=<int32>		*�ݳ��÷�*
//key=itemscore		value=<int32>		*���߼ӷ�*
//-------------------------------------------------------------------------------------------------
#define GM2COM_EVENT_SINGRECORD				0x6001

//-------------------------------------------------------------------------------------------------
//event����:�������
//Э������:
//key=digitalid		value=<int64>		*��ɫ����ID*
//key=serverid		value=<int32>		*������ID*
//key=servername	value=<string>		*����������*
#define GM2COM_EVENT_USERONLINE				0x6002

//-------------------------------------------------------------------------------------------------
//event����:�������
//Э������:
//key=digitalid		value=<int64>		*��ɫ����ID*
//key=serverid		value=<int32>		*������ID*
//key=servername	value=<string>		*����������*
#define GM2COM_EVENT_USEROFFLINE			0x6003


//-------------------------------------------------------------------------------------------------
//event����:���λ��
//Э������:
//key=digitalid		value=<int64>		*��ɫ����ID*
//key=location		value=<string>		*λ������*	
//������ڴ���ʱ,value=lobby
//������ڷ���ʱ,value=room#name=<room's name>\r\nid=<room's id>\r\nroomtype=<room's type>\r\n
#define GM2COM_EVENT_USERLOCATION			0x6004

//-------------------------------------------------------------------------------------------------
//event����:��ҼӺ���
//Э������:
//key=user1		value=<int32>		*��ҽ�ɫID*
//key=user2		value=<int32>		*��һλ��ҽ�ɫID*	
#define GM2COM_EVENT_MAKEFRIEND				0x6005

//-------------------------------------------------------------------------------------------------
//event����:���֮��ʹ�õ��߻���
//Э������:
//key=srcid			value=<int64>		*�������ID*
//key=targetid		value=<int64>		*Ŀ����ҽ�ɫID*	
//key=itemtype		value=<int32>		*��������  1->��Ϸ�ҵ��� 2->����ҵ���*
//key=itemname		value=<string>		*��������*
//key=itemprice		value=<int32>		*���߼�ֵ*
#define GM2COM_EVENT_USEITEM				0x6006

#pragma  pack(pop)

