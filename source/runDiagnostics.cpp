

#include "socketStream.h"
#include <chrono>
#include <fstream>
#include <random>

// function to read a matrix from a text file
std::vector< std::vector<double> > txt2matrix(std::string filename);

// function to create a matrix of random doubles of shape nxm
std::vector< std::vector<double> > random2Dmatrix(unsigned int rows, unsigned int column);


int main(int argc, char **argv){

    // define the variable that holds the server IP. In this case, the server would be a local server.
    char *srvIP = "localhost";

    // if no new server IP is defined from the user, continue with the pre-defined server IP (localhost)
    if(argc!=2){
        std::cout << "No server IP suplied. Continue with localhost" << std::endl;
    }else{
        srvIP=argv[1];
    }

    // create an sockectStream object with the selected server IP address 
    socketStream socketHdlr(srvIP);

    // decalre a variable as a vector of strings for containing the fields of the message
    std::vector <std::string> test_rjson;

    // define the name of the fields
    test_rjson.push_back("name");
    test_rjson.push_back("data");

    // define an object to hold the current time
    auto start = std::chrono::steady_clock::now();

    // initialize the message by setting the fields' names
    if(socketHdlr.initialize_msgStruct(test_rjson)<0){
        std::cerr << "Unable to inizialize message structure" << std::endl;
        return -1;
    }

    // define an object to hold the currect time and print the time taken for initialize the message (in ms)
    auto end = std::chrono::steady_clock::now();
    std::cout << "Message initialization: " << (double)(std::chrono::duration_cast<std::chrono::microseconds>(end-start).count())/1000.0 << " ms" << std::endl;

    // define a which field will be updated and with which value
    std::string sfield("name");
    char *svalue = {"random_data"};

    // measure the time it takes to update the field and print this time
    start = std::chrono::steady_clock::now();
    socketHdlr.updateMSG(sfield, svalue);
    end = std::chrono::steady_clock::now();
    std::cout << "Message update (filed names): " << (double)std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()/1000.0 << " ms" << std::endl;

    // declare a variable that holds the names of the files that contain the data
    std::vector<std::string> fileNames;

    // define the names of the files
    fileNames.push_back("data\\data_example1.txt");
    fileNames.push_back("data\\data_example2.txt");
    fileNames.push_back("data\\data_example3.txt");
    fileNames.push_back("data\\data_example4.txt");


    //////////////     1st attempt to measure the time it takes to update a message with a 2D matrix of doubles and send it     //////////////
    //////////////     number of rows: 100                                                                                      //////////////
    //////////////     number of collumns: 16                                                                                   //////////////

    // define the number of rows
    int samples_window = 100;

    // define the number of collumns
    int nb_channels = 16;

    // create a 2D matrix with the data from a file
    std::vector< std::vector<double> > dataMatrix = txt2matrix(fileNames[0]);

    std::cout << "1) updating the message with a 2D matrix of shape (" << samples_window << "x" << nb_channels << "):" << std::endl;
    
    // declare a variable to hold the computational times of each iteration
    std::vector<double> timings;
    double sumTime = 0;
    
    // define a counter for the number of interations
    int counter = 0;

    // initialize the socket for streaming the data
    if(socketHdlr.initialize_sockeStream()<0){
        std::cerr << "Unable to initialize socket" << std::endl;
        return -1;
    }

    // initialize the connection with the server
    if(socketHdlr.make_connection()<0){
        std::cerr << "Unable to connect to " << srvIP << std::endl;
        return -1;
    }

    // define the field that will be updated
    sfield="data";

    // declare a random matrix to contain the new values
    std::vector< std::vector<double> > tmp_2Dmatrix;

    for (int i=0; i<30; i++){

        // update the random matrix
        tmp_2Dmatrix = random2Dmatrix(samples_window, nb_channels);

        // set the starting time of the computation
        start = std::chrono::steady_clock::now();

        // update the message
        socketHdlr.updateMSG(sfield, tmp_2Dmatrix);

        // send the message
        if(socketHdlr.sendMSg()<0){
            std::cerr << "unable to send message " << std::endl;
            return -1;
        }

        // set the ending time of the computation
        end = std::chrono::steady_clock::now();

        // throw the computational time in the variable "timings" and print it in the terminal
        timings.push_back((double)std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()/1000.0);
        std::cout << "trial " << i+1 << ": "<< timings[i] << " ms" << std::endl;
    }

    // find the average computational time and print it in the terminal
    for(auto& n: timings){
        sumTime +=n;
    }
    std::cout << "average time: " << sumTime/(double)timings.size() << std::endl;

    // delete the contains of the variable (empty the vector)
    timings.clear();

    
    
    // repeat the proceedure 3 more time for different shapes of matrix:

    //////////////     2nd attempt to measure the time it takes to update a message with a 2D matrix of doubles and send it     //////////////
    //////////////     number of rows: 100                                                                                      //////////////
    //////////////     number of collumns: 23                                                                                   //////////////

    dataMatrix = txt2matrix(fileNames[1]);
    std::cout << "2) updating the message with a 2D matrix of shape (" << dataMatrix.size() << "x" << dataMatrix[0].size() << "):" << std::endl;
    
    
    sumTime = 0;
    counter = 0;
    samples_window = 100;

    for(int i=0; i < (int)dataMatrix.size(); i+=samples_window){
        std::vector<std::vector<double>> tmp_mat;
        for(int j=i; j<i+samples_window; j++){
            tmp_mat.push_back(std::vector<double>(dataMatrix[j].begin(), dataMatrix[j].end()));
        }
        // std::cout << "the shape of the matrix: (" << tmp_mat.size() << "," << tmp_mat[0].size() << ")" << std::endl;
        
        start = std::chrono::steady_clock::now();
        socketHdlr.updateMSG(sfield, tmp_mat);
        if(socketHdlr.sendMSg()<0){
            std::cerr << "unable to send message " << std::endl;
            return -1;
        }
        end = std::chrono::steady_clock::now();
        timings.push_back((double)std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()/1000.0);
        std::cout << "trial " << counter+1 << ": "<< timings[counter] << " ms" << std::endl;
        counter++;
    }

    for(auto& n: timings){
        sumTime +=n;
    }
    std::cout << "average time: " << sumTime/(double)timings.size() << std::endl;

    timings.clear();


    //////////////     3rd attempt to measure the time it takes to update a message with a 2D matrix of doubles and send it     //////////////
    //////////////     number of rows: 16                                                                                       //////////////
    //////////////     number of collumns: 100                                                                                  //////////////

    dataMatrix = txt2matrix(fileNames[2]);
    std::cout << "3) updating the message with a 2D matrix of shape (" << dataMatrix.size() << "x" << dataMatrix[0].size() << "):" << std::endl;
    
    sumTime = 0;
    counter = 0;
    samples_window = 100;

    for(int i=0; i < (int)dataMatrix[0].size(); i+=samples_window){
        std::vector<std::vector<double>> tmp_mat;
        for(int j=0; j < (int)dataMatrix.size(); j++){
            tmp_mat.push_back(std::vector<double>(dataMatrix[j].begin()+i, dataMatrix[j].begin()+i+samples_window));
        }
        // std::cout << "the shape of the matrix: (" << tmp_mat.size() << "," << tmp_mat[0].size() << ")" << std::endl;
        
        start = std::chrono::steady_clock::now();
        socketHdlr.updateMSG(sfield, tmp_mat);
        if(socketHdlr.sendMSg()<0){
            std::cerr << "unable to send message " << std::endl;
            return -1;
        }
        end = std::chrono::steady_clock::now();
        timings.push_back((double)std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()/1000.0);
        std::cout << "trial " << counter+1 << ": "<< timings[counter] << " ms" << std::endl;
        counter++;
    }

    for(auto& n: timings){
        sumTime +=n;
    }
    std::cout << "average time: " << sumTime/(double)timings.size() << std::endl;

    timings.clear();

    //////////////     4th attempt to measure the time it takes to update a message with a 2D matrix of doubles and send it     //////////////
    //////////////     number of rows: 23                                                                                       //////////////
    //////////////     number of collumns: 100                                                                                  //////////////


    dataMatrix = txt2matrix(fileNames[3]);
    std::cout << "4) updating the message with a 2D matrix of shape (" << dataMatrix.size() << "x" << dataMatrix[0].size() << "):" << std::endl;
    
    sumTime = 0;
    counter = 0;
    samples_window = 100;

    for(int i=0; i < (int)dataMatrix[0].size(); i+=samples_window){
        std::vector<std::vector<double>> tmp_mat;
        for(int j=0; j < (int)dataMatrix.size(); j++){
            tmp_mat.push_back(std::vector<double>(dataMatrix[j].begin()+i, dataMatrix[j].begin()+i+samples_window));
        }
        // std::cout << "the shape of the matrix: (" << tmp_mat.size() << "," << tmp_mat[0].size() << ")" << std::endl;
        
        start = std::chrono::steady_clock::now();
        socketHdlr.updateMSG(sfield, tmp_mat);
        if(socketHdlr.sendMSg()<0){
            std::cerr << "unable to send message " << std::endl;
            return -1;
        }
        end = std::chrono::steady_clock::now();
        timings.push_back((double)std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()/1000.0);
        std::cout << "trial " << counter+1 << ": "<< timings[counter] << " ms" << std::endl;
        counter++;
    }

    for(auto& n: timings){
        sumTime +=n;
    }
    std::cout << "average time: " << sumTime/(double)timings.size() << std::endl;

    timings.clear();

    // close communication with the server
    socketHdlr.closeCommunication();

    return 0;
}

