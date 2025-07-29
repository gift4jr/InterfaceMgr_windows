/* -----------------------------------------------------
 * File    : TcpServerMgrTest.cpp
 * Desc    : TCP Server Communication Test using TcpServerMgr
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
 * ----------------------------------------------------- */

#include "TcpServerMgrTest.h"

#ifdef __cplusplus  // Indicates to the C++ compiler to compile functions in this block with C-style linkage.
extern "C" {
#endif

	void start_test(void)
	{
		TcpServerMgrTest* pTest = new TcpServerMgrTest();   // Create TcpServerMgrTest instance on heap
		pTest->start();										// Start the TCP server test

		while (1)											// Keep main thread alive to allow test completion
		{
			Sleep(1000);
		}
	}

#ifdef __cplusplus
}
#endif

/*
 * Function : TcpServerMgrTest::TcpServerMgrTest
 * Desc     : Constructor
 * Param    : None
 * Return   : None
 */
TcpServerMgrTest::TcpServerMgrTest()
	: m_pTcpServer1(NULL), m_pTcpServer2(NULL)
{
}

/*
 * Function : TcpServerMgrTest::~TcpServerMgrTest
 * Desc     : Destructor
 * Param    : None
 * Return   : None
 */
TcpServerMgrTest::~TcpServerMgrTest()
{
	delete m_pTcpServer1;
	delete m_pTcpServer2;
}

/*
 * Function : TcpServerMgrTest::start
 * Desc     : Starts TCP Server Communication Test
 * Param    : None
 * Return   : None
 */
void TcpServerMgrTest::start()
{
	///////////////////////////////////////////////////////////////////
	STcpServerMgrInfo stTcpServerMgrInfo1;
	strcpy(stTcpServerMgrInfo1.server_ip, "127.0.0.1");
	//strcpy(stTcpServerMgrInfo1.server_ip, "192.168.0.3");
	stTcpServerMgrInfo1.server_port = 8080;

	//stTcpServerMgrInfo1.client_bind_flag = 1;
	//strcpy(stTcpServerMgrInfo1.client_ip, "127.0.0.1");
	//stTcpServerMgrInfo1.client_port = 6000;

	m_pTcpServer1 = new TcpServerMgr(20000);						// Create TcpServerMgr instance 1
	m_pTcpServer1->setCallback(recvStatusFromTcpServer1, this);		// Register status callback
	m_pTcpServer1->setCallback(recvMsgFromTcpServer1, this);		// Register message callback
	m_pTcpServer1->startTcpServer(stTcpServerMgrInfo1);				// Start TCP Server 1
	///////////////////////////////////////////////////////////////////
	STcpServerMgrInfo stTcpServerMgrInfo2;
	strcpy(stTcpServerMgrInfo2.server_ip, "127.0.0.1");
	stTcpServerMgrInfo2.server_port = 9090;

	//stTcpServerMgrInfo2.client_bind_flag = 1;
	//strcpy(stTcpServerMgrInfo2.client_ip, "127.0.0.1");
	//stTcpServerMgrInfo2.client_port = 6000;

	m_pTcpServer2 = new TcpServerMgr(20000);						// Create TcpServerMgr instance 2
	m_pTcpServer2->setCallback(recvStatusFromTcpServer2, this);		// Register status callback
	m_pTcpServer2->setCallback(recvMsgFromTcpServer2, this);		// Register message callback
	m_pTcpServer2->startTcpServer(stTcpServerMgrInfo2);				// Start TCP Server 2
	///////////////////////////////////////////////////////////////////

	//Sleep(5000);
	
	//m_pTcpServer1->stopTcpServer();
	//m_pTcpServer2->stopTcpServer();
	
	//Sleep(5000);
	
	//m_pTcpServer1->startTcpServer(stTcpServerMgrInfo1);
	//m_pTcpServer2->startTcpServer(stTcpServerMgrInfo2);
}

