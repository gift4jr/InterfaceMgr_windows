/* -----------------------------------------------------
 * File    : TableMgrTest.h
 * Desc    : Table Management Test using TableMgr
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
 * -----------------------------------------------------
 */

#pragma once                    // Prevent multiple inclusion of this header file

#include "TableMgr.h"

class TableMgrTest
{
public:
    TableMgrTest();             // Constructor
    ~TableMgrTest();            // Destructor

    void start();               // Starts Table Management Test

    void ShowTableInfo();       // Displays current table data

private:
    TableMgr* m_pTableMgr;      // Pointer to the TableMgr instance

    static void recvMsgFromTableMgr(void* pThis, int info_type, int nu, int index, char* table, int len);       // Callback function for Table Manager events by numeric key
    static void recvMsgFromTableMgrS(void* pThis, int info_type, char* snu, int index, char* table, int len);   // Callback function for Table Manager events by string key 
};
