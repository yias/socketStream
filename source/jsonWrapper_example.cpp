/**
 * 
 *  Example for using the jsonWrapper class
 *  Developer:  Iason Batzianoulis
 *  email:      iasonbatz@gmail.com
 *  Lisence:     
 *  
*/


#include "jsonWrapper.h"



int main(int argc, char **argv){


    // define initial json string to introduce in the json document
    const char json[] = " { \"aString\" : \"world\", \"anArray\":[1, 2, 3, 4] } ";
    
    // parse the string into a string object
    std::string jsonDocument(json);

    // print the json string before parsing it into the json document
    std::cout << "the json object is: " << jsonDocument << std::endl;

    // parse the json string in a json document
    jsonWrapper testObj(jsonDocument);

    // print the json document as string for checking is it has parsed correctly
    std::cout << "original json document: " << testObj.getAsString() << std::endl;
    
    // print only the value of a specific field of the json document (introduced for debugging purposes)
    testObj.printValue(std::string("aString"));

    // define another string for replacing the initial document
    const char json2[] = " { \"aString\" : \"ZZZ\", \"anArray\":[0.1, 2.6, 3, 124.56] } ";

    // update the json document with the new string (the old string will no longer exist as it will be replaced from the new string)
    testObj.updateDoc(std::string(json2));

    // print the new json document as string
    std::cout << "updated json document: " << testObj.getAsString() << std::endl;

    // update a specific field of the document (the old value of the field will be replaced by the new)
    testObj.parseValue(std::string("aString"), std::string("newVal"));

    // print the updated json document as a string
    std::cout << "updated json document, after updating a field: " << testObj.getAsString() << std::endl;
    
    // get the value of a specific field, which contains a string, and print it
    std::string trStr = testObj.getField<rapdJson_types::String>(std::string("aString"));

    std::cout << "The value of " << std::string("aString") << " is: " << trStr << std::endl;

    // get the value of a specific field, which contains a vector of doubles, and print it
    std::vector<double> tr = testObj.getField<rapdJson_types::VecD>(std::string("anArray"));
    std::cout << "The value of " << std::string("anArray") << " is: " << std::endl;
    for (int i= 0; i< (int)tr.size(); i++){
        std::cout << tr[i] << ((i== ((int)tr.size()-1)) ? "\n": ", " );
    }
    
    testObj.printValue(std::string("aString"));
    
    return 0;


}