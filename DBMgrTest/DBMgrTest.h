/* -----------------------------------------------------
 * File    : DBMgrTest.h
 * Desc    : Database Management Test using DBMgr
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
 * -----------------------------------------------------
 */

#pragma once    // Prevent multiple inclusion of this header file

#include "DBMgr.h"

#pragma pack(1) // Set structure alignment to 1-byte boundary (no padding)

 // DB Write test structure
struct stInType
{
    int seq;                        // INT NOT NULL PRIMARY KEY
    char code[10];                  // CHAR(10) 
    char num[20];                   // VARCHAR(20) 
    char data[350];                 // VARCHAR(4000) 
    double cost;                    // DOUBLE PRECISION
    SQL_TIMESTAMP_STRUCT time;      // DATETIME / DATETIME2 / TIMESTAMP
};

// DB Read test structure
struct stOutType
{
    char seq[10];                  
    char code[10];                 
    char num[20];                  
    char data[350];                
    char cost[8];                  
    char time[30];                 
};

#pragma pack()                      // Restore default packing alignment

class DBMgrTest
{
public:
    DBMgrTest();                    // Constructor
    ~DBMgrTest();                   // Destructor

    void start();                   // Starts Database Management Test
    void readTable(DBMgr* pDBMgr);  // Reads data from DBMgr instance

private:
    DBMgr* m_pDBMgr;                // Pointer to DBMgr instance 
};
