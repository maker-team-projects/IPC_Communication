import os

fifo_path = 'FIFO_File/serial_fifo'

def consumer():
    if not os.path.exists(fifo_path):
        os.mkfifo(fifo_path)

    with open(fifo_path, 'r') as fifo:
        while True:
            message = fifo.readline().strip()
            if message:
                print(f"Received: {message}")

consumer()
