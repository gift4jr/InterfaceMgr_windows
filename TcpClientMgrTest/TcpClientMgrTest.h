/* -----------------------------------------------------
 * File    : TcpClientMgrTest.h
 * Desc    : TCP Client Communication Test using TcpClientMgr
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
 * -----------------------------------------------------
 */

#pragma once                                    // Prevent multiple inclusion of this header file

#include "TcpClientMgr.h"

class TcpClientMgrTest
{
public:
    TcpClientMgrTest();                         // Constructor
    ~TcpClientMgrTest();                        // Destructor

    void start();                               // Starts TCP Client Communication Test

private:
    TcpClientMgr* m_pTcpClient1;                // Pointer to TcpClientMgr instance 1
    TcpClientMgr* m_pTcpClient2;                // Pointer to TcpClientMgr instance 2

    HANDLE m_thread;                            // Handle to transmitter thread
    bool m_threadAlive;                         // Flag to control transmitter loop

    void txTask();                              // Transmitter thread
    static void txTaskEntry(void* pThis);       // Static entry point for txTask

    static void recvStatusFromTcpClient1(void* pThis, int status);  // Callback function for receiving status from TcpClientMgr instance 1
    static void recvStatusFromTcpClient2(void* pThis, int status);  // Callback function for receiving status from TcpClientMgr instance 2

    static void recvMsgFromTcpClient1(void* pThis, STimeInfo rcvTime, char* msg, int msgLen);   // Callback function for receiving message from TcpClientMgr instance 1
    static void recvMsgFromTcpClient2(void* pThis, STimeInfo rcvTime, char* msg, int msgLen);   // Callback function for receiving message from TcpClientMgr instance 2
};
