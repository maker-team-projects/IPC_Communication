# shared_memory_reader.py
import mmap
import posix_ipc

shm_name = "/my_shared_memory"
SIZE = 1024

# 공유 메모리 열기
shm = posix_ipc.SharedMemory(shm_name)
mm = mmap.mmap(shm.fd, SIZE)

# 공유 메모리에서 데이터 읽기
mm.seek(0)
data = mm.read(SIZE)
print("Data read from shared memory:", data.decode('utf-8'))

# 공유 메모리 닫기
mm.close()
shm.close_fd()
