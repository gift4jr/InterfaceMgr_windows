/* -----------------------------------------------------
 * File    : PipeMgrTest.cpp
 * Desc    : Pipe Communication Test using PipeMgr
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
 * ----------------------------------------------------- */

#include "PipeMgrTest.h"

#ifdef __cplusplus  // Indicates to the C++ compiler to compile functions in this block with C-style linkage.
extern "C" {
#endif

	void start_test(void)
	{
		PipeMgrTest* pTest = new PipeMgrTest();   // Create PipeMgrTest instance on heap
		pTest->start();                           // Start the PipeMgr test

		while (1)								  // Keep main thread alive to allow test completion
		{
			Sleep(1000);
		}
	}

#ifdef __cplusplus
}
#endif

/*
 * Function : PipeMgrTest::PipeMgrTest
 * Desc     : Constructor
 * Param    : None
 * Return   : None
 */
PipeMgrTest::PipeMgrTest()
	: m_pPipe1(NULL), m_pPipe2(NULL),
	m_pipe1rxThread(NULL), m_pipe1txThread(NULL),
	m_pipe2rxThread(NULL), m_pipe2txThread(NULL),
	m_threadAlive(false)
{
}

/*
 * Function : PipeMgrTest::~PipeMgrTest
 * Desc     : Destructor
 * Param    : None
 * Return   : None
 */
PipeMgrTest::~PipeMgrTest()
{
	m_threadAlive = false;

	delete m_pPipe1;
	delete m_pPipe2;

	if (m_pipe1rxThread)
	{
		WaitForSingleObject(m_pipe1rxThread, INFINITE);
		CloseHandle(m_pipe1rxThread);
		m_pipe1rxThread = NULL;
	}

	if (m_pipe1txThread)
	{
		WaitForSingleObject(m_pipe1txThread, INFINITE);
		CloseHandle(m_pipe1txThread);
		m_pipe1txThread = NULL;
	}

	if (m_pipe2rxThread)
	{
		WaitForSingleObject(m_pipe2rxThread, INFINITE);
		CloseHandle(m_pipe2rxThread);
		m_pipe2rxThread = NULL;
	}

	if (m_pipe2txThread)
	{
		WaitForSingleObject(m_pipe2txThread, INFINITE);
		CloseHandle(m_pipe2txThread);
		m_pipe2txThread = NULL;
	}
}

/*
 * Function : PipeMgrTest::start
 * Desc     : Starts Pipe Communication Test
 * Param    : None
 * Return   : None
 */
void PipeMgrTest::start()
{
	m_threadAlive = true;

	int sel = 0; 

	if (sel == 0)		// 0: ITC build -> generates PipeMgrTest.exe
	{
		m_pPipe1 = new PipeMgr(10000);			// test for ITC	(m_pipe1rxThread <- m_pipe1txThread)
		m_pipe1rxThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)rxTask1Entry, this, 0, NULL);
		m_pipe1txThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)txTask1Entry, this, 0, NULL);

		Sleep(1);

		m_pPipe2 = new PipeMgr(10000);			// test for ITC (m_pipe2rxThread <- m_pipe2txThread)
		m_pipe2rxThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)rxTask2Entry, this, 0, NULL);
		m_pipe2txThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)txTask2Entry, this, 0, NULL);
	}
	else if (sel == 1)	// 1: IPC (Node1 build) test -> change file name to PipeMgrTest1.exe after build
	{
		m_pPipe1 = new PipeMgr(10000, 1, 2);	// test for IPC (m_pipe1rxThread <- m_pipe2txThread)	

		m_pipe1rxThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)rxTask1Entry, this, 0, NULL);
		m_pipe1txThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)txTask1Entry, this, 0, NULL);
	}
	else if (sel == 2)	// 2: IPC (Node2 build) test -> change file name to PipeMgrTest2.exe after build
	{
		m_pPipe2 = new PipeMgr(10000, 2, 1);	// test for IPC (m_pipe2rxThread <- m_pipe1txThread)
		m_pipe2rxThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)rxTask2Entry, this, 0, NULL);
		m_pipe2txThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)txTask2Entry, this, 0, NULL);
	}
}

