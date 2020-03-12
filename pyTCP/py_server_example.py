#!/usr/bin/env python


import socketStream
import threading
import time
import json
import argparse
import numpy as np


def main(args):
    sockHndlr = socketStream.socketStream(IPaddress = args.host, port = args.port)

    sockHndlr.make_connection()

    sockHndlr.start_receiveing()

    while(sockHndlr.isClientConnected()):
        if sockHndlr.isFirstMsgReceived():
            tt=sockHndlr.get_latest()
            print(tt.get("name"))
            test=tt.get("data")
            rt=np.array(test, dtype=np.float32)
            print(rt[0][:2])

    sockHndlr.close_communication()


if __name__ == '__main__':
	parser = argparse.ArgumentParser(description='TCP server for receiving inputs from a client with socketStream')
	parser.add_argument('--host', type=str, help= 'the IP of the server', default='localhost')
	parser.add_argument('--port', type=int, help= 'the port on which the server is listening', default=10352)
	args=parser.parse_args()
	main(args)