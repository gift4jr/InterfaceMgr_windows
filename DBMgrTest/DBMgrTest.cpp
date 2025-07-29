/* -----------------------------------------------------
 * File    : DBMgrTest.cpp
 * Desc    : Database Management Test using DBMgr
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
 * ----------------------------------------------------- */

#include "DBMgrTest.h"

#ifdef __cplusplus  // Indicates to the C++ compiler to compile functions in this block with C-style linkage.
extern "C" {
#endif

    void start_test(void)
    {
        DBMgrTest* pTest = new DBMgrTest(); // Create DBMgrTest instance on heap
        pTest->start();                     // Start DBMgr test

        while (1)                           // Keep main thread alive to allow test completion
        {
            Sleep(1000);                    
        }
    }

#ifdef __cplusplus
}
#endif

/*
 * Function : DBMgrTest::DBMgrTest
 * Desc     : Constructor
 * Param    : None
 * Return   : None
 */
DBMgrTest::DBMgrTest()
	: m_pDBMgr(NULL)  
{
}

/*
 * Function : DBMgrTest::~DBMgrTest
 * Desc     : Destructor
 * Param    : None
 * Return   : None
 */
DBMgrTest::~DBMgrTest()
{
    delete m_pDBMgr;
}

/*
 * Function : DBMgrTest::start
 * Desc     : Starts Database Management Test
 *            performs insert, update operations, and reads data back
 * Param    : None
 * Return   : None
 */
