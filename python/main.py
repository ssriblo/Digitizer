import serial
import time
import sys
from cobs import cobs

PRINTLOG = 0

def printLog(a):
    if( PRINTLOG > 0):
        print(a)

def main(arg):
    global PRINTLOG
    if(len(arg) > 1):
        # arg[1] printLog flag
        print(">>>-1")
        PRINTLOG = int(arg[1])
        print(PRINTLOG)

    ser = serial.Serial()
    ser.baudrate = 115200*8
    ser.port = "/dev/ttyUSB0" #'COM12'
#    ser.set_buffer_size(rx_size = 12800, tx_size = 12800)
    print(ser)
    ser.open()

    buf = bytearray()
    count = 0 #-1 # if count<0 then non-stop, if count=0 stop after NUMBER reading
    NUMBER = 6
    print("***********************************************************************")
    while True:
        chunk = ser.read(66)
        if chunk != None:
            if(count >=0): 
                count = count + 1
            printLog(f'************ COUNT={count}')
            if(count > 5):
                return
            buf.extend(chunk)
            bytes_to_drop = handle_buf(buf)
            del buf[0:bytes_to_drop]  
        
def handle_buf(buf):
    byte_proc = 0
    zero_index = buf.index(0)
    printLog(f'zero index={zero_index}')
    printLog(f'handle_buff(): \n{buf.hex()}')
    if(len(buf) > zero_index + 65):
        cobs_buff = buf[zero_index + 1:zero_index+66]
        printLog(f'COBS BUFF: \n{cobs_buff.hex()}')
        decoded = cobs.decode(cobs_buff)
        printLog(f'DECODED: \n{decoded.hex()}')
        return zero_index + 66
    else:
        return 0
    
if __name__ == '__main__':
    main(sys.argv)    