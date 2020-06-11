/**
 *  Copyright (C) 2020 Iason Batzianoulis
 * 
 *  jsonWrapper source file
 *    
 *  Developer:  Iason Batzianoulis
 *  email:      iasonbatz@gmail.com
 *  License:    GNU GPLv3
 * 
**/

#include "jsonWrapper.hpp"


jsonWrapper::jsonWrapper(){

    std::cout << "[jsonWrapper] Hello from jsonWrapper" << std::endl;

    isobjectok = false;
}

jsonWrapper::jsonWrapper(std::string jsonString){

    if (jsonDoc.Parse(jsonString.c_str()).HasParseError()){
        std::cerr << "[jsonWrapper] Parsing rapidjson error. Unable to parse json string" << std::endl; 
        std::cout << jsonString << std::endl;
    }

    if(jsonDoc.IsObject()){
        isobjectok = true;
    }else{
        isobjectok = false;
        std::cout << "is not an objsect\n";
    }

        
}

int jsonWrapper::updateDoc(std::string jsonString){

    if (jsonDoc.Parse(jsonString.c_str()).HasParseError()){
        std::cerr << "[jsonWrapper] Parsing rapidjson error. Unable to parse json string" << std::endl; 
    }

    if(jsonDoc.IsObject()){
        isobjectok = true;
        return 0;
    }else{
        isobjectok = false;
        return -1;
    }
    
    
}

int jsonWrapper::parseValue(std::string field, std::string value){
    if(isobjectok){
        
        if(jsonDoc.HasMember(field.c_str())){
            const char *t_value = value.c_str();
            rapidjson::Value a;
            a.SetString(t_value, std::strlen(t_value), this->jsonDoc.GetAllocator());
            jsonDoc[field.c_str()] = a;
        }else{
            std::cerr << "[jsonWrapper] Not valid field name" << std::endl;
            return -1;
        }
    }else{
        std::cerr << "[jsonWrapper] The json document is NOT initialized" << std::endl;
        return -2;
    }


    return 0;

}

int jsonWrapper::parseValue(std::string field, std::vector<double> value){
    
    if(isobjectok){
        if(jsonDoc.HasMember(field.c_str())){
            rapidjson::Value t_a(rapidjson::kArrayType);
            rapidjson::Document::AllocatorType& allocator = jsonDoc.GetAllocator();
            for(int j= 0; j<(int)value.size(); j++){
                t_a.PushBack(value[j],allocator);
            }
            if(!jsonDoc[field.c_str()].IsArray()){
                jsonDoc[field.c_str()].SetArray();
            }
            jsonDoc[field.c_str()]=t_a;
        }else{
            std::cerr << "[jsonWrapper] Not valid field name" << std::endl;
            return -1;
        }
    }else{
        std::cerr << "[jsonWrapper] The json document is NOT initialized" << std::endl;
        return -2;
    }

    return 0;

}

std::string jsonWrapper::getAsString(){
    t_buffer.Clear();
    t_writer.Reset(t_buffer);
    jsonDoc.Accept(t_writer);

    return t_buffer.GetString();
}

int jsonWrapper::fromFile(std::string fName){

    /**
     * Function to parse json objects from file
     * 
     * Inputs:
     *      fname:  the name of the file
     *      d:      a rapidjson document pointer to contain the json object read from the file
     * 
     * Outputs:
     *      0:      Success
     *     -1:      Unable to open file
     *     -2:      The json string is not properly parsed from rapidJson
     */


    FILE* fpr = fopen(fName.c_str(), "r");

    if (fpr == NULL){
        return -1;
    }
    

    char rBuffer[2048];

    rapidjson::FileReadStream is(fpr, rBuffer, sizeof(rBuffer));

    std::string jsonString;
    
    jsonDoc.ParseStream(is);

    if(jsonDoc.IsObject()){
        isobjectok = true;
    }else{
        isobjectok = false;
        return -2;
    }
    
    return 0;

}


int jsonWrapper::saveToFile(std::string fName){
    /**
     * Function to save json objects to a file
     * 
     * Inputs:
     *      fname:  the name of the file to save the json object
     *
     * 
     * Outputs:
     *      0:      Success
     *     -1:      Unable to open file
     * 
     */

    FILE* fp = fopen(fName.c_str(), "w");

    if (fp == NULL){
        return -1;
    }

    char buffer[2048];
    rapidjson::FileWriteStream os(fp, buffer, sizeof(buffer));

    rapidjson::Writer< rapidjson::FileWriteStream > writer(os);

    jsonDoc.Accept(writer);

    fclose(fp);

    return 0;
}


bool jsonWrapper::hasField(std::string fldName){
    
    if(isobjectok){
        if(jsonDoc.HasMember(fldName.c_str())){
            return true;
        }else{
            return false;
        }
    }else{
        std::cout << "[jsonWrapper] The json Object is not initialized" << std::endl;
        return false;
    }

}


jsonWrapper::~jsonWrapper(){
    isobjectok = false;
    t_buffer.Clear();
}
