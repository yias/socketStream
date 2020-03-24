/**
 * 
 * 
 *  jsonWrapper C++ class
 *  
 *  [description]
 *  
 *  Developer:  Iason Batzianoulis
 *  email:      iasonbatz@gmail.com
 *  Lisence:     
 * 
**/

#ifndef JSONWRAPPER_H_
#define JSONWRAPPER_H_

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>



namespace rapdJson_types{
    typedef int                                 Int;
    typedef float                               Float;
    typedef double                              Double;           
    typedef std::vector<double>                 VecD;
    typedef std::vector< std::vector<double> >  Vec2DD;
    typedef std::vector<float>                  VecFt;
    typedef std::vector< std::vector<float> >   Vec2DF;
    typedef std::vector<int>                    VecI;
    typedef std::vector< std::vector<int> >     Vec2DI;
    typedef std::string                         String;
    typedef std::vector< std::string >          VecString;

    enum rapdJson_types{
    RAPIDJSON_NULL,
    RAPIDJSON_FALSE,
    RAPIDJSON_TRUE,
    RAPIDJSON_OBJECT,
    RAPIDJSON_ARRAY,
    RAPIDJSON_STRING,
    RAPIDJSON_NUMBER

    };
}



class jsonWrapper{

    rapidjson::Document jsonDoc;

    rapidjson::StringBuffer t_buffer;
    rapidjson::Writer<rapidjson::StringBuffer> t_writer;

    bool isobjectok;

    template <class T> 
    struct getValueHelper;


    public:

        jsonWrapper();
        ~jsonWrapper();
        jsonWrapper(std::string jsonString);

        int updateDoc(std::string jsonString);
        int parseValue(std::string field, std::string value);
        int parseValue(std::string field, std::vector<double> value);
        void printValue(std::string fldName){std::cout<< jsonDoc[fldName.c_str()].GetString() << std::endl; }
        std::string getAsString();

        template<class T>
        T getField(std::string fieldName);
        auto& getField(std::string fieldName);


};

template<>
struct jsonWrapper::getValueHelper<rapdJson_types::String>{
    static std::string getVal(void *usrPtr, std::string fldName){
        jsonWrapper *tmp = static_cast<jsonWrapper *>(usrPtr);
        return tmp->jsonDoc[fldName.c_str()].GetString();
    }
};


template<>
struct jsonWrapper::getValueHelper<rapdJson_types::VecD>{
    static rapdJson_types::VecD getVal(void *usrPtr, std::string fldName){
        jsonWrapper *tmp = static_cast<jsonWrapper *>(usrPtr);
        rapdJson_types::VecD trm;
        const rapidjson::Value& a = tmp->jsonDoc[fldName.c_str()];
        std::transform(a.Begin(),a.End(),std::back_inserter(trm),[](const rapidjson::Value &tt){return tt.GetDouble();});
        return trm;
    }
};


template<class T>
T jsonWrapper::getField(std::string fieldName){
    // if(jsonDoc.HasMember(fieldName.c_str())){
    //     std::cout << "Yes it has \n";
    // }
    
    // T rtr = jsonWrapper::getValueHelper<T>::getVal(this, fieldName); 
    return jsonWrapper::getValueHelper<T>::getVal(this, fieldName);
}


// ------------------------------------------------------------------------------------------------ //
// ------------------------------------------------------------------------------------------------ //
// ------------------------------------------------------------------------------------------------ //


// inline auto& jsonWrapper::getField(std::string fieldName){
//     // T returnObj;
//     // auto returnObj;

//     std::vector<double> trm;

//     if(isobjectok){
//         if(jsonDoc.HasMember(fieldName.c_str())){
//             int memberType = jsonDoc[fieldName.c_str()].GetType();
//             std::cout << memberType << std::endl;
//             // if(memberType == RAPIDJSON_STRING){
//             //     return jsonDoc[fieldName.c_str()].GetString();
//             // }
//             if(memberType == RAPIDJSON_ARRAY){
                
//                 const rapidjson::Value& a = jsonDoc[fieldName.c_str()];
//                 jsonDoc[fieldName.c_str()].GetArray().Begin();
                
//                 // auto tre=std::vector <decltype(a[0])> (a.Size());
//                 // auto ert = a[0];
//                 std::transform(a.Begin(),a.End(),std::back_inserter(trm),[](const rapidjson::Value &tt){return tt.GetDouble();});
                
                
//                 for (int i = 0; i < (int)trm.size(); i++){
//                     std::cout << trm[i]<< ", ";
//                     std::cout << typeid(trm[i]).name() << std::endl;
//                 }
                
//             }
//         //}else{
//         //     std::cerr << "[jsonWrapper] Not valid field name" << std::endl;
//         //     return -1;
//         }
//     // }else{
//     //     std::cerr << "[jsonWrapper] The json document is NOT initialized" << std::endl;
//     //     return -2;
//     }

//     // std::cout << "T   (array):           " <<  std::is_array<decltype(T)>::value << '\n';
//     // std::cout << "T   (arithmetic):      " <<  std::is_arithmetic<T>::value << '\n';

//     // std::cout << "vA   (arithmetic):           " <<  std::is_arithmetic<T>::value << '\n';
//     std::cout <<"testnnnnnnnnnnnnnn\n";
//     return trm;
// }



#endif