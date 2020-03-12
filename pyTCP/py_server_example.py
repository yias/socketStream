#!/usr/bin/env python


import socketStream
import threading
import time
import json

def testthread(sSobg):
	while(sSobg.isClientConnected()):
		mesg=sSobg.get_latest()
		print("ok")


def main():
	
	sockHndlr = socketStream.socketStream(IPaddress = '128.178.145.79')

	sockHndlr.make_connection()

	sockHndlr.start_receiveing()

	# sockHndlr.runReceiver()

	# msg_receiver = threading.Thread(target = sockHndlr.run, name= 'daemon')

	# msg_receiver.start()

	# msg_receiver.join()


	# print('test')

	# msg_update = threading.Thread(target=testthread, args=sockHndlr)

	# msg_update.start()

	# sockHndlr.make_connection()

	# sockHndlr.start()
	# sockHndlr.join()

	# print("okkkkkkkkkkkkkkkkkkkkkkkkkkkokok")
	# print(sockHndlr.isClientConnected())

	counter=0
	while (sockHndlr.isClientConnected()):
		tt=sockHndlr.get_latest()
		print(tt[0])
		# print("test")

		# try:
			
		# except KeyboardInterrupt:
			
		# 	break
		# finally:
		# 	pass

	sockHndlr.close_communication()
	






if __name__ == '__main__':
	main()