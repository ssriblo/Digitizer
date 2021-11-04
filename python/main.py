import serial
import time
import sys
from cobs import cobs
import matplotlib.pyplot as plt
import array
import numpy as np
#import notch
from filter import lp

PRINTLOG = False
PRINTSOURCE = False
NUMBER =1_000 #10000
data = array.array('I')

def printLog(a):
    if( PRINTLOG ):
        print(a)

def main(arg):
    global PRINTLOG
    global PRINTSOURCE
    global NUMBER
    if(len(arg)==1):
        print("try format for run:")
        print("python3 main.py [<NUMBER=1000>] [<PRINTSOURCE=False>] [<PRINTLOG=False>]")
    if(len(arg) > 1):
        NUMBER = int(arg[1])
        if(len(arg)>2):
            PRINTSOURCE = bool(arg[2])
            if(len(arg)>3):
                PRINTLOG = bool(arg[3])

    ser = serial.Serial()
    ser.baudrate = 115200*8
    ser.port = "/dev/ttyUSB0" #'COM12'
#    ser.set_buffer_size(rx_size = 12800, tx_size = 12800)
    print(ser)
    ser.open()

    buf = bytearray()
    count = 0 #-1 # if count<0 then non-stop, if count=0 stop after NUMBER readingconda
    print("***********************************************************************")
    time_1 = time.time()
    print(time_1)
    print("***********************************************************************")

    while True:
        chunk = ser.read(66)
        if chunk != None:
            if(count >=0): 
                count = count + 1
            print(f'\r         COUNT={count}', end="", flush=True)
            if(count > NUMBER):
                x = np.arange(0, data.buffer_info()[1], 1)
                size = data.buffer_info()[1]
                print(f'\nSIZE={size}')
#                plt.plot(x,data)
#                plt.plot(data)               
                time_2 = time.time()
                time_interval = time_2 - time_1
                print(time_interval)
                sample_us = 16*2
                lp(data, sample_us, size, PRINTSOURCE)
                plt.show()
                return
            buf.extend(chunk)
            bytes_to_drop = handle_buf(buf)
            del buf[0:bytes_to_drop]  
        
def handle_buf(buf):
    byte_proc = 0
    zero_index = buf.index(0)
    printLog(f'zero index={zero_index}')
    printLog(f'handle_buff(): \n{buf.hex()}')
    odd = 1
    if(len(buf) > zero_index + 65):
        cobs_buff = buf[zero_index + 1:zero_index+66]
        printLog(f'COBS BUFF: \n{cobs_buff.hex()}')
        decoded = cobs.decode(cobs_buff)
        printLog(type(decoded))
        printLog(f'DECODED: \n{decoded.hex()}')
        for byte in decoded:
#            print(f'byte={byte} odd={odd}')
            if(odd == 1):
                byte_msb = byte
                odd = 0
            else:
                word = byte_msb + byte*256
                odd = 1
#                print(f'\t\tword={word}')
                data.append(word)
            

        return zero_index + 66
    else:
        return 0
    
if __name__ == '__main__':
    main(sys.argv)    