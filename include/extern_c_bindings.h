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
            // std::cout << inFields[i] << std::endl;
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
        // std::cout << clientID << std::endl;
        return ssOb->set_clientName(clientID);
    }

    int EXPORT ss_updateMSG_char(socketStream *ssOb, char* field, char *value){
        std::string inField(field);
        std::string inValue(value);
        std::cout << inValue << std::endl;
        return ssOb->updateMSG(inField, value);
    }

    int EXPORT ss_updateMSG_int(socketStream *ssOb, char* field, int value){
        std::string inField(field);
        return ssOb->updateMSG(inField, value);
    }

    int EXPORT ss_updateMSG_double(socketStream *ssOb, char* field, double value){
        std::string inField(field);
        return ssOb->updateMSG(inField, value);
    }

    int EXPORT ss_updateMSG_intArray(socketStream *ssOb, char* field, int *value, int arraylength){
        std::string inField(field);
        return ssOb->updateMSG(inField, value, arraylength);
    }

    int EXPORT ss_updateMSG_doubleArray(socketStream *ssOb, char* field, double *value, int arraylength){
        std::string inField(field);
        return ssOb->updateMSG(inField, value, arraylength);
    }

    int EXPORT ss_updateMSG_matInt(socketStream *ssOb, char* field, int **value, int x_length, int *y_length){
        std::string inField(field);
        std::vector< std::vector<int> > inValue(x_length, std::vector<int>());
        for (int i=0; i<x_length; i++){
            inValue[i]=std::vector<int>(*value[i], y_length[i]);
        }
        return ssOb->updateMSG(inField, inValue);
    }

    int EXPORT ss_updateMSG_matDouble(socketStream *ssOb, char* field, double **value, int x_length, int *y_length){
        std::string inField(field);
        std::vector< std::vector<double> > inValue(x_length, std::vector<double>());
        for (int i=0; i<x_length; i++){
            inValue[i]=std::vector<double>(*value[i], y_length[i]);
        }
        return ssOb->updateMSG(inField, inValue);
    }

    void EXPORT ss_closeCommunication(socketStream* ssObj){
        ssObj->closeCommunication();
        delete ssObj;
    }
}
