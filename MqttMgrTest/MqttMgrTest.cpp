/* -----------------------------------------------------
 * File    : MqttMgrTest.cpp
 * Desc    : MQTT Communication Test using MqttMgr
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
 * ----------------------------------------------------- */

#include "MqttMgrTest.h"

#ifdef __cplusplus  // Indicates to the C++ compiler to compile functions in this block with C-style linkage.
extern "C" {
#endif

    void start_test(void)
    {
        MqttMgrTest* pTest = new MqttMgrTest();     // Create MqttMgrTest instance on heap
        pTest->start();                             // Start MqttMgr test

        while (1)                                   // Keep main thread alive to allow test completion
        {
            Sleep(1000);
        }
    }

#ifdef __cplusplus
}
#endif

/*
 * Function : MqttMgrTest::MqttMgrTest
 * Desc     : Constructor
 * Param    : None
 * Return   : None
 */
MqttMgrTest::MqttMgrTest()
	: m_pMqtt1(NULL), m_pMqtt2(NULL),
	m_thread(NULL), m_threadAlive(false)
{
}

/*
 * Function : MqttMgrTest::~MqttMgrTest
 * Desc     : Destructor
 * Param    : None
 * Return   : None
 */
MqttMgrTest::~MqttMgrTest()
{
    m_threadAlive = false;

    delete m_pMqtt1;
    delete m_pMqtt2;

    if (m_thread)
    {
        WaitForSingleObject(m_thread, INFINITE);
        CloseHandle(m_thread);
        m_thread = NULL;
    }
}

/*
 * Function : MqttMgrTest::start
 * Desc     : Starts MQTT Test
 * Param    : None
 * Return   : None
 */
void MqttMgrTest::start()
{
    m_threadAlive = true;

    m_pMqtt1 = new MqttMgr((char*)"T1", 1000);      // Create MqttMgr instance 1 as subscriber for T1 topic
    m_pMqtt1->setCallback(recvMsgFromMqtt1, this);  // Register message callback
    m_pMqtt1->initMqtt((char*)"127.0.0.1");         // Initializes MQTT with Broker Info

	m_pMqtt2 = new MqttMgr((char*)"T2", 1000);      // Create MqttMgr instance 2 as subscriber for T2 topic
    m_pMqtt2->setCallback(recvMsgFromMqtt2, this);  // Register message callback
    m_pMqtt2->initMqtt((char*)"127.0.0.1");         // Initializes MQTT with Broker Info

	// Create transmitter thread
    m_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)txTaskEntry, this, 0, NULL);
}

/*
 * Function : MqttMgrTest::txTaskEntry
 * Desc     : Static entry point for txTask
 * Param    : pThis - pointer to MqttMgrTest instance
 * Return   : None
 */
void MqttMgrTest::txTaskEntry(void* pThis)
{
    MqttMgrTest* pMqttMgrTest = (MqttMgrTest*)pThis;
    pMqttMgrTest->txTask();
}

/*
 * Function : MqttMgrTest::txTask
 * Desc     : Transmitter thread
 * Param    : None
 * Return   : None
 */
void MqttMgrTest::txTask()
{
    int cnt = 1;

    char buf1[1000] = "";  
    char buf2[1000] = "";

    while (m_threadAlive)
    {
        sprintf(buf1, "T2 topic message from T1 #%d", cnt++);
        m_pMqtt1->sendMsg((char*)"T2", buf1, 1000);         // Publish T2 topic

        sprintf(buf2, "T1 topic message from T2 #%d", cnt++);
        m_pMqtt2->sendMsg((char*)"T1", buf2, 1000);         // Publish T1 topic

        Sleep(1);
    }
}

/*
 * Function : MqttMgrTest::recvMsgFromMqtt1
 * Desc     : Callback Function for receiving message from MqttMgr instance 1
 * Param    : pThis - pointer to MqttMgrTest instance
 *            rcvTime - timestamp of message reception
 *            msg - received message
 *            msgLen - message length
 * Return   : None
 */
void MqttMgrTest::recvMsgFromMqtt1(void* pThis, STimeInfo rcvTime, char* msg, int msgLen)
{
    MqttMgrTest* pMqttMgrTest = (MqttMgrTest*)pThis;

    printf("[m_pMqtt1][%04d/%02d/%02d][%02d:%02d:%02d.%03d][%s][%d bytes]\n",
        rcvTime.usYear, rcvTime.ucMonth, rcvTime.ucDay,
        rcvTime.ucHour, rcvTime.ucMin, rcvTime.ucSec, rcvTime.uiMiliSec,
        msg, msgLen);
}

/*
 * Function : MqttMgrTest::recvMsgFromMqtt2
 * Desc     : Callback Function for receiving message from MqttMgr instance 2
 * Param    : pThis - pointer to MqttMgrTest instance
 *            rcvTime - timestamp of message reception
 *            msg - received message
 *            msgLen - message length
 * Return   : None
 */
void MqttMgrTest::recvMsgFromMqtt2(void* pThis, STimeInfo rcvTime, char* msg, int msgLen)
{
    MqttMgrTest* pMqttMgrTest = (MqttMgrTest*)pThis;

    printf("[m_pMqtt2][%04d/%02d/%02d][%02d:%02d:%02d.%03d][%s][%d bytes]\n",
        rcvTime.usYear, rcvTime.ucMonth, rcvTime.ucDay,
        rcvTime.ucHour, rcvTime.ucMin, rcvTime.ucSec, rcvTime.uiMiliSec,
        msg, msgLen);
}
