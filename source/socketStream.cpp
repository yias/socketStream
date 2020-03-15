
#include "socketStream.h"


socketStream::socketStream(void){

    ConnectSocket = INVALID_SOCKET;

    ListenSocket = INVALID_SOCKET;                      // for the server

    result = NULL;
    
    ptr = NULL;

    isServer = false;
    serverRunning = false;

    msg_idf = "!&?5";

    endMSG = "!3tt";

    ec_id = "\ne@c";

    headerSize=8;

    bufferSize=16;

    msgOverhead = 0;

    msgHeader = std::string(headerSize, ' ');

    msgOHstring = std::string(std::to_string(bufferSize).length(), ' ');

    Host_IP = DEFAULT_HOST_IP;

    Host_Port = DEFAULT_PORT;

    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    isComActive = false;

    msgInitilized = false;

    useHashKey = true;

    minMsgSize = std::strlen(msg_idf) + headerSize + std::strlen(endMSG) + std::to_string(bufferSize).length() +1;

    std::cout << "[socketStream] Starting socketStream with default parameters" << std::endl;
    std::cout << "[socketStream] IP address: " << Host_IP << ", Port: " << Host_Port << std::endl;
}


socketStream::socketStream(const char* svrIPAddress, const int socketStreamMode){

    ConnectSocket = INVALID_SOCKET;

    result = NULL;
    
    ptr = NULL;

    if(socketStreamMode==SOCKETSTREAM_SERVER){
        isServer = true;
    }else{
        isServer = false;
    }
    

    msg_idf = "!&?5";

    endMSG = "!3tt";

    ec_id = "\ne@c";

    headerSize = 8;

    bufferSize=16;

    msgOverhead = 0;

    msgHeader = std::string(headerSize, ' ');

    msgOHstring = std::string(std::to_string(bufferSize).length(), ' ');

    Host_IP = svrIPAddress;

    Host_Port = DEFAULT_PORT;

    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if(isServer){
        hints.ai_flags = AI_PASSIVE;                   // for the server
        serverRunning = false;
    } 

    isComActive = false;

    msgInitilized = false;

    useHashKey = true;

    minMsgSize = std::strlen(msg_idf) + headerSize + std::strlen(endMSG) + std::to_string(bufferSize).length() + 1;

    std::cout << "[socketStream] Starting up socketStream on server address " << Host_IP << " and default port " << Host_Port << std::endl;
}


socketStream::socketStream(const char* svrIPAddress, int srvPosrt, const int socketStreamMode){

    ConnectSocket = INVALID_SOCKET;

    result = NULL;
    
    ptr = NULL;

    if(socketStreamMode==SOCKETSTREAM_SERVER){
        isServer = true;
    }else{
        isServer = false;
    }

    msg_idf = "!&?5";

    endMSG = "!3tt";

    ec_id = "\ne@c";

    headerSize=8;

    bufferSize=16;

    msgOverhead = 0;

    msgHeader = std::string(headerSize, ' ');

    msgOHstring = std::string(std::to_string(bufferSize).length(), ' ');

    Host_IP = svrIPAddress;

    Host_Port = (unsigned int)srvPosrt;

    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if(isServer){
        hints.ai_flags = AI_PASSIVE;                   // for the server
        serverRunning = false;
    } 

    isComActive = false;

    msgInitilized = false;

    useHashKey = true;

    minMsgSize = std::strlen(msg_idf) + headerSize + std::strlen(endMSG) + std::to_string(bufferSize).length()+1;

    std::cout << "[socketStream] Starting socketStream on server address " << Host_IP << " and port " << Host_Port << std::endl;
}

