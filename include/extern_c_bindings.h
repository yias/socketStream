/**
 *  Copyright (C) 2020 Iason Batzianoulis
 *  
 *  socketStream C++ extern C bindings
 *  
 *  
 *  Developer:  Iason Batzianoulis
 *  email:      iasonbatz@gmail.com
 *  License:    GNU GPLv3
 * 
**/

#include "socketStream.h"


extern "C"{
    EXPORT socketStream* create_socketStream(const char* svrIPAddress, int srvPosrt, int socketStreamMode){
        // std::string hIP (svrIPAddress);
        // std::cout << hIP << std::endl;
        return new socketStream(svrIPAddress, srvPosrt, socketStreamMode);
    }

    int EXPORT ss_initialize(socketStream *ssObj){
        return ssObj->initialize_socketStream();
    }

    int EXPORT ss_make_connection(socketStream *ssOb){
        return ssOb->make_connection();
    }

    int EXPORT ss_setBufferSize(socketStream *ssOb, int bufferSize){
        if (bufferSize < 0){
            std::cout << "[socketStream] The bufer should be greater than zero" << std::endl;
            std::cout << "[socketStream] Unable to update buffer size" << std::endl;
            return -1;
        }else{
            return ssOb->setBufferSize((unsigned int)bufferSize);
        }
    }

    int EXPORT ss_setNumberOfConnections(socketStream *ssOb, int nbConnections){
        if (nbConnections <= 0){
            std::cout << "[socketStream] The maximum number of connections should be greater than zero" << std::endl;
            std::cout << "[socketStream] Unable to update the maximum number of connections" << std::endl;
            return -2;
        }else{
            return ssOb->setNumberOfConnections((unsigned int)nbConnections);
        }
    }

    int EXPORT ss_initialize_msgStruct(socketStream *ssOb, const char** fields, int nbFields){
        std::vector<std::string> inFields(nbFields, std::string());
        for (int i=0; i<nbFields; i++){
            inFields[i]=std::string(fields[i]);
            std::cout << inFields[i] << std::endl;
        }
        return ssOb->initialize_msgStruct(inFields);
    }

    int EXPORT ss_printMSGcontents(socketStream *ssOb){
        return ssOb->printMSGcontents();
    }

    int EXPORT ss_printMSGcontentsTypes(socketStream *ssOb){
        return ssOb->printMSGcontentsTypes();
    }

    int EXPORT ss_printMSGString(socketStream *ssOb){
        return ssOb->printMSGString();
    }

    int EXPORT ss_set_clientName(socketStream *ssOb, char* cID){
        std::string clientID (cID);
        return ssOb->set_clientName(clientID);
    }

    void EXPORT ss_closeCommunication(socketStream* ssObj){
        ssObj->closeCommunication();
        delete ssObj;
    }
}
