/* -----------------------------------------------------
 * File    : UdpMgrTest.cpp
 * Desc    : UDP Communication Test using UdpMgr
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
 * ----------------------------------------------------- */

#include "UdpMgrTest.h"

#ifdef __cplusplus  // Indicates to the C++ compiler to compile functions in this block with C-style linkage.
extern "C" {
#endif

    void start_test(void)
    {
        UdpMgrTest* pTest = new UdpMgrTest();   // Create UdpMgrTest instance on heap
        pTest->start();                         // Start the UdpMgr test

        while (1)                               // Keep main thread alive to allow test completion
        {
            Sleep(1000);
        }
    }

#ifdef __cplusplus
}
#endif

/*
 * Function : UdpMgrTest::UdpMgrTest
 * Desc     : Constructor
 * Param    : None
 * Return   : None
 */
UdpMgrTest::UdpMgrTest()
    : m_pUdp1(NULL), m_pUdp2(NULL),
    m_thread(NULL), m_threadAlive(false),
    m_multicastFlag(false)
{
}

/*
 * Function : UdpMgrTest::~UdpMgrTest
 * Desc     : Destructor
 * Param    : None
 * Return   : None
 */
UdpMgrTest::~UdpMgrTest()
{
    m_threadAlive = false;

    delete m_pUdp1;
    delete m_pUdp2;

    if (m_thread)
    {
        WaitForSingleObject(m_thread, INFINITE);
        CloseHandle(m_thread);
        m_thread = NULL;
    }
}

/*
 * Function : UdpMgrTest::start
 * Desc     : Starts UDP Communication Test
 * Param    : None
 * Return   : None
 */
void UdpMgrTest::start()
{
    m_threadAlive = true;

    printf("Starting UDP communication(m_pUdp1 <-> m_pUdp2) after 3 seconds...\n");
    Sleep(3000);

    /////////////////////////////////////////////////////////////
    m_multicastFlag = false;

    SUdpMgrInfo stUdpMgrInfo;

    // Setup and start m_pUdp1
    strcpy(stUdpMgrInfo.src_ip, "127.0.0.1");
    strcpy(stUdpMgrInfo.dst_ip, "127.0.0.1");
    stUdpMgrInfo.src_port = 20000;
    stUdpMgrInfo.dst_port = 20001;

    m_pUdp1 = new UdpMgr(10000);                        // Create UdpMgr instance 1
    m_pUdp1->setCallback(recvMsgFromUdp1, this);        // Register message callback
	m_pUdp1->startUdp(stUdpMgrInfo);                    // Start UDP 1

    // Setup and start m_pUdp2
    strcpy(stUdpMgrInfo.src_ip, "127.0.0.1");
    strcpy(stUdpMgrInfo.dst_ip, "127.0.0.1");
    stUdpMgrInfo.src_port = 20001;
    stUdpMgrInfo.dst_port = 20000;

	m_pUdp2 = new UdpMgr(10000);    		            // Create UdpMgr instance 2    
	m_pUdp2->setCallback(recvMsgFromUdp2, this);        // Register message callback
	m_pUdp2->startUdp(stUdpMgrInfo);                    // Start UDP 2

    // Create transmitter thread
    m_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)txTaskEntry, this, 0, NULL);

    Sleep(3000);

    m_pUdp1->stopUdp();
    m_pUdp2->stopUdp();

    printf("Stopping UDP communication...\n");
    printf("Restarting UDP communication(m_pUdp1 <-> m_pUdp2) after 3 seconds...\n");

    Sleep(3000);

    // Restart with new ports
    stUdpMgrInfo.src_port = 40000;
    stUdpMgrInfo.dst_port = 40001;
    m_pUdp1->startUdp(stUdpMgrInfo);

    stUdpMgrInfo.src_port = 40001;
    stUdpMgrInfo.dst_port = 40000;
    m_pUdp2->startUdp(stUdpMgrInfo);

    Sleep(3000);

    m_pUdp1->stopUdp();
    m_pUdp2->stopUdp();

    printf("Stopping UDP communication...\n");
    printf("Starting Multicast UDP communication(m_pUdp1 -> m_pUdp2) after 3 seconds...\n");

    Sleep(3000);

    /////////////////////////////////////////////////////////////
	m_multicastFlag = true;

    SUdpMgrInfo stMulticastInfo;

    stMulticastInfo.multicast_flag = 1; // Enable multicast sending
    strcpy(stMulticastInfo.multi_ip, "224.100.100.1");
    stMulticastInfo.multi_port = 20001;

    m_pUdp1 = new UdpMgr(10000);
    m_pUdp1->startUdp(stMulticastInfo);

    /////////////////////////////////////////////////////////////
    stMulticastInfo.multicast_flag = 2; // Enable multicast receiving
    strcpy(stMulticastInfo.multi_ip, "224.100.100.1");
    stMulticastInfo.multi_port = 20001;

    m_pUdp2 = new UdpMgr(10000);
    m_pUdp2->setCallback(recvMsgFromUdp2, this);
    m_pUdp2->startUdp(stMulticastInfo);

    /////////////////////////////////////////////////////////////
}

