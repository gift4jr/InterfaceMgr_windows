/* -----------------------------------------------------
 * File    : MsgQMgrTest.h
 * Desc    : Message Queue Test using MsgQMgr 
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
 * -----------------------------------------------------
 */

#pragma once                                // Prevent multiple inclusion of this header file

#include "MsgQMgr.h"

#pragma pack(1)                             // Set structure alignment to 1-byte boundary (no padding)

 // test structure
struct ss
{
    int a;
    int b;
    char c[150];
};

#pragma pack()                              // Restore default packing alignment

class MsgQMgrTest
{
public:
    MsgQMgrTest();                          // Constructor
    ~MsgQMgrTest();                         // Destructor

    void start();                           // Starts Message Queue Test

private:
    MsgQMgr* m_pMsgQ1;                      // Pointer to MsgQMgr instance 1
    MsgQMgr* m_pMsgQ2;                      // Pointer to MsgQMgr instance 2

    HANDLE m_msgQ1rxThread;                 // Handle to msgQ 1 receiver thread
    HANDLE m_msgQ1txThread;                 // Handle to msgQ 1 transmitter thread
    HANDLE m_msgQ2rxThread;                 // Handle to msgQ 2 receiver thread
    HANDLE m_msgQ2txThread;                 // Handle to msgQ 2 transmitter thread

    void rxTask1();                         // Receiver thread 1
    void txTask1();                         // Transmitter thread 1
    void rxTask2();                         // Receiver thread 2
    void txTask2();                         // Transmitter thread 2

    static void rxTask1Entry(void* pThis);  // Static entry point for rxTask1
    static void txTask1Entry(void* pThis);  // Static entry point for txTask1
    static void rxTask2Entry(void* pThis);  // Static entry point for rxTask2
    static void txTask2Entry(void* pThis);  // Static entry point for txTask2
};
