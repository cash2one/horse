/**
*	SuperStar 服务器的版本信息
*/

#define _Stringizing(v)    #v
#define SS_VerJoin(a, b, c, d)  _Stringizing(a.b.c.d)
#define SS_STR_BuildDate		TEXT(__DATE__)
#define SS_STR_BuildTime		TEXT(__TIME__)
#define SS_STR_BuilDateTime	TEXT(__DATE__) TEXT(" ") TEXT(__TIME__)

//-----------------game server version-----------------
#define GAMESERVER_File_Major		1
#define GAMESERVER_File_Minor		0
#define GAMESERVER_File_Release	0
#define GAMESERVER_File_Build		7

#define GAMESERVER_File_Version			TEXT(SS_VerJoin(GAMESERVER_File_Major, \
																		GAMESERVER_File_Minor, \
																		GAMESERVER_File_Release, \
																		GAMESERVER_File_Build))

#define GAMESERVER_Product_Major		1
#define GAMESERVER_Product_Minor		0
#define GAMESERVER_Product_Release	0
#define GAMESERVER_Product_Revision	7

#define GAMESERVER_Product_Version			TEXT(SS_VerJoin(GAMESERVER_Product_Major, \
																		GAMESERVER_Product_Minor, \
																		GAMESERVER_Product_Release, \
																		GAMESERVER_Product_Revision))

//-----------------character server version-----------------
#define CHARASERVER_File_Major		1
#define CHARASERVER_File_Minor		0
#define CHARASERVER_File_Release	0
#define CHARASERVER_File_Build		5

#define CHARASERVER_File_Version			TEXT(SS_VerJoin(CHARASERVER_File_Major, \
																		CHARASERVER_File_Minor, \
																		CHARASERVER_File_Release, \
																		CHARASERVER_File_Build))

#define CHARASERVER_Product_Major		1
#define CHARASERVER_Product_Minor		0
#define CHARASERVER_Product_Release	0
#define CHARASERVER_Product_Revision	5

#define CHARASERVER_Product_Version			TEXT(SS_VerJoin(CHARASERVER_Product_Major, \
																		CHARASERVER_Product_Minor, \
																		CHARASERVER_Product_Release, \
																		CHARASERVER_Product_Revision))

//-----------------audio server version-----------------
#define AUDIOSERVER_File_Major		1
#define AUDIOSERVER_File_Minor		0
#define AUDIOSERVER_File_Release	0
#define AUDIOSERVER_File_Build		7

#define AUDIOSERVER_File_Version			TEXT(SS_VerJoin(AUDIOSERVER_File_Major, \
																		AUDIOSERVER_File_Minor, \
																		AUDIOSERVER_File_Release, \
																		AUDIOSERVER_File_Build))

#define AUDIOSERVER_Product_Major		1
#define AUDIOSERVER_Product_Minor		0
#define AUDIOSERVER_Product_Release		0
#define AUDIOSERVER_Product_Revision	7

#define AUDIOSERVER_Product_Version			TEXT(SS_VerJoin(AUDIOSERVER_Product_Major, \
																		AUDIOSERVER_Product_Minor, \
																		AUDIOSERVER_Product_Release, \
																		AUDIOSERVER_Product_Revision))
//-----------------broadcast server version-----------------
#define BROADCASTSERVER_File_Major		1
#define BROADCASTSERVER_File_Minor		0
#define BROADCASTSERVER_File_Release	0
#define BROADCASTSERVER_File_Build		5

#define BROADCASTSERVER_File_Version			TEXT(SS_VerJoin(BROADCASTSERVER_File_Major, \
																		BROADCASTSERVER_File_Minor, \
																		BROADCASTSERVER_File_Release, \
																		BROADCASTSERVER_File_Build))

#define BROADCASTSERVER_Product_Major		1
#define BROADCASTSERVER_Product_Minor		0
#define BROADCASTSERVER_Product_Release		0
#define BROADCASTSERVER_Product_Revision	5

#define BROADCASTSERVER_Product_Version			TEXT(SS_VerJoin(BROADCASTSERVER_Product_Major, \
																		BROADCASTSERVER_Product_Minor, \
																		BROADCASTSERVER_Product_Release, \
																		BROADCASTSERVER_Product_Revision))
//-----------------shop server version-----------------
#define SHOPSERVER_File_Major		1
#define SHOPSERVER_File_Minor		0
#define SHOPSERVER_File_Release		0
#define SHOPSERVER_File_Build		5

#define SHOPSERVER_File_Version			TEXT(SS_VerJoin(SHOPSERVER_File_Major, \
																		SHOPSERVER_File_Minor, \
																		SHOPSERVER_File_Release, \
																		SHOPSERVER_File_Build))

#define SHOPSERVER_Product_Major		1
#define SHOPSERVER_Product_Minor		0
#define SHOPSERVER_Product_Release		0
#define SHOPSERVER_Product_Revision		5

#define SHOPSERVER_Product_Version			TEXT(SS_VerJoin(SHOPSERVER_Product_Major, \
																		SHOPSERVER_Product_Minor, \
																		SHOPSERVER_Product_Release, \
																		SHOPSERVER_Product_Revision))
//-----------------interface server version-----------------
#define INTERFACESERVER_File_Major		1
#define INTERFACESERVER_File_Minor		0
#define INTERFACESERVER_File_Release	0
#define INTERFACESERVER_File_Build		5

#define INTERFACESERVER_File_Version			TEXT(SS_VerJoin(INTERFACESERVER_File_Major, \
																		INTERFACESERVER_File_Minor, \
																		INTERFACESERVER_File_Release, \
																		INTERFACESERVER_File_Build))

