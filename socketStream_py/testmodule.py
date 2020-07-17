#!/usr/bin/env python

import sys
import ctypes
import numpy as np

if sys.platform == 'linux':
    lib = ctypes.cdll.LoadLibrary('libsocketStream.so')
else:
    lib = ctypes.cdll.LoadLibrary('..\\bin\\win32\\dll\\x64\\libsocketStream.dll')



class socketStream(object):
    def __init__(self, svrIP="localhost", svrPort=10352, freq=20, nb_channels=16):
        lib.socketStream.argtypes = [ctypes.c_char_p]
        lib.socketStream.restype = ctypes.c_void_p
        self.obj = lib.socketStream(svrIP.encode())