/*
 * Function : TcpServerMgrTest::recvStatusFromTcpServer1
 * Desc     : Callback function for receiving status from TcpServerMgr instance 1 
 * Param    : pThis       - pointer to TcpServerMgrTest instance
 *            client_ip   - IP of the client
 *            client_port - port of the client
 *            status      - connection status code
 * Return   : None
 */
void TcpServerMgrTest::recvStatusFromTcpServer1(void* pThis, char* client_ip, int client_port, int status)
{
	printf("[m_pTcpServer1](Client %s:%d)[status : %d]\n", client_ip, client_port, status);
}

/*
 * Function : TcpServerMgrTest::recvStatusFromTcpServer2
 * Desc     : Callback function for receiving status from TcpServerMgr instance 2
 * Param    : pThis       - pointer to TcpServerMgrTest instance
 *            client_ip   - IP of the client
 *            client_port - port of the client
 *            status      - connection status code
 * Return   : None
 */
void TcpServerMgrTest::recvStatusFromTcpServer2(void* pThis, char* client_ip, int client_port, int status)
{
	printf("[m_pTcpServer2](Client %s:%d)[status : %d]\n", client_ip, client_port, status);
}

/*
 * Function : TcpServerMgrTest::recvMsgFromTcpServer1
 * Desc     : Callback function for receiving message from TcpServerMgr instance 1 
 * Param    : pThis       - pointer to TcpServerMgrTest instance
 *            client_ip   - IP of the client
 *            client_port - port of the client
 *            rcvTime     - receive timestamp
 *            msg         - received message buffer
 *            msgLen      - length of received message
 * Return   : None
 */
void TcpServerMgrTest::recvMsgFromTcpServer1(void* pThis, char* client_ip, int client_port, STimeInfo rcvTime, char* msg, int msgLen)
{
	TcpServerMgrTest* pTcpServerMgrTest = (TcpServerMgrTest*)pThis;

	printf("[m_pTcpServer1][%04d/%02d/%02d][%02d:%02d:%02d.%03d](Client %s:%d)[%s][%d bytes]\n",
		rcvTime.usYear, rcvTime.ucMonth, rcvTime.ucDay,
		rcvTime.ucHour, rcvTime.ucMin, rcvTime.ucSec, rcvTime.uiMiliSec,
		client_ip, client_port, msg, msgLen);

	char buffer[20000] = { 0 };

	snprintf(buffer, sizeof(buffer), "Server to Client(%s:%d)", client_ip, client_port);
	pTcpServerMgrTest->m_pTcpServer1->sendMsg(client_ip, client_port, buffer, sizeof(buffer));
}

/*
 * Function : TcpServerMgrTest::recvMsgFromTcpServer2
 * Desc     : Callback function for receiving message from TcpServerMgr instance 2
 * Param    : pThis       - pointer to TcpServerMgrTest instance
 *            client_ip   - IP of the client
 *            client_port - port of the client
 *            rcvTime     - receive timestamp
 *            msg         - received message buffer
 *            msgLen      - length of received message
 * Return   : None
 */
void TcpServerMgrTest::recvMsgFromTcpServer2(void* pThis, char* client_ip, int client_port, STimeInfo rcvTime, char* msg, int msgLen)
{
	TcpServerMgrTest* pTcpServerMgrTest = (TcpServerMgrTest*)pThis;

	printf("[m_pTcpServer2][%04d/%02d/%02d][%02d:%02d:%02d.%03d](Client %s:%d)[%s][%d bytes]\n",
		rcvTime.usYear, rcvTime.ucMonth, rcvTime.ucDay,
		rcvTime.ucHour, rcvTime.ucMin, rcvTime.ucSec, rcvTime.uiMiliSec,
		client_ip, client_port, msg, msgLen);

	char buffer[20000] = { 0 };

	snprintf(buffer, sizeof(buffer), "Server to Client(%s:%d)", client_ip, client_port);
	pTcpServerMgrTest->m_pTcpServer2->sendMsg(client_ip, client_port, buffer, sizeof(buffer));
}
