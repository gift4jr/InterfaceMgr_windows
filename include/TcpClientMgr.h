/* -----------------------------------------------------
 * File    : TcpClientMgr.h
 * Desc    : TCP Client Communication Manager
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
 * -----------------------------------------------------
 */

#pragma once    // Prevent multiple inclusion of this header file

#include "MsgQMgr.h"
#include "TimeMgr.h"

#pragma comment(lib, "ws2_32.lib")   // Link with Winsock library

#pragma pack(1) // Set structure alignment to 1-byte boundary (no padding)

// TcpClientMgr Configuration Structure
struct STcpClientMgrInfo
{
	int	server_port = 8888;				// Server port to connect to
	int	client_port = 7777;				// Client port number (for binding, if needed - optional)
	char server_ip[16] = "127.0.0.1";	// Server IP address to connect to
	char client_ip[16] = "127.0.0.1";   // Client IP address number (for binding, if needed - optional)
	char client_bind_flag = 0;			// Flag to bind Client IP/port (1: bind, 0: don't bind)
};

#pragma pack()  // Restore default packing alignment

// Callback function type for receiving TCP message
typedef void (TcpClientCallbackFunction)(void* pThis, STimeInfo rcvTime, char* msg, int msgLen);	

// Callback function type for TCP connection status
typedef void (TcpClientStatusCallbackFunction)(void* pThis, int status);							

class TcpClientMgr
{
public:
	TcpClientMgr(int maxMsgBytes = 1000);						// Constructor
	virtual ~TcpClientMgr();									// Destructor

	TcpClientMgr(const TcpClientMgr&) = delete;            		// Copy constructor deleted to prevent shallow copy of internal buffer
	TcpClientMgr& operator=(const TcpClientMgr&) = delete; 		// Copy assignment operator deleted to avoid multiple deletes on the same memory
	TcpClientMgr(TcpClientMgr&&) = delete;                 		// Move constructor deleted to ensure unique ownership of buffer
	TcpClientMgr& operator=(TcpClientMgr&&) = delete;      		// Move assignment operator deleted to prevent multiple ownership issues

	void setCallback(TcpClientCallbackFunction* pCallbackFunction, void* pThis);				// Register callback function for receiving TCP message
	void setCallback(TcpClientStatusCallbackFunction* pStatusCallbackFunction, void* pThis);	// Register callback function for TCP connection status
		
	void startTcpClient(STcpClientMgrInfo stTcpClientMgrInfo);	// Start TCP client with specified configuration
	void stopTcpClient();										// Stop TCP client and clean up threads/sockets
		
	int sendMsg(char* msg, int msgLen);							// Send a message to the server

private:
	STcpClientMgrInfo m_stTcpClientMgrInfo;						// TCP client configuration
	SOCKET m_clientSocket;										// Client socket descriptor

	HANDLE m_thread0, m_thread1, m_thread2;						// Handles to threads for initializing, socket I/O and message queue processing

	TcpClientCallbackFunction* m_pCallbackFunction;				// Pointer to message callback function
	void* m_pThis;												// Pointer to instance for message callback

	TcpClientStatusCallbackFunction* m_pStatusCallbackFunction; // Pointer to status callback function
	void* m_pStatusThis;										// Pointer to instance for status callback

	MsgQMgr* m_pMsgQ;											// Pointer to message queue

	char* m_rxBuffer1;											// Pointer to buffer for receiving message from server socket
	char* m_rxBuffer2;											// Pointer to buffer for receiving message from message queue

	int m_maxMsgBytes;											// Maximum length of one message
	int m_chStatus;												// Internal status indicator 

	bool m_standby;												// Indicates if the client is in standby state
	bool m_working;												// Indicates if the client is actively working
	bool m_starting;											// Indicates if the client is in startup phase
	bool m_initSuccess;											// Indicates if initialization was successful
	bool m_manualStop;											// Indicates if the client was manually stopped

	void initTcpClient();										// Thread for initializing TCP client
	void rxTask1();												// Thread for receiving message from server socket
	void rxTask2();												// Thread for receiving message from message queue

	static void initTcpClientEntry(void* pThis); 				// Static entry point for initTcpClient
	static void rxTask1Entry(void* pThis);						// Static entry point for rxTask1
	static void rxTask2Entry(void* pThis);						// Static entry point for rxTask2
};
