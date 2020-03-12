#!/usr/bin/env python

from threading import Event

class Future(object):
    def __init__(self):
        self._ev = Event()

    def set_result(self, result):
        self._result = result
        self._ev.set()

    def set_exception(self, exc):
        self._exc = exc
        self._ev.set()

    def result(self):
        self._ev.wait()
        if hasattr(self, '_exc'):
            raise self._exc
        return self._result