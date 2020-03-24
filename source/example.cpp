
#include "socketStream.h"

int main(int argc, char **argv){

    // define the variable that holds the server IP. In this case, the server would be a local server.
    const char *srvIP = "localhost";

    // if no new server IP is defined from the user, continue with the pre-defined server IP (localhost)
    if(argc!=2){
        std::cout << "No server IP suplied. Continue with localhost" << std::endl;
    }else{
        srvIP=argv[1];
    }

    // create an sockectStream object with the selected server IP address 
    socketStream socketHdlr(srvIP);

    // decalre a variable as a vector of strings for containing the fields of the message
    std::vector <std::string> test_fields;

    // define the name of the fields
    test_fields.push_back("name");
    test_fields.push_back("data");

    // initialize the message by setting the fields' names
    if(socketHdlr.initialize_msgStruct(test_fields)<0){
        std::cerr << "Unable to inizialize message structure" << std::endl;
        return -1;
    }

    // define a which field will be updated and with which value
    std::string sfield("name");
    const char *svalue = {"Tom"};

    if(socketHdlr.updateMSG(sfield, svalue)){
        std::cerr << "Unable to update the message" << std::endl;
        return -2;
    }

    // define a 2D matrix and update the field "data" of the message
    double t_value1[] = {1.5,4.67,50.095,14.99,12};
    double t_value2[] = {3.2,15.4,1502.898,12,5.4};
    std::vector< std::vector<double> > t_value(2);
    t_value[0]=std::vector<double>(t_value1, t_value1 +(sizeof(t_value1)/sizeof(t_value1[0])));
    t_value[1]=std::vector<double>(t_value2, t_value2 +(sizeof(t_value2)/sizeof(t_value2[0])));

    sfield="data";

    // update the message
    if(socketHdlr.updateMSG(sfield, t_value)){
        std::cerr << "Unable to update the message" << std::endl;
        return -3;
    }

    socketHdlr.printMSGString();


    // initialize the socket
    if(socketHdlr.initialize_socketStream()<0){
        std::cerr << "Unable to initialize socket" << std::endl;
        return -4;
    }

    // set client's name
    socketHdlr.set_clientName("example");

    // attemp a connection with the server
    if(socketHdlr.make_connection()<0){
        std::cerr << "Unable to connect to " << srvIP << std::endl;
        return -5;
    }

    
    // send the message to the server
    if(socketHdlr.sendMSg()<0){
        std::cerr << "unable to send message " << std::endl;
        return -6;
    }


    // close communication with the server
    socketHdlr.closeCommunication();

    return 0;
}

    // sfield="year";
    // double t_value1[] = {1.5,4.67,50.095,14.99,12};
    // double t_value2[] = {3.2,15.4,1502.898,12,5.4};
    // std::vector< std::vector<double> > t_value(2);
    // t_value[0]=std::vector<double>(t_value1, t_value1 +(sizeof(t_value1)/sizeof(t_value1[0])));
    // t_value[1]=std::vector<double>(t_value2, t_value2 +(sizeof(t_value2)/sizeof(t_value2[0])));
    
    // start = std::chrono::steady_clock::now();
    // socketHdlr.updateMSG(sfield, t_value);
    // end = std::chrono::steady_clock::now();
    // std::cout << "Message update (array of arrays): " << (double)std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()/1000.0 << " ms" << std::endl;
     

    // if(socketHdlr.initialize_sockeStream()<0){
    //     std::cerr << "Unable to initialize socket" << std::endl;
    //     return -1;
    // }

    // if(socketHdlr.make_connection()<0){
    //     std::cerr << "Unable to connect to " << srvIP << std::endl;
    //     return -1;
    // }
    // start = std::chrono::steady_clock::now();
    // if(socketHdlr.sendMSg()<0){
    //     std::cerr << "unable to send message " << std::endl;
    //     return -1;
    // }
    // end = std::chrono::steady_clock::now();
    // std::cout << "Sending message: " << (double)std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()/1000.0 << " ms" << std::endl;

    // // std::cout << "the sent message: " << socketHdlr.getFullmsg() << std::endl;

    // t_value[0][1]=45.4;
    // t_value[1][2]=1111.456;
    // start = std::chrono::steady_clock::now();
    // socketHdlr.updateMSG(sfield, t_value);
    // if(socketHdlr.sendMSg()<0){
    //     std::cerr << "unable to send message " << std::endl;
    //     return -1;
    // }
    // end = std::chrono::steady_clock::now();
    // std::cout << "Update and send message: " << (double)std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()/1000.0 << " ms" << std::endl;

    // start = std::chrono::steady_clock::now();
    // socketHdlr.updateMSG(sfield, t_value);
    // if(socketHdlr.sendMSg()<0){
    //     std::cerr << "unable to send message " << std::endl;
    //     return -1;
    // }
    // end = std::chrono::steady_clock::now();
    // std::cout << "Update and send message 2: " << (double)std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()/1000.0 << " ms" << std::endl;

    // // std::cout << "the sent message: " << socketHdlr.getFullmsg() << std::endl;

    // t_value[0][1]=12.6874;
    // t_value[1][2]=222.5609;
    // socketHdlr.setHashKey(false);

    // start = std::chrono::steady_clock::now();
    // socketHdlr.updateMSG(sfield, t_value);
    
    // if(socketHdlr.sendMSg()<0){
    //     std::cerr << "unable to send message " << std::endl;
    //     return -1;
    // }
    // end = std::chrono::steady_clock::now();
    // std::cout << "Update and send message (without checksum): " << (double)std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()/1000.0 << " ms" << std::endl;

    // start = std::chrono::steady_clock::now();
    // socketHdlr.updateMSG(sfield, t_value);
    
    // if(socketHdlr.sendMSg()<0){
    //     std::cerr << "unable to send message " << std::endl;
    //     return -1;
    // }
    // end = std::chrono::steady_clock::now();
    // std::cout << "Update and send message (without checksum) 2: " << (double)std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()/1000.0 << " ms" << std::endl;

    // // std::cout << "the sent message: " << socketHdlr.getFullmsg() << std::endl;
