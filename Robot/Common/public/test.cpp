#define NET_PORT 1800
#define IP_ADDR XXX

int main(void)
{
	int dst_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (dst_sockfd) {

	}

	struct sockaddr_in attr = {
		.sin_family = AF_INET;
		.sin_port   = htons(NET_PORT);
		.sin_addr   = {
			s_addr = inet_addr(IP_ADDR),
		}
	};

	if (connect(dst_sockfd, (struct sockaddr *)&attr, sizeof(attr)) < 0) {
		close(dst_sockfd);
	}

	unsigned char buf[20] = {0};	//socket buffer

	write(dst_sockfd, buf, sizeof(buf))

	close(dst_sockfd);

	CMarkupSTL xml;
	xml.SetDoc("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
	xml.AddElem("UserInfo");
	xml.IntoElem();
	xml.AddElem("UserID","luo");
	xml.AddElem("UserID","lin");
	xml.OutOfElem();
	xml.Save("UserInfo.xml");

	return 0;
}