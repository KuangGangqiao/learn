#include<unistd.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/ip.h>
#include<netinet/udp.h>
#include<memory.h>
#include<stdlib.h>
#include<linux/if_ether.h>
#include<linux/if_packet.h> // sockaddr_ll
#include<arpa/inet.h>
#include<netinet/if_ether.h>

#include<errno.h>
#include<netinet/ether.h>
#include<net/if.h>
#include<string.h>

#include <linux/errqueue.h>
#include <linux/net_tstamp.h>
#include <linux/sockios.h>
#include <sys/socket.h>
#include <sys/ioctl.h>


//接收二层包，并把数据输出
int main(int argc, char **argv) {
	int err;
	struct ifreq ifreq;
	int sock, n;
	char buf[1024];
	int buflen = 60;
	uint8_t sendbuffer[1024];
	struct sockaddr_ll addr;
	char control[256];
	struct msghdr msg;
	struct iovec iov = {buf, buflen};
	int cnt;


	//获取所有数据
	if ((sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
	{
		perror("socket");
		exit(1);
	}
	socklen_t addr_length = sizeof(struct sockaddr_ll);

	// init ioctl
	memset(&ifreq, 0, sizeof(ifreq));
	strncpy(ifreq.ifr_name, "ens3", sizeof(ifreq.ifr_name) - 1);
	printf("raw_fd: %d\n", sock);
	err = ioctl(sock, SIOCGIFINDEX, &ifreq);
	if (err < 0) {
		printf("ioctl SIOCGIFINDEX failed: %m");
		return err;
	}
	// bind fd to dev
	memset(&addr, 0, sizeof(addr));
	addr.sll_ifindex = ifreq.ifr_ifindex;
	addr.sll_family = AF_PACKET;
	addr.sll_protocol = htons(ETH_P_ALL);
	if (bind(sock, (struct sockaddr *) &addr, sizeof(addr))) {
		printf("bind failed: %m\n");
		return -1;
	}

	//recv data
	memset(control, 0, sizeof(control));
	memset(&msg, 0, sizeof(msg));
#if 0
	if (addr) {
		msg.msg_name = &addr->ss;
		msg.msg_namelen = sizeof(addr->ss);
	}
#endif
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = control;
	msg.msg_controllen = sizeof(control);


	cnt = recvmsg(sock, &msg, MSG_DONTWAIT);
	printf("cnt: %d\n", cnt);
}
