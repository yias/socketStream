#!/usr/bin/env python


import socketStream
import argparse
import numpy as np


def main(args):
    sockHndlr = socketStream.socketStream(IPaddress = args.host, port = args.port)

    counter=0
    while(True):
        # print(counter)
        try:
            if sockHndlr.sockectStream_ok():
                tt=sockHndlr.get_latest()
                print(tt.get("name"))
                test=tt.get("data")
                rt=np.array(test, dtype=np.float32)
                print(rt[0][:2])

        except KeyboardInterrupt:
            break

    sockHndlr.close_communication()


if __name__ == '__main__':
	parser = argparse.ArgumentParser(description='TCP server for receiving inputs from a client with socketStream')
	parser.add_argument('--host', type=str, help= 'the IP of the server', default='localhost')
	parser.add_argument('--port', type=int, help= 'the port on which the server is listening', default=10352)
    parser.add_argument('--buffersize', type=int, help= 'the osize of the buffer for pakets receiving', default=128)
	args=parser.parse_args()
	main(args)