#define INTERFACESERVER_Product_Major		1
#define INTERFACESERVER_Product_Minor		0
#define INTERFACESERVER_Product_Release		0
#define INTERFACESERVER_Product_Revision	5

#define INTERFACESERVER_Product_Version			TEXT(SS_VerJoin(INTERFACESERVER_Product_Major, \
																		INTERFACESERVER_Product_Minor, \
																		INTERFACESERVER_Product_Release, \
																		INTERFACESERVER_Product_Revision))
//-----------------superstarws version-----------------
#define SUPERSTARWS_File_Major		1
#define SUPERSTARWS_File_Minor		0
#define SUPERSTARWS_File_Release	0
#define SUPERSTARWS_File_Build		5

#define SUPERSTARWS_File_Version			TEXT(SS_VerJoin(SUPERSTARWS_File_Major, \
																		SUPERSTARWS_File_Minor, \
																		SUPERSTARWS_File_Release, \
																		SUPERSTARWS_File_Build))

#define SUPERSTARWS_Product_Major		1
#define SUPERSTARWS_Product_Minor		0
#define SUPERSTARWS_Product_Release		0
#define SUPERSTARWS_Product_Revision	5

#define SUPERSTARWS_Product_Version			TEXT(SS_VerJoin(SUPERSTARWS_Product_Major, \
																		SUPERSTARWS_Product_Minor, \
																		SUPERSTARWS_Product_Release, \
																		SUPERSTARWS_Product_Revision))

//-----------------FriendClub server version-----------------
#define FRIENDCLUBSERVER_File_Major		1
#define FRIENDCLUBSERVER_File_Minor		0
#define FRIENDCLUBSERVER_File_Release	0
#define FRIENDCLUBSERVER_File_Build		3

#define FRIENDCLUBSERVER_File_Version			TEXT(SS_VerJoin(FRIENDCLUBSERVER_File_Major, \
																		FRIENDCLUBSERVER_File_Minor, \
																		FRIENDCLUBSERVER_File_Release, \
																		FRIENDCLUBSERVER_File_Build))

#define FRIENDCLUBSERVER_Product_Major		1
#define FRIENDCLUBSERVER_Product_Minor		0
#define FRIENDCLUBSERVER_Product_Release	0
#define FRIENDCLUBSERVER_Product_Revision	3

#define FRIENDCLUBSERVER_Product_Version			TEXT(SS_VerJoin(FRIENDCLUBSERVER_Product_Major, \
																		FRIENDCLUBSERVER_Product_Minor, \
																		FRIENDCLUBSERVER_Product_Release, \
																		FRIENDCLUBSERVER_Product_Revision))

//-----------------statistic server version-----------------
#define STATISTICSERVER_File_Major		1
#define STATISTICSERVER_File_Minor		0
#define STATISTICSERVER_File_Release	0
#define STATISTICSERVER_File_Build		5

#define STATISTICSERVER_File_Version			TEXT(SS_VerJoin(STATISTICSERVER_File_Major, \
	STATISTICSERVER_File_Minor, \
	STATISTICSERVER_File_Release, \
	STATISTICSERVER_File_Build))

#define STATISTICSERVER_Product_Major		1
#define STATISTICSERVER_Product_Minor		0
#define STATISTICSERVER_Product_Release	0
#define STATISTICSERVER_Product_Revision	5

#define STATISTICSERVER_Product_Version			TEXT(SS_VerJoin(STATISTICSERVER_Product_Major, \
	STATISTICSERVER_Product_Minor, \
	STATISTICSERVER_Product_Release, \
	STATISTICSERVER_Product_Revision))

//-----------------web interface version-----------------
#define WEBINTERFACE_File_Major		1
#define WEBINTERFACE_File_Minor		0
#define WEBINTERFACE_File_Release	0
#define WEBINTERFACE_File_Build		1

#define WEBINTERFACE_File_Version			TEXT(SS_VerJoin(WEBINTERFACE_File_Major, \
																		WEBINTERFACE_File_Minor, \
																		WEBINTERFACE_File_Release, \
																		WEBINTERFACE_File_Build))

#define WEBINTERFACE_Product_Major		1
#define WEBINTERFACE_Product_Minor		0
#define WEBINTERFACE_Product_Release	0
#define WEBINTERFACE_Product_Revision	1

#define WEBINTERFACE_Product_Version			TEXT(SS_VerJoin(WEBINTERFACE_Product_Major, \
																		WEBINTERFACE_Product_Minor, \
																		WEBINTERFACE_Product_Release, \
																		WEBINTERFACE_Product_Revision))

//-----------------community server version-----------------
#define COMMUNITYSERVER_File_Major		1
#define COMMUNITYSERVER_File_Minor		0
#define COMMUNITYSERVER_File_Release	0
#define COMMUNITYSERVER_File_Build		1

#define COMMUNITYSERVER_File_Version			TEXT(SS_VerJoin(COMMUNITYSERVER_File_Major, \
	COMMUNITYSERVER_File_Minor, \
	COMMUNITYSERVER_File_Release, \
	COMMUNITYSERVER_File_Build))

#define COMMUNITYSERVER_Product_Major		1
#define COMMUNITYSERVER_Product_Minor		0
#define COMMUNITYSERVER_Product_Release	0
#define COMMUNITYSERVER_Product_Revision	1

#define COMMUNITYSERVER_Product_Version			TEXT(SS_VerJoin(COMMUNITYSERVER_Product_Major, \
	COMMUNITYSERVER_Product_Minor, \
	COMMUNITYSERVER_Product_Release, \
	COMMUNITYSERVER_Product_Revision))
