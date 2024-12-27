import mmap
import posix_ipc
import atexit

def Exit():
    print("Program Exit")
    mm.close()
    shm.close_fd()

shm_name = "/my_shared_memory"
SIZE = 1024

# 공유 메모리 열기
shm = posix_ipc.SharedMemory(shm_name)
mm = mmap.mmap(shm.fd, SIZE)

atexit.register(Exit)

while True:
    # 공유 메모리에서 데이터 읽기
    mm.seek(0)
    data = mm.read(SIZE)
    print(data.decode('utf-8'))

