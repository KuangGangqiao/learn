1.包含头文件

    #include <sys/types.h>
    #include <sys/socket.h>
    #include <sys/un.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <string.h>
    #include <time.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <net/if.h>

2.设置src和dest,同时定义一个全局的socket，我们以单列模式为列子

    #define DEST_IP "192.168.1.53"
    #define DEST_PORT 996
    #define SRC_IP "192.168.1.56"
    #define SRC_PORT 998
    #define TEST_MESSAGE "This test udp message!"
    #define NETWORK_CAP "ens33"
    unsigned int m_OutSocket;
    struct sockaddr_in m_SocketOut;

DEST：目的IP，要发送到哪个ip上

SRC：本机IP，也就是本机ip，这个决定了选择哪个ip发送包，哪个网卡绑定了这个ip就使用哪个网卡
3.设置初始化函数

    void init_udp_socket(){
    }

3.1 首先初始化socket

m_OutSocket = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);

AF_INET:ipv4的通讯协议

sock_raw:接收tcp udp icmp sctp协议，同时不收本机发出去的数据包的回应，其次是不会收不属于本IP的包

ipproto_udp:udp协议
3.2 设置socket属性为自己构造hand，告诉socket不要给我们构造hand头

    bool flag = true;
    setsockopt(m_OutSocket, IPPROTO_IP, IP_HDRINCL, (char*)&flag, sizeof(flag));

IPPROTO_IP:ipv4协议

IP_HDRINCL:用户需要自己构造协议包头，这里用户可以自己指定选择使用哪个IP，若不使用这个协议，socket会自动选择一个ip使用
3.3 设置send延迟时间

需要注意linux下的send时间参数是一个结构体

    struct timeval nTimeOver={10,0};
    setsockopt(m_OutSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&nTimeOver,sizeof(struct timeval));

3.4 指定当前socket的dest ip与dest port

其实可以不指定，但是不指定不行，因为我们需要自己构造hand，如果这一步不指定则无法构造dest ip和prot这是socket发送包的协议规范

我们使用了SOCK_RAW协议，所以不会收到反馈的包，这个只是为了符合socket的规范

    m_SocketOut.sin_family = AF_INET;
    m_SocketOut.sin_addr.s_addr = inet_addr(DEST_IP);
    m_SocketOut.sin_port = (u_short)DEST_PORT;

3.5 绑定网卡

     struct ifreq ifr;
     memset(&ifr, 0, sizeof(ifr));
     strncpy(ifr.ifr_name, NETWORK_CAP, strlen(NETWORK_CAP));
     setsockopt(m_OutSocket, SOL_SOCKET, SO_BINDTODEVICE, (char *)&ifr, sizeof(ifr));

到这一步初始化函数就写完了

接下来我们写发送函数

发送函数比较复杂，因为我们要构造udp的hand包

4. 编写发送函数

函数原型

    void send(const char* buff,int length){
        if( buff == NULL){
            return;
        }
    }

在开始之前我们在外面定义IP，UDP的hand包：

IP：

    typedef struct stuIPHEADER
    {
            unsigned char h_lenver;			// 8bit 4bit版本 + 4bit首部长度 (h_lenver &0xf) * 4
            unsigned char tos;				// 8bit 服务类型
            unsigned short total_len;		// 16bit 总长度(字节数)
            unsigned short ident;			// 16bit 标识
            unsigned short frag_and_flags;	// 16bit 3bit标志 + 13bit片偏移
            unsigned char ttl;				// 8bit 生存时间(TTL)
            unsigned char proto;			// 8bit 上层协议
            unsigned short checksum;		// 16bit 检验和
            unsigned int sourceIP;			// 32bit 源IP地址
            unsigned int destIP;			// 32bit 目的IP地址
    }IPHEADER, *LPIPHEADER;

UDP：

    typedef struct _UDP_HEADER {
        unsigned short    nSourPort ;            // 源端口号
        unsigned short    nDestPort ;            // 目的端口号
        unsigned short    nLength ;				 // 数据包长度
        unsigned short    nCheckSum ;            // 校验和
    } UDP_HEADER, *PUDP_HEADER ;

其次还有PSD的：

    typedef struct _PSD_HEADER{
    	unsigned long         saddr;	//源IP地址
    	unsigned long         daddr;	//目的IP地址
    	char                  mbz;		//置空(0)
    	char                  ptcl;		//协议类型
    	unsigned short        plen;     //TCP/UDP数据包的长度(即从TCP/UDP报头算起到数据包结束的长度 单位:字节)
    } UDP_PSDHEADER,*PUDP_PSDHEADER ;

然后在定义一个发送体

    #define MAX_UDP_DATA_LEN 65536
    char SendBuf[MAX_UDP_DATA_LEN];

然后构造在函数体里定义出来，并构造它

    IPHEADER ipHeader;
    memset(&ipHeader,0,sizeof(IPHEADER));

