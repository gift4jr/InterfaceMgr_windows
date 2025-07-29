/* -----------------------------------------------------
 * File    : PipeMgr.h
 * Desc    : Pipe Communication Manager
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
 * -----------------------------------------------------
 */

#pragma once    // Prevent multiple inclusion of this header file

#include <windows.h>
#include <stdio.h>
#include <mutex>

class PipeMgr
{
public:
    PipeMgr(int maxMsgBytes = 1000, int srcId = 0, int dstId = 0); // Constructor
    virtual ~PipeMgr();                                            // Destructor

    PipeMgr(const PipeMgr&) = delete;            		// Copy constructor deleted to prevent shallow copy of internal buffer
    PipeMgr& operator=(const PipeMgr&) = delete; 		// Copy assignment operator deleted to avoid multiple deletes on the same memory
    PipeMgr(PipeMgr&&) = delete;                 		// Move constructor deleted to ensure unique ownership of buffer
    PipeMgr& operator=(PipeMgr&&) = delete;      		// Move assignment operator deleted to prevent multiple ownership issues

    int write(char* buffer, int len);           		// Writes a message to the pipe (-1: write error, -2: object deleted)
    int read(char* buffer);                     		// Reads a message from the pipe (-1: read error, -2: object deleted)
    bool getIpcReady();                         		// Returns whether IPC connection is successfully initialized for reading

private:
    int m_srcId, m_dstId;                       		// Source and Destination IDs for IPC
    int m_maxMsgBytes;                          		// Maximum allowed message size
    int m_maxBufferSize;                        		// Maximum buffer size 

    bool m_initRxSuccess;                       		// Flag indicating successful RX pipe initialization
    bool m_initTxSuccess;                       		// Flag indicating successful TX pipe initialization

    char m_pipeAlive;                           		// Pipe status flag (used to signal termination)

    HANDLE m_hReadPipe, m_hWritePipe;           		// Handles for the read and write pipes
    HANDLE m_thread1, m_thread2;                		// Threads for reading from and writing to the pipe

    std::mutex m_mutex;                         		// Mutex for thread-safe pipe access

    void initPipe();                            		// Initialize pipe connection based on ITC or IPC  mode

    void writePipeSpawn();                      		// Thread for createing write pipe (waiting until peer is ready)
    void readPipeSpawn();                       		// Thread for createing read pipe (waiting until peer is ready)

    static void writePipeSpawnEntry(void* pThis);      	// Static entry point for writePipeSpawn
    static void readPipeSpawnEntry(void* pThis);       	// Static entry point for readPipeSpawn
};
