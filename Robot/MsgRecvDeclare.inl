//������Ϣ�ĺ�
RECV_PRIVATE
	//LOGO_SEVER��Ϣ
	RECV_MSG(MS_S_ACK_DISCONNECT          ,MSG_Disconnect)          /*������Ϣ��������Ͽ�������*/
	RECV_MSG(LOGIN_S_ACK_AUTH             ,MSG_EnterGame)           /*���յ�¼�Ƿ�ɹ�����ͷ���wRetCodeΪ0�ǳɹ���1����ʧ��*/
	RECV_MSG(LOGIN_S_ACK_SELGROUP         ,MSG_GroupList)           /*���շ�������IP,�˿ںţ�linkID*/
	//GAME_GS��Ϣ
	RECV_MSG(GS_S_NOT_HEART               ,MSG_ResiveHert)          //����������Ϣ
	RECV_MSG(GS_CHARA_S_NOT_CHARALIST     ,MSG_GetRoleList)         /*���ս�ɫ�б�*/
// 	RECV_MSG(GS_CHARA_S_ACK_CHARNAME	  ,MSG_GetRandCharaName)     /*���������ɫ��*/
	RECV_MSG(GS_CHARA_S_ACK_CREATE        ,MSG_CreateCharaCallBack)  /*������ɫ���سɹ���Ϣ*/
	RECV_MSG(GS_S_ACK_AUTH                ,MSG_GetCharaAllInfo)      //��������������Ϣ
	//�������
	RECV_MSG(GS_S_PLAYER_ITEM_ACK         ,MSG_GetPlayerPackInfo)	 //�������ﱳ����Ϣ  ֱ�ӱ����ڰ���������Ҫ���½���
	RECV_MSG(GS_S_PLAYER_ITEM_IND         ,MSG_AddAndSetProp)        //�������߸���
	RECV_MSG(GS_S_USE_ITEM_ACK            ,MSG_UseProp)              //ʹ�õ���
	//�̳�
	RECV_MSG(GS_S_SHOP_ITEM_ACK           ,MSG_GetShoppingPropList)  //�����̳ǵ����б�
	RECV_MSG(GS_S_SHOP_BUY_ITEM_ACK       ,MSG_BuyProp)              //�������
	//����������Ϣ
	RECV_MSG(GS_S_TOP500_INFO_ACK         ,MSG_GetLadderInfo)        //��ȡ���а���Ϣ
	//����
	RECV_MSG(GS_S_BOARDNOTICE_IND         ,MSG_GetGongGaoInfo)       //��פϵͳ����
	RECV_MSG(GS_S_PLAYER_BOARDNOTICE_IND  ,MSG_GetPlayerGongGaoInfo) //��ҹ�����Ϣ
	//���ֶһ�
	RECV_MSG(GS_S_EXCHANGE_ITEM_ACK       ,MSG_GetExchangePropList)   //��û����̳��б�
// 	RECV_MSG(GS_S_EXCHANGE_ACK            ,MSG_ExchangeProp)          //�һ���Ϸ�ڵ���
	RECV_MSG(GS_S_ORDER_ACK               ,MSG_OrderProp)             //�һ�ʵ�����
	//BUFF���
	RECV_MSG(GS_S_BUFF_ACQUIRE_IND        ,MSG_AddBuff)                //���һ��BUFF
	RECV_MSG(GS_S_BUFF_LOST_IND           ,MSG_LoseBuff)               //ʧȥһ��buff
	//�ؿ����
	RECV_MSG(GS_S_STAGE_LIST_ACK          ,MSG_GetChoseMapList)        //���ѡ��ؿ��б�
	RECV_MSG(GS_S_STAGE_START_ACK         ,MSG_GetMapInfo)             //��ó�������
	RECV_MSG(GS_S_STAGE_BALLSEL_ACK       ,MSG_SendBallReback)         //����С�򷵻�
	RECV_MSG(GS_S_STAGE_SCORE_ACK         ,MSG_ComputerCore)           //�����Ϸ������
	//��ֵ��� 
	RECV_MSG(GS_S_COST_LIST_ACK           ,MSG_GetBuyRollCallList)      //��ó�ֵ�����б