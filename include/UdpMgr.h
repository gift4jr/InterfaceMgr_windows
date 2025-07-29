/* -----------------------------------------------------
 * File    : UdpMgr.h
 * Desc    : UDP Communication Manager
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
 * -----------------------------------------------------
 */

#pragma once    // Prevent multiple inclusion of this header file

#include <ws2tcpip.h>
#include "MsgQMgr.h"
#include "TimeMgr.h"

#pragma comment(lib, "ws2_32.lib")   // Link with Winsock library

#pragma pack(1) // Set structure alignment to 1-byte boundary (no padding)

// UdpMgr Configuration Structure
struct SUdpMgrInfo
{
    int src_port = 7777;                            // Source port number
    int dst_port = 8888;                            // Destination port number
    int multi_port = 20001;                         // Multicast port number
    char src_ip[16] = "0.0.0.0";                    // Source IP address
    char dst_ip[16] = "127.0.0.1";                  // Destination IP address
    char multi_ip[16] = "224.100.100.1";            // Multicast group IP address
    char multicast_flag = 0;                        // 0: unicast, 1: multicast send, 2: multicast receive
};

#pragma pack()  // Restore default packing alignment

// Callback function type for receiving UDP message
typedef void (UdpCallbackFunction)(void* pThis, STimeInfo rcvTime, char* msg, int msgLen);  

class UdpMgr
{
public:
    UdpMgr(int maxMsgBytes = 1000);                 // Constructor
    virtual ~UdpMgr();                              // Destructor

    UdpMgr(const UdpMgr&) = delete;                 // Copy constructor deleted to prevent shallow copy of internal buffer
    UdpMgr& operator=(const UdpMgr&) = delete;      // Copy assignment operator deleted to avoid multiple deletes on the same memory
    UdpMgr(UdpMgr&&) = delete;                      // Move constructor deleted to ensure unique ownership of buffer
    UdpMgr& operator=(UdpMgr&&) = delete;           // Move assignment operator deleted to prevent multiple ownership issues

    void setCallback(UdpCallbackFunction* pCallbackFunction, void* pThis); // Register callback function for receiving UDP message

    void startUdp(SUdpMgrInfo stUdpMgrInfo);        // Start UDP communication with specified configuration
    void stopUdp();                                 // Stop UDP communication and release resources
    void resetMUdpRcv();                            // Reset multicast UDP group membership (drop and re-add).

    int sendMsg(char* msg, int msgLen);             // Send a message to the destination

private:
    SOCKET m_udpSocket;                             // UDP socket descriptor

    struct sockaddr_in m_dstAddr;                   // Destination socket address
    struct ip_mreq m_add_membership_val;            // Multicast membership settings
	
	HANDLE m_thread1, m_thread2;		  		    // Handles to threads for socket I/O and message queue processing

    UdpCallbackFunction* m_pCallbackFunction;       // Pointer to message callback function
    void* m_pThis;                                  // Pointer to instance for message callback

	MsgQMgr* m_pMsgQ;							    // Pointer to message queue

	char* m_rxBuffer1;							    // Pointer to buffer for receiving message from udp socket
	char* m_rxBuffer2;								// Pointer to buffer for receiving message from message queue

	int m_maxMsgBytes;							    // Maximum length of one message

	void rxTask1();									// Thread for receiving message from udp socket
	void rxTask2();									// Thread for receiving message from message queue

	static void rxTask1Entry(void* pThis);			// Static entry point for rxTask1
	static void rxTask2Entry(void* pThis);			// Static entry point for rxTask2
};
