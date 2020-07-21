/**
 * 
 *  source file for running diagnostics for the client performance
 * 
 *  Developer:  Iason Batzianoulis
 *  email:      iasonbatz@gmail.com
 *       
 *  
*/

#include "socketStream.h"
#include <chrono>
#include <ctime>
#include <fstream>
#include <random>
#include <codecvt>

#ifdef __linux__ || __APPLE__
#include <sys/stat.h>
#include <sys/types.h>
#endif

// function to read a matrix from a text file
std::vector< std::vector<double> > txt2matrix(std::string filename);

// function to create a matrix of random doubles of shape nxm
std::vector< std::vector<double> > random2Dmatrix(unsigned int rows, unsigned int column);

// function to run the diagnostics
int cTimings(int samples_window, int nb_channels, std::string sfield, socketStream& socketHdlr, std::ofstream& wfile); 

#ifdef __linux__ || __APPLE__
    int dirExists(std::string tpath);
#endif


int main(int argc, char **argv){

    #ifdef _WIN32
        if(CreateDirectory("logfiles",NULL) || ERROR_ALREADY_EXISTS == GetLastError()){
            std::cout << "A folder logfiles is created" << std::endl;
        }
    #else
        std::string curent_path=get_current_dir_name();
        std::string dir_path = curent_path + "/logfiles";
        if (!dirExists(dir_path)){
            if(mkdir(dir_path.c_str(),0777)==0){
                std::cout << "A folder logfiles is created" << std::endl;
            }else{
                std::cout<<"Unable to create the folder:\n"<<dir_path<<"\n";
            }

            dir_path.clear();
    }

    #endif


    // local current time to put it in the name of the logfile
    std::time_t rawtime;
    struct tm *timeinfo;
    char time_buffer[80];

    std::time(&rawtime);
    timeinfo = localtime(&rawtime);

    std::strftime(time_buffer, sizeof(time_buffer), "%Y%m%d_%H_%M_%S", timeinfo);
    
    std::string covString(time_buffer);
    #ifdef _WIN32
        std::string logfileName = "logfiles\\diagnostics_logfile_" + covString +".txt";
    #else
        std::string logfileName = "logfiles/diagnostics_logfile_" + covString +".txt";
    #endif

    // declare an ofstream object for writing to file
    std::ofstream wfile;
    wfile.open(logfileName);
    if(!wfile.is_open()){
        std::cerr << "Unable to open file" <<std::endl;
        return -1;
    }


    // define the variable that holds the server IP. In this case, the server would be a local server.
    const char *srvIP = "localhost";

    // if no new server IP is defined from the user, continue with the pre-defined server IP (localhost)
    if(argc!=2){
        std::cout << "No server IP suplied. Continue with localhost" << std::endl;
    }else{
        srvIP=argv[1];
    }

    // create an sockectStream object with the selected server IP address 
    socketStream socketHdlr(srvIP);

    // set client's name
    socketHdlr.set_clientName("diagnostics");

    // set the size of the buffer
    // socketHdlr.setBufferSize(64);

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
    wfile << "Message initialization: " << (double)(std::chrono::duration_cast<std::chrono::microseconds>(end-start).count())/1000.0 << " ms\n";

    // define a which field will be updated and with which value
    std::string sfield("name");
    const char *svalue = {"random_data"};

    // measure the time it takes to update the field and print this time and write it to the logfile
    start = std::chrono::steady_clock::now();
    socketHdlr.updateMSG(sfield, svalue);
    end = std::chrono::steady_clock::now();
    std::cout << "Message update (field names): " << (double)std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()/1000.0 << " ms" << std::endl;
    wfile << "Message update (field names): " << (double)(std::chrono::duration_cast<std::chrono::microseconds>(end-start).count())/1000.0 << " ms\n\n";

    // initialize the socket for streaming the data
    if(socketHdlr.initialize_socketStream()<0){
        std::cerr << "Unable to initialize socket" << std::endl;
        return -1;
    }

    // initialize the connection with the server
    if(socketHdlr.make_connection()<0){
        std::cerr << "Unable to connect to " << srvIP << std::endl;
        return -1;
    }


    //////////////     1st attempt to measure the time it takes to update a message with a 2D matrix of doubles and send it     //////////////
    //////////////     number of rows: 100                                                                                      //////////////
    //////////////     number of collumns: 16                                                                                   //////////////

    // define the number of rows
    int samples_window = 100;

    // define the number of collumns
    int nb_channels = 16;

    // define the field that will be updated
    sfield="data";

    std::cout << "1) updating the message with a 2D matrix of shape (" << samples_window << "x" << nb_channels << "):" << std::endl;
    wfile << "1) updating the message with a 2D matrix of shape (" << samples_window << "x" << nb_channels << "):\n";

    // update and send the message 30 times
    cTimings(samples_window, nb_channels, sfield, socketHdlr, wfile);

    // repeat the proceedure for different shapes of matrix:

    //////////////     2nd attempt to measure the time it takes to update a message with a 2D matrix of doubles and send it     //////////////
    //////////////     number of rows: 100                                                                                      //////////////
    //////////////     number of collumns: 23                                                                                   //////////////

    // define the number of rows
    samples_window = 100;

    // define the number of collumns
    nb_channels = 23;

    std::cout << "2) updating the message with a 2D matrix of shape (" << samples_window << "x" << nb_channels << "):" << std::endl;
    wfile << "2) updating the message with a 2D matrix of shape (" << samples_window << "x" << nb_channels << "):" << std::endl;

    cTimings(samples_window, nb_channels, sfield, socketHdlr, wfile);


    //////////////     3rd attempt to measure the time it takes to update a message with a 2D matrix of doubles and send it     //////////////
    //////////////     number of rows: 16                                                                                       //////////////
    //////////////     number of collumns: 100                                                                                  //////////////

    // define the number of rows
    samples_window = 16;

    // define the number of collumns
    nb_channels = 100;

    std::cout << "3) updating the message with a 2D matrix of shape (" << samples_window << "x" << nb_channels << "):" << std::endl;
    wfile << "3) updating the message with a 2D matrix of shape (" << samples_window << "x" << nb_channels << "):\n";

    cTimings(samples_window, nb_channels, sfield, socketHdlr, wfile);


    //////////////     4th attempt to measure the time it takes to update a message with a 2D matrix of doubles and send it     //////////////
    //////////////     number of rows: 23                                                                                       //////////////
    //////////////     number of collumns: 100                                                                                  //////////////

    // define the number of rows
    samples_window = 23;

    // define the number of collumns
    nb_channels = 100;

    std::cout << "4) updating the message with a 2D matrix of shape (" << samples_window << "x" << nb_channels << "):" << std::endl;
    wfile << "4) updating the message with a 2D matrix of shape (" << samples_window << "x" << nb_channels << "):\n";

    cTimings(samples_window, nb_channels, sfield, socketHdlr, wfile);

    //////////////     5th attempt to measure the time it takes to update a message with a 2D matrix of doubles and send it     //////////////
    //////////////     number of rows: 4                                                                                        //////////////
    //////////////     number of collumns: 150                                                                                  //////////////

    // define the number of rows
    samples_window = 4;

    // define the number of collumns
    nb_channels = 150;

    std::cout << "5) updating the message with a 2D matrix of shape (" << samples_window << "x" << nb_channels << "):" << std::endl;
    wfile << "5) updating the message with a 2D matrix of shape (" << samples_window << "x" << nb_channels << "):\n";

    cTimings(samples_window, nb_channels, sfield, socketHdlr, wfile);

    //////////////     6th attempt to measure the time it takes to update a message with a 2D matrix of doubles and send it     //////////////
    //////////////     number of rows: 4                                                                                        //////////////
    //////////////     number of collumns: 150                                                                                  //////////////
    //////////////     without sending checksum                                                                                 //////////////

    // deactivate checksum
    socketHdlr.setHashKey(false);

    // define the number of rows
    samples_window = 4;

    // define the number of collumns
    nb_channels = 150;

    std::cout << "6) updating the message with a 2D matrix of shape (" << samples_window << "x" << nb_channels << ") and send without checksum:" << std::endl;
    wfile << "6) updating the message with a 2D matrix of shape (" << samples_window << "x" << nb_channels << ") and send without checksum:\n";

    cTimings(samples_window, nb_channels, sfield, socketHdlr, wfile);

    //////////////     7th attempt to measure the time it takes to update a message with a 2D matrix of doubles and send it     //////////////
    //////////////     number of rows: 4                                                                                        //////////////
    //////////////     number of collumns: 150                                                                                  //////////////
    //////////////     without sending checksum                                                                                 //////////////

    // define the number of rows
    samples_window = 16;

    // define the number of collumns
    nb_channels = 100;

    std::cout << "7) updating the message with a 2D matrix of shape (" << samples_window << "x" << nb_channels << ") and send without checksum:" << std::endl;
    wfile << "7) updating the message with a 2D matrix of shape (" << samples_window << "x" << nb_channels << ") and send without checksum:\n";

    cTimings(samples_window, nb_channels, sfield, socketHdlr, wfile);

    //////////////     8th attempt to measure the time it takes to update a message with a 2D matrix of doubles and send it     //////////////
    //////////////     number of rows: 4                                                                                        //////////////
    //////////////     number of collumns: 150                                                                                  //////////////
    //////////////     without sending checksum                                                                                 //////////////

    // define the number of rows
    samples_window = 16;

    // define the number of collumns
    nb_channels = 40;

    std::cout << "8) updating the message with a 2D matrix of shape (" << samples_window << "x" << nb_channels << ") and send without checksum:" << std::endl;
    wfile << "8) updating the message with a 2D matrix of shape (" << samples_window << "x" << nb_channels << ") and send without checksum:\n";

    cTimings(samples_window, nb_channels, sfield, socketHdlr, wfile);

   
    // close communication with the server
    socketHdlr.closeCommunication();

    // close logfile
    wfile.close();

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
    
    // std::cout << "the shape of the matrix: (" << dmatrix.size() << "," << dmatrix[0].size() << ")" << std::endl;
    
    return dmatrix;

}

