

#include "socketStream.h"
#include <chrono>
#include <fstream>

std::vector< std::vector<double> > txt2matrix(std::string filename);


int main(int argc, char **argv){

    char *srvIP = "localhost";//"128.178.145.67";

    if(argc!=2){
        std::cout << "No server IP suplied. Continue with localhost" << std::endl;
    }else{
        srvIP=argv[1];
    }

    socketStream socketHdlr(srvIP);

    std::vector <std::string> test_rjson;

    test_rjson.push_back("name");
    test_rjson.push_back("data");

    auto start = std::chrono::steady_clock::now();

    if(socketHdlr.initialize_msgStruct(test_rjson)<0){
        std::cerr << "Unable to inizialize message structure" << std::endl;
        return -1;
    }

    auto end = std::chrono::steady_clock::now();
    std::cout << "Message initialization: " << (double)(std::chrono::duration_cast<std::chrono::microseconds>(end-start).count())/1000.0 << " ms" << std::endl;

    std::string sfield("name");
    char *svalue = {"Iason"};

    start = std::chrono::steady_clock::now();
    socketHdlr.updateMSG(sfield, svalue);
    end = std::chrono::steady_clock::now();
    std::cout << "Message update (filed names): " << (double)std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()/1000.0 << " ms" << std::endl;

    std::vector<std::string> fileNames;
    fileNames.push_back("data\\data_example1.txt");
    fileNames.push_back("data\\data_example2.txt");
    fileNames.push_back("data\\data_example3.txt");
    fileNames.push_back("data\\data_example4.txt");


    std::cout << "1) updating the message with a 2D matrix of shape (100x16):" << std::endl;
    
    std::vector< std::vector<double> > dataMatrix = txt2matrix(fileNames[0]);

    std::vector< std::vector<double> >::const_iterator first;
    std::vector< std::vector<double> >::const_iterator last;
    std::vector<double> timings;
    double sumTime = 0;
    int counter = 0;

    if(socketHdlr.initialize_sockeStream()<0){
        std::cerr << "Unable to initialize socket" << std::endl;
        return -1;
    }

    if(socketHdlr.make_connection()<0){
        std::cerr << "Unable to connect to " << srvIP << std::endl;
        return -1;
    }

    sfield="data";
    for(int i=0; i< dataMatrix.size(); i+=100){
        std::vector<std::vector<double>> tmp_mat;
        for(int j=i; j<i+100; j++){
            tmp_mat.push_back(std::vector<double>(dataMatrix[j].begin(), dataMatrix[j].end()));
        }
        std::cout << "the shape of the matrix: (" << tmp_mat.size() << "," << tmp_mat[0].size() << ")" << std::endl;
        
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

    ////////////////////////////////////////////

    std::cout << "2) updating the message with a 2D matrix of shape (100x23):" << std::endl;
    
    dataMatrix = txt2matrix(fileNames[1]);
    timings.clear();
    sumTime = 0;
    counter = 0;

    for(int i=0; i< dataMatrix.size(); i+=100){
        std::vector<std::vector<double>> tmp_mat;
        for(int j=i; j<i+100; j++){
            tmp_mat.push_back(std::vector<double>(dataMatrix[j].begin(), dataMatrix[j].end()));
        }
        std::cout << "the shape of the matrix: (" << tmp_mat.size() << "," << tmp_mat[0].size() << ")" << std::endl;
        
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

    ///////////////////////////////////////////////////////////////////////////////////

    std::cout << "3) updating the message with a 2D matrix of shape (16x100):" << std::endl;
    
    dataMatrix = txt2matrix(fileNames[2]);
    timings.clear();
    sumTime = 0;
    counter = 0;

    for(int i=0; i< dataMatrix[0].size(); i+=100){
        std::vector<std::vector<double>> tmp_mat;
        for(int j=0; j<dataMatrix.size(); j++){
            tmp_mat.push_back(std::vector<double>(dataMatrix[j].begin()+i, dataMatrix[j].begin()+i+100));
        }
        std::cout << "the shape of the matrix: (" << tmp_mat.size() << "," << tmp_mat[0].size() << ")" << std::endl;
        
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

    ///////////////////////////////////////////////////////////////////////////////////

    std::cout << "4) updating the message with a 2D matrix of shape (23x100):" << std::endl;
    
    dataMatrix = txt2matrix(fileNames[3]);
    timings.clear();
    sumTime = 0;
    counter = 0;

    for(int i=0; i< dataMatrix[0].size(); i+=100){
        std::vector<std::vector<double>> tmp_mat;
        for(int j=0; j<dataMatrix.size(); j++){
            tmp_mat.push_back(std::vector<double>(dataMatrix[j].begin()+i, dataMatrix[j].begin()+i+100));
        }
        std::cout << "the shape of the matrix: (" << tmp_mat.size() << "," << tmp_mat[0].size() << ")" << std::endl;
        
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


    

    // sfield="year";
    // double t_value1[] = {1.5,4.67,50.095,14.99,12};
    // double t_value2[] = {3.2,15.4,1502.898,12,5.4};
    // std::vector< std::vector<double> > t_value(2);
    // t_value[0]=std::vector<double>(t_value1, t_value1 +(sizeof(t_value1)/sizeof(t_value1[0])));
    // t_value[1]=std::vector<double>(t_value2, t_value2 +(sizeof(t_value2)/sizeof(t_value2[0])));
    
    // start = std::chrono::steady_clock::now();
    // socketHdlr.updateMSG(sfield, t_value);
    // end = std::chrono::steady_clock::now();
    // std::cout << "Message update (array of arrays): " << (double)std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()/1000.0 << " ms" << std::endl;
     

    // if(socketHdlr.initialize_sockeStream()<0){
    //     std::cerr << "Unable to initialize socket" << std::endl;
    //     return -1;
    // }

    // if(socketHdlr.make_connection()<0){
    //     std::cerr << "Unable to connect to " << srvIP << std::endl;
    //     return -1;
    // }
    // start = std::chrono::steady_clock::now();
    // if(socketHdlr.sendMSg()<0){
    //     std::cerr << "unable to send message " << std::endl;
    //     return -1;
    // }
    // end = std::chrono::steady_clock::now();
    // std::cout << "Sending message: " << (double)std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()/1000.0 << " ms" << std::endl;

    // // std::cout << "the sent message: " << socketHdlr.getFullmsg() << std::endl;

    // t_value[0][1]=45.4;
    // t_value[1][2]=1111.456;
    // start = std::chrono::steady_clock::now();
    // socketHdlr.updateMSG(sfield, t_value);
    // if(socketHdlr.sendMSg()<0){
    //     std::cerr << "unable to send message " << std::endl;
    //     return -1;
    // }
    // end = std::chrono::steady_clock::now();
    // std::cout << "Update and send message: " << (double)std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()/1000.0 << " ms" << std::endl;

    // start = std::chrono::steady_clock::now();
    // socketHdlr.updateMSG(sfield, t_value);
    // if(socketHdlr.sendMSg()<0){
    //     std::cerr << "unable to send message " << std::endl;
    //     return -1;
    // }
    // end = std::chrono::steady_clock::now();
    // std::cout << "Update and send message 2: " << (double)std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()/1000.0 << " ms" << std::endl;

    // // std::cout << "the sent message: " << socketHdlr.getFullmsg() << std::endl;

    // t_value[0][1]=12.6874;
    // t_value[1][2]=222.5609;
    // socketHdlr.setHashKey(false);

    // start = std::chrono::steady_clock::now();
    // socketHdlr.updateMSG(sfield, t_value);
    
    // if(socketHdlr.sendMSg()<0){
    //     std::cerr << "unable to send message " << std::endl;
    //     return -1;
    // }
    // end = std::chrono::steady_clock::now();
    // std::cout << "Update and send message (without checksum): " << (double)std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()/1000.0 << " ms" << std::endl;

    // start = std::chrono::steady_clock::now();
    // socketHdlr.updateMSG(sfield, t_value);
    
    // if(socketHdlr.sendMSg()<0){
    //     std::cerr << "unable to send message " << std::endl;
    //     return -1;
    // }
    // end = std::chrono::steady_clock::now();
    // std::cout << "Update and send message (without checksum) 2: " << (double)std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()/1000.0 << " ms" << std::endl;

    // // std::cout << "the sent message: " << socketHdlr.getFullmsg() << std::endl;


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

    std::cout << "the shape of the matrix: (" << dmatrix.size() << "," << dmatrix[0].size() << ")" << std::endl;

    return dmatrix;

}