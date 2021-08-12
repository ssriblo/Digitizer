import serial
ser = serial.Serial()
ser.baudrate = 115200
ser.port = 'COM11'
print(ser)
ser.open()

# while True:
#     bytesToRead = ser.inWaiting()
#     if bytesToRead > 0 :
#         array = ser.read(bytesToRead)
#         print(f'bytesToRead {bytesToRead} \n')
#         for byte in array:
#             print(byte),
#             pass

while True:
    arr = ser.read(10)
    if arr != None:
        print(f'LEN={len(arr)}')
        bstr = bytearray(b'')
        for byte in arr:
            bstr.append(byte)
        print(bstr)


