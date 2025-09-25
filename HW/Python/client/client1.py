import socket
import threading


nickname = str(input("輸入你的暱稱: "))

# 連線至伺服器
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(('127.0.0.1', 55555))

# 偵聽伺服器的廣播
def receive():
    while True:
        try:
            # 從伺服器接收訊息
            # If 'NICK' Send Nickname
            message = client.recv(1024).decode('utf-8')
            if message == 'NICK':
                client.send(nickname.encode('utf-8'))
            else:
                print(message)
        except:
            # 當出錯時關閉client
            print("出現錯誤!")
            client.close()
            break

# 傳送至伺服器
def write():
    while True:
        message = '{}: {}'.format(nickname, input(''))
        print(nickname + " > ",end="")
        client.send(message.encode('utf-8'))

# 偵聽和送出訊息的線程
receive_thread = threading.Thread(target=receive)
receive_thread.start()

write_thread = threading.Thread(target=write)
write_thread.start()             