/* -----------------------------------------------------
 * File    : TcpClientMgrTest.cpp
 * Desc    : TCP Client Communication Test using TcpClientMgr
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
 * ----------------------------------------------------- */

#include "TcpClientMgrTest.h"

#ifdef __cplusplus  // Indicates to the C++ compiler to compile functions in this block with C-style linkage.
extern "C" {
#endif

    void start_test(void)
    {
        TcpClientMgrTest* pTest = new TcpClientMgrTest();  // Create TcpClientMgrTest instance on heap
        pTest->start();                                    // Start the TcpClientMgr test

        while (1)                                          // Keep main thread alive to allow test completion
        {
            Sleep(1000);
        }
    }

#ifdef __cplusplus
}
#endif

/*
 * Function : TcpClientMgrTest::TcpClientMgrTest
 * Desc     : Constructor
 * Param    : None
 * Return   : None
 */
TcpClientMgrTest::TcpClientMgrTest()
    : m_pTcpClient1(NULL), m_pTcpClient2(NULL),
    m_thread(NULL), m_threadAlive(false)
{
}

/*
 * Function : TcpClientMgrTest::~TcpClientMgrTest
 * Desc     : Destructor
 * Param    : None
 * Return   : None
 */
TcpClientMgrTest::~TcpClientMgrTest()
{
    m_threadAlive = false;

    delete m_pTcpClient1;
    delete m_pTcpClient2;

    if (m_thread)
    {
        WaitForSingleObject(m_thread, INFINITE);  
        CloseHandle(m_thread);                    
        m_thread = NULL;
    }
}

/*
 * Function : TcpClientMgrTest::start
 * Desc     : Starts TCP Client Communication Test
 * Param    : None
 * Return   : None
 */
void TcpClientMgrTest::start()
{
    m_threadAlive = true;

    /////////////////////////////////////////////////////////////////
    STcpClientMgrInfo stTcpClientMgrInfo1;
    strcpy(stTcpClientMgrInfo1.server_ip, "127.0.0.1");             
	//strcpy(stTcpClientMgrInfo1.server_ip, "59.25.241.80");
    stTcpClientMgrInfo1.server_port = 8080;                         

	//stTcpClientMgrInfo1.client_bind_flag = 1;
	//strcpy(stTcpClientMgrInfo1.client_ip, "127.0.0.1");
	//stTcpClientMgrInfo1.client_port = 6000;

    m_pTcpClient1 = new TcpClientMgr(20000);                        // Create TcpClientMgr instance 1
    m_pTcpClient1->setCallback(recvStatusFromTcpClient1, this);     // Register status callback
    m_pTcpClient1->setCallback(recvMsgFromTcpClient1, this);        // Register message callback
	m_pTcpClient1->startTcpClient(stTcpClientMgrInfo1);             // Start TCP Client 1

    /////////////////////////////////////////////////////////////////
    STcpClientMgrInfo stTcpClientMgrInfo2;
    strcpy(stTcpClientMgrInfo2.server_ip, "127.0.0.1");
	//strcpy(stTcpClientMgrInfo2.server_ip, "59.25.241.80");
    stTcpClientMgrInfo2.server_port = 9090;

    m_pTcpClient2 = new TcpClientMgr(20000);                        // Create TcpClientMgr instance 2
    m_pTcpClient2->setCallback(recvStatusFromTcpClient2, this);     // Register status callback
    m_pTcpClient2->setCallback(recvMsgFromTcpClient2, this);        // Register message callback
    m_pTcpClient2->startTcpClient(stTcpClientMgrInfo2);             // Start TCP Client 2
	///////////////////////////////////////////////////////////////

    // Create transmitter thread
    m_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)txTaskEntry, this, 0, NULL); 

    //Sleep(5000);                      

    //m_pTcpClient1->stopTcpClient();                     // Stop TCP client 1
    //m_pTcpClient2->stopTcpClient();                     // Stop TCP client 2

    //Sleep(5000);            

    //m_pTcpClient1->startTcpClient(stTcpClientMgrInfo1);  // Restart TCP client 1
    //m_pTcpClient2->startTcpClient(stTcpClientMgrInfo2);  // Restart TCP client 2
}

