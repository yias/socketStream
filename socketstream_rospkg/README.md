![](../docs/pics/socketStream_ros_logo.png)

A ros package wrapped around the socketStream. It receives packet from remote client over a TCP/IP connection and publishes the data to a ros topic. The package contains a roscpp and rospy node. The package has the same dependency on the rapidjson as the C++ library and it requires to introduce the path in the environemnt variables as it is described in readme file of the main folder.

The C++ implementaion of md5 checksum is a RSA Data Security, Inc. MD5 Message-Digest Algorithm, borrowed from zedwood in this [link](http://www.zedwood.com/article/cpp-md5-function). Desides the md5 implementation, the rest of the code is distributed under the GNU GPLv3 license.


## Running the ros node
-- for runnng the C++ rosnode:

```bash
$ rosrun socketstream_node socketStream
```

-- for runnng the Python rosnode:

To launch the python node with rosrun, first, make executable the socketStream_rosnode.py by navigating in the folder "scripts" and type the command:

```bash
$ chmod +x socketStream_rosnode.py
```

Then, run command:

```bash
$ rosrun socketstream_node socketStream_rosnode.py
```
