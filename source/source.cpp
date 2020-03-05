

#include "socketStream.h"
#include <algorithm>

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

    if(socketHdlr.initialize_msgStruct(test_rjson)<0){
        std::cerr << "Unable to inizialize message structure" << std::endl;
        return -1;
    }

    std::string sfield("name");
    char *svalue = {"Iason"};
    socketHdlr.updateMSG(sfield, svalue);


    sfield="year";
    double t_value1[] = {1.5,4.67,50.095,14.99,12};
    double t_value2[] = {3.2,15.4,1502.898,12,5.4};
    std::vector< std::vector<double> > t_value(2);
    t_value[0]=std::vector<double>(t_value1, t_value1 +(sizeof(t_value1)/sizeof(t_value1[0])));
    t_value[1]=std::vector<double>(t_value2, t_value2 +(sizeof(t_value2)/sizeof(t_value2[0])));
    socketHdlr.updateMSG(sfield, t_value);
     

    if(socketHdlr.initialize_sockeStream()<0){
        std::cerr << "Unable to initialize socket" << std::endl;
        return -1;
    }

    if(socketHdlr.make_connection()<0){
        std::cerr << "Unable to connect to " << srvIP << std::endl;
        return -1;
    }

    if(socketHdlr.sendMSg()<0){
        std::cerr << "unable to send message " << std::endl;
        return -1;
    }

    std::cout << "the sent message: " << socketHdlr.getFullmsg() << std::endl;

    if(socketHdlr.sendMSg()<0){
        std::cerr << "unable to send message " << std::endl;
        return -1;
    }

    std::cout << "the sent message: " << socketHdlr.getFullmsg() << std::endl;

    socketHdlr.closeCommunication();



    return 0;
}