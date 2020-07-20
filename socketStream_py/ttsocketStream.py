#!/usr/bin/env python

import sys
import ctypes
import numpy as np
import time

import testmodule

tt = testmodule.socketStream()

# tt.setBufferSize(64)

tt.set_clientName('testClient')

ff = ["t1", "t2", "t3","t7", "t4", "t5", "t8"]


if tt.initialize_msgStruct(ff) >= 0:
    print('ok')

tt.updateMSG('t1', 'test')

tt.updateMSG('t2', 123)

tt.updateMSG('t3', 657.96345)

aa = np.array([1,2,5])

tt.updateMSG('t4', aa)

dd = np.array([1.456,2543.7872,5.9981245])

tt.updateMSG('t5', dd)

cc = np.array([[1,7,4,45,100],[5,6,3,-12,432],[123,12,55,-443, -1000]])

tt.updateMSG('t7', cc)

rr = np.array([[1,7,4,45,100],[5,6,3,-12,432],[123.244,12.01451,55.1,-443.12, -1000.0012314]])

tt.updateMSG('t8', rr)

# tt.printMSGString()

tt.printMSGcontents()

tt.printMSGcontentsTypes()

tt.setVerbose(True)

# nM = tt.getFullmsg()

# print(nM)
# print(type(nM))

if tt.initialize_socketStream() == 0:
    if tt.make_connection() >= 0:
        print('connection established')
    else:
        print('unable to connect')
time.sleep(0.01)
while (True):
    try:
        tt.sendMsg()
        if (tt.socketStream_ok()):
            msg, isNew = tt.get_latest()
            # print(isNew)
            # if isNew:
            #     print(msg['name'])
        time.sleep(0.1)
    except KeyboardInterrupt:
        break

tt.closeCommunication()