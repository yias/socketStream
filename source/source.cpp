

#include "socketStream.h"
// #include <algorithm>
#include <chrono>
// #include <ctime>


int main(int argc, char **argv){

    char *srvIP = "localhost";//"128.178.145.67";

    if(argc!=2){
        std::cout << "No server IP suplied. Continue with localhost" << std::endl;
    }else{
        srvIP=argv[1];
    }

    socketStream socketHdlr(srvIP);

    std::vector <std::string> test_rjson;

    test_rjson.push_back("name");
    test_rjson.push_back("age");
    test_rjson.push_back("year");

    auto start = std::chrono::steady_clock::now();

    if(socketHdlr.initialize_msgStruct(test_rjson)<0){
        std::cerr << "Unable to inizialize message structure" << std::endl;
        return -1;
    }

    auto end = std::chrono::steady_clock::now();
    std::cout << "Message initialization: " << (double)(std::chrono::duration_cast<std::chrono::microseconds>(end-start).count())/1000.0 << " ms" << std::endl;

    std::string sfield("name");
    char *svalue = {"Iason"};

    start = std::chrono::steady_clock::now();
    socketHdlr.updateMSG(sfield, svalue);
    end = std::chrono::steady_clock::now();
    std::cout << "Message update (filed names): " << (double)std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()/1000.0 << " ms" << std::endl;


    sfield="year";
    double t_value1[] = {1.5,4.67,50.095,14.99,12};
    double t_value2[] = {3.2,15.4,1502.898,12,5.4};
    std::vector< std::vector<double> > t_value(2);
    t_value[0]=std::vector<double>(t_value1, t_value1 +(sizeof(t_value1)/sizeof(t_value1[0])));
    t_value[1]=std::vector<double>(t_value2, t_value2 +(sizeof(t_value2)/sizeof(t_value2[0])));
    
    start = std::chrono::steady_clock::now();
    socketHdlr.updateMSG(sfield, t_value);
    end = std::chrono::steady_clock::now();
    std::cout << "Message update (array of arrays): " << (double)std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()/1000.0 << " ms" << std::endl;
     

    if(socketHdlr.initialize_sockeStream()<0){
        std::cerr << "Unable to initialize socket" << std::endl;
        return -1;
    }

    if(socketHdlr.make_connection()<0){
        std::cerr << "Unable to connect to " << srvIP << std::endl;
        return -1;
    }
    start = std::chrono::steady_clock::now();
    if(socketHdlr.sendMSg()<0){
        std::cerr << "unable to send message " << std::endl;
        return -1;
    }
    end = std::chrono::steady_clock::now();
    std::cout << "Sending message: " << (double)std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()/1000.0 << " ms" << std::endl;

    // std::cout << "the sent message: " << socketHdlr.getFullmsg() << std::endl;

    t_value[0][1]=45.4;
    t_value[1][2]=1111.456;
    start = std::chrono::steady_clock::now();
    socketHdlr.updateMSG(sfield, t_value);
    if(socketHdlr.sendMSg()<0){
        std::cerr << "unable to send message " << std::endl;
        return -1;
    }
    end = std::chrono::steady_clock::now();
    std::cout << "Update and send message: " << (double)std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()/1000.0 << " ms" << std::endl;

    start = std::chrono::steady_clock::now();
    socketHdlr.updateMSG(sfield, t_value);
    if(socketHdlr.sendMSg()<0){
        std::cerr << "unable to send message " << std::endl;
        return -1;
    }
    end = std::chrono::steady_clock::now();
    std::cout << "Update and send message 2: " << (double)std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()/1000.0 << " ms" << std::endl;

    // std::cout << "the sent message: " << socketHdlr.getFullmsg() << std::endl;

    t_value[0][1]=12.6874;
    t_value[1][2]=222.5609;
    socketHdlr.setHashKey(false);

    start = std::chrono::steady_clock::now();
    socketHdlr.updateMSG(sfield, t_value);
    
    if(socketHdlr.sendMSg()<0){
        std::cerr << "unable to send message " << std::endl;
        return -1;
    }
    end = std::chrono::steady_clock::now();
    std::cout << "Update and send message (without checksum): " << (double)std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()/1000.0 << " ms" << std::endl;

    start = std::chrono::steady_clock::now();
    socketHdlr.updateMSG(sfield, t_value);
    
    if(socketHdlr.sendMSg()<0){
        std::cerr << "unable to send message " << std::endl;
        return -1;
    }
    end = std::chrono::steady_clock::now();
    std::cout << "Update and send message (without checksum) 2: " << (double)std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()/1000.0 << " ms" << std::endl;

    // std::cout << "the sent message: " << socketHdlr.getFullmsg() << std::endl;


    socketHdlr.closeCommunication();


    return 0;
}