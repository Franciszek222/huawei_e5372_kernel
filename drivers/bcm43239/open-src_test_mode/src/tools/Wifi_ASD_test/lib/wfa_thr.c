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
/*       Revision History:
 *       2006/11/10  -- initial created by qhu
 *       2007/02/15  -- WMM Extension Beta released by qhu, mkaroshi
 *       2007/03/30  -- 01.40 WPA2 and Official WMM Beta Release by qhu
 *       2007/04/20 -- 02.00 WPA2 and Official WMM Release by qhu
 *       2007/08/15 --  02.10 WMM-Power Save release by qhu
 *       2007/10/10 --  02.20 Voice SOHO beta -- qhu
 *       2007/11/07 --  02.30 Voice HSO -- qhu
 *
 */

/*
 * For MADWIFI driver, the TOS to 11E queue mapping as:
 *    0x08, 0x20            ----> WME_AC_BK;
 *    0x28, 0xa0            ----> WMC_AC_VI;
 *    0x30, 0xe0 0x88, 0xb8 ----> WME_AC_VO
 *      here 0x88 for UPSD, will be implemented later
 *    all other/default     ----> WME_AC_BE;
 */


#include <stdio.h>

#include <string.h>
#include <stdlib.h>

#ifndef WIN32
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#endif /* !WIN32 */
#include <wfa_debug.h>

#include "wfa_types.h"
#include "wfa_main.h"
#include "wfa_tg.h"
#include "wfa_debug.h"
#include "wfa_tlv.h"
#include "wfa_sock.h"
#include "wfa_rsp.h"
#include "wfa_wmmps.h"
#include "wfa_miscs.h"
#include "wfa_ca_resp.h"
/*
 * external global thread sync variables
 */
#ifdef WIN32
extern HANDLE thr_flag_cond;
extern HANDLE thr_stop_cond;
extern void Win32_tmout_stop_send(LPVOID num);
#endif /* WIN32 */

#ifdef WFA_WMM_EXT
extern tgWMM_t wmm_thr[];
#endif /* WFA_WMM_EXT */

extern tgStream_t *findStreamProfile(int id);
extern int gxcSockfd;
extern int wfaSetProcPriority(int);
extern tgStream_t *gStreams;
extern int IPTVprof;
extern int resetsnd;

#ifdef WFA_WMM_PS_EXT
extern int gtgWmmPS;
extern int psSockfd;
extern int **ac_seq;
int num_hello = 0;
int num_stops = 0;
extern wfaWmmPS_t wmmps_info;
extern void wfaSetDUTPwrMgmt(int mode);
extern void BUILD_APTS_MSG(int msg, unsigned long *txbuf);
extern void wmmps_wait_state_proc();
extern void mpx(char *m, void *buf_v, int len);
extern unsigned int psTxMsg[];
extern unsigned int psRxMsg[];
int msgsize = 256;
extern StationProcStatetbl_t stationProcStatetbl[LAST_TEST + 1][11];
#endif /* WFA_WMM_PS_EXT */

extern void wfaGetSockOpt(int sockfd, int *tosval, socklen_t *size);
extern int wfaSetSockOpt(int sockfd, int *tosval, int size);
void tmout_stop_send(int num);

/*
 * wfaTGSetPrio(): This depends on the network interface card.
 *               So you might want to remap according to the driver
 *               provided.
 *               The current implementation is to set the TOS/DSCP bits
 *               in the IP header
 */
int wfaTGSetPrio(int sockfd, int tgClass)
{
	int tosval;

	socklen_t size = sizeof(tosval);

	wfaGetSockOpt(sockfd, &tosval, &size);

	switch (tgClass) {
	case TG_WMM_AC_BK:
		tosval = 0x20;
		break;
	case TG_WMM_AC_VI:
		tosval = 0xA0;
		break;
	case TG_WMM_AC_UAPSD:
		tosval = 0x88;
		break;
	case TG_WMM_AC_VO:
		tosval = 0xC0;
		break;
	case TG_WMM_AC_BE:
		tosval = 0x00;
	default:
		tosval = 0x00;
		/* default */
		;
	}
#ifdef WFA_WMM_EXT
#ifdef WFA_WMM_PS_EXT
	psTxMsg[1] = tosval;
#endif /* WFA_WMM_PS_EXT*/
#endif /* WFA_WMM_EXT*/

	if (wfaSetSockOpt(sockfd, &tosval, sizeof(tosval)) != 0)
		DPRINT_ERR(WFA_ERR, "Failed to set IP_TOS\n");
	return (tosval == 0xE0)?0xD8:tosval;
}

#ifdef WFA_WMM_EXT
#ifdef WFA_WMM_PS_EXT
/*
 * sender(): This is a generic function to send a packed for the given dsc
 *               (ac:VI/VO/BE/BK), before sending the packet the function
 *               puts the station into the PS mode indicated by psave and
 *               sends the packet after sleeping for sllep_period
 */
