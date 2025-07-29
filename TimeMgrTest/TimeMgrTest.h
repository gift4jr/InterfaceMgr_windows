/* -----------------------------------------------------
 * File    : TimeMgrTest.h
 * Desc    : Time Management Test using TimeMgr
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
 * -----------------------------------------------------
 */

#pragma once                                // Prevent multiple inclusion of this header file

#include "TimeMgr.h"

class TimeMgrTest
{
public:
    TimeMgrTest();                          // Constructor
    ~TimeMgrTest();                         // Destructor

    void start();                           // Starts Time Management Test

    void printTime(char* src, STimeInfo stSTimeInfo); // Print time information

private:
    TimeMgr* m_pTimeMgr1;                   // Pointer to TimeMgr instance 1
    TimeMgr* m_pTimeMgr2;                   // Pointer to TimeMgr instance 2
    TimeMgr* m_pTimeMgr3;                   // Pointer to TimeMgr instance 3
    TimeMgr* m_pTimeMgr4;                   // Pointer to TimeMgr instance 4

    static void time_handler1(void* pThis); // Static callback function for timer 1
    static void time_handler2(void* pThis); // Static callback function for timer 2
    static void time_handler3(void* pThis); // Static callback function for timer 3
    static void time_handler4(void* pThis); // Static callback function for timer 4
};
