/****************************************************************************
 *  (c) Copyright 2007 Wi-Fi Alliance.  All Rights Reserved
 *
 *
 *  LICENSE
 *
 *  License is granted only to Wi-Fi Alliance members and designated
 *  contractors ($B!H(BAuthorized Licensees$B!I(B)..AN  Authorized Licensees are granted
 *  the non-exclusive, worldwide, limited right to use, copy, import, export
 *  and distribute this software:
 *  (i) solely for noncommercial applications and solely for testing Wi-Fi
 *  equipment; and
 *  (ii) solely for the purpose of embedding the software into Authorized
 *  Licensee$B!G(Bs proprietary equipment and software products for distribution to
 *  its customers under a license with at least the same restrictions as
 *  contained in this License, including, without limitation, the disclaimer of
 *  warranty and limitation of liability, below..AN  The distribution rights
 *  granted in clause
 *  (ii), above, include distribution to third party companies who will
 *  redistribute the Authorized Licensee$B!G(Bs product to their customers with or
 *  without such third party$B!G(Bs private label. Other than expressly granted
 *  herein, this License is not transferable or sublicensable, and it does not
 *  extend to and may not be used with non-Wi-Fi applications..AN  Wi-Fi Alliance
 *  reserves all rights not expressly granted herein..AN
 *.AN
 *  Except as specifically set forth above, commercial derivative works of
 *  this software or applications that use the Wi-Fi scripts generated by this
 *  software are NOT AUTHORIZED without specific prior written permission from
 *  Wi-Fi Alliance.
 *.AN
 *  Non-Commercial derivative works of this software for internal use are
 *  authorized and are limited by the same restrictions; provided, however,
 *  that the Authorized Licensee shall provide Wi-Fi Alliance with a copy of
 *  such derivative works under a perpetual, payment-free license to use,
 *  modify, and distribute such derivative works for purposes of testing Wi-Fi
 *  equipment.
 *.AN
 *  Neither the name of the author nor "Wi-Fi Alliance" may be used to endorse
 *  or promote products that are derived from or that use this software without
 *  specific prior written permission from Wi-Fi Alliance.
 *
 *  THIS SOFTWARE IS PROVIDED BY WI-FI ALLIANCE "AS IS" AND ANY EXPRESS OR
 *  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 *  OF MERCHANTABILITY, NON-INFRINGEMENT AND FITNESS FOR A.AN PARTICULAR PURPOSE,
 *  ARE DISCLAIMED. IN NO EVENT SHALL WI-FI ALLIANCE BE LIABLE FOR ANY DIRECT,
 *  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, THE COST OF PROCUREMENT OF SUBSTITUTE
 *  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 *  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE) ARISING IN ANY WAY OUT OF
 *  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. ******************************************************************************
 */


#ifndef _WFA_SOCK_H
#define _WFA_SOCK_H


#ifndef WIN32
#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <sys/time.h>   /* for struct timeval {} */
#include <fcntl.h>      /* for fcntl() */
#include <errno.h>
#include <typedefs.h>
#else
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>      /* for printf() and fprintf() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */

typedef int socklen_t;
#ifndef MSG_DONTWAIT
#define MSG_DONTWAIT 0
#endif
#endif
#include <wlioctl.h>
#define MAX_UDP_LEN     2048

#ifdef WIN32
typedef SOCKET sockfd_t;
#else
typedef int sockfd_t;
#endif
struct sockfds {
	sockfd_t *	agtfd;          /* dut agent main socket fd */
	sockfd_t *	cafd;           /* sock fd to control agent */
	sockfd_t *	tgfd;           /* traffic agent fd         */
	sockfd_t *	tgRevfd;        /*traffic Recv Fd */
	sockfd_t *	wmmfds;         /* wmm stream ids           */
	sockfd_t *	psfd;           /* wmm-ps socket id         */
};

#ifdef WIN32

#ifndef IFNAMSIZ
#define IFNAMSIZ        16
#endif


struct ifmap {
	unsigned int	mem_start;
	unsigned int	mem_end;
	unsigned short	base_addr;
	unsigned char	irq;
	unsigned char	dma;
	unsigned char	port;
};

struct ifreq {
	char ifr_name[IFNAMSIZ];/* Interface name */
	union {
		SOCKADDR_IN	ifr_addr;
		SOCKADDR_IN	ifr_dstaddr;
		SOCKADDR_IN	ifr_broadaddr;
		SOCKADDR_IN	ifr_netmask;
		SOCKADDR_IN	ifr_hwaddr;
		short		ifr_flags;
		int		ifr_ifindex;
		int		ifr_metric;
		int		ifr_mtu;
		struct ifmap	ifr_map;
		char		ifr_slave[IFNAMSIZ];
		char		ifr_newname[IFNAMSIZ];
		char *		ifr_data;
	};
};



#endif


extern int wfaCreateTCPServSock(unsigned short sport);
extern int wfaCreateUDPSock(char *sipaddr, unsigned short sport);
extern int wfaAcceptTCPConn(int servSock);
extern int wfaConnectUDPPeer(int sock, char *dipaddr, unsigned short dport);
extern void wfaSetSockFiDesc(fd_set *sockset, int *, struct sockfds *);
extern int wfaCtrlSend(int sock, unsigned char *buf, int bufLen);
extern int wfaCtrlRecv(int sock, unsigned char *buf);
extern int wfaTrafficSendTo(int sock, char *buf, int bufLen, struct sockaddr *to);
extern int wfaTrafficRecv(int sock, char *buf, struct sockaddr *from);
extern int wfaGetifAddr(char *ifname, struct sockaddr_in *sa);
extern struct timeval *wfaSetTimer(int, int, struct timeval *);
extern int wfaSetSockMcastRecvOpt(int, char *);
extern int wfaSetSockMcastSendOpt(int);
extern int wfaSetProcPriority(int);


#endif /* _WFA_SOCK_H */
