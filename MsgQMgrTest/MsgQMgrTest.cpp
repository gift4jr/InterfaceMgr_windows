/* -----------------------------------------------------
 * File    : MsgQMgrTest.cpp
 * Desc    : Message Queue Test using MsgQMgr
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
 * ----------------------------------------------------- */

#include "MsgQMgrTest.h"

#ifdef __cplusplus  // Indicates to the C++ compiler to compile functions in this block with C-style linkage.
extern "C" {       
#endif

    void start_test(void)
    {
        MsgQMgrTest* pTest = new MsgQMgrTest(); // Create MsgQMgrTest instance on heap
        pTest->start();                         // Start MsgQMgr test

        while (1)                               // Keep main thread alive to allow test completion
        {
            Sleep(1000);
        }
    }

#ifdef __cplusplus
}
#endif

/*
 * Function : MsgQMgrTest::MsgQMgrTest
 * Desc     : Constructor
 * Param    : None
 * Return   : None
 */
MsgQMgrTest::MsgQMgrTest()
    : m_pMsgQ1(NULL), m_pMsgQ2(NULL),
    m_msgQ1rxThread(NULL), m_msgQ1txThread(NULL),
    m_msgQ2rxThread(NULL), m_msgQ2txThread(NULL)
{
}

/*
 * Function : MsgQMgrTest::~MsgQMgrTest
 * Desc     : Destructor
 * Param    : None
 * Return   : None
 */
MsgQMgrTest::~MsgQMgrTest()
{
    delete m_pMsgQ1;
    delete m_pMsgQ2;

    if (m_msgQ1rxThread) 
    {
        WaitForSingleObject(m_msgQ1rxThread, INFINITE);
        CloseHandle(m_msgQ1rxThread);
    }

    if (m_msgQ1txThread) 
    {
        WaitForSingleObject(m_msgQ1txThread, INFINITE);
        CloseHandle(m_msgQ1txThread);
    }

    if (m_msgQ2rxThread) 
    {
        WaitForSingleObject(m_msgQ2rxThread, INFINITE);
        CloseHandle(m_msgQ2rxThread);
    }

    if (m_msgQ2txThread) 
    {
        WaitForSingleObject(m_msgQ2txThread, INFINITE);
        CloseHandle(m_msgQ2txThread);
    }
}

/*
 * Function : MsgQMgrTest::start
 * Desc     : Starts Message Queue Test
 * Param    : None
 * Return   : void
 */
void MsgQMgrTest::start()
{
    m_pMsgQ1 = new MsgQMgr(10000);
    m_msgQ1rxThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)rxTask1Entry, this, 0, NULL);
    m_msgQ1txThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)txTask1Entry, this, 0, NULL);

    Sleep(1);

    m_pMsgQ2 = new MsgQMgr(10000);
    m_msgQ2rxThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)rxTask2Entry, this, 0, NULL);
    m_msgQ2txThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)txTask2Entry, this, 0, NULL);
}

/*
 * Function : MsgQMgrTest::rxTask1Entry
 * Desc     : Static entry point for rxTask1
 * Param    : pThis - pointer to MsgQMgrTest instance
 * Return   : void
 */
void MsgQMgrTest::rxTask1Entry(void* pThis)
{
    MsgQMgrTest* pMsgQMgrTest = (MsgQMgrTest*)pThis;
    pMsgQMgrTest->rxTask1();
}

/*
 * Function : MsgQMgrTest::rxTask1
 * Desc     : Receiver thread 1
 * Param    : None
 * Return   : void
 */
void MsgQMgrTest::rxTask1()
{
    struct ss rrr;

    while (1)
    {
        int ret = m_pMsgQ1->read((char*)(&rrr));
        if (ret > 0)
        {
            printf("m_pMsgQ1 : [len: %d] rrr.a = %d, rrr.b = %d, rrr.c = %s\n", ret, rrr.a, rrr.b, rrr.c);
        }

        if (ret == -2)
            break;  // Queue closed
    }
}

/*
 * Function : MsgQMgrTest::txTask1Entry
 * Desc     : Static entry point for txTask1
 * Param    : pThis - pointer to MsgQMgrTest instance
 * Return   : void
 */
void MsgQMgrTest::txTask1Entry(void* pThis)
{
    MsgQMgrTest* pMsgQMgrTest = (MsgQMgrTest*)pThis;
    pMsgQMgrTest->txTask1();
}

/*
 * Function : MsgQMgrTest::txTask1
 * Desc     : Transmitter thread 1
 * Param    : None
 * Return   : void
 */
void MsgQMgrTest::txTask1()
{
    int cnt = 100;
    struct ss ttt;

    while (1)
    {
        ttt.a = 1;
        ttt.b = cnt++;
        sprintf(ttt.c, "m_pMsgQ1 tx msg");

        int ret = m_pMsgQ1->write((char*)(&ttt), sizeof(ttt));

        if (ret == -2)
            break;  // Queue closed

        Sleep(1);
    }
}

/*
 * Function : MsgQMgrTest::rxTask2Entry
 * Desc     : Static entry point for rxTask2
 * Param    : pThis - pointer to MsgQMgrTest instance
 * Return   : void
 */
void MsgQMgrTest::rxTask2Entry(void* pThis)
{
    MsgQMgrTest* pMsgQMgrTest = (MsgQMgrTest*)pThis;
    pMsgQMgrTest->rxTask2();
}

/*
 * Function : MsgQMgrTest::rxTask2
 * Desc     : Receiver thread 2
 * Param    : None
 * Return   : void
 */
void MsgQMgrTest::rxTask2()
{
    struct ss rrr;

    while (1)
    {
        int ret = m_pMsgQ2->read((char*)(&rrr));
        if (ret > 0)
        {
            printf("m_pMsgQ2 : [len: %d] rrr.a = %d, rrr.b = %d, rrr.c = %s\n", ret, rrr.a, rrr.b, rrr.c);
        }

        if (ret == -2)
            break;  // Queue closed
    }
}

/*
 * Function : MsgQMgrTest::txTask2Entry
 * Desc     : Static entry point for txTask2
 * Param    : pThis - pointer to MsgQMgrTest instance
 * Return   : void
 */
void MsgQMgrTest::txTask2Entry(void* pThis)
{
    MsgQMgrTest* pMsgQMgrTest = (MsgQMgrTest*)pThis;
    pMsgQMgrTest->txTask2();
}

/*
 * Function : MsgQMgrTest::txTask2
 * Desc     : Transmitter thread 2
 * Param    : None
 * Return   : void
 */
void MsgQMgrTest::txTask2()
{
    int cnt = 200;
    struct ss ttt;

    while (1)
    {
        ttt.a = 2;
        ttt.b = cnt++;
        sprintf(ttt.c, "m_pMsgQ2 tx msg");

        int ret = m_pMsgQ2->write((char*)(&ttt), sizeof(ttt));

        if (ret == -2)
            break;  // Queue closed

        Sleep(1);
    }
}
