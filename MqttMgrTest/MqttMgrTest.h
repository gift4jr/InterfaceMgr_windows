/* -----------------------------------------------------
 * File    : MqttMgrTest.h
 * Desc    : MQTT Communication Test using MqttMgr
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
 * -----------------------------------------------------
 */

#pragma once                                // Prevent multiple inclusion of this header file

#include "MqttMgr.h"

class MqttMgrTest
{
public:
    MqttMgrTest();                          // Constructor
    ~MqttMgrTest();                         // Destructor

    void start();                           // Starts MQTT Test

private:
    MqttMgr* m_pMqtt1;                      // Pointer to MqttMgr instance 1               
	MqttMgr* m_pMqtt2;                      // Pointer to MqttMgr instance 2

    HANDLE m_thread;                        // Handle to transmitter thread
    bool m_threadAlive;                     // Flag to control transmitter loop

    void txTask();                          // Transmitter thread
    static void txTaskEntry(void* pThis);   // Static entry point for txTask

    static void recvMsgFromMqtt1(void* pThis, STimeInfo rcvTime, char* msg, int msgLen);    // Callback function for receiving message from MqttMgr instance 1
	static void recvMsgFromMqtt2(void* pThis, STimeInfo rcvTime, char* msg, int msgLen);    // Callback function for receiving message from MqttMgr instance 2
};

/* Test Procedure
0. Install MQTT Broker (mosquitto-2.0.18-install-windows-x64)
=> Installed in C:\Program Files\mosquitto folder

1. To enable remote access, open mosquitto.conf file in C:\Program Files\mosquitto folder
   using Notepad with administrator privileges on the node where MQTT Broker is installed.
    --------------------------------------------------------------------------------------
    => Add the following at the bottom of the # Listeners section (for remote access)
    # =========
    # Listeners
    # =========
    listener 1883
    --------------------------------------------------------------------------------------
    => Add the following at the bottom of the # Security section
    # ========
    # Security
    # ========
    allow_anonymous true
    --------------------------------------------------------------------------------------

2. Run MQTT Broker with the command below (make sure to disable firewall!)
------------------------------
cd c:\Program Files\Mosquitto
mosquitto -c mosquitto.conf -v
------------------------------

3. On pub and sub nodes, run applications using the MQTT Broker's IP, port, and topic name.
Sample test commands:
---------------------------------------------------------
cd c:\Program Files\Mosquitto
mosquitto_pub -h ip(Broker node) -t topic_name -m "test msg"
ex) mosquitto_pub -h 127.0.0.1 -t T1 -m "test msg" 
---------------------------------------------------------
cd c:\Program Files\Mosquitto
ex) mosquitto_sub -h 127.0.0.1 -t T1
mosquitto_sub -h ip(Broker node) -t topic_name
---------------------------------------------------------
*/