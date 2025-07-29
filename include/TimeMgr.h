/* -----------------------------------------------------
 * File    : TimeMgr.h
 * Desc    : Time Manager (Timer, Timeout, Date, Time)
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
 * -----------------------------------------------------
 */

#pragma once        // Prevent multiple inclusion of this header file

#define NOMINMAX    // Prevent min and max macro definitions in windows.h

#include <windows.h>
#include <time.h>
#include <sys/timeb.h>
#include <mutex>

#pragma comment(lib, "winmm.lib")        // Link with winmm.lib to improve timer accuracy

#define TIMEMGR_MAX_TIMER_NUM 500        // Maximum number of timers supported

// Callback function type for timer events
typedef void (TimeMgrCallbackFunction)(void* pThis);     

#pragma pack(1) // Set structure alignment to 1-byte boundary (no padding)

// TimeMgr Structure
struct STimerInfo
{
    bool isID;                                              // Flag indicating if timer ID is valid
    bool isUsed;                                            // Flag indicating if the timer slot is in use
    HANDLE timerQueueID;                                    // Handle to the timer queue
    HANDLE timerID;                                         // Handle to the timer object
    TimeMgrCallbackFunction* pCallbackFunction;             // Callback function pointer to invoke on timeout
    void* pThis;                                            // User-defined pointer passed to the callback
};

// Time Structure
struct STimeInfo
{
    unsigned int    uiTotalSec;    				            // Total seconds 
    unsigned short  usYear;        				            // Year 
    unsigned char   ucMonth;       				            // Month 
    unsigned char   ucDay;         				            // Day of month 
    unsigned char   ucHour;        				            // Hour 
    unsigned char   ucMin;         				            // Minute 
    unsigned char   ucSec;         				            // Second 
    unsigned int    uiMiliSec;     				            // Milliseconds
};

#pragma pack()  // Restore default packing alignment

class TimeMgr
{
public:
    TimeMgr();                                   			// Constructor
    virtual ~TimeMgr();                          			// Destructor

    TimeMgr(const TimeMgr&) = delete;            			// Copy constructor deleted to prevent shallow copy of internal buffer
    TimeMgr& operator=(const TimeMgr&) = delete; 			// Copy assignment operator deleted to avoid multiple deletes on the same memory
    TimeMgr(TimeMgr&&) = delete;                 			// Move constructor deleted to ensure unique ownership of buffer
    TimeMgr& operator=(TimeMgr&&) = delete;      			// Move assignment operator deleted to prevent multiple ownership issues

    void setCallback(TimeMgrCallbackFunction* pCallbackFunction, void* pThis); // Register callback function for timer events

    void startTimer(long delayMiliSec, long periodMiliSec); // Start a periodic timer with initial delay and repeat interval
    void startTimeout(long leftMiliSec);                    // Start a one-shot timeout timer with given delay

    void stopTimer();                                       // Stop currently running periodic timer
    void stopTimeout();                                     // Stop currently running timeout timer

    STimeInfo getTime(int type = 1, int totalGmtSec = -1);  // Retrieve current system time (0: GMT, 1: KST)

private:
    int m_timerID;                                          // Internal ID to manage unique timer instances

    int findNewTimerID();                                   // Find and reserve a new available timer ID

    static void __stdcall queueTimerCallback(void* pThis, BOOLEAN TimerOrWaitFired); // Static callback function for Windows timer queue.

    void set_timer(long delayMiliSec, long periodMiliSec);  // Create and start a timer in the Windows timer queue
    void del_timer();                                       // Delete the timer from the Windows timer queue and release resources.
    void destroy_timer();                                   // Delete the timer and clear callback/context
};

extern STimeInfo getCurrentTime(int type = 1, int totalGmtSec = -1); // Global function to get current time (0: GMT, 1: KST)
