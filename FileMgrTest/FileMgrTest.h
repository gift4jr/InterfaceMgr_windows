/* -----------------------------------------------------
 * File    : FileMgrTest.h
 * Desc    : File Management Test using FileMgr
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
 * -----------------------------------------------------
 */

#pragma once                // Prevent multiple inclusion of this header file

#include "FileMgr.h"

#pragma pack(1)             // Set structure alignment to 1-byte boundary (no padding)

// test structure
struct stTypeICD
{
    int len;          
    char track[100]; 
};

#pragma pack()              // Restore default packing alignment

class FileMgrTest
{
public:
    FileMgrTest();          // Constructor
    ~FileMgrTest();         // Destructor

    void start();           // Starts File Management Test

private:
    FileMgr* m_pFileMgr;    // Pointer to FileMgr instance 
};