/*
 * Function : PipeMgrTest::rxTask1Entry
 * Desc     : Static entry point for rxTask1
 * Param    : pThis - pointer to PipeMgrTest instance
 * Return   : None
 */
void PipeMgrTest::rxTask1Entry(void* pThis)
{
	PipeMgrTest* pPipeMgrTest = (PipeMgrTest*)pThis;
	pPipeMgrTest->rxTask1();
}

/*
 * Function : PipeMgrTest::rxTask1
 * Desc     : Receiver thread 1
 * Param    : None
 * Return   : None
 */
void PipeMgrTest::rxTask1()
{
	struct ss rrr;

	while (1)
	{
		int ret = m_pPipe1->read((char*)&rrr);
		if (ret > 0)
		{
			printf("m_pPipe1 : [len: %d] rrr.a = %d, rrr.b = %d, rrr.c = %s \n", ret, rrr.a, rrr.b, rrr.c);
		}

		if (ret == -2)
			break;	
	}
}

/*
 * Function : PipeMgrTest::txTask1Entry
 * Desc     : Static entry point for txTask1
 * Param    : pThis - pointer to PipeMgrTest instance
 * Return   : None
 */
void PipeMgrTest::txTask1Entry(void* pThis)
{
	PipeMgrTest* pPipeMgrTest = (PipeMgrTest*)pThis;
	pPipeMgrTest->txTask1();
}

/*
 * Function : PipeMgrTest::txTask1
 * Desc     : Transmitter thread 1
 * Param    : None
 * Return   : None
 */
void PipeMgrTest::txTask1()
{
	int cnt = 1;
	struct ss ttt;

	while (m_threadAlive)
	{
		memset(&ttt, 0, sizeof(ttt));
		ttt.a = 1;
		ttt.b = cnt++;
		sprintf(ttt.c, "m_pPipe1 tx msg");

		m_pPipe1->write((char*)&ttt, sizeof(ttt));
		Sleep(1);
	}
}

/*
 * Function : PipeMgrTest::rxTask2Entry
 * Desc     : Static entry point for rxTask2
 * Param    : pThis - pointer to PipeMgrTest instance
 * Return   : None
 */
void PipeMgrTest::rxTask2Entry(void* pThis)
{
	PipeMgrTest* pPipeMgrTest = (PipeMgrTest*)pThis;
	pPipeMgrTest->rxTask2();
}

/*
 * Function : PipeMgrTest::rxTask2
 * Desc     : Receiver thread 2
 * Param    : None
 * Return   : None
 */
void PipeMgrTest::rxTask2()
{
	struct ss rrr;

	while (1)
	{
		int ret = m_pPipe2->read((char*)&rrr);
		if (ret > 0)
		{
			printf("m_pPipe2 : [len: %d] rrr.a = %d, rrr.b = %d, rrr.c = %s \n", ret, rrr.a, rrr.b, rrr.c);
		}

		if (ret == -2)
			break;	
	}
}

/*
 * Function : PipeMgrTest::txTask2Entry
 * Desc     : Static entry point for txTask2
 * Param    : pThis - pointer to PipeMgrTest instance
 * Return   : None
 */
void PipeMgrTest::txTask2Entry(void* pThis)
{
	PipeMgrTest* pPipeMgrTest = (PipeMgrTest*)pThis;
	pPipeMgrTest->txTask2();
}

/*
 * Function : PipeMgrTest::txTask2
 * Desc     : Transmitter thread 2
 * Param    : None
 * Return   : None
 */
void PipeMgrTest::txTask2()
{
	int cnt = 1;
	struct ss ttt;

	while (m_threadAlive)
	{
		ttt.a = 2;
		ttt.b = cnt++;
		sprintf(ttt.c, "m_pPipe2 tx msg");

		m_pPipe2->write((char*)&ttt, sizeof(ttt));  
		Sleep(1);                                  
	}
}