std::vector< std::vector<double> > txt2matrix(std::string filename){

    std::vector< std::vector<double> > dmatrix;

    std::ifstream infile(filename, std::ifstream::in);

    if(infile.is_open()){
        std::string line;
        while(std::getline(infile, line)){

            std::istringstream ss(line);
            std::string tmp_nb;
            std::vector <double> record;
            while(std::getline(ss,tmp_nb,',')){
                record.push_back(std::atof(tmp_nb.c_str()));
            }
            dmatrix.push_back(record);
        }
        infile.close();
    }else{
        std::cerr << "Unable to open the file" << filename << std::endl;
    }

    // std::cout << "the shape of the matrix: (" << dmatrix.size() << "," << dmatrix[0].size() << ")" << std::endl;

    return dmatrix;

}

std::vector< std::vector<double> > random2Dmatrix(unsigned int rows, unsigned int columns){

    std::vector< std::vector<double> > dmatrix(rows, std::vector<double> (columns));

    double lower_bound = 0;
    double upper_bound = 10000;
    std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
    std::default_random_engine re;

    for (int i = 0; i < (int)dmatrix.size(); i++){
        for (int j = 0; j < (int)dmatrix[i].size(); j++){
            dmatrix[i][j] = unif(re);
        }
    }
    
    std::cout << "the shape of the matrix: (" << dmatrix.size() << "," << dmatrix[0].size() << ")" << std::endl;
    
    return dmatrix;

}