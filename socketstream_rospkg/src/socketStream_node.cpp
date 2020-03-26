/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */


#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float64MultiArray.h"
#include "std_msgs/MultiArrayDimension.h"

#include <sstream>

#include "socketStream.h"
#include "jsonWrapper.hpp"


int main(int argc, char **argv)
{

    // define the variable that holds the server IP. In this case, the server would be a local server.
    const char *srvIP = "localhost";

    // define port for the communication
    int svrPort = 10352;

    // if no new server IP is defined from the user, continue with the pre-defined server IP (localhost)
    if(argc!=2){
        std::cout << "No server IP provided. Continue with localhost" << std::endl;
    }else{
        srvIP=argv[1];
    }

    // create an sockectStream object with the selected server IP address and set it up as a server
    socketStream svrHdlr(srvIP, svrPort, SOCKETSTREAM::SOCKETSTREAM_SERVER);

    // initialize socketStream
    svrHdlr.initialize_socketStream();

    // activate the server
    svrHdlr.runServer();

    // define a string object to receive the data
    std::string sockStream_msg;
    
    // define a boolean variable for checking if the message is new or not
    bool isNew = false;

    // a 2D matrix of doubles to store the received data
    std::vector< std::vector<double> > mat_double;
  
    // define ros node
    ros::init(argc, argv, "socketStream_node");
    
    ros::NodeHandle n;

    // define ros publishers
    ros::Publisher sockStream_publisher_string = n.advertise<std_msgs::String>("socketStreamer/String", 2);       // for publishing the message as a string

    ros::Publisher sockStream_publisher_mArray = n.advertise<std_msgs::Float64MultiArray>("socketStreamer/MultiArray", 2);      // for publishing the contains of a field as a matrix

    // define ros message

    // string
    std_msgs::String string_msg;

    // multi-array
    std_msgs::Float64MultiArray mArray_msg;

    // set-up the layout of the message and the labels of the dimensions 
    mArray_msg.layout.dim.push_back(std_msgs::MultiArrayDimension());
    mArray_msg.layout.dim[0].label = "channels";

    mArray_msg.layout.dim.push_back(std_msgs::MultiArrayDimension());
    mArray_msg.layout.dim[1].label = "samples";

    // helper vector for serialization
    std::vector<double> helper_vec;

    // set the frequncy to 10Hz
    ros::Rate loop_rate(10);


    while (ros::ok())
    {
        
        if(svrHdlr.socketStream_ok()){
            // if the socketStream server is active, receive a message from the client with name "sendWithTime"
            sockStream_msg = svrHdlr.get_latest("sendWtime", &isNew);

            if(isNew){

                // if the message is new, publish the message to the ros-topics

                // parse the json string in a json document
                jsonWrapper testObj(sockStream_msg);

                // get the contains of the field "data"
                mat_double = testObj.getField<rapidJson_types::Mat2DD>(std::string("data"));

                // set-up the size and stride of each dimension
                mArray_msg.layout.dim[0].size = mat_double.size();
                mArray_msg.layout.dim[0].stride = mat_double.size()*mat_double[0].size(); // assuming that all the rows of a vector is of equal length
                mArray_msg.layout.dim[1].size = mat_double[0].size();
                mArray_msg.layout.dim[1].stride = mat_double[0].size();

                // reshape the received data matrix row-wise (row-major)
                helper_vec.resize(mat_double.size()*mat_double[0].size());
                for(int i = 0; i < (int)mat_double.size(); i++){
                    std::copy(mat_double[i].begin(), mat_double[i].end(), helper_vec.begin()+i*mat_double[i].size());
                }

                // place the data to the multi-array message
                mArray_msg.data = helper_vec;
                
                // publish the multi-array message
                sockStream_publisher_mArray.publish(mArray_msg);
                
                // add data to the String ros-message
                string_msg.data = sockStream_msg;

                // publish the string message
                sockStream_publisher_string.publish(string_msg);

                // uncomment the following command if you want to print ros loginfo in the console
                // ROS_INFO("%s", msg.data.c_str());

                // clear the helper vector
                helper_vec.clear();
            }
        }

        ros::spinOnce();

        loop_rate.sleep();
    }

    // kill all the communications and the socket
    svrHdlr.closeCommunication();


    return 0;
}