int socketStream::initialize_socketStream(){

    #ifdef _WIN32
        // Initialize Winsock
        iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
        if (iResult != 0) {
            std::cerr << "[socketStream] WSAStartup failed with error: " << iResult << std::endl;
            return iResult;
        }
    #endif

    std::ostringstream i2s;
    i2s << Host_Port;

    // Resolve the server address and port
    iResult = getaddrinfo(Host_IP, i2s.str().c_str(), &hints, &result);
    if ( iResult != 0 ) {
        std::cerr << "[socketStream] Getaddrinfo failed with error: " << iResult << std::endl;
        #ifdef _WIN32
            WSACleanup();
        #endif
        return iResult;
    }

    if(isServer){
        // Create a SOCKET for connecting to server
        ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (ListenSocket == INVALID_SOCKET) {
            std::cerr << "[socketStream] Socket failed with error:";
            #ifdef _WIN32
                std::cerr << WSAGetLastError(); ;
                WSACleanup();
            #endif
            std::cerr << std::endl;
            freeaddrinfo(result);
            return -3;
        }

        // Setup the TCP listening socket
        iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            std::cerr << "[socketStream] Bind failed with error:";
            #ifdef _WIN32
                std::cerr << WSAGetLastError();
                WSACleanup();
            #endif
            std::cerr << std::endl;
            closesocket(ListenSocket);
            freeaddrinfo(result);
            return -4;
        }

        freeaddrinfo(result);
    }

    return 0;

}


int socketStream::initialize_socketStream(const char* svrIPAddress, int srvPosrt){
    
    // setting the server IP address and port
    Host_IP = svrIPAddress;

    Host_Port = (unsigned int)srvPosrt;

     #ifdef _WIN32
        // Initialize Winsock
        iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
        if (iResult != 0) {
            std::cerr << "[socketStream] WSAStartup failed with error: " << iResult << std::endl;
            return iResult;
        }
    #endif

    std::ostringstream i2s;
    i2s << Host_Port;

    // Resolve the server address and port
    iResult = getaddrinfo(Host_IP, i2s.str().c_str(), &hints, &result);
    if ( iResult != 0 ) {
        std::cerr << "getaddrinfo failed with error: " << iResult << std::endl;;
        #ifdef _WIN32
            WSACleanup();
        #endif
        return iResult;
    }

    if(isServer){
        // Create a SOCKET for connecting to server
        ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (ListenSocket == INVALID_SOCKET) {
            std::cerr << "[socketStream] Socket failed with error:";
            #ifdef _WIN32
                std::cerr << WSAGetLastError(); ;
                WSACleanup();
            #endif
            std::cerr << std::endl;
            freeaddrinfo(result);
            return -3;
        }

        // Setup the TCP listening socket
        iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            std::cerr << "[socketStream] Bind failed with error:";
            #ifdef _WIN32
                std::cerr << WSAGetLastError();
                WSACleanup();
            #endif
            std::cerr << std::endl;
            closesocket(ListenSocket);
            freeaddrinfo(result);
            return -4;
        }

        freeaddrinfo(result);
    }

    return 0;

}

int socketStream::make_connection(){

    std::cout << "[socketStream] Attempting to connect to server with address " << Host_IP << " in the port " << Host_Port << std::endl;

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            std::cerr << "[socketStream] socket failed with error: " <<  WSAGetLastError() << std::endl;
            #ifdef _WIN32
                WSACleanup();
            #endif
            return -1;
        }

        // Connect to server.
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        std::cerr << "[socketStream] Unable to connect to server!" << std::endl;
        #ifdef _WIN32
            WSACleanup();
        #endif
        return -2;
    }

    std::cout << "[socketStream] Connected to server " << Host_IP << " in port " << Host_Port << std::endl;

    isComActive = true;

    return 0;

}

