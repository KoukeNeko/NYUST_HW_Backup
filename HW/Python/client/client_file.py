import socket
import sys
from rich.console import Console

s = socket.socket()
Console().print("請輸入接收者IP[dim] (不輸入則使用預設localhost)[/]")
IP = str(input()) #選擇傳送IP
if len(IP) <= 0: #使用預設
    IP = "localhost"
s.connect((IP, 5000))
if len(sys.argv) <= 1:
    print("參數錯誤，請輸入檔案路徑")
    exit(1)
filename = (sys.argv[1].split('.')[-2]+"_接收"+"." +sys.argv[1].split('.')[-1]).split('\\')[1]
#由範例檔案 .\2022台灣燈會在高雄 愛河打造國際級智慧控制燈光系統.mp4 切出檔案名稱和副檔名
s.send(str(filename).encode('utf-8'))  #告知伺服器要開啟的檔案名稱
file = open(sys.argv[1], "rb") #客戶端讀取檔案
data = file.read(1024) #傳送
print("傳送中...")
while data:
    Console().print("[dim].[/]", end="")
    s.send(data)
    data = file.read(1024)
file.close() #關檔案
s.send(b"DONE") #告知伺服器傳送完畢
Console().print("[blink][完成傳送][/]")
print(s.recv(1024).decode('utf-8')) #伺服器回傳完成關檔案
s.shutdown(2)
s.close()