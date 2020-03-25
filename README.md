![](docs/pics/socketStream_logo.png)

A cros-platform C++ library and Python module for exchanging packets over a TCP/IP communication. The socketSteam class supports both client and server implementations on C++ and python. The packets follow the json structure, making them compatible with other languages too. The C++ library supports multi-client server, whilst the server of the python class services only one client. 

The C++ implementaion of md5 checksum is a RSA Data Security, Inc. MD5 Message-Digest Algorithm, borrowed from zedwood in this [link](http://www.zedwood.com/article/cpp-md5-function). Desides the md5 implementation, the rest of the code is distributed under the GNU GPLv3 license.

## Set-up
1) Open a terminal or command prompt (on Windows) and navigate to your workspace. In your workspace folder, create a folder called "dependencies" to contain the cpp dependencies (if it doesn't already exist)

```bash
$ mkdir dependencies
```

2) go inside the folder dependencies and clone the rapidjson library from their [github repo](https://github.com/Tencent/rapidjson/)

```bash
$ cd dependencies
$ git clone https://github.com/Tencent/rapidjson.git
```

3) create an environment variable to contain the path of the dependencies directory

-- On Linux

Open the .bashrc file in your hone directory

```bash
$ cd
$ sudo gedit .bashrc
```

add the following line at the end of the file

export CPP_DEPENDENCIES="PATH/TO/DEPENDENCIES/DIRECTORY/dependencies"

*the "PATH/TO/DEPENDENCIES/DIRECTORY" corresponds to the path where of the folder "dependencies" that is created on the 1st step. If you don't know the path, navigate to the folder "dependencies" in a terminal (or command prompt on Windows) and type "pwd", it will print the path of this directory 

close the .bashrc file and relaunch the terminal. If you want to be sure that the variable is set properly, type the following command in the terminal (or command prompt on Windows)

```bash
$ echo $CPP_DEPENDENCIES
```

and it should print the path on the folder "dependencies"

-- On Windows

![](docs/pics/windows_env_variables_directions.png)

4) clone the socketStream package in workspace directory

```bash
$ git clone https://github.com/yias/socket_jsonCpp.git
```

## Running the ros node
-- for runnng the C++ rosnode:

```bash
$ git clone https://github.com/yias/socket_jsonCpp.git
```

-- for runnng the Python rosnode:

```bash
$ git clone https://github.com/yias/socket_jsonCpp.git
```

