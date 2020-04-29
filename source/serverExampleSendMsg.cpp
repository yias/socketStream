/**
 * 
 *  Examples on how to use the socketStream class for implementing a TCP server 
 *  with sending messages to clients
 * 
 *  Developer:  Iason Batzianoulis
 *  email:      iasonbatz@gmail.com
 *  
 *  
*/

#include "socketStream.h"
#include "jsonWrapper.hpp"

int main(int argc, char **argv){
    
    // define the variable that holds the server IP. In this case, the server would be a local server.
    const char *srvIP = "localhost";

    int svrPort = 10352;

    // if no new server IP is defined from the user, continue with the pre-defined server IP (localhost)
    if(argc!=2){
        std::cout << "No server IP provided. Continue with localhost" << std::endl;
    }else{
        srvIP=argv[1];
    }

    // create an sockectStream object with the selected server IP address and set it up as a server
    socketStream svrHdlr(srvIP, svrPort, SOCKETSTREAM::SOCKETSTREAM_SERVER);

    // decalre a variable as a vector of strings for containing the fields of the message
    std::vector <std::string> test_fields;

    // define the name of the fields
    test_fields.push_back("name");
    test_fields.push_back("data");

    // initialize the message by setting the fields' names
    if(svrHdlr.initialize_msgStruct(test_fields)<0){
        std::cerr << "Unable to inizialize message structure" << std::endl;
        return -1;
    }

    // define a which field will be updated and with which value
    std::string sfield("name");
    const char *svalue = {"Tom"};

    if(svrHdlr.updateMSG(sfield, svalue)){
        std::cerr << "Unable to update the message" << std::endl;
        return -2;
    }

    // define a 2D matrix and update the field "data" of the message
    double t_value1[] = {1.5, 4.67, 50.095, 14.99, 12, 45.11, 7.986, 134.67, 99.324, 0.452, 1.5, 4.67, 50.095, 14.99, 12, 45.11, 7.986, 134.67, 99.324, 0.452,1.5, 4.67, 50.095, 14.99, 12, 45.11, 7.986, 134.67, 99.324, 0.452, 3.2, 15.4, 1502.898, 12, 5.4, 0.569, 12.33, 5311.1, 234.22, 14.45, 3.2, 15.4, 1502.898, 12, 5.4, 0.569, 12.33, 5311.1, 234.22, 14.45};
    double t_value2[] = {3.2, 15.4, 1502.898, 12, 5.4, 0.569, 12.33, 5311.1, 234.22, 14.45, 1.5, 4.67, 50.095, 14.99, 12, 45.11, 7.986, 134.67, 99.324, 0.452,1.5, 4.67, 50.095, 14.99, 12, 45.11, 7.986, 134.67, 99.324, 0.452, 3.2, 15.4, 1502.898, 12, 5.4, 0.569, 12.33, 5311.1, 234.22, 14.45, 3.2, 15.4, 1502.898, 12, 5.4, 0.569, 12.33, 5311.1, 234.22, 14.45};
    double t_value3[] = {1.5, 4.67, 50.095, 14.99, 12, 45.11, 7.986, 134.67, 99.324, 0.452, 1.5, 4.67, 50.095, 14.99, 12, 45.11, 7.986, 134.67, 99.324, 0.452,1.5, 4.67, 50.095, 14.99, 12, 45.11, 7.986, 134.67, 99.324, 0.452, 3.2, 15.4, 1502.898, 12, 5.4, 0.569, 12.33, 5311.1, 234.22, 14.45, 3.2, 15.4, 1502.898, 12, 5.4, 0.569, 12.33, 5311.1, 234.22, 14.45};
    double t_value4[] = {3.2, 15.4, 1502.898, 12, 5.4, 0.569, 12.33, 5311.1, 234.22, 14.45, 1.5, 4.67, 50.095, 14.99, 12, 45.11, 7.986, 134.67, 99.324, 0.452,1.5, 4.67, 50.095, 14.99, 12, 45.11, 7.986, 134.67, 99.324, 0.452, 3.2, 15.4, 1502.898, 12, 5.4, 0.569, 12.33, 5311.1, 234.22, 14.45, 3.2, 15.4, 1502.898, 12, 5.4, 0.569, 12.33, 5311.1, 234.22, 14.45};
    double t_value5[] = {1.5, 4.67, 50.095, 14.99, 12, 45.11, 7.986, 134.67, 99.324, 0.452, 1.5, 4.67, 50.095, 14.99, 12, 45.11, 7.986, 134.67, 99.324, 0.452,1.5, 4.67, 50.095, 14.99, 12, 45.11, 7.986, 134.67, 99.324, 0.452, 3.2, 15.4, 1502.898, 12, 5.4, 0.569, 12.33, 5311.1, 234.22, 14.45, 3.2, 15.4, 1502.898, 12, 5.4, 0.569, 12.33, 5311.1, 234.22, 14.45};
    double t_value6[] = {3.2, 15.4, 1502.898, 12, 5.4, 0.569, 12.33, 5311.1, 234.22, 14.45, 1.5, 4.67, 50.095, 14.99, 12, 45.11, 7.986, 134.67, 99.324, 0.452,1.5, 4.67, 50.095, 14.99, 12, 45.11, 7.986, 134.67, 99.324, 0.452, 3.2, 15.4, 1502.898, 12, 5.4, 0.569, 12.33, 5311.1, 234.22, 14.45, 3.2, 15.4, 1502.898, 12, 5.4, 0.569, 12.33, 5311.1, 234.22, 14.45};
    std::vector< std::vector<double> > t_value(6);
    t_value[0]=std::vector<double>(t_value1, t_value1 +(sizeof(t_value1)/sizeof(t_value1[0])));
    t_value[1]=std::vector<double>(t_value2, t_value2 +(sizeof(t_value2)/sizeof(t_value2[0])));
    t_value[2]=std::vector<double>(t_value3, t_value3 +(sizeof(t_value3)/sizeof(t_value3[0])));
    t_value[3]=std::vector<double>(t_value4, t_value4 +(sizeof(t_value4)/sizeof(t_value4[0])));
    t_value[4]=std::vector<double>(t_value5, t_value5 +(sizeof(t_value5)/sizeof(t_value5[0])));
    t_value[5]=std::vector<double>(t_value6, t_value6 +(sizeof(t_value6)/sizeof(t_value6[0])));

    sfield="data";

    // update the message
    if(svrHdlr.updateMSG(sfield, t_value)){
        std::cerr << "Unable to update the message" << std::endl;
        return -3;
    }

    // initialize socketStream
    svrHdlr.initialize_socketStream();

    // activate the server
    svrHdlr.runServer();

    // define a string object to receive the data
    std::string msg;
    rapidjson::Document tmp_doc;

    //  define a boolean variable for checking if the message is new or not
    bool isNew = false;

    // a 2D matrix of doubles to store the received data
    std::vector< std::vector<double> > mat_double;
    
    // run until the key "q" is pressed in the keyboard
    while(true){

        if(svrHdlr.socketStream_ok()){
            svrHdlr.sendMSg2Client(0);
        }

        // check if a key is hit on the keyboard
        // if yes, check if this key is "q"
        // if yes, break the loob, otherwise continue
        if(kbhit()){
            #ifdef _WIN32
                if(getch()=='q')
                    break;
            #endif
            #ifdef __linux__
                if(getch()=='q')
                    break;
            #endif
        }
    } 
    
    // kill all the communications and the socket
    svrHdlr.closeCommunication();

    return 0;
}