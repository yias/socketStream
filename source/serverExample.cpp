/**
 * 
 *  Examples on how to use the socketStream class for implementing a TCP server
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
            // if the socketStream server is active, receive a message from the client with the name "py_example"
            msg = svrHdlr.get_latest("sendWtime", &isNew);
            if(isNew){
                // if the message is new:
                // parse the json string in a json document
                jsonWrapper testObj(msg);

                // get the contains of the field "data"
                mat_double = testObj.getField<rapidJson_types::Mat2DD>(std::string("data"));

                // print the message to the console (uncomment the folling lines to print to the console)
                // std::cout << "The value of " << std::string("data") << " is: " << std::endl;
                // for (int i= 0; i< (int)mat_double.size(); i++){
                //     for (int j=0; j< (int)mat_double[i].size(); j++){
                //         std::cout << mat_double[i][j] << ((j== ((int)mat_double[i].size()-1)) ? "\n": ", " );
                //     }
                // }

                // // the message in a raw (string) format
                // std::cout << msg << std::endl;
            }
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