/**
    Header file for the class socketStream


**/

#ifndef SOCKETSTREAM_H
#define SOCKETSTREAM_H


#include "md5.h"

#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

// include library for socket programming
#ifdef _WIN32
    #include <winsock2.h>
    #include <windows.h>
    #include <ws2tcpip.h>
    #include <conio.h>
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <unistd.h>
#endif


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

// include rapidjson for message structure
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include <chrono>
#include <thread>
#include <mutex>

#define DEFAULT_BUFLEN 2048
#define DEFAULT_PORT 10352
#define DEFAULT_HOST_IP "localhost"
#define SOCKETSTREAM_SERVER 1
#define SOCKETSTREAM_CLIENT 0
#define MAX_NB_CONNECTIONS 10


class socketStream{

    #ifdef _WIN32
        // define a struct for storing infor regarding the socket
        WSADATA wsaData;
    #endif

    // define socket object
    SOCKET ConnectSocket; // = INVALID_SOCKET;

    SOCKET ListenSocket;                            // for the server
    std::vector <SOCKET> clientsSockets;
    std::vector <std::string> clientsAddresses;
    std::vector <bool> firstMsgReceived;
    std::vector <bool> isNewMsgReceived;
    std::vector <bool> connetionSlots;
    std::vector <std::string> clientIDs;
    std::vector <std::string> clientMsgs;

    std::mutex threadMutex;

    struct addrinfo *result, *ptr, hints;

    char recvbuf[DEFAULT_BUFLEN];

    int iResult;

    int recvbuflen = DEFAULT_BUFLEN;

    bool isServer;
    bool serverRunning;

    unsigned int bufferSize;                                                        // the buffer size on the clients side

    const char *Host_IP;                                                            // the IP of the server

    unsigned int Host_Port;                                                         // the port to be used

    unsigned int msg_buffer;                                                        // the message buffer

    bool isComActive;                                                               // a flag for checking if the communication with the server is active

    rapidjson::Document dDoc;                                                       // a json object to store the data to a specific structure

    rapidjson::StringBuffer str_buffer;                                             // buffer for stringifing the json object
    
    rapidjson::Writer<rapidjson::StringBuffer> writer;                              // writer to stringify the json object
    
    // variables used in the communication protocol
    const char *msg_idf;                                                                  // the new packet identifier  
    const char *endMSG;                                                                   // the end-of-message identifier
    const char *ec_id;                                                                    // the end-of-connection identifier
    unsigned int headerSize;                                                        // the header of the packet containing the number of bytes to be streamed
    unsigned int minMsgSize;                                                        // the size of the packet if it doesn't include the message
    unsigned int msgOverhead;
    // unsigned int msgOverhead_buffer;

    std::string clientName;
    std::string msgHeader;                                                          // the header of the message containing the size of the data
    std::string msg2send;                                                           // the message to be sent
    std::string final_msg;                                                          // the message including the header and the message identifications
    std::string msgOHstring;
    std::string msg2return; // for server

    bool msgInitilized;                                                             // a flag to check if the message is initialized

    bool useHashKey;                                                                // a flag indicating if a md5 checksum will be included in the message or not

    int printArray(rapidjson::Value::ConstMemberIterator itr);                      // printing an array from iterator

    int checkKeyPressed(std::string checkkey);

    std::string messageExtractor(std::string fullmsg, bool* msgValidity);

    int runReceiver(int connectionID);

    int findEmptySlot();

    bool handshake_client(SOCKET conc, int strlength, int slotNb);

    bool handshake_server(int strlength);

    std::string randomString(int strlength);

public:

    socketStream(void);                                                                                         // empty constructor, setting the default values, default: TCP client

    socketStream(const char* scrIPAdress);                    // constructor with setting the server IP address

    socketStream(const char* svrIPAddress, int srvPosrt, const int socketStreamMode = SOCKETSTREAM_CLIENT);     // constructor with setting the server IP address and port

    int initialize_socketStream();                                                                              // initialize the socketStream object

    int initialize_socketStream(const char* svrIPAddress, int srvPosrt);                                        // initialize the socketStream object re-setting the server IP address and port

    int make_connection();                                                                                      // connect with the server

    int initialize_msgStruct(std::vector<std::string> fields);                                                  // setting the fields of the message 

    int printMSGcontents();                                                                                     // print the contents of the message

    int printMSGcontentsTypes();                                                    // print everything that is contained in the message

    int printMSGString();                                                           // get the message contents as strins

    int set_clientName(std::string cID);

    int updateMSG(std::string field, const char *value);                                  // update the specific field of the message (for strings)
    int updateMSG(std::string field, int *value, int arraylength);                  // update the specific field of the message (for array of integers)
    int updateMSG(std::string field, double *value, int arraylength);               // update the specific field of the message (for array of doubles)
    int updateMSG(std::string field, std::vector <int> value);                      // update the specific field of the message (for vector of integers)
    int updateMSG(std::string field, std::vector <double> value);                   // update the specific field of the message (for vector of doubles)
    int updateMSG(std::string field, std::vector <std::vector <int>> value);        // update the specific field of the message (for 2D matrix of integers)
    int updateMSG(std::string field, std::vector <std::vector <double>> value);     // update the specific field of the message (for 2D matrix of doubles)

    int sendMSg();                                                                  // send message to the server

    int closeCommunication();                                                       // close the communication with the server

    int setHashKey(bool hKey);                                                      // setting the value of useHashKey

    int setHeaderSize(unsigned int hSize);                                          // setting the header size

    std::string getFullmsg();                                                       // returning the full message as a string

    int wait_connections();

    int runServer();

    std::string get_latest();
    std::string get_latest(std::string cltName, bool* newMSG);

    bool sockectStream_ok();

    ~socketStream(void);                                                            // destructor

    
};




#endif