#!/usr/bin/env python
"""
    Copyright (C) 2020 Iason Batzianoulis
    
	Developer: Iason Batzianoulis
	Maintaner: Iason Batzianoulis
	email: iasonbatz@gmail.com
	Description: 
	Python interface of socketStream
     a class that handles 

    License:    GNU GPLv3
"""

import sys
import ctypes
import numpy as np
import json
import time


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

        lib.ss_updateMSG_intArray.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.POINTER(ctypes.c_int), ctypes.c_int]
        lib.ss_updateMSG_intArray.restype = ctypes.c_int

        lib.ss_updateMSG_doubleArray.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.POINTER(ctypes.c_double), ctypes.c_int]
        lib.ss_updateMSG_doubleArray.restype = ctypes.c_int

        lib.ss_updateMSG_matInt.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.POINTER(ctypes.c_int), ctypes.c_int, ctypes.c_int]
        lib.ss_updateMSG_matInt.restype = ctypes.c_int

        lib.ss_updateMSG_matDouble.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.POINTER(ctypes.c_double), ctypes.c_int, ctypes.c_int]
        lib.ss_updateMSG_matDouble.restype = ctypes.c_int

        lib.ss_sendMsg.argtypes = [ctypes.c_void_p]
        lib.ss_sendMsg.restype = ctypes.c_int

        lib.ss_sendMSg2Client_int.argtypes = [ctypes.c_void_p, ctypes.c_int]
        lib.ss_sendMSg2Client_int.restype = ctypes.c_int

        lib.ss_sendMSg2Client_str.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
        lib.ss_sendMSg2Client_str.restype = ctypes.c_int

        lib.ss_sendMSg2All.argtypes = [ctypes.c_void_p]
        lib.ss_sendMSg2All.restype = ctypes.c_int

        lib.ss_make_connection.argtypes = [ctypes.c_void_p]
        lib.ss_make_connection.restype = ctypes.c_int

        lib.ss_setHashKey.argtypes = [ctypes.c_void_p, ctypes.c_bool]
        lib.ss_setHashKey.restype = ctypes.c_int

        lib.ss_setVerbose.argtypes = [ctypes.c_void_p, ctypes.c_bool]
        lib.ss_setVerbose.restype = ctypes.c_int

        lib.ss_setHeaderSize.argtypes = [ctypes.c_void_p, ctypes.c_int]
        lib.ss_setHeaderSize.restype = ctypes.c_int

        lib.ss_runServer.argtypes = [ctypes.c_void_p]
        lib.ss_runServer.restype = ctypes.c_int

        lib.ss_socketStream_ok.argtypes = [ctypes.c_void_p]
        lib.ss_socketStream_ok.restype = ctypes.c_bool

        lib.ss_getFullmsg.argtypes = [ctypes.c_void_p]
        lib.ss_getFullmsg.restype = ctypes.c_char_p

        lib.ss_get_latest.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_bool)]
        lib.ss_get_latest.restype = ctypes.c_char_p

        lib.ss_get_latest_fromClient.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.POINTER(ctypes.c_bool)]
        lib.ss_get_latest_fromClient.restype = ctypes.c_char_p

        self.obj = lib.create_socketStream(svrIP.encode('utf-8'), svrPort, socketStreamMode)

    def initialize_socketStream(self):
        return lib.ss_initialize(self.obj)

    def make_connection(self):
        tt = lib.ss_make_connection(self.obj)
        time.sleep(0.001)
        return tt

    def setBufferSize(self, buffersize):
        return lib.ss_setBufferSize(self.obj, buffersize)
    
    def initialize_msgStruct(self, fields):
        nbFields = len(fields)
        fields_bytes = []
        for i in range(nbFields):
            fields_bytes.append(bytes(fields[i], 'utf-8'))
        arr = (ctypes.c_char_p * (nbFields))()
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
            return lib.ss_updateMSG_char(self.obj, field.encode('utf-8'), value.encode('utf-8'))
        if type(value) == int:
            return lib.ss_updateMSG_int(self.obj, field.encode('utf-8'), value)
        if type(value) == float:
            return lib.ss_updateMSG_double(self.obj, field.encode('utf-8'), value)
        if type(value) == np.ndarray:
            if len(value.shape) == 1:
                if value.dtype == np.int64 or value.dtype == np.int32 or value.dtype == np.int16 or value.dtype == np.int8:
                    value = value.astype(np.int32)
                    # print(value)
                    return lib.ss_updateMSG_intArray(self.obj, field.encode('utf-8'), value.ctypes.data_as(ctypes.POINTER(ctypes.c_int)), value.shape[0])
                else:
                    if value.dtype == np.double or value.dtype == np.float or value.dtype == np.float32:
                        value = value.astype(np.double)
                        return lib.ss_updateMSG_doubleArray(self.obj, field.encode('utf-8'), value.ctypes.data_as(ctypes.POINTER(ctypes.c_double)), value.shape[0])
                    else:
                        print('[socketStream_py] Unknown data type for array')
                        return -4
            else:
                if len(value.shape) == 2:
                    rows, columns = value.shape
                    if value.dtype == np.int64 or value.dtype == np.int32 or value.dtype == np.int16 or value.dtype == np.int8:
                        value = value.astype(np.int32)
                        value = value.reshape((1, np.prod(value.shape)))
                        return lib.ss_updateMSG_matInt(self.obj, field.encode('utf-8'), value.ctypes.data_as(ctypes.POINTER(ctypes.c_int)), rows, columns)
                    else:
                        if value.dtype == np.double or value.dtype == np.float or value.dtype == np.float32:
                            value = value.astype(np.double)
                            value = value.reshape((1, np.prod(value.shape)))
                            return lib.ss_updateMSG_matDouble(self.obj, field.encode('utf-8'), value.ctypes.data_as(ctypes.POINTER(ctypes.c_double)), rows, columns)
                        else:
                            print('[socketStream_py] Unknown data type for array')
                        return -4
                else:
                    print('[socketStream_py] socketStream does not currently support more than 2D matrices.')
                    return -5

    def sendMsg(self):
        return lib.ss_sendMsg(self.obj)
    
    def sendMSg2Client(self, clID):
        if type(clID) == str:
            return lib.ss_sendMSg2Client_str(self.obj, clID)
        if type(clID) == int:
            return lib.ss_sendMSg2Client_int(self.obj, clID)
        return -3
    
    def sendMSg2All(self):
        return lib.ss_sendMSg2All(self.obj)

    def closeCommunication(self):
        lib.ss_closeCommunication(self.obj)

    def setHashKey(self, value):
        value = bool(value)
        return lib.ss_setHashKey(self.obj, value)

    def setVerbose(self, value):
        return lib.ss_setVerbose(self.obj, value)
    
    def setHeaderSize(self, hSize):
        return lib.ss_setHeaderSize(self.obj, hSize)

    def runServer(self):
        return lib.ss_runServer(self.obj)

    def socketStream_ok(self):
        return lib.ss_socketStream_ok(self.obj)

    def getFullmsg(self):
        tt = lib.ss_getFullmsg(self.obj)
        return ctypes.c_char_p(tt).value.decode('utf-8')

    def get_latest(self, cltName=None):
        time.sleep(0.1)
        cflag = ctypes.c_bool()
        if cltName is None:
            tt = lib.ss_get_latest(self.obj, ctypes.byref(cflag))
        else:
            tt = lib.ss_get_latest_fromClient(self.obj, cltName,  ctypes.byref(cflag))
        isNew = cflag.value

        try:
            msg = ctypes.c_char_p(tt).value.decode('utf-8')

        except UnicodeDecodeError:
            print(tt)
            isNew = False

        msg = ctypes.c_char_p(tt).value.decode('utf-8')
        if isNew:
            jsonMSG = json.loads(msg)
            return jsonMSG
        else:
            return None
        