void DBMgrTest::start()
{
	// Before running the following test, create the database [myDB]
	
    int i, ret;
    char queryStr[10000];

    m_pDBMgr = new DBMgr();  // Create DBMgr instance

	// Open database connection
	//=====================================================================================================================
	ret = m_pDBMgr->openDB(1, "MySQL ODBC 8.3 ANSI Driver", "127.0.0.1", 3306, "root", "1234", "myDB");
	//ret = m_pDBMgr->openDB(1, "MySQL ODBC 8.3 UNICODE Driver", "127.0.0.1", 3306, "root", "1234", "myDB");
	//---------------------------------------------------------------------------------------------------------------------
	//ret = m_pDBMgr->openDB(2, "SQL Server", "127.0.0.1", 1433, "sa", "1234", "myDB");
	//ret = m_pDBMgr->openDB(2, "SQL Server", "192.168.0.3", 1433, "sa", "1234", "myDB");
	//ret = m_pDBMgr->openDB(2, "ODBC Driver 17 for SQL Server", "127.0.0.1", 1433, "sa", "1234", "myDB");
	//---------------------------------------------------------------------------------------------------------------------
	//ret = m_pDBMgr->openDB(3, "Oracle in OraDB21Home1", "127.0.0.1", 1433, "system", "1234", "XE");
    //=====================================================================================================================
    if (ret > 0)
        printf("openDB ok!\n");
    else
        printf("openDB error!\n");

    // Drop table if exists
    memset(queryStr, 0, sizeof(queryStr));
    sprintf(queryStr, "DROP TABLE %s", "myTable");

	if (m_pDBMgr->sqlQuery(queryStr))
		printf("drop table ok\n");
	else
		printf("drop table error\n");

    // Create table with columns and types
    memset(queryStr, 0, sizeof(queryStr));
    //==========================================================================================================================================
    // MySQL & MS-SQL -> time DATETIME
	sprintf(queryStr, "CREATE TABLE %s (%s)", "myTable", 
                      "seq INT NOT NULL PRIMARY KEY, code CHAR(10), num VARCHAR(20), data VARCHAR(350), cost DOUBLE PRECISION, time DATETIME");
    //--------------------------------------------------------------------------------------------------------------------------------------------
    //// MS-SQL -> time DATETIME2
    //sprintf(queryStr, "CREATE TABLE %s (%s)", "myTable", 
    //                  "seq INT NOT NULL PRIMARY KEY, code CHAR(10), num VARCHAR(20), data VARCHAR(350), cost DOUBLE PRECISION, time DATETIME2");	
    //--------------------------------------------------------------------------------------------------------------------------------------------
    //// Oracle -> time TIMESTAMP
    //sprintf(queryStr, "CREATE TABLE %s (%s)", "myTable", 
    //                  "seq INT NOT NULL PRIMARY KEY, code CHAR(10), num VARCHAR(20), data VARCHAR(350), cost DOUBLE PRECISION, time TIMESTAMP");   
    //==========================================================================================================================================
    if (m_pDBMgr->sqlQuery(queryStr))
        printf("create table ok\n");
    else
        printf("create table error\n");

    stInType writeType;

    // Insert test data into table
    for (i = 0; i < 50; i++)
    {
        // Fill writeType fields with sample data
        writeType.seq = i;
        memset(writeType.code, 0, sizeof(writeType.code));
        sprintf(writeType.code, "code1 %d", i);
        memset(writeType.num, 0, sizeof(writeType.num));
        sprintf(writeType.num, "%d%d.%d%d", i, i, i, i);
        memset(writeType.data, 0, sizeof(writeType.data));
        sprintf(writeType.data, "data1 test1  test2   test3 %d", i);
        writeType.cost = (double)i + 0.1;

        // Set datetime fields
        writeType.time.year = 2024;
        writeType.time.month = 8;
        writeType.time.day = 12;
        writeType.time.hour = 14;
        writeType.time.minute = 30;
        writeType.time.second = 15;
        writeType.time.fraction = 123;

        // Format datetime to string
        char datetime2str[30];
        sprintf(datetime2str, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
            writeType.time.year, writeType.time.month, writeType.time.day,
            writeType.time.hour, writeType.time.minute, writeType.time.second,
            writeType.time.fraction);
        printf("%s\n", datetime2str);

        // Insert data into table
        memset(queryStr, 0, sizeof(queryStr));
        sprintf(queryStr, "INSERT INTO %s VALUES(%d, '%s', '%s', '%s', %lf, '%s')",
            "myTable", writeType.seq, writeType.code, writeType.num, writeType.data, writeType.cost, datetime2str);
		//sprintf(queryStr, "INSERT OPENQUERY(MYSQL_LINK, 'SELECT * FROM myDB.dbo.myTable') VALUES (%d, '%s', '%s', '%s', %lf, '%s')", 
		//										writeType.seq, writeType.code, writeType.num, writeType.data, writeType.cost, datetime2str);

        if (m_pDBMgr->sqlQuery(queryStr))
            printf("insert ok\n");
        else
            printf("insert error\n");
    }

    readTable(m_pDBMgr);

    // Update test data in table
    for (i = 0; i < 50; i++)
    {
        writeType.seq = i;
        memset(writeType.code, 0, sizeof(writeType.code));
        sprintf(writeType.code, "code2 %d", i);
        memset(writeType.num, 0, sizeof(writeType.num));
        sprintf(writeType.num, "%d%d.%d%d", i, i, i, i);
        memset(writeType.data, 0, sizeof(writeType.data));
        sprintf(writeType.data, "data2 test1  test2   test3 %d", i);
        writeType.cost = (double)i + 0.2;

        // Update datetime fields
		//writeType.time = { 2024, 8, 13, 23, 40, 20, 456 };
        writeType.time.year = 2024;
        writeType.time.month = 8;
        writeType.time.day = 13;
        writeType.time.hour = 23;
        writeType.time.minute = 40;
        writeType.time.second = 20;
        writeType.time.fraction = 456;

        // Format datetime to string
        char datetime2str[30];
        sprintf(datetime2str, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
            writeType.time.year, writeType.time.month, writeType.time.day,
            writeType.time.hour, writeType.time.minute, writeType.time.second,
            writeType.time.fraction);
        printf("%s\n", datetime2str);

        // Update data in table
        memset(queryStr, 0, sizeof(queryStr));
        sprintf(queryStr, "UPDATE %s SET code='%s', num='%s', data='%s', cost=%lf, time='%s' WHERE seq=%d",
            "myTable", writeType.code, writeType.num, writeType.data, writeType.cost, datetime2str, writeType.seq);
		//sprintf(queryStr, "UPDATE OPENQUERY(MYSQL_LINK, 'SELECT * FROM myDB.dbo.myTable WHERE seq=%d') SET code='%s', num='%s', data='%s', cost=%lf, time='%s'",
		//								writeType.seq, writeType.code, writeType.num, writeType.data, writeType.cost, datetime2str);

		//DELETE
		//sprintf(queryStr, "DELETE OPENQUERY(MYSQL_LINK, 'SELECT * FROM myDB.dbo.myTable WHERE seq=%d')", writeType.seq);

        if (m_pDBMgr->sqlQuery(queryStr))
            printf("update ok\n");
        else
            printf("update error\n");
    }

    readTable(m_pDBMgr);
}

