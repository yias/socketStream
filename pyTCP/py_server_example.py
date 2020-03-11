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

	sockHndlr.run()

	# sockHndlr.runReceiver()

	msg_receiver = threading.Thread(target = sockHndlr.runReceiver)

	msg_receiver.start()

	print('test')

	msg_update = threading.Thread(target=testthread, args=sockHndlr)

	msg_update.start()

	counter=0
	while (sockHndlr.isClientConnected()):
		# msg = sockHndlr.get_latest()
		counter +=1
		# print(counter)
		# with('app.json','w') as msg:
		# 	json.dump(sockHndlr.get_latest(), msg)
		# msg = json.load(sockHndlr.get_latest())
		# msg = sockHndlr.get_latest()
		# print(type(msg))
		# print(msg.get("name"))
		# ttr=json.loads(msg)
		# print(ttr.get("name"))

	sockHndlr.close_communication()






if __name__ == '__main__':
	main()