int socketStream::initialize_msgStruct(std::vector <std::string> fields){
    
    // declare a string object to throw the fields
    std::string initial_rjson_obj;

    // construct the string with a proper syntax
    initial_rjson_obj += "{\"";

    for (int i=0; i<fields.size(); i++){
        initial_rjson_obj += fields[i];
        initial_rjson_obj += "\" : null, \"";
    }

    initial_rjson_obj.erase(initial_rjson_obj.length()-3);
    initial_rjson_obj += "}";

    // check if all the fields are 
    if(dDoc.Parse(initial_rjson_obj.c_str()).HasParseError()){
        return -1;
    }

    // check if json object is properly created
    assert(dDoc.IsObject());

    // check if a fields are properly defined
    for(int i = 0; i < fields.size(); i++ ){
        if(!dDoc.HasMember(fields[i].c_str())){
            std::cout << "dDoc doesn't have a member " << fields[i] << std::endl;
        }
    }

    std::cout << "[socketStream] The message struct initialized successfully" << std::endl;

    msgInitilized = true;

    return 0;
}


int socketStream::updateMSG(std::string field, const char *value){

    if(msgInitilized){
        if(dDoc.HasMember(field.c_str())){
            dDoc[field.c_str()].SetString(value, strlen(value));
        }else{
            std::cerr << "[socketStream] Not valid field name" << std::endl;
        }
    }else{
        std::cerr << "[socketStream] The message struct is NOT initialized" << std::endl;
        return -1;
    }

    return 0;
}


int socketStream::updateMSG(std::string field, int *value, int arraylength){


    if(msgInitilized){
        if(dDoc.HasMember(field.c_str())){
            rapidjson::Value t_a(rapidjson::kArrayType);
            rapidjson::Document::AllocatorType& allocator = dDoc.GetAllocator();
            for(int j= 0; j<arraylength; j++){
                t_a.PushBack(value[j],allocator);
            }
            if(!dDoc[field.c_str()].IsArray()){
                dDoc[field.c_str()].SetArray();
            }
            dDoc[field.c_str()]=t_a;
        }else{
            std::cerr << "[socketStream] Not valid field name" << std::endl;
        }
    }else{
        std::cerr << "[socketStream] The message struct is NOT initialized" << std::endl;
        return -1;
    }

    return 0;
}


int socketStream::updateMSG(std::string field, double *value, int arraylength){


    if(msgInitilized){
        if(dDoc.HasMember(field.c_str())){
            rapidjson::Value t_a(rapidjson::kArrayType);
            rapidjson::Document::AllocatorType& allocator = dDoc.GetAllocator();
            for(int j= 0; j<arraylength; j++){
                t_a.PushBack(value[j],allocator);
            }
            if(!dDoc[field.c_str()].IsArray()){
                dDoc[field.c_str()].SetArray();
            }
            dDoc[field.c_str()]=t_a;
        }else{
            std::cerr << "[socketStream] Not valid field name" << std::endl;
        }
    }else{
        std::cerr << "[socketStream] The message struct is NOT initialized" << std::endl;
        return -1;
    }

    return 0;
}


int socketStream::updateMSG(std::string field, std::vector <int> value){

    if(msgInitilized){
        if(dDoc.HasMember(field.c_str())){
            rapidjson::Value t_a(rapidjson::kArrayType);
            rapidjson::Document::AllocatorType& allocator = dDoc.GetAllocator();
            for(int j= 0; j<(int)value.size(); j++){
                t_a.PushBack(value[j],allocator);
            }
            if(!dDoc[field.c_str()].IsArray()){
                dDoc[field.c_str()].SetArray();
            }
            dDoc[field.c_str()]=t_a;
        }else{
            std::cerr << "[socketStream] Not valid field name" << std::endl;
        }
    }else{
        std::cerr << "[socketStream] The message struct is NOT initialized" << std::endl;
        return -1;
    }

    return 0;
}


int socketStream::updateMSG(std::string field, std::vector <double> value){

    if(msgInitilized){
        if(dDoc.HasMember(field.c_str())){
            rapidjson::Value t_a(rapidjson::kArrayType);
            rapidjson::Document::AllocatorType& allocator = dDoc.GetAllocator();
            for(int j= 0; j<(int)value.size(); j++){
                t_a.PushBack(value[j],allocator);
            }
            if(!dDoc[field.c_str()].IsArray()){
                dDoc[field.c_str()].SetArray();
            }
            dDoc[field.c_str()]=t_a;
        }else{
            std::cerr << "[socketStream] Not valid field name" << std::endl;
        }
    }else{
        std::cerr << "[socketStream] The message struct is NOT initialized" << std::endl;
        return -1;
    }

    return 0;
}


