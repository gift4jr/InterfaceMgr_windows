/* -----------------------------------------------------
 * File    : TimeMgrTest.cpp
 * Desc    : Time Management Test using TimeMgr
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
 * ----------------------------------------------------- */

#include "TimeMgrTest.h"

#ifdef __cplusplus  // Indicates to the C++ compiler to compile functions in this block with C-style linkage
extern "C" {
#endif

    void start_test(void)
    {
        TimeMgrTest* pTest = new TimeMgrTest();   // Create TimeMgrTest instance on heap
        pTest->start();                           // Start the TimeMgr test

        while (1)                                 // Keep main thread alive to allow test completion
        {
            Sleep(1000);
        }
    }

#ifdef __cplusplus
}
#endif

/*
 * Function : TimeMgrTest::TimeMgrTest
 * Desc     : Constructor
 * Param    : None
 * Return   : None
 */
TimeMgrTest::TimeMgrTest()
    : m_pTimeMgr1(NULL), m_pTimeMgr2(NULL),
    m_pTimeMgr3(NULL), m_pTimeMgr4(NULL)
{
}

/*
 * Function : TimeMgrTest::~TimeMgrTest
 * Desc     : Destructor
 * Param    : None
 * Return   : None
 */
TimeMgrTest::~TimeMgrTest()
{
    delete m_pTimeMgr1;
    delete m_pTimeMgr2;
    delete m_pTimeMgr3;
    delete m_pTimeMgr4;
}

/*
 * Function : TimeMgrTest::start
 * Desc     : Starts Time Management Test
 * Param    : None
 * Return   : None
 */
void TimeMgrTest::start()
{
    STimeInfo stSTimeInfo = getCurrentTime();    // Get current time

    printf("[%04d.%02d.%02d][%02d:%02d:%02d.%03d] \n",
        stSTimeInfo.usYear, stSTimeInfo.ucMonth, stSTimeInfo.ucDay,
        stSTimeInfo.ucHour, stSTimeInfo.ucMin, stSTimeInfo.ucSec, stSTimeInfo.uiMiliSec);

    // Create timer instances
    m_pTimeMgr1 = new TimeMgr();
    m_pTimeMgr2 = new TimeMgr();
    m_pTimeMgr3 = new TimeMgr();
    m_pTimeMgr4 = new TimeMgr();

    // Register timer callback functions
    m_pTimeMgr1->setCallback(TimeMgrTest::time_handler1, this);
    m_pTimeMgr2->setCallback(TimeMgrTest::time_handler2, this);
    m_pTimeMgr3->setCallback(TimeMgrTest::time_handler3, this);
    m_pTimeMgr4->setCallback(TimeMgrTest::time_handler4, this);

    printf("Start 1ms Timers After 1 Sec\n");

    // Start 1ms timers after 1 second
    m_pTimeMgr1->startTimer(1000, 1);
    m_pTimeMgr2->startTimer(1000, 1);
    m_pTimeMgr3->startTimer(1000, 1);
    m_pTimeMgr4->startTimer(1000, 1);

    Sleep(3000);  

    printf("Change Timers From 1ms To 1s\n");

    // Change timer interval
    m_pTimeMgr1->startTimer(0, 1000);
    m_pTimeMgr2->startTimer(0, 1000);
    m_pTimeMgr3->startTimer(0, 1000);
    m_pTimeMgr4->startTimer(0, 1000);

    Sleep(3000);

    printf("Stop Timers...\n");

    // Stop periodic timers
    m_pTimeMgr1->stopTimer();
    m_pTimeMgr2->stopTimer();
    m_pTimeMgr3->stopTimer();
    m_pTimeMgr4->stopTimer();

    printf("/////////////////////////////////////////// \n");
    printf("Timeout test Start....\n");

    int cnt = 5;

    while (cnt)
    {
        printf("called time_handler1 [%d] \n", cnt);
        printf("called time_handler2 [%d] \n", cnt);
        printf("called time_handler3 [%d] \n", cnt);
        printf("called time_handler4 [%d] \n", cnt);

        // Start timeouts 
        m_pTimeMgr1->startTimeout(2000);
        m_pTimeMgr2->startTimeout(3000);
        m_pTimeMgr3->startTimeout(4000);
        m_pTimeMgr4->startTimeout(5000);

        cnt--;

        Sleep(1000);
    }

    Sleep(8000);  

    // Stop all timeouts
    m_pTimeMgr1->stopTimeout();
    m_pTimeMgr2->stopTimeout();
    m_pTimeMgr3->stopTimeout();
    m_pTimeMgr4->stopTimeout();

    printf("Timeout test End....\n");
}

/*
 * Function : TimeMgrTest::time_handler1
 * Desc     : Static callback function for timer 1
 * Param    : pThis - pointer to TimeMgrTest instance
 * Return   : None
 */
void TimeMgrTest::time_handler1(void* pThis)
{
    TimeMgrTest* pTimeMgrTest = (TimeMgrTest*)(pThis);
    STimeInfo stSTimeInfo = getCurrentTime();
    pTimeMgrTest->printTime("time_handler1", stSTimeInfo);
}

/*
 * Function : TimeMgrTest::time_handler2
 * Desc     : Static callback function for timer 2
 * Param    : pThis - pointer to TimeMgrTest instance
 * Return   : None
 */
void TimeMgrTest::time_handler2(void* pThis)
{
    TimeMgrTest* pTimeMgrTest = (TimeMgrTest*)(pThis);
    STimeInfo stSTimeInfo = getCurrentTime();
    pTimeMgrTest->printTime("time_handler2", stSTimeInfo);
}

/*
 * Function : TimeMgrTest::time_handler3
 * Desc     : Static callback function for timer 3
 * Param    : pThis - pointer to TimeMgrTest instance
 * Return   : None
 */
void TimeMgrTest::time_handler3(void* pThis)
{
    TimeMgrTest* pTimeMgrTest = (TimeMgrTest*)(pThis);
    STimeInfo stSTimeInfo = getCurrentTime();
    pTimeMgrTest->printTime("time_handler3", stSTimeInfo);
}

/*
 * Function : TimeMgrTest::time_handler4
 * Desc     : Static callback function for timer 4
 * Param    : pThis - pointer to TimeMgrTest instance
 * Return   : None
 */
void TimeMgrTest::time_handler4(void* pThis)
{
    TimeMgrTest* pTimeMgrTest = (TimeMgrTest*)(pThis);
    STimeInfo stSTimeInfo = getCurrentTime();
    pTimeMgrTest->printTime("time_handler4", stSTimeInfo);
}

/*
 * Function : TimeMgrTest::printTime
 * Desc     : Prints formatted time from callback handler
 * Param    : src         - identifier string for the handler
 *          : stSTimeInfo - current time info structure
 * Return   : None
 */
void TimeMgrTest::printTime(char* src, STimeInfo stSTimeInfo)
{
    printf("[%04d.%02d.%02d][%02d:%02d:%02d.%03d] of %s\n",
        stSTimeInfo.usYear, stSTimeInfo.ucMonth, stSTimeInfo.ucDay,
        stSTimeInfo.ucHour, stSTimeInfo.ucMin, stSTimeInfo.ucSec, stSTimeInfo.uiMiliSec, src);
}
