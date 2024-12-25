import os

r, w = os.pipe()

read_pipe = os.fdopen(r, 'r')
write_pipe = os.fdopen(w, 'w')

def consumer():
    while True:
        message = read_pipe.readline().strip()
        if message:
            print(f"Consuming: {message}")

consumer()