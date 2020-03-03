/**
    Header file for the class socketStream


**/

#ifndef SOCKETSTREAM_H
#define SOCKETSTREAM_H

#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <vector>
#include <string>

// include library for socket programming
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "10352"
#define DEFAULT_HOST_IP "192.168.1.104"


class socketStream{

    // define a struct for storing infor regarding the socket
    WSADATA wsaData;

    // define socket object
    SOCKET ConnectSocket; // = INVALID_SOCKET;

    struct addrinfo *result, *ptr, hints;

    char recvbuf[DEFAULT_BUFLEN];

    int iResult;

    int recvbuflen = DEFAULT_BUFLEN;

    char *Host_IP;                                                          // the IP of the server

    unsigned int Host_Port;                                                 // the port to be used

    unsigned int msg_buffer;                                                // the message buffer

    bool isComActive;                                                       // a flag for checking if the communication with the server is active

    rapidjson::Document dDoc;                                               // a json object to store the data to a specific structure

    rapidjson::StringBuffer str_buffer;                                     // buffer for stringify the json object
    
    // variables used in the communication protocol
    char *msg_idf;                                                          // the new message identifier  
    char *endMSG;                                                           // the end-of-message identifier
    char *ec_id;                                                            // the end-of-connection identifier

    std::string msg2send;                                                   // the message to be sent

    bool msgInitilized;                                                     // a flag to check if the message is initialized

public:

    socketStream();                                                         // empty constructor, setting the default values

    socketStream(char* scrIPAdress);                                        // constructor with setting the server IP address

    socketStream(char* svrIPAddress, int srvPosrt);                         // constructor with setting the server IP address and port

    int initialize_sockeStream();                                           // initialize the socketStream object

    int initialize_sockeStream(char* svrIPAddress, int srvPosrt);           // initialize the socketStream object re-setting the server IP address and port

    int make_connection();                                                  // connect with the server

    int initialize_msgStruct(std::vector<std::string> fields);              // setting the fields of the message 

    int printMSGcontents();                                                 // print the contents of the message

    int printMSGcontentsTypes();                                            // print everything that is contained in the message

    int getMSGString();                                                     // get the message contents as strins

    
    int updateMSG(std::string field, char *value);                   // update the specific field of the message
    int updateMSG(std::string field, int *value, int arraylength);                   // update the specific field of the message

    int sendMSg();                                                          // send message to the server

    int closeCommunication();                                               // close the communication with the server

    ~socketStream(void);                                                    // destructor

    
};




#endif