#!/usr/bin/env python
"""
    developer: Iason Batzianoulis
    maintaner: Iason Batzianoulis
    email: iasonbatz@gmail.com
    description: 
    This scripts is an example on how to use the socketStream client for listening messages from a server
"""

import socketStream
import argparse
import numpy as np


def main(args):
    sockClient = socketStream.socketStream(IPaddress = args.host, port = args.port, bufferSize = args.buffersize, isServer = False)

    sockClient.set_clientName("py_listener_example")

    sockClient.initialize_msgStruct(["name","data"])

    sockClient.updateMSG("name","Mary")

    sockClient.updateMSG("data", [[2.4,5.6,783.01],[34,55.6,1.2]])

    sockClient.make_connection()

    if sockClient.isConnected():
        # print('test main')
        while(True):
            try:
                # print('socketStream ok: ', sockClient.socketStream_ok())
                if sockClient.socketStream_ok():
                    tt, isnew = sockClient.get_latest()
                    # print(tt)
                    if isnew:
                        test = tt.get("data")
                        rt = np.array(test, dtype=np.float32)
                        # print('I got data')
                        print(rt.shape)
                    # print(rt)
                    sockClient.sendMSG()
            except KeyboardInterrupt:
                break
           
    
    sockClient.close_communication()




if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='TCP server for receiving inputs from a client with socketStream')
    parser.add_argument('--host', type=str, help= 'the IP of the server', default='localhost')
    parser.add_argument('--port', type=int, help= 'the port on which the server is listening', default=10352)
    parser.add_argument('--buffersize', type=int, help= 'the size of the buffer for pakets receiving', default=16)
    args=parser.parse_args()
    main(args)