int socketStream::updateMSG(std::string field, std::vector <std::vector <int> > value){

    if(msgInitilized){
        if(dDoc.HasMember(field.c_str())){
            rapidjson::Value t_a(rapidjson::kArrayType);
            rapidjson::Document::AllocatorType& allocator = dDoc.GetAllocator();
            t_a.Reserve(value.size(),allocator);
            for(int j= 0; j<(int)value.size(); j++){
                rapidjson::Value t_b(rapidjson::kArrayType);
                t_b.Reserve(value[j].size(),allocator);
                for(int k=0; k<(int)value[j].size(); k++){
                    t_b.PushBack(value[j][k], allocator);
                }
                t_a.PushBack(t_b,allocator);
            }
            if(!dDoc[field.c_str()].IsArray()){
                dDoc[field.c_str()].SetArray();
            }
            dDoc[field.c_str()]=t_a;
        }else{
            std::cerr << "[socketStream] Not valid field name" << std::endl;
        }
    }else{
        std::cerr << "[socketStream] The message struct is NOT initialized" << std::endl;
        return -1;
    }

    return 0;
}


int socketStream::updateMSG(std::string field, std::vector <std::vector <double> > value){

    if(msgInitilized){
        if(dDoc.HasMember(field.c_str())){
            rapidjson::Value t_a(rapidjson::kArrayType);
            rapidjson::Document::AllocatorType& allocator = dDoc.GetAllocator();
            t_a.Reserve(value.size(),allocator);
            for(int j= 0; j<(int)value.size(); j++){
                rapidjson::Value t_b(rapidjson::kArrayType);
                t_b.Reserve(value[j].size(),allocator);
                for(int k=0; k<(int)value[j].size(); k++){
                    t_b.PushBack(value[j][k], allocator);
                }
                t_a.PushBack(t_b,allocator);
            }
            if(!dDoc[field.c_str()].IsArray()){
                dDoc[field.c_str()].SetArray();
            }
            dDoc[field.c_str()]=t_a;
        }else{
            std::cerr << "[socketStream] Not valid field name" << std::endl;
        }
    }else{
        std::cerr << "[socketStream] The message struct is NOT initialized" << std::endl;
        return -1;
    }

    return 0;
}

int socketStream::printArray(rapidjson::Value::ConstMemberIterator itr){

    
    std::cout << itr->name.GetString() << " = [";
    for(rapidjson::SizeType j =0 ; j<itr->value.Size(); j++){
        if(itr->value.IsArray()){
            std::cout << "[";
            for(rapidjson::SizeType k =0 ; k<itr->value[j].Size()-1; k++){
                std::cout << itr->value[j][k].GetDouble() << ", ";
            }
             std::cout << itr->value[j][itr->value[j].Size()-1].GetDouble() << "]";
            if(j!=itr->value.Size()-1){
               std::cout << std::endl; 
            }
        }else{
            std::cout << itr->value[j].GetDouble() << ", ";
        }
    }
    std::cout << "]" << std::endl; 

    return 0;
}