int sender(char psave, int sleep_period, int dsc)
{
	int r;

	PRINTF("\nsleeping for %d", sleep_period);
	wfaSetDUTPwrMgmt(psave);
	uapsd_sleep(sleep_period);
	PRINTF("\nAfter create sending %d\n", dsc);
	create_apts_msg(APTS_DEFAULT, psTxMsg, wmmps_info.my_sta_id);
	wfaTGSetPrio(psSockfd, dsc);
	PRINTF("\nAfter create");
	PRINTF("\nlock met");
	r = wfaTrafficSendTo(psSockfd, (char *)psTxMsg, msgsize, (struct sockaddr *)&wmmps_info.psToAddr);
	return r;
}

/*
 * wfaStaSndConfirm(): This function sends the confirm packet
 *                which is sent after the console sends the
 *                test name to the station
 */
int WfaStaSndConfirm(char psave, int sleep_period, int *state)
{
	int r;
	static int num_hello = 0;

	wfaSetDUTPwrMgmt(psave);
	if (!num_hello)
		create_apts_msg(APTS_CONFIRM, psTxMsg, 0);
	r = wfaTrafficSendTo(psSockfd, (char *)psTxMsg, msgsize, (struct sockaddr *)&wmmps_info.psToAddr);
	(*state)++;

	return 0;
}

/*
 * WfaStaSndVO(): This function sends a AC_VO packet
 *                after the time specified by sleep_period
 *                and advances to the next state for the given test case
 */
int WfaStaSndVO(char psave, int sleep_period, int *state)
{
	int r;
	static int en = 1;

	PRINTF("\r\nEnterring WfastasndVO %d", en++);
	if ((r = sender(psave, sleep_period, TG_WMM_AC_VO)) >= 0)
		(*state)++;
	else
		PRINTF("\r\nError\n");

	return 0;
}

/*
 * WfaStaSnd2VO(): This function sends two AC_VO packets
 *                after the time specified by sleep_period
 *                and advances to the next state for the given test case
 */
int WfaStaSnd2VO(char psave, int sleep_period, int *state)
{
	int r;
	static int en = 1;

	PRINTF("\r\nEnterring WfastasndVO %d", en++);
	if ((r = sender(psave, sleep_period, TG_WMM_AC_VO)) >= 0) {
		r = wfaTrafficSendTo(psSockfd, (char *)psTxMsg, msgsize, (struct sockaddr *)&wmmps_info.psToAddr);
		mpx("STA msg", psTxMsg, 64);
		(*state)++;
	} else {
		PRINTF("\r\nError\n");
	}

	return 0;
}

/*
 * WfaStaSndVI(): This function sends a AC_VI packet
 *                after the time specified by sleep_period
 *                and advances to the next state for the given test case
 */
int WfaStaSndVI(char psave, int sleep_period, int *state)
{
	int r;
	static int en = 1;

	PRINTF("\r\nEnterring WfastasndVI %d", en++);
	if ((r = sender(psave, sleep_period, TG_WMM_AC_VI)) >= 0)
		(*state)++;

	return 0;
}

/*
 * WfaStaSndBE(): This function sends a AC_BE packet
 *                after the time specified by sleep_period
 *                and advances to the next state for the given test case
 */
int WfaStaSndBE(char psave, int sleep_period, int *state)
{
	int r;
	static int en = 1;

	PRINTF("\r\nEnterring WfastasndBE %d", en++);
	if ((r = sender(psave, sleep_period, TG_WMM_AC_BE)) >= 0)
		(*state)++;

	return 0;
}
/*
 * WfaStaSndBK(): This function sends a AC_BK packet
 *                after the time specified by sleep_period
 *                and advances to the next state for the given test case
 */
int WfaStaSndBK(char psave, int sleep_period, int *state)
{
	int r;
	static int en = 1;

	PRINTF("\r\nEnterring WfastasndBK %d", en++);
	if ((r = sender(psave, sleep_period, TG_WMM_AC_BK)) >= 0)
		(*state)++;

	return 0;
}

/*
 * WfaStaSndVOCyclic(): The function is the traffic generator for the L.1 test
 *                      caseThis function sends 3000 AC_VO packet
 *                      after the time specified by sleep_period (20ms)
 */
int WfaStaSndVOCyclic(char psave, int sleep_period, int *state)
{
	int i;
	static int en = 1;

	for (i = 0; i < 3000; i++) {
		PRINTF("\r\nEnterring WfastasndVOCyclic %d", en++);
		sender(psave, sleep_period, TG_WMM_AC_VO);
		if (!(i % 50)) {
			PRINTF(".");
			fflush(stdout);
		}
	}
	(*state)++;
	return 0;
}
#endif /* WFA_WMM_PS_EXT */
#endif /* WFA_WMM_EXT */
