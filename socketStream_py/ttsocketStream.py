#!/usr/bin/env python

import sys
import ctypes
import numpy as np

import testmodule

tt = testmodule.socketStream()

tt.setBufferSize(64)

tt.set_clientName('testClient')

ff = ["t1", "t2", "t3","t7", "t4", "t5", "t8", "t9", "t10"]


if tt.initialize_msgStruct(ff) >= 0:
    print('ok')

tt.updateMSG('t1', 'test')

tt.updateMSG('t2', 3)

tt.updateMSG('t3', 657.96345)

aa = np.array([1,2,5])

tt.updateMSG('t4', aa)

tt.printMSGString()

if tt.initialize_socketStream() == 0:
    if tt.make_connection() >= 0:
        print('connection established')
    else:
        print('unable to connect')


tt.closeCommunication()