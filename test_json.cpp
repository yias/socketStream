
#include <iostream>
#include <vector>
#include <string>

#include "rapidjson/writer.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"



int main(){

    const char* json_object ="{\"name\":\"Ted\",\"age\":15}";

    rapidjson::Document dDoc;
    dDoc.Parse(json_object);

    std::vector < std::string > t_msg{"test1", "test2"};
    
    for (const std::string& word : t_msg){
        std::cout << word << " ";
    }
    std::cout<<std::endl;

    // stringify the jason object
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    dDoc.Accept(writer);

    std::cout << buffer.GetString() << std::endl;

    return 0;
}
