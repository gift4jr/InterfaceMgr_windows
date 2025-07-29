/* -----------------------------------------------------
 * File    : TableMgrTest.cpp
 * Desc    : Table Management Test using TableMgr
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
 * ----------------------------------------------------- */

#include "TableMgrTest.h"

#ifdef __cplusplus  // Indicates to the C++ compiler to compile functions in this block with C-style linkage
extern "C" {
#endif

    void start_test(void)
    {
        TableMgrTest* pTest = new TableMgrTest();       // Create TableMgrTest instance on heap
        pTest->start();                                 // Start the TableMgr test

        while (1)                                       // Keep main thread alive to allow test completion
        {
            Sleep(1000);
        }
    }

#ifdef __cplusplus
}
#endif

/*
 * Function : TableMgrTest::TableMgrTest
 * Desc     : Constructor
 * Param    : None
 * Return   : None
 */
TableMgrTest::TableMgrTest()
    : m_pTableMgr(NULL)
{
}

/*
 * Function : TableMgrTest::~TableMgrTest
 * Desc     : Destructor
 * Param    : None
 * Return   : None
 */
TableMgrTest::~TableMgrTest()
{
    delete m_pTableMgr;
}

/*
 * Function : TableMgrTest::start
 * Desc     : Starts Table Management Test
 * Param    : None
 * Return   : None
 */
void TableMgrTest::start()
{
    /////////////////////////////////////////////////////////
    m_pTableMgr = new TableMgr();                             // Create TableMgr instance
    m_pTableMgr->setCallback(recvMsgFromTableMgr, this);      // Register callback function for Table Manager events by numeric key
    m_pTableMgr->setCallback(recvMsgFromTableMgrS, this);     // Register callback function for Table Manager events by string key
    m_pTableMgr->initTableMgr(12, 250, 200);                  // Initialize the table manager 
    /////////////////////////////////////////////////////////

    int i, len;
    char nuBuf[10] = { 0, };
    char* snu;
    int nu;
    char setBuffer[200];    

    // Insert initial data
    for (i = 0; i < 250; i++)
    {
        sprintf(nuBuf, "nu%d", i);
        snu = nuBuf;
        nu = i;

        sprintf(setBuffer, "%d%s", i,
            "nnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn");
        len = strlen(setBuffer);

        m_pTableMgr->insertData(nu, setBuffer, len); // by numeric key
        // m_pTableMgr->insertData(snu, setBuffer, len); // by string key
    }

    Sleep(3000);  

    // Update data
    for (i = 0; i < 250; i++)
    {
        sprintf(nuBuf, "nu%d", i);
        snu = nuBuf;
        nu = i;

        sprintf(setBuffer, "%d%s", i,
            "uuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu");
        len = strlen(setBuffer);

        m_pTableMgr->insertData(nu, setBuffer, len); // by numeric key
        // m_pTableMgr->insertData(snu, setBuffer, len); // by string key
    }

    Sleep(3000);  

    // Redundant update
    for (i = 0; i < 250; i++)
    {
        sprintf(nuBuf, "nu%d", i);
        snu = nuBuf;
        nu = i;

        sprintf(setBuffer, "%d%s", i,
            "uuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu");
        len = strlen(setBuffer);

        m_pTableMgr->insertData(nu, setBuffer, len); // by numeric key
        // m_pTableMgr->insertData(snu, setBuffer, len); // by string key
    }

    Sleep(3000);  

    ShowTableInfo();  // Display all data

    Sleep(9000);  

    printf("Left %d info\n", m_pTableMgr->getDataInfoCnt());  // Show remaining data
}

/*
 * Function : TableMgrTest::recvMsgFromTableMgr
 * Desc     : Callback function for Table Manager events by numeric key (create, update, delete)
 * Param    : pThis      - Pointer to instance
 *          : info_type  - Event type
 *          : nu         - Numeric key
 *          : index      - Table index
 *          : info       - Pointer to data
 *          : len        - Length of data
 * Return   : None
 */
