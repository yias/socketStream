#!/usr/bin/env python

import sys
import ctypes
import numpy as np

if sys.platform == 'linux':
    lib = ctypes.cdll.LoadLibrary('libsocketStream.so')
else:
    lib = ctypes.cdll.LoadLibrary('..\\bin\\win32\\dll\\x64\\libsocketStream.dll')



class socketStream(object):
    def __init__(self, svrIP="localhost", svrPort=10352, socketStreamMode=0):
        lib.create_socketStream.argtypes = [ctypes.c_char_p, ctypes.c_int, ctypes.c_int]
        lib.create_socketStream.restype = ctypes.c_void_p

        lib.ss_initialize.argtypes = [ctypes.c_void_p]
        lib.ss_initialize.restype = ctypes.c_int

        lib.ss_make_connection.argtypes = [ctypes.c_void_p]
        lib.ss_make_connection.restype = ctypes.c_int

        lib.ss_setBufferSize.argtypes = [ctypes.c_void_p, ctypes.c_int]
        lib.ss_setBufferSize.restype = ctypes.c_int

        lib.ss_closeCommunication.argtypes = [ctypes.c_void_p]
        lib.ss_closeCommunication.restype = ctypes.c_void_p

        lib.ss_initialize_msgStruct.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_char_p), ctypes.c_int]
        lib.ss_initialize_msgStruct.restype = ctypes.c_int

        lib.ss_printMSGcontents.argtypes = [ctypes.c_void_p]
        lib.ss_printMSGcontents.restype = ctypes.c_int

        lib.ss_printMSGcontentsTypes.argtypes = [ctypes.c_void_p]
        lib.ss_printMSGcontentsTypes.restype = ctypes.c_int

        lib.ss_printMSGString.argtypes = [ctypes.c_void_p]
        lib.ss_printMSGString.restype = ctypes.c_int

        lib.ss_set_clientName.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
        lib.ss_set_clientName.restype = ctypes.c_int

        lib.ss_updateMSG_char.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_char_p]
        lib.ss_updateMSG_char.restype = ctypes.c_int

        lib.ss_updateMSG_int.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_int]
        lib.ss_updateMSG_int.restype = ctypes.c_int

        lib.ss_updateMSG_double.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_double]
        lib.ss_updateMSG_double.restype = ctypes.c_int

        lib.ss_updateMSG_intArray.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.POINTER(ctypes.c_int)]
        lib.ss_updateMSG_intArray.restype = ctypes.c_int

        lib.ss_make_connection.argtypes = [ctypes.c_void_p]
        lib.ss_make_connection.restype = ctypes.c_int

        self.obj = lib.create_socketStream(svrIP.encode('utf-8'), svrPort, socketStreamMode)

    def initialize_socketStream(self):
        return lib.ss_initialize(self.obj)

    def make_connection(self):
        return lib.ss_make_connection(self.obj)

    def setBufferSize(self, buffersize):
        return lib.ss_setBufferSize(self.obj, buffersize)
    
    def initialize_msgStruct(self, fields):
        nbFields = len(fields)
        fields_bytes = []
        for i in range(nbFields):
            fields_bytes.append(bytes(fields[i], 'utf-8'))
        arr = (ctypes.c_char_p * (nbFields))()
        print(fields_bytes)
        arr[:] = fields_bytes
        return lib.ss_initialize_msgStruct(self.obj, arr, nbFields)

    def printMSGcontents(self):
        return lib.ss_printMSGcontents(self.obj)

    def printMSGcontentsTypes(self):
        return lib.ss_printMSGcontentsTypes(self.obj)

    def printMSGString(self):
        return lib.ss_printMSGString(self.obj)

    def set_clientName(self, clientName):
        return lib.ss_set_clientName(self.obj, clientName.encode('utf-8'))

    def updateMSG(self, field, value):
        if type(value) == str:
            print('test: ', value)
            return lib.ss_updateMSG_char(self.obj, field.encode('utf-8'), value.encode('utf-8'))
        if type(value) == int:
            return lib.ss_updateMSG_int(self.obj, field.encode('utf-8'), value)
        if type(value) == float:
            return lib.ss_updateMSG_double(self.obj, field.encode('utf-8'), value)
        if type(value) == np.ndarray:
            if len(value.shape) == 1:
                if value.dtype == np.int64 or value.dtype == np.int32 or value.dtype == np.int16 or value.dtype == np.int8:
                    value = value.astype(np.int32)
                    return lib.ss_updateMSG_intArray(self.obj, field.encode('utf-8'), value.ctypes.data_as(ctypes.POINTER(ctypes.c_int)), value.shape[0])

    def closeCommunication(self):
        lib.ss_closeCommunication(self.obj)


