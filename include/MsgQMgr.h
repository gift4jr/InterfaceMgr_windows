/* -----------------------------------------------------
 * File    : MsgQMgr.h
 * Desc    : Message Queue Manager using Ring Buffer
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
 * -----------------------------------------------------
 */

#pragma once        // Prevent multiple inclusion of this header file

#define NOMINMAX    // Prevent min and max macro definitions in windows.h

#include <windows.h>
#include <algorithm>
#include <mutex>
#include <condition_variable>

class MsgQMgr
{
public:
    MsgQMgr(int maxMsgSize = 1000);              // Constructor
    virtual ~MsgQMgr();                          // Destructor

    MsgQMgr(const MsgQMgr&) = delete;            // Copy constructor deleted to prevent shallow copy of internal buffer
    MsgQMgr& operator=(const MsgQMgr&) = delete; // Copy assignment operator deleted to avoid multiple deletes on the same memory
    MsgQMgr(MsgQMgr&&) = delete;                 // Move constructor deleted to ensure unique ownership of buffer
    MsgQMgr& operator=(MsgQMgr&&) = delete;      // Move assignment operator deleted to prevent multiple ownership issues

    int write(const char* buffer, int len);      // Write a message to the queue 
    int read(char* buffer);                      // Read a message from the queue 

private:
    int m_maxBufferSize;                         // Total size of the ring buffer in bytes
    char* m_buffer;                              // Pointer to the ring buffer memory
    int m_head;                                  // Read pointer (head index)
    int m_tail;                                  // Write pointer (tail index)
    bool m_msgQAlive;                            // Flag indicating whether the queue is active

    std::mutex m_mutex;                          // Mutex for synchronizing access to the queue
    std::condition_variable m_notEmpty;          // Condition variable to wait for data availability
    std::condition_variable m_notFull;           // Condition variable to wait for space availability

    void bufferWrite(const char* src, int len);  // Writes data into the ring buffer
    void bufferRead(char* dst, int len);         // Reads data from the ring buffer

    int availableData();                         // Returns the amount of data currently in the buffer
    int availableSpace();                        // Returns the available space in the buffer
};
