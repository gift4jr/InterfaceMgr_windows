/* -----------------------------------------------------
 * File    : SerialMgrTest.h
 * Desc    : Serial Communication Test using SerialMgr
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
 * ----------------------------------------------------- */

#pragma once                                // Prevent multiple inclusion of this header file

#include "SerialMgr.h"

class SerialMgrTest
{
public:
    SerialMgrTest(void);                    // Constructor
    ~SerialMgrTest(void);                   // Destructor

    void start();                           // Starts Serial Communication Test

private:
    SerialMgr* m_pSerial1;                  // Pointer to SerialMgr instance 1
    SerialMgr* m_pSerial2;                  // Pointer to SerialMgr instance 2

    HANDLE m_thread;                        // Handle to transmitter thread
    bool m_threadAlive;                     // Flag to control transmitter loop

    void txTask();                          // Transmitter thread
    static void txTaskEntry(void* pThis);   // Static entry point for txTask

    static void recvMsgFromSerial1(void* pThis, STimeInfo rcvTime, char* msg, int msgLen);  // Callback function for receiving message from SerialMgr instance 1
    static void recvMsgFromSerial2(void* pThis, STimeInfo rcvTime, char* msg, int msgLen);  // Callback function for receiving message from SerialMgr instance 2
};
