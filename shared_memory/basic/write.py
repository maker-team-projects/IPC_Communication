# shared_memory_writer.py
import mmap
import posix_ipc

shm_name = "/my_shared_memory"
SIZE = 1024

# 공유 메모리 생성
shm = posix_ipc.SharedMemory(shm_name, posix_ipc.O_CREAT, size=SIZE)
mm = mmap.mmap(shm.fd, SIZE)

# 공유 메모리에 데이터 쓰기
message = "Hello from Python!"
mm.write(message.encode('utf-8'))

# 공유 메모리 닫기
mm.close()
shm.close_fd()
