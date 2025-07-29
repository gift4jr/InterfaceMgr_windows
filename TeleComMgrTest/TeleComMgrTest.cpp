/* -----------------------------------------------------
 * File    : TeleComMgrTest.cpp
 * Desc    : Tele Communication Test using TeleComMgr
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
 * ----------------------------------------------------- */

#include "TeleComMgrTest.h"

#ifdef __cplusplus  // Indicates to the C++ compiler to compile functions in this block with C-style linkage.
extern "C" {
#endif

    void start_test(void)
    {
        TeleComMgrTest* pTest = new TeleComMgrTest();  // Create TeleComMgrTest instance on heap
        pTest->start();                                // Start the TeleComMgr test

        while (1)                                      // Keep main thread alive to allow test completion
        {
            Sleep(1000);
        }
    }

#ifdef __cplusplus
}
#endif

/*
 * Function : TeleComMgrTest::TeleComMgrTest
 * Desc     : Constructor
 * Param    : None
 * Return   : None
 */
TeleComMgrTest::TeleComMgrTest()
    : m_pTeleCom1(NULL), m_pTeleCom2(NULL),
    m_thread(NULL), m_threadAlive(false)
{
}

/*
 * Function : TeleComMgrTest::~TeleComMgrTest
 * Desc     : Destructor
 * Param    : None
 * Return   : None
 */
TeleComMgrTest::~TeleComMgrTest()
{
    m_threadAlive = false;

    delete m_pTeleCom1;
    delete m_pTeleCom2;

    if (m_thread)
    {
        WaitForSingleObject(m_thread, INFINITE);
        CloseHandle(m_thread);
        m_thread = NULL;
    }
}

/*
 * Function : TeleComMgrTest::start
 * Desc     : Starts Tele Communication Test 
 * Param    : None
 * Return   : None
 */
void TeleComMgrTest::start()
{
    m_threadAlive = true;

    m_pTeleCom1 = new TeleComMgr("AAA", 10000);             // Create TeleComMgr instance 1
    m_pTeleCom1->setCallback(recvMsgFromTeleCom1, this);    // Register message callback

	m_pTeleCom2 = new TeleComMgr("BBB", 10000);             // Create TeleComMgr instance 2
	m_pTeleCom2->setCallback(recvMsgFromTeleCom2, this);    // Register message callback

    // Create transmitter thread
    m_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)txTaskEntry, this, 0, NULL);
}

/*
 * Function : TeleComMgrTest::txTaskEntry
 * Desc     : Static entry point for txTask
 * Param    : pThis - pointer to TeleComMgrTest instance
 * Return   : None
 */
void TeleComMgrTest::txTaskEntry(void* pThis)
{
    TeleComMgrTest* pTeleComMgrTest = (TeleComMgrTest*)pThis;
    pTeleComMgrTest->txTask();
}

/*
 * Function : TeleComMgrTest::txTask
 * Desc     : Transmitter thread
 * Param    : None
 * Return   : None
 */
void TeleComMgrTest::txTask()
{
    int ret;
    int cnt = 1;

    char buf1[10000] = "";  
    char buf2[10000] = ""; 

    while (m_threadAlive)
    {
        sprintf(buf1, "AAA is sending to BBB %d", cnt++);
        m_pTeleCom1->sendMsg("BBB", buf1, 10000);

        Sleep(1);

        sprintf(buf2, "BBB is sending to AAA %d", cnt++);
        m_pTeleCom2->sendMsg("AAA", buf2, 10000);

        Sleep(1);
    }
}

/*
 * Function : TeleComMgrTest::recvMsgFromTeleCom1
 * Desc     : Callback function for receiving message from TeleComMgr instance 1
 * Param    : pThis   - pointer to TeleComMgrTest instance
 *            rcvTime - receive time info
 *            msg     - received message buffer
 *            msgLen  - length of received message
 * Return   : None
 */
void TeleComMgrTest::recvMsgFromTeleCom1(void* pThis, STimeInfo rcvTime, char* msg, int msgLen)
{
    TeleComMgrTest* pTeleComMgrTest = (TeleComMgrTest*)pThis;

    printf("[m_pTeleCom1][%04d/%02d/%02d][%02d:%02d:%02d.%03d][%s][%d bytes]\n",
        rcvTime.usYear, rcvTime.ucMonth, rcvTime.ucDay,
        rcvTime.ucHour, rcvTime.ucMin, rcvTime.ucSec, rcvTime.uiMiliSec,
        msg, msgLen);
}

/*
 * Function : TeleComMgrTest::recvMsgFromTeleCom2
 * Desc     : Callback function for receiving message from TeleComMgr instance 2
 * Param    : pThis   - pointer to TeleComMgrTest instance
 *            rcvTime - receive time info
 *            msg     - received message buffer
 *            msgLen  - length of received message
 * Return   : None
 */
void TeleComMgrTest::recvMsgFromTeleCom2(void* pThis, STimeInfo rcvTime, char* msg, int msgLen)
{
    TeleComMgrTest* pTeleComMgrTest = (TeleComMgrTest*)pThis;

    printf("[m_pTeleCom2][%04d/%02d/%02d][%02d:%02d:%02d.%03d][%s][%d bytes]\n",
        rcvTime.usYear, rcvTime.ucMonth, rcvTime.ucDay,
        rcvTime.ucHour, rcvTime.ucMin, rcvTime.ucSec, rcvTime.uiMiliSec,
        msg, msgLen);
}
