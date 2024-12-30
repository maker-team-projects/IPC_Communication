import socket

HOST = '192.168.1.177'  # 아두이노의 IP 주소
PORT = 80

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    s.sendall(b'Hello from Python!\n')
    data = s.recv(1024)

print('Received', repr(data))
