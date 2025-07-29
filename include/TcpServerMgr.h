/* -----------------------------------------------------
 * File    : TcpServerMgr.h
 * Desc    : TCP Server Communication Manager
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
 * -----------------------------------------------------
 */

#pragma once    // Prevent multiple inclusion of this header file

#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <atomic>
#include <functional>
#include <map>
#include <string>

#include "MsgQMgr.h"
#include "TimeMgr.h"

#pragma comment(lib, "ws2_32.lib")   // Link with Winsock library

#pragma pack(1) // Set structure alignment to 1-byte boundary (no padding)

// TcpServerMgr Configuration Structure
struct STcpServerMgrInfo
{
    int server_port = 8888;             // Server port
    int client_port = 7777;             // Expected client port number (for binding, if needed - optional)
    char server_ip[16] = "127.0.0.1";   // Server IP address
    char client_ip[16] = "127.0.0.1";   // Expected client IP address number (for binding, if needed - optional)
    char client_bind_flag = 0;          // Flag to bind Client IP/port (1: bind, 0: don't bind)
};

#pragma pack()  // Restore default packing alignment

// Callback function type for receiving TCP message
typedef void (TcpServerCallbackFunction)(void* pThis, char* client_ip, int client_port, STimeInfo rcvTime, char* msg, int msgLen);  

// Callback function type for TCP connection status
typedef void (TcpServerStatusCallbackFunction)(void* pThis, char* client_ip, int client_port, int status);                          

class TcpServerMgr
{
public:
    TcpServerMgr(int maxMsgBytes = 1000);                   // Constructor
    virtual ~TcpServerMgr();                                // Destructor

    TcpServerMgr(const TcpServerMgr&) = delete;            // Copy constructor deleted to prevent shallow copy of internal buffer
    TcpServerMgr& operator=(const TcpServerMgr&) = delete; // Copy assignment operator deleted to avoid multiple deletes on the same memory
    TcpServerMgr(TcpServerMgr&&) = delete;                 // Move constructor deleted to ensure unique ownership of buffer
    TcpServerMgr& operator=(TcpServerMgr&&) = delete;      // Move assignment operator deleted to prevent multiple ownership issues

    void setCallback(TcpServerCallbackFunction* pCallbackFunction, void* pThis);                // Register callback function for receiving TCP message
    void setCallback(TcpServerStatusCallbackFunction* pStatusCallbackFunction, void* pThis);    // Register callback function for TCP connection status

    void startTcpServer(STcpServerMgrInfo stTcpServerMgrInfo);  // Start TCP server with specified configuration
    void stopTcpServer();                                       // Stop TCP server and close all client connections

    int sendMsg(char* client_ip, int client_port, char* msg, int msgLen);   // Send a message to specific client

private:
    STcpServerMgrInfo m_stTcpServerMgrInfo;                     // TCP server configuration
    SOCKET m_serverSocket;                                      // Listening socket
    sockaddr_in m_address;                                      // Server socket address

    std::thread m_socketThread, m_msgQThread;                   // Threads for socket I/O and message queue processing
    std::vector<std::thread> m_threads;                         // Thread pool for handling multiple clients
    std::mutex m_mutex;                                         // Mutex for thread-safe access to shared resources
    std::map<std::string, SOCKET> m_clientSockets;              // Map of connected client sockets (key: IP+port string)

    TcpServerCallbackFunction* m_pCallbackFunction;             // Pointer to message callback function
    void* m_pThis;                                              // Pointer to instance for message callback

    TcpServerStatusCallbackFunction* m_pStatusCallbackFunction; // Pointer to status callback function
    void* m_pStatusThis;                                        // Pointer to instance for status callback

    MsgQMgr* m_pMsgQ;                                           // Pointer to message queue
	char* m_rxBuffer;                                           // Pointer to buffer for receiving message from client socket

    int m_maxMsgBytes;                                          // Maximum allowed message size
    int m_clientsCnt;                                           // Connected client count

    void waitClients();                                         // Thread for waiting and accepting new client connections
	void rxTask(SOCKET clientSocket);                           // Thread for receiving message from client socket
    void msgQrxTask();                                          // Thread for receiving message from message queue
};
