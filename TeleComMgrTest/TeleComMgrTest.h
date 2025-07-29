/* -----------------------------------------------------
 * File    : TeleComMgrTest.h
 * Desc    : Tele Communication Test using TeleComMgr
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
 * -----------------------------------------------------
 */

#pragma once                                // Prevent multiple inclusion of this header file

#include "TeleComMgr.h"

class TeleComMgrTest
{
public:
    TeleComMgrTest();                       // Constructor
    ~TeleComMgrTest();                      // Destructor

    void start();                           // Starts Tele Communication Test 

private:
    TeleComMgr* m_pTeleCom1;                // Pointer to TeleComMgr instance 1
    TeleComMgr* m_pTeleCom2;                // Pointer to TeleComMgr instance 2

    HANDLE m_thread;                        // Handle for the transmit thread
    bool m_threadAlive;                     // Status flag for the transmit thread

    void txTask();                          // Transmitter thread
    static void txTaskEntry(void* pThis);   // Static entry point for txTask

    static void recvMsgFromTeleCom1(void* pThis, STimeInfo rcvTime, char* msg, int msgLen);  // Callback function for receiving message from TeleComMgr instance 1
	static void recvMsgFromTeleCom2(void* pThis, STimeInfo rcvTime, char* msg, int msgLen);  // Callback function for receiving message from TeleComMgr instance 2
};
