/* -----------------------------------------------------
 * File    : DBMgr.h
 * Desc    : Database Manager using ODBC
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
 * -----------------------------------------------------
 */

#pragma once    // Prevent multiple inclusion of this header file

#include "windows.h"
#include "stdio.h"

#include <sql.h>
#include <sqlext.h> 
#include <mutex>

class DBMgr
{
public:
    DBMgr();                                    // Constructor
    virtual ~DBMgr();                           // Destructor

    DBMgr(const DBMgr&) = delete;               // Copy constructor deleted to prevent shallow copy of internal buffer
    DBMgr& operator=(const DBMgr&) = delete;    // Copy assignment operator deleted to avoid multiple deletes on the same memory
    DBMgr(DBMgr&&) = delete;                    // Move constructor deleted to ensure unique ownership of buffer
    DBMgr& operator=(DBMgr&&) = delete;         // Move assignment operator deleted to prevent multiple ownership issues

    int openDB(int type, char* driver, char* dbIP, int dbPort, char* userId, char* userPwd, char* dbName); // Opens a database connection using ODBC with the specified parameters
    void closeDB();                             // Closes the database connection and releases all ODBC handles
	HSTMT sqlQuery(char* query);                // Executes given SQL query, and returns ODBC statement handle if successful, NULL on error

private:
    std::mutex m_mutex;                         // Mutex for thread safety

    HENV  m_henv;                               // ODBC environment handle
    HDBC  m_hdbc;                               // ODBC database connection handle
    HSTMT m_hstmt;                              // ODBC statement handle
};