构造代码：

    void ConstructIPHeader(IPHEADER *pIpHeader, int dataLength)
    {
     
    	pIpHeader->h_lenver			= 0x45; 	//ip v4
    	pIpHeader->tos				= 0;
    	pIpHeader->total_len			= htons(dataLength);
    	pIpHeader->ident            		= htons(rand());
    	pIpHeader->frag_and_flags		= 0;
    	pIpHeader->ttl				= 128;
    	pIpHeader->proto			= IPPROTO_UDP;
    	pIpHeader->checksum			= 0;
    	pIpHeader->sourceIP        		= inet_addr(SRC_IP);
    	pIpHeader->destIP			= inet_addr(DEST_IP);
    }

构造UDP包：

    UDP_HEADER udpHeader;
    memset(&udpHeader,0,sizeof(UDP_HEADER));

构造代码：

    void ConstructUdpHeader(UDP_HEADER *pUdpHeader, int dataLength)
    {
        pUdpHeader->nSourPort	= htons((unsigned short)(SRC_PORT));
        pUdpHeader->nDestPort	= htons((unsigned short)(DEST_PORT));
        pUdpHeader->nLength	= htons(sizeof(UDP_HEADER) + dataLength);
        pUdpHeader->nCheckSum	= 0;
    }

初始化发送体，并把hand、buff copy进去

     memset(SendBuf, 0, MAX_UDP_DATA_LEN);
     memcpy(SendBuf, &ipHeader, sizeof(IPHEADER));
     memcpy(SendBuf+sizeof(IPHEADER), &udpHeader, sizeof(UDP_HEADER));
     memcpy(SendBuf+sizeof(IPHEADER)+sizeof(UDP_HEADER), buff, length);	

最后构造报文

这段报文代码非本人原创，是由socket编程开源里构造udp报文代码里直接取得。

构造报文代码：

    unsigned short CalculateChecksum(char *buffer1, int len1, char *buffer2, int len2)
    {
    	unsigned long checksum=0;
    	unsigned short* buffer;
    	int i=0;
    	buffer = (unsigned short*) buffer1;
    	for (i=0; i<int(len1/sizeof(unsigned short)); i++)
    		checksum += buffer[i];
    	buffer = (unsigned short*) buffer2;
    	for (i=0; i<int(len2/sizeof(unsigned short)); i++)
    		checksum += buffer[i];
    	if ((len2 & 0x1) != 0) 
            checksum += (unsigned char) buffer2[len2-1];
    	checksum = (checksum >> 16) + (checksum & 0xffff);
    	checksum += (checksum >>16); 
    	return (unsigned short)(~checksum); 
    }

然后在基于报文函数写IP报文与UDP报文

构造IP报文函数：

    void FinalIPHeader(char *pIpAndDataBuffer, int length)
    {
    	IPHEADER* pIpHeader = (IPHEADER*) pIpAndDataBuffer;
    	char* pDataBuffer = pIpAndDataBuffer + sizeof(IPHEADER);
    	int dataLen = length - sizeof(IPHEADER);
    	pIpHeader->checksum = CalculateChecksum(pIpAndDataBuffer, sizeof(IPHEADER),         pDataBuffer, dataLen);
    	pIpHeader->checksum = CalculateChecksum(pIpAndDataBuffer, sizeof(IPHEADER), pDataBuffer, 0);
    }

构造UDP报文函数：

    void CRawSocket::FinalUdpHeader(char *pUdpAndDataBuffer, int length)
    {
    	UDP_PSDHEADER UDP_PSD_HEADER;
    	memset(&UDP_PSD_HEADER,0,sizeof(UDP_PSDHEADER));
    	UDP_HEADER* pUdpHeader = (UDP_HEADER*) pUdpAndDataBuffer;
    	char* pDataBuffer = pUdpAndDataBuffer + sizeof(UDP_HEADER);
    	int dataLen = length - sizeof(UDP_HEADER);

    	UDP_PSD_HEADER.saddr = inet_addr(SRC_IP);
    	UDP_PSD_HEADER.daddr = inet_addr(DEST_IP);
    	UDP_PSD_HEADER.mbz   = 0; 
    	UDP_PSD_HEADER.ptcl  = IPPROTO_UDP; 
    	UDP_PSD_HEADER.plen  = ::htons(length); 

    	pUdpHeader->nCheckSum = CalculateChecksum((char*) &UDP_PSD_HEADER, 
        sizeof(UDP_PSD_HEADER), pUdpAndDataBuffer, length);
    }

然后我们构造一下：

        FinalIPHeader(SendBuf,length+sizeof(IPHEADER)+sizeof(UDP_HEADER));
    	FinalUdpHeader(SendBuf+sizeof(IPHEADER),length+sizeof(UDP_HEADER));

然后我们就可以调用send_to来为我们发送，因为send_to不会构造hand包，所以这里一定要用send_to

sendto(m_OutSocket,SendBuf,length+sizeof(IPHEADER)+sizeof(UDP_HEADER), 0,(struct sockaddr*) &m_SocketOut,
原文链接：https://blog.csdn.net/bjbz_cxy/article/details/115667776
