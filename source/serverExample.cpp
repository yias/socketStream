

#include "socketStream.h"

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
    socketStream svrHdlr(srvIP, svrPort, SOCKETSTREAM_SERVER);

    svrHdlr.initialize_socketStream();

    svrHdlr.runServer();


    std::string msg;
    rapidjson::Document tmp_doc;
    int counter = 0;
    bool isNew = false;

    while(true){
        if(svrHdlr.sockectStream_ok()){
            msg = svrHdlr.get_latest("py_example", &isNew);
            if(isNew){
                std::cout << msg << std::endl;
            }
        }

        if(kbhit()){
            #ifdef _WIN32
                if(getch()=='q')
                    break;
            #endif
            #ifdef linux
                if(getch()=='q')
                    break;
            #endif
        }
    } 

    svrHdlr.closeCommunication();

    return 0;
}