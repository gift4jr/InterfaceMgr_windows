/* -----------------------------------------------------
 * File    : PipeMgrTest.h
 * Desc    : Pipe Communication Test using PipeMgr
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
 * -----------------------------------------------------
 */

#pragma once                                // Prevent multiple inclusion of this header file

#include "PipeMgr.h"

#pragma pack(1)                             // Set structure alignment to 1-byte boundary (no padding)

 // test structure
struct ss
{
    int a;          
    int b;          
    char c[150];    
};

#pragma pack()                              // Restore default packing alignment

class PipeMgrTest
{
public:
    PipeMgrTest();                          // Constructor
    ~PipeMgrTest();                         // Destructor

    void start();                           // Starts Pipe Communication Test

private:
    PipeMgr* m_pPipe1;                      // Pointer to PipeMgr instance 1
    PipeMgr* m_pPipe2;                      // Pointer to PipeMgr instance 2

    bool m_threadAlive;                     // Thread control flag

    HANDLE m_pipe1rxThread;                 // Handle to pipe 1 receiver thread
    HANDLE m_pipe1txThread;                 // Handle to pipe 1 transmitter thread
    HANDLE m_pipe2rxThread;                 // Handle to pipe 2 receiver thread
    HANDLE m_pipe2txThread;                 // Handle to pipe 2 transmitter thread

    void rxTask1();                         // Receiver thread 1
    void txTask1();                         // Transmitter thread 1
    void rxTask2();                         // Receiver thread 2
    void txTask2();                         // Transmitter thread 2

    static void rxTask1Entry(void* pThis);  // Static entry point for rxTask1
    static void txTask1Entry(void* pThis);  // Static entry point for txTask1
    static void rxTask2Entry(void* pThis);  // Static entry point for rxTask2
    static void txTask2Entry(void* pThis);  // Static entry point for txTask2
};
