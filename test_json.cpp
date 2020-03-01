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
#define HOST_IP "192.168.1.104"




int main(int argc, char **argv){

    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    const char *sendbuf = "this is a test";

    memset(&hints, 0, sizeof hints);

    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;
    
    // Validate the parameters
    if (argc != 2) {
        printf("usage: %s server-name\n", argv[0]);
        return 1;
    }    

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    std::cout<<argv[1]<<"\n";

    // Resolve the server address and port
    iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    //  define message identifier
	const char* msg_idf="!&?5";

    //  end-of-message identifier
	const char* endMSG="!3tt";

    //  end-connection identifier
	const char* ec_id="\ne@c";


    const char* json_object ="{\"name\":\"Ted\",\"age\":15}";

    rapidjson::Document dDoc;
    dDoc.Parse(json_object);

    // std::vector < std::string > t_msg{"test1", "test2"};
    
    // for (const std::string& word : t_msg){
    //     std::cout << word << " ";
    // }
    // std::cout<<std::endl;

    // stringify the jason object
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    dDoc.Accept(writer);

    std::cout << buffer.GetString() << std::endl;
    std::cout<<"sending message beginning id ..."<<std::endl;
    // Send an initial buffer
    iResult = send( ConnectSocket, msg_idf, (int)strlen(msg_idf), 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    std::cout<<"message-id sent"<<std::endl;

    printf("Bytes Sent: %ld\n", iResult);

    std::cout<<"sending message ..."<<std::endl;
    const char* t_msg = buffer.GetString();
    iResult = send( ConnectSocket, t_msg, (int)strlen(t_msg), 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    std::cout<<"message sent"<<std::endl;

    printf("Bytes Sent: %ld\n", iResult);

    std::cout<<"sending end-of-message ..."<<std::endl;
    iResult = send( ConnectSocket, endMSG, (int)strlen(endMSG), 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    std::cout<<"end-of-message sent"<<std::endl;

    printf("Bytes Sent: %ld\n", iResult);

    std::cout<<"sending end-connection ..."<<std::endl;

    iResult = send( ConnectSocket, ec_id, (int)strlen(ec_id), 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    std::cout<<"end-connection sent"<<std::endl;


     // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    
    printf("Bytes Sent: %ld\n", iResult);


    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}
