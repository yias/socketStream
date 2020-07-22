#!/usr/bin/env python
"""
    developer: Iason Batzianoulis
    maintaner: Iason Batzianoulis
    email: iasonbatz@gmail.com
    description: 
    This scripts is an example on how to use the socketStream server for listening to inputs from a client
"""

import argparse
import numpy as np
import sys
from socketStream_py import socketStream


def main(args):
    sockHndlr = socketStream.socketStream(svrIP = args.host, svrPort = args.port, socketStreamMode = 1)
    
    sockHndlr.initialize_msgStruct(["name", "data", "dataInt"])

    sockHndlr.updateMSG("name","Mary")

    sockHndlr.updateMSG("data", np.array([[2.4,5.6,783.01],[34,55.6,1.2]]))

    sockHndlr.updateMSG("dataInt", np.array([720, 1280, 3]))

    everything_ok = False
    if sockHndlr.initialize_socketStream() == 0:
        if sockHndlr.runServer() == 0:
            everything_ok = True
    
    if everything_ok:
        # counter=0
        while(True):
            try:
                if sockHndlr.socketStream_ok():
                    tt=sockHndlr.get_latest()
                    # print(tt)
                    if tt is not None:
                        msgData=tt['data']
                        rt=np.array(msgData, dtype=np.float32)
                        print(rt.shape)
                sockHndlr.sendMSg2All()

            except KeyboardInterrupt:
                break

    sockHndlr.closeCommunication()


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='TCP server for receiving inputs from a client with socketStream')
    parser.add_argument('--host', type=str, help= 'the IP of the server', default='localhost')
    parser.add_argument('--port', type=int, help= 'the port on which the server is listening', default=10352)
    parser.add_argument('--buffersize', type=int, help= 'the size of the buffer for pakets receiving', default=16)
    args=parser.parse_args()
    main(args)