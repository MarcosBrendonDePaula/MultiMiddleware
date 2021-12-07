import binascii
import struct

class Buffer:
    def __init__(self, str_ = "", maxSize = 1500):
        self.Data         = str_
        self.ActualSize   = len(str_)

    def __str__(self):
        return str.encode(self.str_)

    def setData(self,newData=[]):
        self.ActualSize = len(newData)
        self.Data       = newData
        pass

    def __index__(self, index):
        return self.Data[index]