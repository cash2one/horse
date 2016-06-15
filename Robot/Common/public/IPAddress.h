#pragma once

class CIPAddress
{
protected:
			
	union IP_ADDRESS
	{
		sockaddr_in	SocketAddress;
		struct 
		{
			ULONG64 Address;
			ULONG64 Reserve;
		};		
		
	};
	
	IP_ADDRESS m_IPAddress;
public:
	CIPAddress(void)
	{
		m_IPAddress.Address=0;
		m_IPAddress.Reserve=0;
		m_IPAddress.SocketAddress.sin_family=PF_INET;
	}
	CIPAddress(const CIPAddress& IPAddress)
	{
		m_IPAddress=IPAddress.m_IPAddress;
	}
	CIPAddress(const sockaddr_in& SocketAddress)
	{
		m_IPAddress.SocketAddress=SocketAddress;
		m_IPAddress.SocketAddress.sin_family=PF_INET;
	}
	CIPAddress(ULONG64 Address)
	{
		m_IPAddress.Address=Address;
		m_IPAddress.Reserve=0;
		m_IPAddress.SocketAddress.sin_family=PF_INET;
	}
	CIPAddress(DWORD IP,WORD Port)
	{
		m_IPAddress.SocketAddress.sin_addr.S_un.S_addr=IP;
		m_IPAddress.SocketAddress.sin_port=htons(Port);
		m_IPAddress.Reserve=0;
		m_IPAddress.SocketAddress.sin_family=PF_INET;
	}
	CIPAddress(LPCTSTR szIP,WORD Port)
	{
		LPHOSTENT pHost = gethostbyname(szIP);
		if(pHost)
		{
			m_IPAddress.SocketAddress.sin_addr.S_un.S_addr=*(ULONG *)pHost->h_addr_list[0];
		}			
		m_IPAddress.SocketAddress.sin_port= htons(Port);
		m_IPAddress.Reserve=0;
		m_IPAddress.SocketAddress.sin_family=PF_INET;
	}
	~CIPAddress(void)
	{
	}

	CIPAddress& operator=(const CIPAddress& IPAddress)
	{
		m_IPAddress=IPAddress.m_IPAddress;
		return *this;
	}

	bool operator==(const CIPAddress& IPAddress)
	{
		return m_IPAddress.Address==IPAddress.m_IPAddress.Address;
	}

	bool operator!=(const CIPAddress& IPAddress)
	{
		return m_IPAddress.Address!=IPAddress.m_IPAddress.Address;
	}

	operator sockaddr_in()
	{
		return m_IPAddress.SocketAddress;
	}

	sockaddr_in& GetSockAddr()
	{
		return m_IPAddress.SocketAddress;
	}

	operator ULONG64()
	{
		return m_IPAddress.Address;
	}

	DWORD GetIP() const
	{
		return m_IPAddress.SocketAddress.sin_addr.S_un.S_addr;
	}

	LPCTSTR GetIPString() const
	{//claus
		LPCTSTR ip = inet_ntoa(m_IPAddress.SocketAddress.sin_addr);
		return inet_ntoa(m_IPAddress.SocketAddress.sin_addr);
	}

	WORD GetPort() const
	{
		//claus
		WORD port = ntohs(m_IPAddress.SocketAddress.sin_port);
		return ntohs(m_IPAddress.SocketAddress.sin_port);
	}

	void SetIP(DWORD IP)
	{
		m_IPAddress.SocketAddress.sin_addr.S_un.S_addr=IP;
	}

	void SetIP(LPCTSTR IP)
	{
		LPHOSTENT pHost = gethostbyname(IP);
		if(pHost)
		{
			m_IPAddress.SocketAddress.sin_addr.S_un.S_addr=*(ULONG *)pHost->h_addr_list[0];
		}		
	}

	void SetPort(WORD Port)
	{
		m_IPAddress.SocketAddress.sin_port=htons(Port);
	}

};

class CIPAddressPair
{
protected:
	CIPAddress	m_LocalIPAddress;
	CIPAddress	m_RemoteIPAddress;
public:
	CIPAddressPair()
	{
	}
	CIPAddressPair(const CIPAddressPair& IPAddressPair)
	{
		m_LocalIPAddress=IPAddressPair.m_LocalIPAddress;
		m_RemoteIPAddress=IPAddressPair.m_RemoteIPAddress;
	}
	CIPAddressPair(const CIPAddress& LocalIPAddress,const CIPAddress& RemoteIPAddress)
	{
		m_LocalIPAddress=LocalIPAddress;
		m_RemoteIPAddress=RemoteIPAddress;
	}
	~CIPAddressPair()
	{
	}
	CIPAddressPair& operator=(const CIPAddressPair& IPAddressPair)
	{
		m_LocalIPAddress=IPAddressPair.m_LocalIPAddress;
		m_RemoteIPAddress=IPAddressPair.m_RemoteIPAddress;
	}
	bool operator==(const CIPAddressPair& IPAddressPair)
	{
		return m_LocalIPAddress==IPAddressPair.m_LocalIPAddress&&
			m_RemoteIPAddress==IPAddressPair.m_RemoteIPAddress;
	}
	bool operator!=(const CIPAddressPair& IPAddressPair)
	{
		return m_LocalIPAddress!=IPAddressPair.m_LocalIPAddress||
			m_RemoteIPAddress!=IPAddressPair.m_RemoteIPAddress;
	}
	void SetLocalIPAddress(const CIPAddress& LocalIPAddress)
	{
		m_LocalIPAddress=LocalIPAddress;
	}
	void SetRemoteIPAddress(const CIPAddress& RemoteIPAddress)
	{
		m_RemoteIPAddress=RemoteIPAddress;
	}
	CIPAddress& GetLocalIPAddress()
	{
		return m_LocalIPAddress;
	}
	CIPAddress& GetRemoteIPAddress()
	{
		return m_RemoteIPAddress;
	}
};
