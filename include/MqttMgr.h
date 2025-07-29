/* -----------------------------------------------------
 * File    : MqttMgr.h
 * Desc    : MQTT Communication Manager using Mosquitto
 * Author  : Jiryong Kim
 * Created : 2010
 * Updated : July 29, 2025
 * -----------------------------------------------------
 */

#pragma once    // Prevent multiple inclusion of this header file

#include "mosquitto.h"
#include "MsgQMgr.h"
#include "TimeMgr.h"

// Callback function type for receiving MQTT message
typedef void (MqttCallbackFunction)(void* pThis, STimeInfo rcvTime, char* msg, int msgLen); 

class MqttMgr
{
public:
    MqttMgr(char* nodeNmae, int maxMsgBytes = 1000);    // Constructor
    virtual ~MqttMgr();                                 // Destructor

    MqttMgr(const MqttMgr&) = delete;                   // Copy constructor deleted to prevent shallow copy of internal buffer
    MqttMgr& operator=(const MqttMgr&) = delete;        // Copy assignment operator deleted to avoid multiple deletes on the same memory
    MqttMgr(MqttMgr&&) = delete;                        // Move constructor deleted to ensure unique ownership of buffer
    MqttMgr& operator=(MqttMgr&&) = delete;             // Move assignment operator deleted to prevent multiple ownership issues

    void setCallback(MqttCallbackFunction* pCallbackFunction, void* pThis); // Register callback function for receiving MQTT message

    void initMqtt(char* mqtt_host = (char*)"127.0.0.1", int mqtt_port = 1883, int mqtt_keepalive = 60); // Initializes MQTT for publish/subscribe and creates communication threads.

    int sendMsg(char* dstTopic, char* msg, int msgLen, char qos = 0); // Publishes message to specified topic with QoS level
	// 0 - At most once (fire and forget), 1 - At least once (guaranteed delivery, may duplicate), 2 - Exactly once (guaranteed delivery without duplicates)

private:
    HANDLE m_thread1, m_thread2;                // Handles to threads for MQTT and message queue processing
    std::mutex m_mutex;                         // Mutex for thread-safe operations

    char* m_sub_topic;                          // Subscription topic name
    char* m_pub_topic;                          // Publication topic name

    struct mosquitto* m_pub_mosq;               // Mosquitto handle for publishing
    struct mosquitto* m_sub_mosq;               // Mosquitto handle for subscribing

    MsgQMgr* m_pMsgQ;                           // Pointer to message queue

    MqttCallbackFunction* m_pCallbackFunction;  // Pointer to message callback function
    void* m_pThis;                              // Pointer to instance for message callback

    char* m_rxMsg1;                             // Pointer to buffer for receiving message from MQTT
    char* m_rxMsg2;                             // Pointer to buffer for receiving message from message queue

    int m_maxMsgBytes;                          // Maximum allowed message size

    bool m_initSuccess;                         // MQTT initialization success flag

    void rxTask1();                             // Thread for receiving message from MQTT
    void sub_connect_callback(int result);                          // Callback for subscription connection
    void sub_message_callback(const struct mosquitto_message* msg); // Callback for incoming message
    void rxTask2();                             // Thread for receiving message from message queue

    static void rxTask1Entry(void* pThis);      // Static entry point for rxTask1
    static void sub_connect_callbackEntry(struct mosquitto* mosq, void* obj, int result);                              // Static entry point for sub_connect_callback
    static void sub_message_callbackEntry(struct mosquitto* sub_mosq, void* obj, const struct mosquitto_message* msg); // Static entry point for sub_message_callback
    static void rxTask2Entry(void* pThis);      // Static entry point for rxTask2

};
