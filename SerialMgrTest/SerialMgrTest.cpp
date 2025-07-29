/* -----------------------------------------------------
 * File    : SerialMgrTest.cpp
 * Desc    : Serial Communication Test using SerialMgr
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
 * ----------------------------------------------------- */

#include "SerialMgrTest.h"

#ifdef __cplusplus  // Indicates to the C++ compiler to compile functions in this block with C-style linkage.
extern "C" {
#endif

    void start_test(void)
    {
        SerialMgrTest* pTest = new SerialMgrTest();  // Create SerialMgrTest instance on heap
        pTest->start();                              // Start the SerialMgr test

        while (1)                                    // Keep main thread alive to allow test completion
        {
            Sleep(1000);
        }
    }

#ifdef __cplusplus
}
#endif

/*
 * Function : SerialMgrTest::SerialMgrTest
 * Desc     : Constructor
 * Param    : None
 * Return   : None
 */
SerialMgrTest::SerialMgrTest(void)
    : m_pSerial1(NULL), m_pSerial2(NULL),
    m_thread(NULL), m_threadAlive(false)
{
}

/*
 * Function : SerialMgrTest::~SerialMgrTest
 * Desc     : Destructor
 * Param    : None
 * Return   : None
 */
SerialMgrTest::~SerialMgrTest(void)
{
    m_threadAlive = false;

    delete m_pSerial1;
    delete m_pSerial2;

    if (m_thread)
    {
        WaitForSingleObject(m_thread, INFINITE);  
        CloseHandle(m_thread);                    
        m_thread = NULL;
    }
}

/*
 * Function : SerialMgrTest::start
 * Desc     : Starts Serial Communication Test
 * Param    : None
 * Return   : None
 */
void SerialMgrTest::start()
{
    m_threadAlive = true;

    printf("Starting Serial communication(m_pSerial1 <-> m_pSerial2) after 3 seconds...\n");
    Sleep(3000);  

    // Setup and start SerialMgr instance 
    SSerialMgrInfo stSerialLibInfo;
    stSerialLibInfo.baudRate = 38400;
    stSerialLibInfo.byteSize = 8;
    stSerialLibInfo.parity = 0;
    stSerialLibInfo.stopBit = 0;

    sprintf(stSerialLibInfo.serial_port, "COM2");

    m_pSerial1 = new SerialMgr(1000);                   // Create SerialMgr instance 1
    m_pSerial1->setCallback(recvMsgFromSerial1, this);  // Register message callback
    m_pSerial1->startSerial(stSerialLibInfo);           // Start Serial 1

    sprintf(stSerialLibInfo.serial_port, "COM3");

	m_pSerial2 = new SerialMgr(1000);                   // Create SerialMgr instance 2  
    m_pSerial2->setCallback(recvMsgFromSerial2, this);  // Register message callback
    m_pSerial2->startSerial(stSerialLibInfo);           // Start Serial 2

    // Create transmitter thread
    m_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)txTaskEntry, this, 0, NULL);

    Sleep(3000); 

    m_pSerial1->stopSerial();
    m_pSerial2->stopSerial();

    printf("Stopping Serial communication...\n");
    printf("Restarting Serial communication(m_pSerial1 <-> m_pSerial2) after 3 seconds...\n");
    Sleep(3000);

    sprintf(stSerialLibInfo.serial_port, "COM2");
    m_pSerial1->startSerial(stSerialLibInfo);

    sprintf(stSerialLibInfo.serial_port, "COM3");
    m_pSerial2->startSerial(stSerialLibInfo);
}

/*
 * Function : SerialMgrTest::txTaskEntry
 * Desc     : Static entry point for txTask
 * Param    : pThis - pointer to SerialMgrTest instance
 * Return   : None
 */
void SerialMgrTest::txTaskEntry(void* pThis)
{
    SerialMgrTest* pSerialLibTest = (SerialMgrTest*)pThis;
    pSerialLibTest->txTask();  
}

/*
 * Function : SerialMgrTest::txTask
 * Desc     : Transmitter thread
 * Param    : None
 * Return   : None
 */
void SerialMgrTest::txTask()
{
    int ret;
    int cnt = 1;
    char buf[10000];

    while (m_threadAlive)
    {
        sprintf(buf, "test %d", cnt);

        m_pSerial1->sendMsg(buf, 1000);  
        m_pSerial2->sendMsg(buf, 1000);  

        Sleep(1);  
        cnt++;
    }
}

/*
 * Function : SerialMgrTest::recvMsgFromSerial1
 * Desc     : Callback function for receiving message from SerialMgr instance 1
 * Param    : pThis   - pointer to SerialMgrTest instance
 *            rcvTime - reception timestamp
 *            msg     - received message
 *            msgLen  - message length
 * Return   : None
 */
void SerialMgrTest::recvMsgFromSerial1(void* pThis, STimeInfo rcvTime, char* msg, int msgLen)
{
    SerialMgrTest* pSerialLibTest = (SerialMgrTest*)pThis;

    printf("[m_pSerial1][%04d/%02d/%02d][%02d:%02d:%02d.%03d][%s][%d bytes]\n",
        rcvTime.usYear, rcvTime.ucMonth, rcvTime.ucDay,
        rcvTime.ucHour, rcvTime.ucMin, rcvTime.ucSec, rcvTime.uiMiliSec,
        msg, msgLen);
}

/*
 * Function : SerialMgrTest::recvMsgFromSerial2
 * Desc     : Callback function for receiving message from SerialMgr instance 2
 * Param    : pThis   - pointer to SerialMgrTest instance
 *            rcvTime - reception timestamp
 *            msg     - received message
 *            msgLen  - message length
 * Return   : None
 */
void SerialMgrTest::recvMsgFromSerial2(void* pThis, STimeInfo rcvTime, char* msg, int msgLen)
{
    SerialMgrTest* pSerialLibTest = (SerialMgrTest*)pThis;

    printf("[m_pSerial2][%04d/%02d/%02d][%02d:%02d:%02d.%03d][%s][%d bytes]\n",
        rcvTime.usYear, rcvTime.ucMonth, rcvTime.ucDay,
        rcvTime.ucHour, rcvTime.ucMin, rcvTime.ucSec, rcvTime.uiMiliSec,
        msg, msgLen);
}