/*
 * Function : UdpMgrTest::txTaskEntry
 * Desc     : Static entry point for txTask
 * Param    : pThis - pointer to UdpMgrTest instance
 * Return   : None
 */
void UdpMgrTest::txTaskEntry(void* pThis)
{
    UdpMgrTest* pUdpMgrTest = (UdpMgrTest*)pThis;
    pUdpMgrTest->txTask();
}

/*
 * Function : UdpMgrTest::txTask
 * Desc     : Transmitter thread
 * Param    : None
 * Return   : None
 */
void UdpMgrTest::txTask()
{
    int ret;
    int cnt = 1;
    char buf[1000];

    while (m_threadAlive)
    {
        sprintf(buf, "test %d", cnt++);
        m_pUdp1->sendMsg(buf, strlen(buf) + 1);
        Sleep(10);

        if (!m_multicastFlag)
        {
            sprintf(buf, "test %d", cnt++);
            m_pUdp2->sendMsg(buf, strlen(buf) + 1);
            Sleep(10);
        }
    }
}

/*
 * Function : UdpMgrTest::recvMsgFromUdp1
 * Desc     : Callback function for receiving message from UdpMgr instance 1
 * Param    : pThis   - pointer to UdpMgrTest instance
 *            rcvTime - received time information
 *            msg     - received message buffer
 *            msgLen  - length of received message
 * Return   : None
 */
void UdpMgrTest::recvMsgFromUdp1(void* pThis, STimeInfo rcvTime, char* msg, int msgLen)
{
    UdpMgrTest* pUdpMgrTest = (UdpMgrTest*)pThis;

    printf("[m_pUdp1][%04d/%02d/%02d][%02d:%02d:%02d.%03d][%s][%d bytes]\n",
        rcvTime.usYear, rcvTime.ucMonth, rcvTime.ucDay,
        rcvTime.ucHour, rcvTime.ucMin, rcvTime.ucSec, rcvTime.uiMiliSec,
        msg, msgLen);
}

/*
 * Function : UdpMgrTest::recvMsgFromUdp2
 * Desc     : Callback function for receiving message from UdpMgr instance 2
 * Param    : pThis   - pointer to UdpMgrTest instance
 *            rcvTime - received time information
 *            msg     - received message buffer
 *            msgLen  - length of received message
 * Return   : None
 */
void UdpMgrTest::recvMsgFromUdp2(void* pThis, STimeInfo rcvTime, char* msg, int msgLen)
{
    UdpMgrTest* pUdpMgrTest = (UdpMgrTest*)pThis;

    printf("[m_pUdp2][%04d/%02d/%02d][%02d:%02d:%02d.%03d][%s][%d bytes]\n",
        rcvTime.usYear, rcvTime.ucMonth, rcvTime.ucDay,
        rcvTime.ucHour, rcvTime.ucMin, rcvTime.ucSec, rcvTime.uiMiliSec,
        msg, msgLen);
}