void TableMgrTest::recvMsgFromTableMgr(void* pThis, int info_type, int nu, int index, char* info, int len)
{
    TableMgrTest* pTableMgrTest = (TableMgrTest*)pThis;

    if (info_type == TABLEMGR_NEW)
    {
        printf("[m_pTableMgr] Table Created! [TN:%d][Index:%d][len:%d][%s] \n", nu, index, len, info);
    }
    else if (info_type == TABLEMGR_UPDATE)
    {
        printf("[m_pTableMgr] Table Updated! [TN:%d][Index:%d][len:%d][%s] \n", nu, index, len, info);
    }
    else if (info_type == TABLEMGR_SAME)
    {
        printf("[m_pTableMgr] Table Same! [TN:%d][Index:%d][len:%d][%s] \n", nu, index, len, info);
    }
    else if (info_type == TABLEMGR_DELETE)
    {
        STimeInfo stSTimeInfo = getCurrentTime();
        printf("[%04d.%02d.%02d][%02d:%02d:%02d.%03d] \n",
            stSTimeInfo.usYear, stSTimeInfo.ucMonth, stSTimeInfo.ucDay,
            stSTimeInfo.ucHour, stSTimeInfo.ucMin, stSTimeInfo.ucSec, stSTimeInfo.uiMiliSec);

        printf("[m_pTableMgr] Table Deleted! [TN:%d][Index:%d][len:%d][%s] \n", nu, index, len, info);
    }
    else if (info_type == TABLEMGR_DELETE_ALL)
    {
        printf("[m_pTableMgr] All Tables Deleted! [TN:%d][Index:%d][len:%d][%s] \n", nu, index, len, info);
    }
}

/*
 * Function : TableMgrTest::recvMsgFromTableMgrS
 * Desc     : Callback function for Table Manager events by string key (create, update, delete)
 * Param    : pThis      - Pointer to instance
 *          : info_type  - Event type
 *          : snu        - String key
 *          : index      - Table index
 *          : info       - Pointer to data
 *          : len        - Length of data
 * Return   : None
 */
void TableMgrTest::recvMsgFromTableMgrS(void* pThis, int info_type, char* snu, int index, char* info, int len)
{
    TableMgrTest* pTableMgrTest = (TableMgrTest*)pThis;

    if (info_type == TABLEMGR_NEW)
    {
        printf("[m_pTableMgr] Table Created! [TN:%s][Index:%d][len:%d][%s] \n", snu, index, len, info);
    }
    else if (info_type == TABLEMGR_UPDATE)
    {
        printf("[m_pTableMgr] Table Updated! [TN:%s][Index:%d][len:%d][%s] \n", snu, index, len, info);
    }
    else if (info_type == TABLEMGR_SAME)
    {
        printf("[m_pTableMgr] Table Same! [TN:%s][Index:%d][len:%d][%s] \n", snu, index, len, info);
    }
    else if (info_type == TABLEMGR_DELETE)
    {
        STimeInfo stSTimeInfo = getCurrentTime();
        printf("[%04d.%02d.%02d][%02d:%02d:%02d.%03d] \n",
            stSTimeInfo.usYear, stSTimeInfo.ucMonth, stSTimeInfo.ucDay,
            stSTimeInfo.ucHour, stSTimeInfo.ucMin, stSTimeInfo.ucSec, stSTimeInfo.uiMiliSec);

        printf("[m_pTableMgr] Table Deleted! [TN:%s][Index:%d][len:%d][%s] \n", snu, index, len, info);
    }
    else if (info_type == TABLEMGR_DELETE_ALL)
    {
        printf("[m_pTableMgr] All Tables Deleted! [TN:%s][Index:%d][len:%d][%s] \n", snu, index, len, info);
    }
}

/*
 * Function : TableMgrTest::ShowTableInfo
 * Desc     : Displays all table data using both numeric and string keys
 * Param    : None
 * Return   : None
 */
void TableMgrTest::ShowTableInfo()
{
    int i, s, ret, nu;
    char* snu;
    char getBuffer[1000];

    for (i = 0; i <= m_pTableMgr->getDataMaxIndex(); i++)
    {
        // Retrieve and display info by numeric key
        nu = m_pTableMgr->getDataNumber(i);
        ret = m_pTableMgr->getDataInfo(nu, getBuffer);
        if (ret > 0)
        {
            printf("m_pTableMgr [TN:%d][Index:%d] Table Info [len=%d] : ", nu, i, ret);
            for (s = 0; s < ret; s++)
                printf("%c ", (unsigned char)getBuffer[s]);
            printf("\n");
        }

        // Retrieve and display info by string key
        snu = m_pTableMgr->getDataSNumber(i);
        ret = m_pTableMgr->getDataInfo(snu, getBuffer);
        if (ret > 0)
        {
            printf("m_pTableMgr [TN:%s][Index:%d] Table Info [len=%d] : ", snu, i, ret);
            for (s = 0; s < ret; s++)
                printf("%c ", (unsigned char)getBuffer[s]);
            printf("\n");
        }
    }
}
