import socket

s = socket.socket()
s.bind(("", 5000))
s.listen(1)
c,a = s.accept()
filename = c.recv(1024).decode('utf-8')
filetodown = open(filename , "wb")
print("正在接收....")
while True:
   print(".")
   data = c.recv(1024)
   if data == b"DONE":
           print("完成接收.")
           break
   filetodown.write(data)
filetodown.close()
c.send(str("正在中斷連線").encode('utf-8'))
c.shutdown(2)
c.close()
s.close()