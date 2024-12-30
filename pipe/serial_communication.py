import serial
import time
import os

fifo_path = 'FIFO_File/serial_fifo'

if not os.path.exists(fifo_path):
    os.mkfifo(fifo_path)

ser = serial.Serial('/dev/ttyUSB0', 115200)
time.sleep(2)

def send_int_array(data):
    ser.write((data + '\n').encode())
    
with open(fifo_path, 'r') as fifo:
    while True:
        message = fifo.readline().strip()
        if message:
            send_int_array(message)
ser.close()