/*
 * Function : TcpClientMgrTest::txTaskEntry
 * Desc     : Static entry point for txTask
 * Param    : pThis - pointer to TcpClientMgrTest instance
 * Return   : None
 */
void TcpClientMgrTest::txTaskEntry(void* pThis)
{
    TcpClientMgrTest* pTcpClientTest = (TcpClientMgrTest*)pThis;
    pTcpClientTest->txTask();  
}

/*
 * Function : TcpClientMgrTest::txTask
 * Desc     : Transmitter thread
 * Param    : None
 * Return   : None
 */
void TcpClientMgrTest::txTask()
{
    int cnt = 1;
    char buf[20000];

    while (m_threadAlive)
    {
        sprintf(buf, "Client to Server : test %d", cnt++);
        m_pTcpClient1->sendMsg(buf, sizeof(buf));  

        Sleep(1);

        sprintf(buf, "Client to Server : test %d", cnt++);
        m_pTcpClient2->sendMsg(buf, sizeof(buf));  

        Sleep(1);
    }
}

/*
 * Function : TcpClientMgrTest::recvStatusFromTcpClient1
 * Desc     : Callback function for receiving status from TcpClientMgr instance 1
 * Param    : pThis - pointer to TcpClientMgrTest instance
 *            status - connection status code
 * Return   : None
 */
void TcpClientMgrTest::recvStatusFromTcpClient1(void* pThis, int status)
{
    printf("[m_pTcpClient1][status : %d]\n", status);
}

/*
 * Function : TcpClientMgrTest::recvStatusFromTcpClient2
 * Desc     : Callback function for receiving status from TcpClientMgr instance 2
 * Param    : pThis - pointer to TcpClientMgrTest instance
 *            status - connection status code
 * Return   : None
 */
void TcpClientMgrTest::recvStatusFromTcpClient2(void* pThis, int status)
{
    printf("[m_pTcpClient2][status : %d]\n", status);
}

/*
 * Function : TcpClientMgrTest::recvMsgFromTcpClient1
 * Desc     : Callback function for receiving message from TcpClientMgr instance 1
 * Param    : pThis   - pointer to TcpClientMgrTest instance
 *            rcvTime - time of message receipt
 *            msg     - received message
 *            msgLen  - message length
 * Return   : None
 */
void TcpClientMgrTest::recvMsgFromTcpClient1(void* pThis, STimeInfo rcvTime, char* msg, int msgLen)
{
    TcpClientMgrTest* pTcpClientTest = (TcpClientMgrTest*)pThis;

    printf("[m_pTcpClient1][%04d/%02d/%02d][%02d:%02d:%02d.%03d][%s][%d bytes]\n",
        rcvTime.usYear, rcvTime.ucMonth, rcvTime.ucDay,
        rcvTime.ucHour, rcvTime.ucMin, rcvTime.ucSec, rcvTime.uiMiliSec,
        msg, msgLen);
}

/*
 * Function : TcpClientMgrTest::recvMsgFromTcpClient2
 * Desc     : Callback function for receiving message from TcpClientMgr instance 2
 * Param    : pThis   - pointer to TcpClientMgrTest instance
 *            rcvTime - time of message receipt
 *            msg     - received message
 *            msgLen  - message length
 * Return   : None
 */
void TcpClientMgrTest::recvMsgFromTcpClient2(void* pThis, STimeInfo rcvTime, char* msg, int msgLen)
{
    TcpClientMgrTest* pTcpClientTest = (TcpClientMgrTest*)pThis;

    printf("[m_pTcpClient2][%04d/%02d/%02d][%02d:%02d:%02d.%03d][%s][%d bytes]\n",
        rcvTime.usYear, rcvTime.ucMonth, rcvTime.ucDay,
        rcvTime.ucHour, rcvTime.ucMin, rcvTime.ucSec, rcvTime.uiMiliSec,
        msg, msgLen);
}
