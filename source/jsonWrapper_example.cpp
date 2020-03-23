

#include "jsonWrapper.h"
#include <type_traits>


int main(int argc, char **argv){

    const char json[] = " { \"aString\" : \"world\", \"anArray\":[1, 2, 3, 4] } ";
    
    std::string jsonDocument(json);

    std::cout << "the json object is: " << jsonDocument << std::endl;

    jsonWrapper testObj(jsonDocument);

    std::cout << "original: " << testObj.getAsString() << std::endl;
    testObj.printValue(std::string("aString"));

    const char json2[] = " { \"aString\" : \"ZZZ\", \"anArray\":[1, 2, 3, 4] } ";

    testObj.updateDoc(std::string(json2));

    std::cout << "after updateDoc: " << testObj.getAsString() << std::endl;

    testObj.printValue(std::string("aString"));

    // testObj.parseValue(std::string("aString"), std::string("newVal"));
    testObj.printValue(std::string("aString"));

    std::cout << "after parseValue: " << testObj.getAsString() << std::endl;
    testObj.printValue(std::string("aString"));

    // std::string tr = testObj.getField<std::string>(std::string("aString"));

    std::string trStr = testObj.getField<std::string>(std::string("aString"));
    std::cout<<trStr<<std::endl;
    std::vector<double> tr = testObj.getField<VecOfD >(std::string("anArray"));
    for (int i= 0; i< (int)tr.size(); i++){
        std::cout << tr[i] << ", ";
    }
    std::cout << std::endl;
    // std::cout << tr << std::endl;
    testObj.printValue(std::string("aString"));
    
    std::cout << "tst:\n";

    int x=12;
    int y=4;


    std::cout << "x % y: " << x % y << std::endl;

    std::cout << "x % (x-1): " << (((x & (x-1)) == 0)? "test1" : "test2") << std::endl;

    std::cout << "y % (y-1): " << ((y & (y-1)) == 0) << std::endl;

    // std::vector<double> tr = testObj.getField<std::vector<double>>(std::string("anArray"));

    // std::vector<double> vA;
    // std::vector< std::vector<double> > vvA;
    // std::vector<std::string> vStr;
    // int a=1;
    // double d=1;
    // std::string tStr;

    // std::cout << "vA   (arithmetic):           " <<  std::is_arithmetic<decltype(vA)>::value << '\n';
    // // std::cout << "vvA  (arithmetic):           " <<  std::is_arithmetic<vvA>::value << '\n';
    // // std::cout << "vStr (arithmetic):           " <<  std::is_arithmetic<vStr>::value << '\n';
    // std::cout << "a    (arithmetic):           " <<  std::is_arithmetic<decltype(a)>::value << '\n';
    // std::cout << "d    (arithmetic):           " <<  std::is_arithmetic<decltype(d)>::value << '\n';
    // // std::cout << "tStr (arithmetic):           " <<  std::is_arithmetic<tStr>::value << '\n';

    // std::cout << "vA   (array):           " <<  std::is_array<decltype(vA)>::value << '\n';
    // // std::cout << "vvA  (array):           " <<  std::is_array<vvA>::value << '\n';
    // std::cout << "vStr (array):           " <<  std::is_array<vStr>::value << '\n';
    // std::cout << "a    (array):           " <<  std::is_array<a>::value << '\n';
    // std::cout << "d    (array):           " <<  std::is_array<d>::value << '\n';
    // std::cout << "tStr (array):           " <<  std::is_array<tStr>::value << '\n';

    // std::cout << "vA   (integral):           " <<  std::is_integral<vA>::value << '\n';
    // std::cout << "vvA  (integral):           " <<  std::is_integral<vvA>::value << '\n';
    // std::cout << "vStr (integral):           " <<  std::is_integral<vStr>::value << '\n';
    // std::cout << "a    (integral):           " <<  std::is_integral<a>::value << '\n';
    // std::cout << "d    (integral):           " <<  std::is_integral<d>::value << '\n';
    // std::cout << "tStr (integral):           " <<  std::is_integral<tStr>::value << '\n';
    // std::cout << typeid(vA).name() << std::endl;
    // std::cout << typeid(vvA).name() << std::endl;
    // std::cout << typeid(testObj).name() << std::endl;
    // std::cout << typeid(a).name() << std::endl;
    // std::cout << typeid(d).name() << std::endl;
    // std::cout << "vA   (class):           " <<  std::is_class<decltype(vA)>::value << '\n';
    // // std::cout << "vvA  (class):           " <<  std::is_class<vvA>::value << '\n';
    // // std::cout << "vStr (class):           " <<  std::is_class<vStr>::value << '\n';
    // std::cout << "a    (class):           " <<  std::is_class<decltype(a)>::value << '\n';
    // std::cout << "d    (class):           " <<  std::is_class<d>::value << '\n';
    // std::cout << "tStr (class):           " <<  std::is_class<tStr>::value << '\n';
    // std::cout << "testObj (class):        " <<  std::is_class<testObj>::value << '\n'; 

    // std::cout << "vA   (pointer):           " <<  std::is_pointer<vA>::value << '\n';
    // std::cout << "vvA  (pointer):           " <<  std::is_pointer<vvA>::value << '\n';
    // std::cout << "vStr (pointer):           " <<  std::is_pointer<vStr>::value << '\n';
    // std::cout << "a    (pointer):           " <<  std::is_pointer<a>::value << '\n';
    // std::cout << "d    (pointer):           " <<  std::is_pointer<d>::value << '\n';
    // std::cout << "tStr (pointer):           " <<  std::is_pointer<tStr>::value << '\n';
    // std::cout << "testObj (pointer):        " <<  std::is_pointer<testObj>::value << '\n';

    // std::cout << "vA   (null_pointer):           " <<  std::is_null_pointer<vA>::value << '\n';
    // std::cout << "vvA  (null_pointer):           " <<  std::is_null_pointer<vvA>::value << '\n';
    // std::cout << "vStr (null_pointer):           " <<  std::is_null_pointer<vStr>::value << '\n';
    // std::cout << "a    (null_pointer):           " <<  std::is_null_pointer<a>::value << '\n';
    // std::cout << "d    (null_pointer):           " <<  std::is_null_pointer<d>::value << '\n';
    // std::cout << "tStr (null_pointer):           " <<  std::is_null_pointer<tStr>::value << '\n';
    // std::cout << "testObj (null_pointer):        " <<  std::is_null_pointer<testObj>::value << '\n';


    // testObj = jsonWrapper(jsonDocument);






}