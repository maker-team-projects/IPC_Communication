import time, os
import sys, select, termios, tty

settings = termios.tcgetattr(sys.stdin)

up_input = {
    'q': 0,
    'w': 1,
    'e': 2,
    'r': 3,
    't': 4,
}
down_input = {
    'a': 0,
    's': 1,
    'd': 2,
    'f': 3,
    'g': 4,
}
feedback_fifo_path = 'FIFO_File/keyboard_feedback_fifo'
keyboard_fifo_path = 'FIFO_File/keyboard_fifo'

def get_key(settings):
    if os.name == 'nt':
        return msvcrt.getch().decode('utf-8')
    tty.setraw(sys.stdin.fileno())
    rlist, _, _ = select.select([sys.stdin], [], [], 0.1)
    if rlist:
        key = sys.stdin.read(1)
    else:
        key = ''

    termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)
    return key

if not os.path.exists(feedback_fifo_path):
    os.mkfifo(feedback_fifo_path)
if not os.path.exists(keyboard_fifo_path):
    os.mkfifo(keyboard_fifo_path)

speed = 1
with open(keyboard_fifo_path, 'w') as keyboard_fifo:
    with open(feedback_fifo_path, 'r') as feedback_fifo:
        while True:
            key = get_key(settings)
            message = feedback_fifo.readline().strip()
            if message:
                int_array = list(map(int, message.strip().split(",")))
                if key in up_input.keys():
                    if int_array[up_input[key]] + speed >= 180:
                        int_array[up_input[key]] = 180
                    else:
                        int_array[up_input[key]] += speed
                    print(int_array)
                elif key in down_input.keys():
                    if int_array[down_input[key]] - speed <= 0:
                        int_array[down_input[key]] = 0
                    else:
                        int_array[down_input[key]] -= speed
                    print(int_array)
                elif key == 'o':
                    speed += 1
                    print("Speed:", speed)
                elif key == 'l':
                    speed -= 1
                    print("Speed:", speed)
                elif key == 'p':
                    break
                keyboard_fifo.write(",".join(list(map(str, int_array)))+"\n")
                keyboard_fifo.flush()            
    