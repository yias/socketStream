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

char *returnedMsgStr;

extern "C"{
    EXPORT socketStream* create_socketStream(const char* svrIPAddress, int srvPosrt, int socketStreamMode){
        
        /* Initial memory allocation */
        returnedMsgStr = (char *) malloc(15);

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

    int EXPORT ss_updateMSG_char(socketStream *ssOb, char* field, const char *value){
        std::string inField(field);
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

    int EXPORT ss_updateMSG_matInt(socketStream *ssOb, char* field, int *value, int x_length, int y_length){
        std::string inField(field);
        std::vector< std::vector<int> > inValue(x_length, std::vector<int>(y_length));
        for (int i=0; i<x_length; i++){
            inValue[i]=std::vector<int>(value + i*y_length, value + (i+1)*y_length);
        }
        return ssOb->updateMSG(inField, inValue);
    }

    int EXPORT ss_updateMSG_matDouble(socketStream *ssOb, char* field, double *value, int x_length, int y_length){
        std::string inField(field);
        std::vector< std::vector<double> > inValue(x_length, std::vector<double>(y_length));
        for (int i=0; i<x_length; i++){
            inValue[i]=std::vector<double>(value + i*y_length, value + (i+1)*y_length);
        }
        return ssOb->updateMSG(inField, inValue);
    }

    int EXPORT ss_sendMsg(socketStream *ssOb){
        return ssOb->sendMSg();
    }

    int EXPORT ss_sendMSg2Client_int(socketStream *ssOb, int clID){
        return ssOb->sendMSg2Client(clID);
    }

    int EXPORT ss_sendMSg2Client_str(socketStream *ssOb, const char *clID){
        std::string clID_str(clID);
        return ssOb->sendMSg2Client(clID_str);
    }

    int EXPORT ss_sendMSg2All(socketStream *ssOb){
        return ssOb->sendMSg2All();
    }

    void EXPORT ss_closeCommunication(socketStream* ssObj){
        ssObj->closeCommunication();
        delete ssObj;
        free(returnedMsgStr);
    }

    int EXPORT ss_setHashKey(socketStream* ssObj, bool hKey){
        return ssObj->setHashKey(hKey);
    }

    int EXPORT ss_setVerbose(socketStream* ssObj, bool _verbose){
        return ssObj->setVerbose(_verbose);
    }

    int EXPORT ss_setHeaderSize(socketStream* ssObj, int hSize){
        if (hSize<0){
            std::cout << "[socketStream] The size of the header should be greater than zero" << std::endl;
            return -1;
        }else{
            return ssObj->setHeaderSize((unsigned int)hSize);
        }
    }

    EXPORT char* ss_getFullmsg(socketStream* ssObj){
        std::string tt = ssObj->getFullmsg();
        returnedMsgStr = (char *) realloc(returnedMsgStr, sizeof(char) * tt.length() ); 
        if( returnedMsgStr == NULL) exit(1);
        strcpy(returnedMsgStr , tt.c_str());
        return returnedMsgStr;
    }

    int EXPORT ss_runServer(socketStream* ssObj){
        return ssObj->runServer();
    }

    EXPORT const char* ss_get_latest(socketStream* ssObj, bool *newMSG){
        
        std::string tt = ssObj->get_latest(newMSG);
        if (tt.length()==0){
            tt = " ";
        }
        
        returnedMsgStr = (char *) realloc(returnedMsgStr, sizeof(char) * tt.length() ); // + (size_t)1 
        
        strcpy(returnedMsgStr , tt.c_str());
        return returnedMsgStr;
    }

    EXPORT const char* ss_get_latest_fromClient(socketStream* ssObj, const char* cltName, bool *newMSG){
        std::string cName(cltName);
        std::string tt = ssObj->get_latest(cName, newMSG);
        if (tt.length()==0){
            tt = " ";
        }
        returnedMsgStr = (char *) realloc(returnedMsgStr, sizeof(char) * tt.length() ); // + (size_t)1 
        // if (returnedMsgStr == NULL) exit(1);
        strcpy(returnedMsgStr , tt.c_str());
        return returnedMsgStr;
    }

    bool EXPORT ss_socketStream_ok(socketStream* ssObj){
        return ssObj->socketStream_ok();
    }

}
