#!/usr/bin/env python

import sys
import ctypes
import numpy as np
import time

from socketStream_py import socketStream

ssClient = socketStream.socketStream()

# tt.setBufferSize(64)

ssClient.set_clientName('testClient')

fields_names = ["t1", "t2", "t3","t7", "t4", "t5", "data"]


if ssClient.initialize_msgStruct(fields_names) >= 0:
    print('ok')

ssClient.updateMSG('t1', 'test')

ssClient.updateMSG('t2', 123)

ssClient.updateMSG('t3', 657.96345)

aa = np.array([1,2,5])

ssClient.updateMSG('t4', aa)

dd = np.array([1.456,2543.7872,5.9981245])

ssClient.updateMSG('t5', dd)

cc = np.array([[1,7,4,45,100],[5,6,3,-12,432],[123,12,55,-443, -1000]])

ssClient.updateMSG('t7', cc)

rr = np.array([[1,7,4,45,100],[5,6,3,-12,432],[123.244,12.01451,55.1,-443.12, -1000.0012314]])

ssClient.updateMSG('data', rr)

# tt.printMSGString()

ssClient.printMSGcontents()

ssClient.printMSGcontentsTypes()

ssClient.setVerbose(True)

if ssClient.initialize_socketStream() == 0:
    if ssClient.make_connection() >= 0:
        print('connection established')
    else:
        print('unable to connect')
counter = 0
while (True):
    try:
        print(counter)
        ssClient.sendMsg()
        if (ssClient.socketStream_ok()):
            print('tt')
            msg = ssClient.get_latest()
            if msg is not None:
                msgData = np.array(msg['data'])
                print(type(msgData), msgData.shape, msgData.dtype)
                # print(msg['name'])
        time.sleep(0.1)
        counter +=1
    except KeyboardInterrupt:
        break

ssClient.closeCommunication()