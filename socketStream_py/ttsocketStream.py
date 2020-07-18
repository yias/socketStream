#!/usr/bin/env python

import sys
import ctypes
import numpy as np

import testmodule

tt = testmodule.socketStream()

tt.setBufferSize(64)

if tt.initialize_socketStream() == 0:
    if tt.make_connection() >= 0:
        print('connection established')
    else:
        print('unable to connect')

ff = ["t1", "t2", "t3"]

if tt.initialize_msgStruct(ff) >= 0:
    print('ok')


tt.closeCommunication()