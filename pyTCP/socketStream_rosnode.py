#!/usr/bin/env python


# import ros-related modules
import rospy
from std_msgs.msg import Float32MultiArray

# import socketStream to handl the socket communication
import socketStream

# import numpy for easier data manipulation
import numpy as np

def talker():
	pub = rospy.Publisher('socketStreamer', Float32MultiArray, queue_size = 2)
	rospy.init_node('socketStream_node', anonymous = True)
	rate = rospy.Rate(100)

	rosmsg = Float32MultiArray()
	rosmsg.layout.data_offset = 0

	sockHndlr = socketStream.socketStream(IPaddress = "128.178.145.79", port = 10352)

	while not rospy.is_shutdown():
		if sockHndlr.sockectStream_ok():
            socket_msg = sockHndlr.get_latest()
            socket_data = socket_msg.get("data")
            data_matrix = np.array(socket_data, dtype=np.float32)
            data_dimensions = list(socket_msg.shape)
            
            # construct the message
            rosmsg.data = socket_msg.reshape(np.prod(data_dimensions))
            rosmsg.layout.dim = data_dimensions
            rosmsg.layout.dim[0].label = socket_msg.get("name")
            rosmsg.layout.dim[0].size = data_dimensions[0]
            rosmsg.layout.dim[0].stride = np.prod(data_dimensions)
            rosmsg.layout.dim[1].label = socket_msg.get("name")
            rosmsg.layout.dim[1].size = data_dimensions[1]
            rosmsg.layout.dim[1].stride = data_dimensions[1]

            pub.publish(rosmsg)


    sockHndlr.close_communication()


if __name__ == '__main__':

	try:
		talker()
	except rospy.ROSInterruptException:
		pass

