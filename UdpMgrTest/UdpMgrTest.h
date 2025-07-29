/* -----------------------------------------------------
 * File    : UdpMgrTest.h
 * Desc    : UDP Communication Test using UdpMgr
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
 * -----------------------------------------------------
 */

#pragma once                                // Prevent multiple inclusion of this header file

#include "UdpMgr.h"

class UdpMgrTest
{
public:
    UdpMgrTest();                           // Constructor
    ~UdpMgrTest();                          // Destructor

    void start();                           // Starts UDP Communication Test

private:
    UdpMgr* m_pUdp1;                        // Pointer to UdpMgr instance 1
    UdpMgr* m_pUdp2;                        // Pointer to UdpMgr instance 2

    bool m_multicastFlag;                   // Flag indicating whether multicast mode is enabled

    HANDLE m_thread;                        // Handle for the transmit thread
    bool m_threadAlive;                     // Status flag for the transmit thread

    void txTask();                          // Transmitter thread
    static void txTaskEntry(void* pThis);   // Static entry point for txTask

    static void recvMsgFromUdp1(void* pThis, STimeInfo rcvTime, char* msg, int msgLen); // Callback function for receiving message from UdpMgr instance 1
	static void recvMsgFromUdp2(void* pThis, STimeInfo rcvTime, char* msg, int msgLen); // Callback function for receiving message from UdpMgr instance 2
};