int cTimings(int samples_window, int nb_channels, std::string sfield, socketStream& socketHdlr, std::ofstream& wfile){


    // declare a variable to hold the computational times of each iteration
    std::vector<double> timings;
    double sumTime = 0;

    // declare a variable to hold the computational times of updating the field of a message
    std::vector<double> updateTimings;
    double sumUpdate = 0;

    // declare a variable to hold the computational times of sending the message
    std::vector<double> sendTimings;
    double sumSending = 0;

    // define an object to hold the current time
    auto start = std::chrono::steady_clock::now();
    auto end = std::chrono::steady_clock::now();

    int bytesSent = 0 ;
    // declare a random matrix to contain the new values
    std::vector< std::vector<double> > tmp_2Dmatrix;

    for (int i=0; i<30; i++){

        std::cout << "trial " << i+1 << "\n--------" << std::endl;
        wfile << "trial " << i+1 << "\n--------\n";

        // update the random matrix
        tmp_2Dmatrix = random2Dmatrix(samples_window, nb_channels);

        // set the starting time of the computation
        start = std::chrono::steady_clock::now();

        // update the message
        socketHdlr.updateMSG(sfield, tmp_2Dmatrix);
        
        // set the ending time of the computation
        end = std::chrono::steady_clock::now();

        // throw the computational time for updating the message in the variable "updateTimings" and print it in the terminal
        updateTimings.push_back((double)std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()/1000.0);
        // std::cout << "update time: "<< updateTimings[i] << " ms" << std::endl;
        wfile << "update time: "<< updateTimings[i] << " ms\n";

        // set the starting time of the computation
        start = std::chrono::steady_clock::now();

        // send the message
        bytesSent=socketHdlr.sendMSg();

        // set the ending time of the computation
        end = std::chrono::steady_clock::now();

        std::cout << "Bytes sent: " << bytesSent << std::endl;
        wfile << "Bytes sent: " << bytesSent << "\n";

        // throw the computational time for sending the message in the variable "sendTimings" and print it in the terminal
        sendTimings.push_back((double)std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()/1000.0);
        // std::cout << "sending time: "<< sendTimings[i] << " ms" << std::endl;
        wfile << "sending time: "<< sendTimings[i] << " ms\n";

        // throw the overall computational time in the variable "timings" and print it in the terminal
        timings.push_back(updateTimings[i]+sendTimings[i]);
        // std::cout << "overall time: "<< timings[i] << " ms" << std::endl;
        std::cout << std::endl;
        wfile << "overall time: "<< timings[i] << " ms\n \n";
        
    }

    // find the average computational time and print it in the terminal
    for(auto& n: updateTimings){
        sumUpdate +=n;
    }
    std::cout << "average update time: " << sumUpdate/(double)updateTimings.size() << " ms" << std::endl;
    wfile << "average update time: " << sumUpdate/(double)updateTimings.size() << " ms \n";

    // find the average computational time and print it in the terminal
    for(auto& n: sendTimings){
        sumSending +=n;
    }
    std::cout << "average sending time: " << sumSending/(double)sendTimings.size() << " ms" << std::endl;
    wfile << "average sending time: " << sumSending/(double)sendTimings.size() << " ms\n";

    // find the average computational time and print it in the terminal
    for(auto& n: timings){
        sumTime +=n;
    }
    std::cout << "average overall time: " << sumTime/(double)timings.size() << " ms" << std::endl;
    wfile << "average overall time: " << sumTime/(double)timings.size() << " ms\n \n";

    std::cout << std::endl;

    // delete the contains of the variables (empty the vectors)
    updateTimings.clear();
    sendTimings.clear();
    timings.clear();

    return 0;
}

#ifdef __linux__
    int dirExists(std::string tpath){
        /*	This function checks if the directory 'tpath' exists
            if it exists the function returns 0
        */

        struct stat info;

        if (stat(tpath.c_str(), &info) != 0){
            // could not have access in the folder
            return 0;
        }else if (info.st_mode & S_IFDIR){
            // the folder exists
            return 1;
        }
        else{
            // the folder doesn't exist
            return 1;
        }
    }

#endif