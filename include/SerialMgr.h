/* -----------------------------------------------------
 * File    : SerialMgr.h
 * Desc    : Serial Communication Manager
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
 * -----------------------------------------------------
 */

#pragma once    // Prevent multiple inclusion of this header file

#include "MsgQMgr.h"
#include "TimeMgr.h"

#pragma pack(1) // Set structure alignment to 1-byte boundary (no padding)

// SerialMgr Configuration Structure
struct SSerialMgrInfo
{
	char serial_port[10] = "COM1";   					// Serial port name
	DWORD baudRate = 38400;         					// Baud rate 
	unsigned char byteSize = 8; 						// Number of data bits 
	unsigned char parity = 0;   						// Parity (0 = none)
	unsigned char stopBit = 0;  						// Number of stop bits 
};

#pragma pack()  // Restore default packing alignment

// Callback function type for receiving Serial message
typedef void (SerialCallbackFunction)(void* pThis, STimeInfo rcvTime, char* msg, int msgLen);	

class SerialMgr
{
public:
	SerialMgr(int maxMsgBytes = 1000);					// Constructor
	virtual ~SerialMgr();								// Destructor

	SerialMgr(const SerialMgr&) = delete;				// Copy constructor deleted to prevent shallow copy of internal buffer
	SerialMgr& operator=(const SerialMgr&) = delete;	// Copy assignment operator deleted to avoid multiple deletes on the same memory
	SerialMgr(SerialMgr&&) = delete;					// Move constructor deleted to ensure unique ownership of buffer
	SerialMgr& operator=(SerialMgr&&) = delete;			// Move assignment operator deleted to prevent multiple ownership issues

	void setCallback(SerialCallbackFunction* pCallbackFunction, void* pThis); // Register callback function for receiving Serial message

	void startSerial(SSerialMgrInfo stSerialMgrInfo);	// Start Serial communication with specified configuration
	void stopSerial();									// Stop Serial communication and release resources

	int sendMsg(char* msg, int msgLen);					// Send a message to the destination

private:
	SSerialMgrInfo m_stSerialMgrInfo;					// Serial port configuration information

	OVERLAPPED osWrite, osRead, os;						// Overlapped I/O structures for async communication

	HANDLE m_thread1, m_thread2;						// Handles to threads for serial I/O and message queue processing
	HANDLE m_comPort;									// Handle to the serial port

	SerialCallbackFunction* m_pCallbackFunction;		// Pointer to message callback function
	void* m_pThis;										// Pointer to instance for message callback

	MsgQMgr* m_pMsgQ;									// Pointer to message queue

	char* m_rxBuffer1;									// Pointer to buffer for receiving message from serial port
	char* m_rxBuffer2;									// Pointer to buffer for receiving message from message queue

	int m_maxMsgBytes;									// Maximum length of one message

	BOOL setupCommSerial();								// Configures the serial port

	void rxTask1();										// Thread for receiving message from serial port
	void rxTask2();										// Thread for receiving message from message queue

	static void rxTask1Entry(void* pThis);				// Static entry point for rxTask1
	static void rxTask2Entry(void* pThis);				// Static entry point for rxTask2
};
