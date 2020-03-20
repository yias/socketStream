/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */


#include "ros/ros.h"
#include "std_msgs/String.h"

#include <sstream>

#include "socketStream.h"


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
    socketStream svrHdlr(srvIP, svrPort, SOCKETSTREAM_SERVER);

    // initialize socketStream
    svrHdlr.initialize_socketStream();

    // activate the server
    svrHdlr.runServer();

    // define a string object to receive the data
    std::string sockStream_msg;
    
    // define a boolean variable for checking if the message is new or not
    bool isNew = false;
  
    // define ros node
    ros::init(argc, argv, "socketStream_node");
    
    ros::NodeHandle n;

    // define ros publisher
    ros::Publisher sockStream_publisher = n.advertise<std_msgs::String>("socketStreamer", 2);

    // define ros message
    std_msgs::String msg;

    // set the frequncy to 10Hz
    ros::Rate loop_rate(10);


    int count = 0;
    while (ros::ok())
    {
        
        if(svrHdlr.socketStream_ok()){
            // if the socketStream server is active, receive a message from the client with name "sendWithTime"
            sockStream_msg = svrHdlr.get_latest("sendWtime", &isNew);

            if(isNew){
                // if the message is new, publish the message to the ros-topic
                msg.data = sockStream_msg;

                // activate the following command if you want to print ros loginfo in the console
                // ROS_INFO("%s", msg.data.c_str());

                // publish the string message
                sockStream_publisher.publish(msg);
            }
        }

        ros::spinOnce();

        loop_rate.sleep();
        ++count;
    }

    // kill all the communications and the socket
    svrHdlr.closeCommunication();


    return 0;
}
