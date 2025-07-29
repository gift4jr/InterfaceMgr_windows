/* -----------------------------------------------------
 * File    : TcpServerMgrTest.h
 * Desc    : TCP Server Communication Test using TcpServerMgr
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
 * -----------------------------------------------------
 */

#pragma once                                // Prevent multiple inclusion of this header file

#include "TcpServerMgr.h"

class TcpServerMgrTest
{
public:
    TcpServerMgrTest();                     // Constructor
    ~TcpServerMgrTest();                    // Destructor

    void start();                           // Starts TCP Server Communication Test

private:
    TcpServerMgr* m_pTcpServer1;            // Pointer to TcpServerMgr instance 1
    TcpServerMgr* m_pTcpServer2;            // Pointer to TcpServerMgr instance 2

    static void recvStatusFromTcpServer1(void* pThis, char* client_ip, int client_port, int status);    // Callback function for receiving status from TcpServerMgr instance 1 
    static void recvStatusFromTcpServer2(void* pThis, char* client_ip, int client_port, int status);    // Callback function for receiving status from TcpServerMgr instance 2

    static void recvMsgFromTcpServer1(void* pThis, char* client_ip, int client_port, STimeInfo rcvTime, char* msg, int msgLen);     // Callback function for receiving message from TcpServerMgr instance 1 
	static void recvMsgFromTcpServer2(void* pThis, char* client_ip, int client_port, STimeInfo rcvTime, char* msg, int msgLen);     // Callback function for receiving message from TcpServerMgr instance 2
};