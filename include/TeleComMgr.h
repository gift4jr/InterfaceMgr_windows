/* -------------------------------------------------------------
 * File    : TeleComMgr.h
 * Desc    : Tele Communication Manager using Multicast Loopback
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
/* -------------------------------------------------------------
 */

#pragma once    // Prevent multiple inclusion of this header file

#include <ws2tcpip.h>
#include "MsgQMgr.h"
#include "TimeMgr.h"

#pragma comment(lib, "ws2_32.lib")   // Link with Winsock library

#define TELECOM_MAX_USER_NAME_SIZE 25u    // Maximum length for user name

#pragma pack(1) // Set structure alignment to 1-byte boundary (no padding)

// TeleComMgr Structure
struct STeleComHdrType
{
    char src[TELECOM_MAX_USER_NAME_SIZE]; // Source user name
    char dst[TELECOM_MAX_USER_NAME_SIZE]; // Destination user name
    STimeInfo rcvTime;                    // Time the message was received
    char type;                            // Message type (user-defined or system)
};

#pragma pack()  // Restore default packing alignment

// Callback function type for receiving TeleCom message
typedef void (TeleComCallbackFunction)(void* pThis, STimeInfo rcvTime, char* msg, int msgLen);      

// Callback function type for receiving TeleCom system message with header
typedef void (TeleComAllCallbackFunction)(void* pThis, STeleComHdrType hdr, char* msg, int msgLen); 

class TeleComMgr
{
public:
    TeleComMgr(char* userName, int maxMsgBytes = 1000);             // Constructor
    virtual ~TeleComMgr();                                          // Destructor

    TeleComMgr(const TeleComMgr&) = delete;                         // Copy constructor deleted to prevent shallow copy of internal buffer
    TeleComMgr& operator=(const TeleComMgr&) = delete;              // Copy assignment operator deleted to avoid multiple deletes on the same memory
    TeleComMgr(TeleComMgr&&) = delete;                              // Move constructor deleted to ensure unique ownership of buffer
    TeleComMgr& operator=(TeleComMgr&&) = delete;                   // Move assignment operator deleted to prevent multiple ownership issues

    void setCallback(TeleComCallbackFunction* pCallbackFunction, void* pThis);         // Register callback function for receiving TeleCom message
    void setCallback(TeleComAllCallbackFunction* pAllCallbackFunction, void* pThis);   // Register callback function for receiving TeleCom system message with header

    int sendMsg(char* dst, char* msg, int msgLen, char type = 0);   // Send a message to the destination

    void changeUserName(char* userName);                            // Change userName during runtime

private:
    SOCKET m_udpSocket;                                             // UDP socket descriptor

    struct sockaddr_in m_dstAddr;                                   // Destination socket address

    HANDLE m_thread1, m_thread2;                                    // Handles to threads for socket I/O and message queue processing

    char m_userName[TELECOM_MAX_USER_NAME_SIZE];                    // Current user name

    std::mutex m_mutex;                                             // Mutex for thread-safe operations

    TeleComCallbackFunction* m_pCallbackFunction;                   // Pointer to message callback function
    void* m_pThis;                                                  // Pointer to instance for message callback

    TeleComAllCallbackFunction* m_pAllCallbackFunction;             // Pointer to system message callback function
    void* m_pAllThis;                                               // Pointer to instance for system message callback

    MsgQMgr* m_pMsgQ;                                               // Pointer to message queue

    char* m_rxMsg1;                                                 // Pointer to buffer for receiving message from multicast group
    char* m_rxMsg2;                                                 // Pointer to buffer for receiving message from message queue
    char* m_txMsg;                                                  // Pointer to buffer for transmission message to multicast group

    int m_maxMsgBytes;                                              // Maximum allowed message size

    void initTeleCom(char* userName);                               // initializing TeleCom

    void rxTask1();                                                 // Thread for receiving message from multicast group
    void rxTask2();                                                 // Thread for receiving message from message queue

    static void rxTask1Entry(void* pThis);                          // Static entry point for rxTask1
    static void rxTask2Entry(void* pThis);                          // Static entry point for rxTask2
};
