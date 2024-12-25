# shared_memory_writer.py
import mmap
import posix_ipc
import atexit

def Exit():
    print("Program Exit")
    mm.close()
    shm.close_fd()

shm_name = "/my_shared_memory"
SIZE = 1024
loop = 0

# 공유 메모리 생성
shm = posix_ipc.SharedMemory(shm_name, posix_ipc.O_CREAT, size=SIZE)
mm = mmap.mmap(shm.fd, SIZE)

atexit.register(Exit)

while True:
    # 공유 메모리에 데이터 쓰기
    message = "[" + str(loop) + "] Hello from Python!"
    if len(message) >= SIZE: 
        raise ValueError("Data out of range: message is too large for the shared memory size")
    
    mm.seek(0)
    mm.write(b'\x00' * SIZE)
    mm.write(message.encode('utf-8'))
    loop+=1
