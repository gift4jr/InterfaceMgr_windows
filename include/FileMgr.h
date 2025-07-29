/* -----------------------------------------------------
 * File    : FileMgr.h
 * Desc    : File Manager
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
 * -----------------------------------------------------
 */

#pragma once    // Prevent multiple inclusion of this header file

#include "windows.h"
#include "stdio.h"
#include <mutex>

class FileMgr
{
public:
    FileMgr(int maxMsgBytes = 1000);                // Constructor
    virtual ~FileMgr();                             // Destructor

    FileMgr(const FileMgr&) = delete;               // Copy constructor deleted to prevent shallow copy of internal buffer
    FileMgr& operator=(const FileMgr&) = delete;    // Copy assignment operator deleted to avoid multiple deletes on the same memory
    FileMgr(FileMgr&&) = delete;                    // Move constructor deleted to ensure unique ownership of buffer
    FileMgr& operator=(FileMgr&&) = delete;         // Move assignment operator deleted to prevent multiple ownership issues

    void openFile(char* fileName, char* mode);      // Opens file with given name and mode (e.g., "rb", "wb")
    void closeFile();                               // Closes the currently opened file

    int writeFile(char* buffer, int len);           // Writes data to file
    int readFile(char* buffer);                     // Reads data from file

private:
    std::mutex m_mutex;                             // Mutex for thread safety

    int m_maxMsgBytes;                              // Maximum allowed message size
    int m_savedMsgBytes;                            // Total number of bytes saved

    char m_fileName[100];                           // File Name for access

    FILE* m_filePtr;                                // File pointer for read/write operations
};
