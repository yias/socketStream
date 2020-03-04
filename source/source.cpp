

#include "socketStream.h"

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


    std::cout<<"modifying msg"<<std::endl;

    std::string sfield("name");
    char *svalue = {"Iason"};

    socketHdlr.updateMSG(sfield, svalue);
    socketHdlr.getMSGString();
    socketHdlr.printMSGcontents();

    sfield="year";
    double t_value1[] = {1.5,4.67,50.095,14.99,12};
    double t_value2[] = {3.2,15.4,1502.898,12,5.4};
    std::vector< std::vector<double> > t_value(2);
    t_value[0]=std::vector<double>(t_value1, t_value1 +(sizeof(t_value1)/sizeof(t_value1[0])));
    t_value[1]=std::vector<double>(t_value2, t_value2 +(sizeof(t_value2)/sizeof(t_value2[0])));

    std::cout << "t_value:" << std::endl;
    for(int i=0; i<(int)t_value.size(); i++){
        for(int j=0; j<(int)t_value[i].size(); j++){
            std::cout << t_value[i][j] <<", ";
        }
        std::cout << std::endl;
    }

    socketHdlr.updateMSG(sfield, t_value);
    // socketHdlr.updateMSG(sfield, t_value, sizeof(t_value)/sizeof(t_value[0]));
    socketHdlr.printMSGcontentsTypes();
    socketHdlr.getMSGString();
    socketHdlr.printMSGcontents();

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

    socketHdlr.closeCommunication();



    return 0;
}