int socketStream::printMSGcontents(){

    if(msgInitilized){
        for (rapidjson::Value::ConstMemberIterator itr = dDoc.MemberBegin(); itr != dDoc.MemberEnd(); ++itr){
            switch (itr->value.GetType())
            {
            case 0:
                std::cout << itr->name.GetString() << " = NULL" << std::endl; 
                break;
            case 1:
                {
                    std::cout << std::boolalpha;
                    std::cout << itr->name.GetString() << " = " << bool(itr->value.GetBool()) << std::endl; 
                }
                break;
            case 2:
                {   
                    std::cout << std::boolalpha;
                    std::cout << itr->name.GetString() << " = " << bool(itr->value.GetBool()) << std::endl; 
                }
                break;
            case 3:
                {
                    std::cout << itr->name.GetString() << " = [object]" << std::endl;  
                }
                break;
            case 4:
                {
                    printArray(itr);
                    // std::cout << itr->name.GetString() << " = [";
                    // for(rapidjson::SizeType j =0 ; j<itr->value.Size()-1; j++){
                    //     std::cout << itr->value[j].GetDouble() << ", ";
                    // }
                    // std::cout << itr->value[itr->value.Size()-1].GetDouble()<< "]" << std::endl;    
                }
                break; 
            case 5:
                {
                    std::cout << itr->name.GetString() << " = " << itr->value.GetString() << std::endl; 
                }
                break;
                
            case 6:
                {
                    if(itr->value.IsInt64()){
                        std::cout << itr->name.GetString() << " = " << itr->value.GetInt64() << std::endl;
                    }else{
                        std::cout << itr->name.GetString() << " = " << itr->value.GetInt() << std::endl;
                    }
                }
                break;
                
            default:
                {
                    std::cerr << "[socketStream] Unknown type of field" << std::endl;
                }
                break;                
            }
        }
    }

    return 0;
}


int socketStream::printMSGcontentsTypes(){

    // print the types of the message contains

    if(msgInitilized){
        static const char* kTypeNames[] = { "Null", "False", "True", "Object", "Array", "String", "Number" };
        std::cout<< "[socketStream] Message contens types:" << std::endl;
        for (rapidjson::Value::ConstMemberIterator itr = dDoc.MemberBegin(); itr != dDoc.MemberEnd(); ++itr)
            std::cout << itr->name.GetString() << ": [" << kTypeNames[itr->value.GetType()] << "]" << std::endl;
    }else{
        std::cerr << "[socketStream] The message struct is NOT initialized" << std::endl;
        return -1;
    }
    
    return 0;
}

int socketStream::printMSGString(){

    // print the message as a string

    if(msgInitilized){
        rapidjson::StringBuffer t_buffer;
        rapidjson::Writer<rapidjson::StringBuffer> t_writer(t_buffer);
        dDoc.Accept(t_writer);
        std::cout << t_buffer.GetString() << std::endl;
    }else{
        std::cerr << "[socketStream] The message struct is NOT initialized" << std::endl;
        return -1;
    }

    return 0;
}

int socketStream::sendMSg(){
    /** 
     * 
     * The function composes the message and sends it
     * 
     * Return:
     *      the number of bytes sent, if completed correctly
     *      the error type (negative integer), if not completed correctly
     * 
     */
    

    if(msgInitilized){

        // clear the buffers
        str_buffer.Clear();
        final_msg.clear();
        msg2send.clear();

        // rapidjson::Writer<rapidjson::StringBuffer> writer(str_buffer);

        // get the json object in a string format
        writer.Reset(str_buffer);

        dDoc.Accept(writer);

        msg2send = str_buffer.GetString();

        // introduce the lenght of the string in the header of the file
        std::ostringstream i2s;
        i2s << msg2send.length();

        msgHeader.replace(msgHeader.begin(),msgHeader.end()+i2s.str().length()-msgHeader.length(),i2s.str());

        // if a checksum is needed, create and introduce it to the message
        if(useHashKey){
            std::string md5_key = md5(msg2send.c_str());
            msg2send += md5_key;
        }
        
        // compute the overhead of the message
        msgOverhead = (int((msg2send.length()+minMsgSize)/bufferSize)+1)*bufferSize - (msg2send.length()+minMsgSize);

        std::ostringstream i2s2;
        i2s2 << msgOverhead;        

        msgOHstring.replace(msgOHstring.begin(),msgOHstring.end()+i2s2.str().length()-msgOHstring.length(),i2s2.str());


        // compose the final message
        final_msg = msg_idf + msgHeader + std::to_string(int(useHashKey)) + msgOHstring + msg2send + std::string(msgOverhead, ' ') + endMSG;

        // send the message
        if(isComActive){
            // if the communication is active, send message
            iResult = send( ConnectSocket, final_msg.c_str(), (int)final_msg.size(), 0 );
            if (iResult == SOCKET_ERROR) {
                std::cerr << "[socketStream] Send message failed with error: " << WSAGetLastError() << std::endl;
                closesocket(ConnectSocket);
                #ifdef _WIN32
                    WSACleanup();
                #endif
                return -1;
            }
        }else{
            std::cerr << "[socketStream] The communication with the server is not open. " << std::endl;
            return -2;
        }

    }else{
        std::cerr << "[socketStream] The message struct id NOT initialized. " << std::endl;
        return -3;
    }

    // final_msg.clear();
    return iResult;
}

