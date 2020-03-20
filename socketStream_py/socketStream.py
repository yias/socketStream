#!/usr/bin/env python
"""
	developer: Iason Batzianoulis
	maintaner: Iason Batzianoulis
	email: iasonbatz@gmail.com
	description: 
	This scripts defines a class that handles a TCP/IP server for listening to inputs from a client
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

class socketStream():
    def __init__ (self, IPaddress = 'localhost', port = 10352, bufferSize = 128, isServer= True):
        # Create a TCP/IP socket
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        # Bind the socket to the port
        self.server_address = (IPaddress, port)

        print('[socketStream] Starting up on %s port %s' % self.server_address)

        self.isServer = isServer

        # define buffer size
        self.BUFFER_SIZE = bufferSize

        # define a header size of the message
        self.HEADERSIZE = 8

        # define message identifier
        self.msg_idf = "!&?5"

        # end-of-message identifier
        self.endMSG = "!3tt"

        # end-connection identifier
        self.ec_id = "\ne@c"

        # clients name
        self.clientName = ''

        # the number of digits of the buffersize
        self.bfrDigits = len(str(self.BUFFER_SIZE))

        # the lenght of the message identifier
        self.msg_idf_len = len(self.msg_idf)

        # minimum size of the packet
        self.minMsgSize = len(self.msg_idf) + self.HEADERSIZE + len(self.endMSG) + self.bfrDigits+1

        # the use or no of checksum
        self.useHashKey = True

        self.connection_exist = False

        self.isComActive = False

        if self.isServer:
            print("[socketStream] Server mode")
            self.sock.bind(self.server_address)
            # listen for new connections
            self.sock.settimeout(5)
            self.sock.listen(1)
            self.serverRunnig = True
            self.serverThread = threading.Thread(target = self.wait_connections, args = ())
            self.serverThread.daemon = True
            self.serverThread.start()
            self.ReceiverThread = threading.Thread(target=self.runReceiver, args = ())
            self.ReceiverThread.daemon = True
            self.lock = threading.Lock()
            self.firstValueReceived = False
        else:
            print("[socketStream] Client mode")
            self.serverRunnig = False

        self.hc_check=hashlib.md5()

        self.msg_data = {}

        

    def msgExtractor(self, msg):
        msgSize=int(msg[:self.HEADERSIZE])
        msgOverhead = int(msg[self.HEADERSIZE+1:self.HEADERSIZE+1+self.bfrDigits])
        tmp_msg=msg[self.HEADERSIZE+1+self.bfrDigits:self.HEADERSIZE+1+self.bfrDigits+msgSize]
        if(msg[self.HEADERSIZE+1]==1):
            hashcode=msg[self.HEADERSIZE+msgSize:-msgOverhead-len(self.endMSG)]
            self.hc_check.update(tmp_msg)
            if self.hc_check.hexdigest()==hashcode:
                return True, tmp_msg
            else:
                return False, ''
        else:
            return True, tmp_msg

    def randomString(self, strlength=10):
        letters = string.ascii_lowercase
        return ''.join(random.choice(letters) for i in range(strlength))

    def handShake_client(self,conn, strlength):
    
        ping_times=np.empty([10,1], dtype=np.float64)
        compute_times=np.empty([10,1], dtype=np.float64)
        validity_counter=np.empty([10,1], dtype=bool)
        t_time0=0.0

        # receive and send random msgs to the client 10 times
        for i in range(10):
            msg_full=''
            while (True):
                dataT=conn.recv(self.BUFFER_SIZE)
                msg_full+=dataT.decode("utf-8")
                if msg_full[-4:]==self.endMSG:
                    break

            if t_time0!=0.0:
                ping_times[i-1]=time.time()-t_time0
            t_time=time.time()
            msg_validity, tr_msg = self.msgExtractor(msg_full[len(self.msg_idf):])
            validity_counter[i]=msg_validity

            dcdr=hashlib.md5()
            test_msg=self.randomString(strlength)
            dcdr.update(test_msg.encode('utf-8'))
            chSum=dcdr.hexdigest()
            msg_len=('{:<'+str(self.HEADERSIZE)+'}').format(str(len(test_msg)))
            # compute the overhead of the message
            ranString = test_msg + chSum
            msgOverhead = (int((len(ranString)+self.minMsgSize)/self.BUFFER_SIZE)+1)*self.BUFFER_SIZE - (len(ranString)+self.minMsgSize)
            overheadStr = " " * msgOverhead
            msgOverheadSize = ('{:<'+str(self.bfrDigits)+'}').format(str(msgOverhead))
            compute_times[i]=time.time()-t_time
            msg2send = self.msg_idf+msg_len+str(int(self.useHashKey))+msgOverheadSize+(test_msg)+chSum+overheadStr+self.endMSG
            conn.sendall(msg2send.encode('utf-8'))
            t_time0=time.time()

        msg_full=''
        if((1*(validity_counter)).mean()>0.9):
            while (True):
                dataT=conn.recv(self.BUFFER_SIZE)
                msg_full+=dataT.decode('utf-8')
                if msg_full[-4:]==self.endMSG:
                    break

            msg_validity, tr_msg = self.msgExtractor(msg_full[len(self.msg_idf):])
            self.clientName = tr_msg
            print('[socketStream] Compute times: %s %s %s s' %(compute_times.mean(), u'\u00b1', compute_times.std()))
            print('[socketStream] Ping times:  %s %s %s s' %(ping_times[:9].mean(), u'\u00b1', ping_times[:9].std()))
            return True
        else:
            print('[socketStream] Communication is not valid')
            return False

    def handShake_server(self, strlength):
        
        ping_times=np.empty([10,1], dtype=np.float64)
        compute_times=np.empty([10,1], dtype=np.float64)
        validity_counter=np.empty([10,1], dtype=bool)
        t_time0=0.0

        # receive and send random msgs to the client 10 times
        for i in range(10):
            dcdr=hashlib.md5()
            test_msg=self.randomString(strlength)
            dcdr.update(test_msg.encode('utf-8'))
            chSum=dcdr.hexdigest()
            msg_len=('{:<'+str(self.HEADERSIZE)+'}').format(str(len(test_msg)))

            # compute the overhead of the message
            ranString = test_msg + chSum

            msgOverhead = (int((len(ranString)+self.minMsgSize)/self.BUFFER_SIZE)+1)*self.BUFFER_SIZE - (len(ranString)+self.minMsgSize)
            overheadStr = " " * msgOverhead
            compute_times[i]=time.time()-t_time0
            msgOverheadSize = ('{:<'+str(self.bfrDigits)+'}').format(str(msgOverhead))

            msg2send = self.msg_idf+msg_len+str(int(self.useHashKey))+msgOverheadSize+(test_msg)+chSum+overheadStr+self.endMSG

            self.sock.sendall(msg2send.encode('utf-8'))
            t_time0=time.time()
            
            msg_full=''
            while (True):
                dataT=self.sock.recv(self.BUFFER_SIZE)
                msg_full+=dataT.decode("utf-8")
                if msg_full[-4:]==self.endMSG:
                    break

            ping_times[i]=time.time()-t_time0
            t_time0=time.time()
            msg_validity, tr_msg = self.msgExtractor(msg_full[len(self.msg_idf):])
            validity_counter[i]=msg_validity

            

        if((1*(validity_counter)).mean()>0.9):
            dcdr=hashlib.md5()
            test_msg=self.clientName
            dcdr.update(test_msg.encode('utf-8'))
            chSum=dcdr.hexdigest()
            msg_len=('{:<'+str(self.HEADERSIZE)+'}').format(str(len(test_msg)))
            # compute the overhead of the message
            ranString = test_msg + chSum

            msgOverhead = (int((len(ranString)+self.minMsgSize)/self.BUFFER_SIZE)+1)*self.BUFFER_SIZE - (len(ranString)+self.minMsgSize)
            msgOverheadSize = ('{:<'+str(self.bfrDigits)+'}').format(str(msgOverhead))
            overheadStr = " " * msgOverhead
            compute_times[i]=time.time()-t_time0
            msg2send = self.msg_idf+msg_len+str(int(self.useHashKey))+msgOverheadSize+(test_msg)+chSum+overheadStr+self.endMSG
            # print(msg2send)
            self.sock.sendall(msg2send.encode('utf-8'))

            print('[socketStream] Compute times: %s %s %s s' %(compute_times.mean(), u'\u00b1', compute_times.std()))
            print('[socketStream] Ping times:  %s %s %s s' %(ping_times.mean(), u'\u00b1', ping_times.std()))
            return True
        else:
            print('[socketStream] Communication is not valid')
            return False

    def run(self):
        print('[socketStream] Waiting for a connections ... ')
        counter = 0
        while(not self.connection_exist):
            # print(counter)
            counter +=1
            try:
                # check if any connection inquire exists and accept it

                self.connection, self.client_address = self.sock.accept()
                self.connection_exist = True
                print('[socketStream] Connection from ', self.client_address)

                counter = 0
                while(True):
                # retrieve the message identifier. once it is received, compose the message
                    data=self.connection.recv(self.BUFFER_SIZE)
                    data_check = data[:self.msg_idf_len]
                    counter+=1
                    print("messages received",counter)
                    if data_check.decode('utf-8')==self.msg_idf:
                        # receive bytes until the full message is received
                        full_msg=data[self.msg_idf_len:].decode("utf-8")
                        while (True):
                            dataT=self.connection.recv(self.BUFFER_SIZE)
                            full_msg+=dataT.decode("utf-8") 
                            if full_msg[-4:]==self.endMSG:
                                break

                        # extract message
                        msg_validity, tr_msg = self.msgExtractor(full_msg)
                        if msg_validity:
                            self.msg_data=json.loads(tr_msg)
                            # print(self.msg_data.get("name"))

                    if  data_check.decode('utf-8')==self.ec_id:
                        # if end-of-communication identifier received, terminate the connection
                        print('[socketStream] Connection terminated by client ', self.client_address)
                        self.connection.close()
                        self.connection_exist = False
                        break

            # except socket.timeout as e:
            # 	print("socket timeout. retry")
            # 	pass
            except KeyboardInterrupt:
                # if Ctrl+C is pressed in the keyboard, close the connections (if any) and exit
                if self.connection_exist:
                    self.connection.close()
                    self.connection_exist = False
                break
            finally:
                pass
                # print('Waiting for new clients ....')
        print("exit")
        
    def close_communication(self):
        # close communications
        if not self.isServer:
            self.sock.sendall(self.ec_id.encode('utf-8'))
        else:
            self.lock.acquire()
            self.serverRunnig = False
            self.lock.release()
            self.serverThread.join()
            

        if self.connection_exist:
            self.lock.acquire()
            self.connection.close()
            self.connection_exist = False
            self.lock.release()

        self.sock.close()
        print('[socketStream] All connections killed')

    def runReceiver(self):
        counter=0
        while(self.connection_exist):
            # retrieve the message identifier. once it is received, compose the message
            data=self.connection.recv(self.BUFFER_SIZE)
            data_check = data[:self.msg_idf_len]
            counter+=1
            # print("messages received",counter)
            if data_check.decode('utf-8')==self.msg_idf:
                # receive bytes until the full message is received
                full_msg=data[self.msg_idf_len:].decode("utf-8")
                while (True):
                    dataT=self.connection.recv(self.BUFFER_SIZE)
                    full_msg+=dataT.decode("utf-8") 
                    if full_msg[-4:]==self.endMSG:
                        break

                # extract message
                msg_validity, tr_msg = self.msgExtractor(full_msg)
                if msg_validity:
                    self.msg_data=json.loads(tr_msg)
                    if not self.firstValueReceived:
                        self.firstValueReceived = True

            if  data_check.decode('utf-8')==self.ec_id:
                # if end-of-communication identifier received, terminate the connection
                print('[socketStream] Connection terminated by client ', self.client_address)
                self.connection.close()
                self.connection_exist = False
                self.firstValueReceived = False
                break
                
            if not self.serverRunnig:
                break

        if self.connection_exist:
            self.connection.close()
            self.connection_exist = False

    # def runReceiver(self):
    #     counter=0
    #     while(self.connection_exist):
    #         try:
    #             # retrieve the message identifier. once it is received, compose the message
    #             data=self.connection.recv(self.BUFFER_SIZE)
    #             data_check = data[:self.msg_idf_len]
    #             counter+=1
    #             # print("messages received",counter)
    #             if data_check.decode('utf-8')==self.msg_idf:
    #                 # receive bytes until the full message is received
    #                 full_msg=data[self.msg_idf_len:].decode("utf-8")
    #                 while (True):
    #                     dataT=self.connection.recv(self.BUFFER_SIZE)
    #                     full_msg+=dataT.decode("utf-8") 
    #                     if full_msg[-4:]==self.endMSG:
    #                         break

    #                 # extract message
    #                 msg_validity, tr_msg = self.msgExtractor(full_msg)
    #                 if msg_validity:
    #                     self.msg_data=json.loads(tr_msg)
    #                     if not self.firstValueReceived:
    #                         self.firstValueReceived = True

    #             if  data_check.decode('utf-8')==self.ec_id:
    #                 # if end-of-communication identifier received, terminate the connection
    #                 print('[socketStream] Connection terminated by client ', self.client_address)
    #                 self.connection.close()
    #                 self.connection_exist = False
    #                 self.firstValueReceived = False
    #                 break
                
    #             if not self.serverRunnig:
    #                 break
    #         except KeyboardInterrupt:
    #             break

    #     if self.connection_exist:
    #         self.connection.close()
    #         self.connection_exist = False
            

    def get_latest(self):
        self.lock.acquire()
        trace = self.msg_data
        self.lock.release()
        return trace

    def isClientConnected(self):
        return self.connection_exist

    def isFirstMsgReceived(self):
        return self.firstValueReceived

    def isServerRunning(self):
        return self.serverRunnig

    def sockectStream_ok(self):
        return (self.serverRunnig and self.connection_exist and self.firstValueReceived)

    def set_data(self, msg):
        self.msg_data = msg

    def start_receiveing(self):
        self.ReceiverThread.start()

    def wait_connections(self):
        print('[socketStream] Waiting for connections ... ')
        while(self.serverRunnig):
            try:
                # check if any connection inquire exists and accept it
                self.connection, self.client_address = self.sock.accept()
                conn_validity = self.handShake_client(self.connection, 10)
                if conn_validity:
                    print("[socketStream] Connection from " + self.client_address[0] + " with ID: " + self.clientName)
                    testThrd = threading.Thread(target=self.runReceiver, args = ())
                    testThrd.daemon = True
                    self.connection_exist = True
                    testThrd.start()
                    testThrd.join()
                else:
                    print("[socketStream] Handshake protocol failed! Connection from " + self.client_address[0] + " with ID: " + self.clientName + "is rejected.")
            except socket.timeout:
                pass


    def isConnected(self):
        return self.isComActive

    def set_clientName(self, cID):
        self.clientName = cID

    def make_connection(self):
        print("[socketStream] Attempting to connect to server with address" + self.server_address[0] +" in the port %s" , str(self.server_address[1]))
        self.sock.connect(self.server_address)
        conValidity = self.handShake_server(10)
        if conValidity:
            print("[socketStream] Connected to server " + self.server_address[0] +" in port " + str(self.server_address[1]))
            self.isComActive = True
        else:
            print("[socketStream] Handshake protocol failed! Unable to connect to server.")
            print("[socketStream] Closing connection with the server")
            self.sock.close()


    def initialize_msgStruct(self, fields):
        result={fields[i]:[] for i in range(len(fields))}
        frt=json.dumps(result)
        self.msg_data = json.loads(frt)
        print("[socketStream] The message struct initialized successfully")



    def updateMSG(self, field, value):
        if field in list(self.msg_data):
            # tmp={field: value}
            # self.msg_data.update(tmp)
            self.msg_data[field] = value
        else:
            print("[socketStream] Not valid field name")

    def sendMSG(self, msgData=None):
        if msgData is None:
            tmp_msg = json.dumps(self.msg_data)
        else:
            tmp_msg = msgData
        dcdr=hashlib.md5()
        dcdr.update(tmp_msg.encode('utf-8'))
        chSum=dcdr.hexdigest()
        msg_len=('{:<'+str(self.HEADERSIZE)+'}').format(str(len(tmp_msg)))

        # compute the overhead of the message
        ranString = tmp_msg + chSum
        print(self.bfrDigits)

        msgOverhead = (int((len(ranString)+self.minMsgSize)/self.BUFFER_SIZE)+1)*self.BUFFER_SIZE - (len(ranString)+self.minMsgSize)
        msgOverheadSize = ('{:<'+str(self.bfrDigits)+'}').format(str(msgOverhead))
        overheadStr = " " * msgOverhead
        msg2send = self.msg_idf+msg_len+str(int(self.useHashKey))+msgOverheadSize+ranString+chSum+overheadStr+self.endMSG
        self.sock.sendall(msg2send.encode('utf-8'))

  