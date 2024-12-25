import os
import time

fifo_path = 'serial_fifo'

# FIFO 파일 생성
if not os.path.exists(fifo_path):
    os.mkfifo(fifo_path)

def producer():
    with open(fifo_path, 'w') as fifo:
        for i in range(5):
            message = f"Message {i}"
            print(f"Producing: {message}")
            fifo.write(message + '\n')
            fifo.flush()  # 즉시 데이터를 전송하기 위해 flush
            time.sleep(1)  # 1초 대기

producer()
