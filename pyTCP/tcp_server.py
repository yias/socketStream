#!/usr/bin/env python
"""
	developer: Iason Batzianoulis
	maintaner: Iason Batzianoulis
	email: iasonbatz@gmail.com
	description: 
	This scripts creates a TCP/IP server for listening to inputs from a client
"""


# import standard modules 
import numpy as np
import argparse
import hashlib
import random
import string

# import modules for socket programming (TCP/IP connection)
import socket
import sys
import json

# import modules for theading programming
import threading
import time


def calc_checksum(s):
    return '%2X' % (-(sum(ord(c) for c in s) % 256) & 0xFF)

def msgExtractor(msg, hdrSize, endMsgID):
    msgSize=int(msg[:hdrSize].decode('utf-8'))
    tmp_msg=msg[hdrSize:hdrSize+msgSize-sys.getsizeof('')]
    hashcode=msg[hdrSize+msgSize-sys.getsizeof(''):-len(endMsgID)]
    hc_check=hashlib.md5()
    hc_check.update(tmp_msg)
    if hc_check.hexdigest()==hashcode:
        return True, tmp_msg
    else:
        return False, ''


def randomString(strlength=10):
	letters=string.ascii_lowercase
	return ''.join(random.choice(letters) for i in range(strlength))


def handShake(conn, strlength):

	ping_times=np.empty([10,1], dtype=np.float64)
	compute_times=np.empty([10,1], dtype=np.float64)
	validity_counter=np.empty([10,1], dtype=bool)
	endMSG="!3tt"
	HEADERSIZE=4
	t_time0=0.0

	# receive and send random msgs to the client 10 times
	for i in range(10):
		msg_full=''
		while (True):
			dataT=conn.recv(4)
			msg_full+=dataT
			if msg_full[-4:].decode('utf-8')==endMSG:
				break

		if t_time0!=0.0:
			ping_times[i-1]=time.time()-t_time0
		t_time=time.time()
		msg_validity, tr_msg = msgExtractor(msg_full,HEADERSIZE,endMSG)
		validity_counter[i]=msg_validity

		dcdr=hashlib.md5()
		test_msg=randomString(strlength)
		dcdr.update(test_msg)
		chSum=dcdr.hexdigest()
		msg_len=('{:<'+str(HEADERSIZE)+'}').format(str(sys.getsizeof(test_msg)))
		compute_times[i]=time.time()-t_time
		conn.sendall(msg_len.encode('utf-8')+(test_msg).encode('utf-8')+chSum.encode('utf-8')+endMSG.encode('utf-8'))
		t_time0=time.time()

	if((1*(validity_counter)).mean()>0.8):
		print('valid communication established')
		print('compute times: %s %s %s s' %(compute_times.mean(), u'\u00b1', compute_times.std()))
		print('ping times:  %s %s %s s' %(ping_times[:9].mean(), u'\u00b1', ping_times[:9].std()))
		return True
	else:
		print('communication is not valid')
		return False



def main(args):

	# Create a TCP/IP socket
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

	# Bind the socket to the port
	server_address = (args.host, args.port)

	print('starting up on %s port %s' % server_address)

	sock.bind(server_address)

	# initialize client and adresses lists
	# connections_list=list() #np.empty([1,])
	# clients_address=list() #np.empty([1,])

	# define buffer size
	BUFFER_SIZE=4

	# define a header size of the message
	HEADERSIZE=8

	# define message identifier
	msg_idf="!&?5"

	# end-of-message identifier
	endMSG="!3tt"

	# end-connection identifier
	ec_id="\ne@c"
	# listen for new connections
	# sock.settimeout(3)
	sock.listen(6)
	#Wait for a connection
	connection_exist=False
	print('waiting for a connections ... ')

	while(True):
		try:
			# check if any connection inquire exists and accept it
			connection, client_address = sock.accept()
			connection_exist=True
			print('connection from ', client_address)

			# check communication validity with a hand-shake protocol
			# conCheck=handShake(connection,10)
			while(True):
			# retrieve the message identifier. once it is received, compose the message
				data=connection.recv(BUFFER_SIZE)
				# print(data)
				# time.sleep(3)

				if data.decode('utf-8')==msg_idf:
					# print('understood')
					# receive bytes until the full message is received
					full_msg=''
					while (True):
						dataT=connection.recv(1)
						full_msg+=dataT.decode("utf-8") 
						if full_msg[-4:]==endMSG:
							break

				# extract message
				# 	msg_validity, tr_msg = msgExtractor(full_msg,HEADERSIZE,endMSG)
					msg_data=json.loads(full_msg[:-4])
					print(msg_data)
					print('name: %s' %(msg_data.get("name")))
					print('age: %s' %(msg_data.get("age")))

				# 	if msg_validity:
				# 		# if the message is valid, retrieve the data and load them into a json object
				# 		msg_data=json.loads(tr_msg.decode('utf-8'))

				# 		print('translation: %s' %(msg_data.get("translation")))
				# 		print('rotation: %s' %(msg_data.get("rotation")))
				# 		print('button: %s' %(msg_data.get("button")))

				if  data.decode('utf-8')==ec_id:
					# if end-of-communication identifier received, terminate the connection
					print('Connection terminated by client ', client_address)
					connection.close()
					break
            	
		except KeyboardInterrupt:
			# if Ctrl+C is pressed in the keyboard, close the connections (if any) and exit
			if connection_exist:
				connection.close()
			break
		finally:
			print('Waiting for new clients ....')
    
	# close communications
	sock.close()
	print('all connections killed')


		


if __name__ == '__main__':

    parser = argparse.ArgumentParser(description='TCP server for receiving inputs from 3D mouse client')

    parser.add_argument('--host', type=str, help= 'the IP of the server', default='localhost')

    parser.add_argument('--port', type=int, help= 'the port on which the server is listening', default=10352)

    parser.add_argument('--roosnode','--rn', type=bool, help= 'if needed to publish the data to a ros topic', default=False)

    parser.add_argument('--version', '-V', help='show program version', action='store_true')

    args=parser.parse_args()

    main(args)	