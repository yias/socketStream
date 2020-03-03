

#include "socketStream.h"

int main(int argc, char **argv){

    char *srvIP = "128.178.145.67";

    int srvPort = 1443;

    socketStream socketHdlr(srvIP, srvPort);

    std::vector <std::string> test_rjson;

    test_rjson.push_back("name");
    test_rjson.push_back("age");
    test_rjson.push_back("year");

    int tt0=socketHdlr.initialize_msgStruct(test_rjson);

    socketHdlr.printMSGcontentsTypes();
    socketHdlr.getMSGString();

    std::cout<<"modifying msg"<<std::endl;

    std::string sfield("name");
    // std::string svalue("Iason");
    char *svalue = {"Iason"};

    socketHdlr.updateMSG(sfield, svalue);
    socketHdlr.printMSGcontentsTypes();
    socketHdlr.getMSGString();
    socketHdlr.printMSGcontents();

    sfield="year";
    int t_value[] = {1,4,50};
    // std::cout << "the size of value is: " << sizeof(t_value)/sizeof(t_value[0]) <<"\n";

    socketHdlr.updateMSG(sfield, t_value, sizeof(t_value)/sizeof(t_value[0]));
    socketHdlr.printMSGcontentsTypes();
    socketHdlr.getMSGString();
    socketHdlr.printMSGcontents();

    



    return 0;
}