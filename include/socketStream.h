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

// include library for socket programming
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

// include rapidjson for message structure
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"



#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT 10352
#define DEFAULT_HOST_IP "localhost"


class socketStream{

    // define a struct for storing infor regarding the socket
    WSADATA wsaData;

    // define socket object
    SOCKET ConnectSocket; // = INVALID_SOCKET;

    struct addrinfo *result, *ptr, hints;

    char recvbuf[DEFAULT_BUFLEN];

    int iResult;

    int recvbuflen = DEFAULT_BUFLEN;

    char *Host_IP;                                                                  // the IP of the server

    unsigned int Host_Port;                                                         // the port to be used

    unsigned int msg_buffer;                                                        // the message buffer

    bool isComActive;                                                               // a flag for checking if the communication with the server is active

    rapidjson::Document dDoc;                                                       // a json object to store the data to a specific structure

    rapidjson::StringBuffer str_buffer;                                             // buffer for stringify the json object
    
    // variables used in the communication protocol
    char *msg_idf;                                                                  // the new message identifier  
    char *endMSG;                                                                   // the end-of-message identifier
    char *ec_id;                                                                    // the end-of-connection identifier
    unsigned int headerSize;                                                        // the header of the message containing the number of bytes to be streamed

    std::string msgHeader;
    std::string msg2send;                                                           // the message to be sent

    bool msgInitilized;                                                             // a flag to check if the message is initialized

    bool useHashKey;                                                                // a flag indicating if a md5 checksum will be included in the message or not

    int printArray(rapidjson::Value::ConstMemberIterator itr);                      // printing an array from iterator

public:

    socketStream(void);                                                             // empty constructor, setting the default values

    socketStream(char* scrIPAdress);                                                // constructor with setting the server IP address

    socketStream(char* svrIPAddress, int srvPosrt);                                 // constructor with setting the server IP address and port

    int initialize_sockeStream();                                                   // initialize the socketStream object

    int initialize_sockeStream(char* svrIPAddress, int srvPosrt);                   // initialize the socketStream object re-setting the server IP address and port

    int make_connection();                                                          // connect with the server

    int initialize_msgStruct(std::vector<std::string> fields);                      // setting the fields of the message 

    int printMSGcontents();                                                         // print the contents of the message

    int printMSGcontentsTypes();                                                    // print everything that is contained in the message

    int printMSGString();                                                           // get the message contents as strins

    
    int updateMSG(std::string field, char *value);                                  // update the specific field of the message (for strings)
    int updateMSG(std::string field, int *value, int arraylength);                  // update the specific field of the message (for array of integers)
    int updateMSG(std::string field, double *value, int arraylength);               // update the specific field of the message (for array of doubles)
    int updateMSG(std::string field, std::vector <int> value);                      // update the specific field of the message (for vector of integers)
    int updateMSG(std::string field, std::vector <double> value);                   // update the specific field of the message (for vector of doubles)
    int updateMSG(std::string field, std::vector <std::vector <int>> value);        // update the specific field of the message (for 2D matrix of integers)
    int updateMSG(std::string field, std::vector <std::vector <double>> value);     // update the specific field of the message (for 2D matrix of doubles)

    int sendMSg();                                                                  // send message to the server

    int closeCommunication();                                                       // close the communication with the server

    int setHashKey(bool hKey);                                                      // setting the value of useHashKey

    int setHeaderSize(unsigned int hSize);                                                   // setting the header size

    ~socketStream(void);                                                            // destructor

    
};




#endif