int socketStream::closeCommunication(){

    std::cout<<"[socketStream] Ending connection ..."<<std::endl;

    iResult = send( ConnectSocket, ec_id, (int)strlen(ec_id), 0 );
    if (iResult == SOCKET_ERROR) {
        std::cerr << "[socketStream] Send failed with error: " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        #ifdef _WIN32
            WSACleanup();
        #endif
        return -1;
    }

    #ifdef _WIN32
        iResult = shutdown(ConnectSocket, SD_SEND);
        if (iResult == SOCKET_ERROR) {
            std::cerr << "[socketStream] Shutdown failed with error: " << WSAGetLastError() << std::endl;
            closesocket(ConnectSocket);
            WSACleanup();
            return -2;
        }
    #else
        iResult = shutdown(ConnectSocket, SHUT_RDWR);
        if (iResult == 0){
            iResult = close(ConnectSocket);
            return -2;
        }
    #endif

    isComActive = false;

    // cleanup
    if(isServer){
        closesocket(ListenSocket);
        // shutdown the connection since we're done
        for(int i = 0; i < (int)clientsSockets.size(); i++){
            iResult = shutdown(clientsSockets[i], SD_SEND);
            if (iResult == SOCKET_ERROR) {
                std::cerr << "[socketStream] Shutting-down socket client " << i << " failed with error:" << WSAGetLastError() << std::endl;
                WSACleanup();
                return -3;
            }
            closesocket(clientsSockets[i]);
        }
        std::cout << "[socketStream] All sockets are successfully closed" << std::endl;
        std::cout << "[socketStream] Server is shutted-down" << std::endl;
    }
    else{
        closesocket(ConnectSocket);
    }
    
    #ifdef _WIN32
        WSACleanup();
    #endif

    return 0;
}

int socketStream::setHashKey(bool hKey){
    useHashKey=hKey;
    return 0;
}

int socketStream::setHeaderSize(unsigned int hSize){
    headerSize=hSize;
    msgHeader = std::string(headerSize, ' ');
    return 0;
}

std::string socketStream::getFullmsg(){
    return final_msg;
}

int socketStream::runServer(){

    if(!isServer){
        std::cerr << "[socketStream] socketStream is not in the server mode" << std::endl;
        return -1;
    }

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "[socketStream] Listen failed with error: "; 
        #ifdef _WIN32
            std::cerr << WSAGetLastError();
            WSACleanup();
        #endif
        std::cerr << std::endl;
        closesocket(ListenSocket);
        return -2;
    }

    serverRunning = true;

    // Accept a client socket
    SOCKET ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        std::cerr << "[socketStream] Accept failed with error: ";
        #ifdef _WIN32
            std::cerr << WSAGetLastError();
            WSACleanup();
        #endif
        std::cerr << std::endl;
        closesocket(ListenSocket);
        return -2;
    }

    return 0;
}


int socketStream::runReceiver(){
    return 0;
}


int socketStream::get_latest(){
    return 0;
}


socketStream::~socketStream(){
    if(isComActive){
        if(closeCommunication()<0){
            std::cerr << "[socketStream] The socket is NOT successully terminated. The port might still be occupied." << std::endl;
            }
        isComActive = false;
    }
    std::cout << "[socketStream] Connection terminated" << std::endl;
}