/*
 * Function : DBMgrTest::readTable
 * Desc     : Reads data from table
 * Param    : pDBMgr - pointer to DBMgr instance for DB access
 * Return   : None
 */
void DBMgrTest::readTable(DBMgr* pDBMgr)
{
    stOutType readType;
    char queryStr[10000];
	
    memset(queryStr, 0, sizeof(queryStr));

	//--------------------------------------------------------------------------------
	// if local server DB read 
    //--------------------------------------------------------------------------------				
	//sprintf(queryStr, "SELECT * FROM %s", "myDB.dbo.myTable");				
	//sprintf(queryStr, "SELECT * FROM %s", "myDB..myTable");	
	sprintf(queryStr, "SELECT * FROM %s", "myTable");	 		

	//--------------------------------------------------------------------------------
	// if linked server DB read 
	//--------------------------------------------------------------------------------
	//sprintf(queryStr, "SELECT * FROM %s", "MYSQL_LINK.myDB.dbo.myTable");
	//--------------------------------------------------------------------------------
	// linked server DB read using openquery 
    //--------------------------------------------------------------------------------------------------
	//sprintf(queryStr, "SELECT * FROM %s", "OPENQUERY(MYSQL_LINK, 'SELECT * FROM myDB.dbo.myTable')");
	//sprintf(queryStr, "SELECT * FROM %s", "OPENQUERY(MYSQL_LINK, 'SELECT * FROM myDB..myTable')");	
	//--------------------------------------------------------------------------------------------------

    if (HSTMT hstmt = pDBMgr->sqlQuery(queryStr))   // Executes given SQL query, and returns ODBC statement handle if successful, NULL on error
    {
        while (1)
        {
            RETCODE rc = SQLFetch(hstmt);
            if ((rc == SQL_SUCCESS) || (rc == SQL_SUCCESS_WITH_INFO))
            {
                memset(&readType, 0, sizeof(readType));
                SQLGetData(hstmt, 1, SQL_C_CHAR, readType.seq, sizeof(readType.seq), NULL);
                SQLGetData(hstmt, 2, SQL_C_CHAR, readType.code, sizeof(readType.code), NULL);
                SQLGetData(hstmt, 3, SQL_C_CHAR, readType.num, sizeof(readType.num), NULL);
                SQLGetData(hstmt, 4, SQL_C_CHAR, readType.data, sizeof(readType.data), NULL);
                SQLGetData(hstmt, 5, SQL_C_CHAR, readType.cost, sizeof(readType.cost), NULL);
                SQLGetData(hstmt, 6, SQL_C_CHAR, readType.time, sizeof(readType.time), NULL);

                printf("[seq=%s][code=%s][num=%s][data=%s][cost=%s][time=%s]\n",
                    readType.seq, readType.code, readType.num, readType.data, readType.cost, readType.time);
            }
            else
                break;
        }

        SQLFreeStmt(hstmt, SQL_CLOSE);	// Close the result set to allow re-execution of SQL statements without error
    }
}
