/* -----------------------------------------------------
 * File    : TableMgr.h
 * Desc    : Table Manager (Insert, Update, Delete)
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
 * -----------------------------------------------------
 */

#pragma once    // Prevent multiple inclusion of this header file

#include "MsgQMgr.h"
#include "TimeMgr.h"

#include <wtypes.h>
#include <queue>
#include <map>
#include <string>
#include <mutex>

#define TABLEMGR_INSERT				1	
#define TABLEMGR_NEW				1	
#define TABLEMGR_UPDATE				2		
#define TABLEMGR_SAME				3		
#define TABLEMGR_DELETE				4	
#define TABLEMGR_DELETE_ALL			5	

#define TABLEMGRLIB_MAX_SNU_SIZE	10		// Maximum length for string number keys

using namespace std;

#pragma pack(1) // Set structure alignment to 1-byte boundary (no padding)

// TableMgr Structure
struct STableMgrType
{
	int nu;                          		// Numeric key; -1 if unused or for string keys
	int update_flag;                 		// Flag indicating update state (NEW, UPDATE, SAME)
	int manual_flag;                 		// Manual update flag (0 = auto, 1 = manual)
	int infoLen;                    		// Length of info data buffer
	char* info;                     		// Pointer to info data buffer
	char snu[TABLEMGRLIB_MAX_SNU_SIZE];  	// String key (null-terminated)
	double initial_time;            		// Timestamp when entry was created (ms)
	double updated_time;            		// Timestamp when entry was last updated (ms)
};

// TableMgr Update Structure
struct STableMgrUpdateType
{
	int update_type;                 		// Update action type 
	int manual_flag;                 		// Manual update flag
	int len;                        		// Length of info data
	int nu;                         		// Numeric key; -1 if not used
	char snu[TABLEMGRLIB_MAX_SNU_SIZE];  	// String key (null-terminated)
};

#pragma pack()  // Restore default packing alignment

// Callback function type for Table Manager events by numeric key
typedef void (TableMgrCallbackFunction)(void* pThis, int info_type, int nu, int index, char* info, int len);		

// Callback function type for Table Manager events by string key
typedef void (STableMgrCallbackFunction)(void* pThis, int info_type, char* snu, int index, char* info, int len);	

class TableMgr
{
public:
	TableMgr();                                  		// Constructor
	virtual ~TableMgr();                         		// Destructor

	TableMgr(const TableMgr&) = delete;            		// Copy constructor deleted to prevent shallow copy of internal buffer
	TableMgr& operator=(const TableMgr&) = delete; 		// Copy assignment operator deleted to avoid multiple deletes on the same memory
	TableMgr(TableMgr&&) = delete;                 		// Move constructor deleted to ensure unique ownership of buffer
	TableMgr& operator=(TableMgr&&) = delete;      		// Move assignment operator deleted to prevent multiple ownership issues

	void setCallback(TableMgrCallbackFunction* pCallbackFunction, void* pThis);		// Register callback function for Table Manager events by numeric key
	void setCallback(STableMgrCallbackFunction* pSCallbackFunction, void* pThis);	// Register callback function for Table Manager events by string key

	void initTableMgr(int timeoutSec = -1, int maxInfoCnt = 100, int maxInfoSize = 1000);	// Initialize the table manager 

	int insertData(int nu, char* info, int len, int isManual = 0);		// Insert or update entry by numeric key; isManual = 0 (normal), 1 (manual)
	int insertData(char* snu, char* info, int len, int isManual = 0);	// Insert or update entry by string key; isManual = 0 (normal), 1 (manual)

	void deleteData(int nu);							// Delete entry by numeric key
	void deleteData(char* snu);							// Delete entry by string key
	void deleteAllData();								// Delete all entries

	int getDataInfo(int nu, char* info);				// Get info data by numeric key
	int getDataInfo(char* snu, char* info);				// Get info data by string key
	int getDataInfoCnt();								// Get current total number of entries

	int getDataIndex(int nu);							// Get index of data by numeric key
	int getDataIndex(char* snu);						// Get index of data by string key
	int getDataMaxIndex();								// Get maximum index currently used

	int getDataNumber(int index);						// Get numeric key at given index
	char* getDataSNumber(int index);					// Get string key at given index

	int getTableTimeout();								// Get current timeout value
	void setTableTimeout(int timeoutSec);				// Set timeout value

private:	
	TimeMgr* m_pTimeMgr;								// Pointer to TimeMgr for timer events
	MsgQMgr* m_pMsgQ;									// Pointer to message queue manager 

	HANDLE m_thread;									// Thread handle for message queue processing

	TableMgrCallbackFunction* m_pCallbackFunction;		// Pointer to message callback function by numeric key
	STableMgrCallbackFunction* m_pSCallbackFunction;	// Pointer to message callback function by string key
	void* m_pThis;										// Pointer to instance for message callback

	std::mutex m_mutex1;								// Mutexes for thread safety to access Message Queue
	std::mutex m_mutex2;								// Mutexes for thread safety to access Maps
		
	map<int, int> m_iKeyMap;							// Maps for numeric key to indices
	map<string, int> m_sKeyMap;							// Maps for string key to indices
	
	queue<int> m_indexQueue;							// Queue to manage free indices for new entries
		
	STableMgrType* m_tableMgr;							// Array of table entries

	char* m_txBuffer;									// Pointer to buffer for message queue transmit
	char* m_rxBuffer;									// Pointer to buffer for message queue receive
	char* m_delInfo;									// Pointer to buffer for deletion info
		
	int m_timeoutSec;									// Timeout value for entries; -1 means no timeout	
	int m_maxInfoCnt;									// Maximum number of entries supported	
	int m_maxInfoSize;									// Maximum size of info data buffer per entry	
	int m_maxInfoIndex;									// Highest index used in m_tableMgr
	bool m_delAllTableFlag;								// Flag to indicate full deletion requested

	void rxTask();																// Thread for receiving message from message queue
	static void rxTaskEntry(void* pThis);										// Static entry point for rxTask

	void eventTableTimeout();													// Timer callback event handler
	static void eventTableTimeoutEntry(void* pThis);							// Static entry point for eventTableTimeout

	void update_insertData(int nu, char* info, int len, int isManual);			// Processes insert or update of data identified by numeric key
	void update_insertData(char* snu, char* info, int len, int isManual);		// Processes insert or update of data identified by string key
	void update_deleteData(int nu);												// Processes deletion of data identified by numeric key
	void update_deleteData(char* snu);											// Processes deletion of data identified by string key
	void update_deleteAllData();												// Processes deletion of all data entries

	int proc_update_insertData(int nu, char* info, int len, int isManual);		// Internal processing of insert/update by numeric key
	int proc_update_insertData(char* snu, char* info, int len, int isManual);	// Internal processing of insert/update by string key
	int proc_update_deleteData(int nu, char* info, int& len);					// Internal processing of deletion by numeric key
	int proc_update_deleteData(char* snu, char* info, int& len);				// Internal processing to delete data by string key
	void proc_update_deleteAllData();											// Internal processing to delete all data entries
};
