import os, time

r, w = os.pipe()

read_pipe = os.fdopen(r, 'r')
write_pipe = os.fdopen(w, 'w')

def producer():
    for i in range(5):
        message = f"Message {i}"
        print(f"Producing: {message}")
        write_pipe.write(message + '\n')
        write_pipe.flush()
        time.sleep(1)

producer()