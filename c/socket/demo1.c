#include<stdio.h>
#include <sys/queue.h>
#include <time.h>
#include <asm/byteorder.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/filter.h>
#include <linux/if_ether.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netpacket/packet.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <linux/errqueue.h>
#include <linux/net_tstamp.h>
#include <linux/sockios.h>
#include <sys/socket.h>

#define PACKED __attribute__((packed))

typedef	int       Boolean;
typedef uint8_t   Enumeration8;
typedef uint16_t  Enumeration16;
typedef int8_t    Integer8;
typedef uint8_t   UInteger8;
typedef int16_t   Integer16;
typedef uint16_t  UInteger16;
typedef int32_t   Integer32;
typedef uint32_t  UInteger32;
typedef int64_t   Integer64;
typedef uint64_t  UInteger64; /* ITU-T G.8275.2 */
typedef uint8_t   Octet;

/** On the wire time stamp format. */
struct Timestamp {
	uint16_t   seconds_msb; /* 16 bits + */
	uint32_t   seconds_lsb; /* 32 bits = 48 bits*/
	UInteger32 nanoseconds;
} PACKED;

struct ClockIdentity {
	Octet id[8];
};

struct PortIdentity {
	struct ClockIdentity clockIdentity;
	UInteger16           portNumber;
} PACKED;

struct ClockQuality {
	UInteger8     clockClass;
	Enumeration8  clockAccuracy;
	UInteger16    offsetScaledLogVariance;
} PACKED;

struct ptp_header {
	uint8_t             tsmt; /* transportSpecific | messageType */
	uint8_t             ver;  /* minorVersionPTP   | versionPTP  */
	UInteger16          messageLength;
	UInteger8           domainNumber;
	Octet               reserved1;
	Octet               flagField[2];
	Integer64           correction;
	UInteger32          reserved2;
	struct PortIdentity sourcePortIdentity;
	UInteger16          sequenceId;
	UInteger8           control;
	Integer8            logMessageInterval;
} PACKED;

struct announce_msg {
	struct ptp_header    hdr;
	struct Timestamp     originTimestamp;
	Integer16            currentUtcOffset;
	Octet                reserved;
	UInteger8            grandmasterPriority1;
	struct ClockQuality  grandmasterClockQuality;
	UInteger8            grandmasterPriority2;
	struct ClockIdentity grandmasterIdentity;
	UInteger16           stepsRemoved;
	Enumeration8         timeSource;
	uint8_t              suffix[0];
} PACKED;

struct sync_msg {
	struct ptp_header   hdr;
	struct Timestamp    originTimestamp;
} PACKED;

struct delay_req_msg {
	struct ptp_header   hdr;
	struct Timestamp    originTimestamp;
	uint8_t             suffix[0];
} PACKED;

struct follow_up_msg {
	struct ptp_header   hdr;
	struct Timestamp    preciseOriginTimestamp;
	uint8_t             suffix[0];
} PACKED;

struct delay_resp_msg {
	struct ptp_header   hdr;
	struct Timestamp    receiveTimestamp;
	struct PortIdentity requestingPortIdentity;
	uint8_t             suffix[0];
} PACKED;

struct pdelay_req_msg {
	struct ptp_header   hdr;
	struct Timestamp    originTimestamp;
	struct PortIdentity reserved;
} PACKED;

struct pdelay_resp_msg {
	struct ptp_header   hdr;
	struct Timestamp    requestReceiptTimestamp;
	struct PortIdentity requestingPortIdentity;
} PACKED;

struct pdelay_resp_fup_msg {
	struct ptp_header   hdr;
	struct Timestamp    responseOriginTimestamp;
	struct PortIdentity requestingPortIdentity;
	uint8_t             suffix[0];
} PACKED;

struct signaling_msg {
	struct ptp_header   hdr;
	struct PortIdentity targetPortIdentity;
	uint8_t             suffix[0];
} PACKED;

struct management_msg {
	struct ptp_header   hdr;
	struct PortIdentity targetPortIdentity;
	UInteger8           startingBoundaryHops;
	UInteger8           boundaryHops;
	uint8_t             flags; /* reserved | actionField */
	uint8_t             reserved;
	uint8_t             suffix[0];
} PACKED;

struct message_data {
	uint8_t buffer[1500];
} PACKED;

struct ptp_message {
	union {
		struct ptp_header          header;
		struct announce_msg        announce;
		struct sync_msg            sync;
		struct delay_req_msg       delay_req;
		struct follow_up_msg       follow_up;
		struct delay_resp_msg      delay_resp;
		struct pdelay_req_msg      pdelay_req;
		struct pdelay_resp_msg     pdelay_resp;
		struct pdelay_resp_fup_msg pdelay_resp_fup;
		struct signaling_msg       signaling;
		struct management_msg      management;
		struct message_data        data;
	} PACKED;
};

int main(void)
{
	struct ifreq ifreq;
	int err;
	struct sockaddr_ll addr;
	const char *name = "ens3";
	struct eth_hdr *hdr;
	int fd = -1;
	int sock_fd = -1;
	unsigned *ptr = NULL;
	int cnt;
	unsigned char pkt[] = {0x01, 0x1b, 0x19, 0x00, 0x00, 0x00, 0x80, 0x00,
			       0x17, 0x0b, 0x6b, 0x0f, 0x88, 0xf7, 0x00, 0x12,
			       0x00, 0x2c, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
			       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			       0x00, 0x00, 0x08, 0x00, 0x17, 0xff, 0xfe, 0x0b,
			       0x6b, 0x0f, 0x00, 0x01, 0x00, 0x6e, 0x00, 0x00,
			       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			       0x00, 0x00};

	//1. creat fd
	fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (fd < 0) {
		printf("socket failed: %m\n");
		return fd;
	}

	// init ioctl
	memset(&ifreq, 0, sizeof(ifreq));
	strncpy(ifreq.ifr_name, name, sizeof(ifreq.ifr_name) - 1);
	printf("raw_fd: %d\n", fd);
	err = ioctl(fd, SIOCGIFINDEX, &ifreq);
	if (err < 0) {
		printf("ioctl SIOCGIFINDEX failed: %m");
		return err;
	}
	// bind fd to dev
	memset(&addr, 0, sizeof(addr));
	addr.sll_ifindex = ifreq.ifr_ifindex;
	addr.sll_family = AF_PACKET;
	addr.sll_protocol = htons(ETH_P_ALL);
	if (bind(fd, (struct sockaddr *) &addr, sizeof(addr))) {
		printf("bind failed: %m\n");
		return -1;
	}
	cnt = send (fd, &pkt[0], 58, 0);
	if (cnt < 1) {
		printf("send failed\n");
	}

	printf("send success\n");


	return 0;
}
