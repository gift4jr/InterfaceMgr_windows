/* -----------------------------------------------------
 * File    : FileMgrTest.cpp
 * Desc    : File Management Test using FileMgr
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
 * ----------------------------------------------------- */

#include "FileMgrTest.h"

#ifdef __cplusplus  // Indicates to the C++ compiler to compile functions in this block with C-style linkage.
extern "C" {
#endif

    void start_test(void)
    {
        FileMgrTest* pTest = new FileMgrTest();  // Create FileMgrTest instance on heap
        pTest->start();                          // Start FileMgr test

        while (1)                                // Keep main thread alive to allow test completion
        {
            Sleep(1000);
        }
    }

#ifdef __cplusplus
}
#endif

/*
 * Function : FileMgrTest::FileMgrTest
 * Desc     : Constructor
 * Param    : None
 * Return   : None
 */
FileMgrTest::FileMgrTest()
	: m_pFileMgr(NULL) 
{
}

/*
 * Function : FileMgrTest::~FileMgrTest
 * Desc     : Destructor
 * Param    : None
 * Return   : None
 */
FileMgrTest::~FileMgrTest()
{
    delete m_pFileMgr;
}

/*
 * Function : FileMgrTest::start
 * Desc     : Starts File Management Test
 * Param    : None
 * Return   : None
 */
void FileMgrTest::start()
{
    m_pFileMgr = new FileMgr(10000);  // Create FileMgr instance

    int ret;

    m_pFileMgr->openFile("test.dat", "wb");  // Open file in binary write mode

    stTypeICD stSndMsg;

    printf("writing start...\n");

    for (int i = 0; i < 20; i++)
    {
        memset(&stSndMsg, 0, sizeof(stSndMsg));
        sprintf(stSndMsg.track, "test %d", i);
		stSndMsg.len = sizeof(stSndMsg.len) + strlen(stSndMsg.track);

        ret = m_pFileMgr->writeFile((char*)&stSndMsg, sizeof(stSndMsg));  
        if (ret <= 0)
            printf("write error\n");
    }

    printf("writing end...\n\n");

    m_pFileMgr->closeFile();  

    m_pFileMgr->openFile("test.dat", "rb");  // Open file in binary read mode

    stTypeICD stRcvMsg;
    char rBuffer[10000] = "";

    printf("reading start...\n");

    ret = m_pFileMgr->readFile(rBuffer);  
    if (ret <= 0)
        printf("read error\n");

    printf("reading end...\n\n");

    if (ret > 0)
    {
        printf("printing start...\n");

        int index = 0;

        while (ret > 0)
        {
            memset(&stRcvMsg, 0, sizeof(stRcvMsg));
            memcpy(&stRcvMsg, rBuffer + index, sizeof(stRcvMsg));  

            index += sizeof(stRcvMsg);
            ret -= sizeof(stRcvMsg);

            printf("[len=%d] %s \n", stRcvMsg.len, stRcvMsg.track);  
        }

        printf("printing end...\n");
    }
	else
		printf("read error...\n");

    m_pFileMgr->closeFile();  
}
