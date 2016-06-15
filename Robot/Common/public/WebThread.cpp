CWebThread::CWebThread(void)
:m_WebThreadEvent(NULL)
{		
}

CWebThread::~CWebThread(void)
{
}

bool CWebThread::InitWebThread(int QueueSize)
{
	SS_TRY_BEGIN_;

	m_WebThreadEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	if(NULL == m_WebThreadEvent)
	{
		return false;
	}

	m_CheckPackQueue.InitQueue(QueueSize);
	m_CheckFinishPackQueue.InitQueue(QueueSize);

	return Start();
	SS_TRY_END_();
	return false;
}

//初始化执行一次
bool CWebThread::AddTransaction()
{
	SS_TRY_BEGIN_;
socket(AF_INET, SOCK_STREAM, 0)
	CheckPackInfo	msg = {0};		
	//msg.dwSelCharPos = dwSelCharPos;
	//if(serverinfo)
//	{
		memcpy(msg.ThreadMsg,"message",8);
//	}

	int ret = m_CheckPackQueue.PushOne(&msg);
	if( ret == 1 )
	{
		SetEvent(m_WebThreadEvent);
	}
	else if(ret == 0)
	{
		ErrorLog( 3, "m_TransQueue.size = %u",m_CheckPackQueue.GetObjectCount() );
	}
	return ret;
	SS_TRY_END_();

	return false;
}

BOOL CWebThread::OnRun()
{
	SS_TRY_BEGIN_;

	CheckPackInfo * pPack=m_CheckPackQueue.PopOne();
	if( pPack )
	{
		//GetNewCHarName(pPack);
		//todo
		SERVERSTATUS server_status = g_pProcessThread->GetServerStatus();
		switch(server_status)
		{
		//	case eServerFull:											//服务器满员
		//		ReportToWeb(eServerFull);
		//		break;
		//	case eServerNotAvailable:									//服务器不可用
		//		ReportToWeb(eServerNotAvailable);
		//		break;
			case eServerIdle:											//服务器空闲
				ReportToWeb(eServerIdle);
				break;
		//	case eServerModerate:										//服务器适中
		//		ReportToWeb(eServerModerate);
		//		break;
			case eServerCrowded:										//服务器拥挤
				ReportToWeb(eServerCrowded);
				break;
		}
		m_CheckFinishPackQueue.PushOne(pPack);
	}
	else
	{
		WaitForSingleObject( m_WebThreadEvent, INFINITE);
	}

	return TRUE;
	SS_TRY_END_();
	return FALSE;
}

void ReportToWeb(int status)
{
#define NET_PORT 8900
#define IP_ADDR "192.168.3.198"

	int dst_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (dst_sockfd) {
		perror("socket");
		return -1;
	}

	struct sockaddr_in attr = {
		.sin_family = AF_INET;
		.sin_port   = htons(NET_PORT);
		.sin_addr   = {
			.s_addr = inet_addr(IP_ADDR),
		}
	};

	if (connect(dst_sockfd, (struct sockaddr *)&attr, sizeof(attr)) < 0) {
		close(dst_sockfd);
	}

	//拼接字符串
	unsigned char buf[100] = "mobileGameServerStateWeb.17qiqu.com:9090/ids?serverState=";
	//xxx大区,192.168.0.1,8900,1,1;
	if (status == eServerCrowded)
		const unsigned char buf_t[] = "xxx,192.168.0.1,8900,1,1";
	else
		const unsigned char buf_t[] = "xxx,192.168.0.1,8900,0,1";
	strcat(buf, buf_t);

	//发送
	write(dst_sockfd, buf, sizeof(buf));
	memset(buf, 0, sizeof(buf));

	close(dst_sockfd);
}
