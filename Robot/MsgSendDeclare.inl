//////////����logo_sever��Ϣ
void SendEnterGame(const char* name,const char* password);          //���͵�¼����
void SendLeaveGame();												//���͵�¼����
void SendSelgroup(const char* buf);                                 //���ͻ�ȡ��Ϸ���б�
void SendDisLogoSever();											//������logo_sever�Ͽ�

//////////����game_sever��Ϣ
void SendHertMsg();                                                //����������Ϣ
void SendGetRole(const char* buf);                                 //�����¼��֤
void SendGetRandCharaName();                                       //�����������ɫ��
void SendCreateChara(const char* charaName);					   //���󴴽���ɫ

//////////���͵��������Ϣ
void SendGetPlayerPackInfo();                    //����������ݣ���������
void SendGetShoppingPropList();                  //�������̳��б�
void SendBuyProp(int id,int num);                //�̳ǹ������
void SendUseProp(int propID,int num,int id);     //ʹ�õ���

//////////��������������Ϣ
void SendGetLadder();                            //��������������ǰ100λ

//////////���ֶһ�������Ϣ
void SendGetExchangePropLisg();								//�����û��ֶһ��̳��б�
// void SendExchangeProp(unsigned int id,unsigned int num);	//�һ���Ϸ�ڵ���
void SendOrderProp(OrderItemReq  *item);					//�һ�ʵ�����

//////////�ؿ�
void SendGetChoseMapList();									//������ѡ�񳡾��б�
void SendGetMapInfo(int id);								//���󳡾�����
void SendChoseBall(int id);									//����ѡ�����С��
void SendComputerCore(int senceID,const char* str);         //������Ϸ���������

//////////��ֵ
void SendGetBuyRollCallList();                    //��ó�ֵ�����б�
