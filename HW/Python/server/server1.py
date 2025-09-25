import socket
import threading

# 連線資料
host = '127.0.0.1'
port = 55555

# 開始伺服器
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((host, port))
server.listen()

# 存放顯示名稱和client物件的list
clients = []
nicknames = []

# 廣播消息
def broadcast(message , current_client = None):
    for client in clients:
        if client != current_client: #不會傳送傳送者的訊息給傳送者
            client.send(message)

# 接收來自client的訊息
def handle(client):
    while True:
        try:
            # 廣播消息給所有client
            message = client.recv(1024)
            broadcast(message, client)
        except:
            # 中斷連線的處理
            index = clients.index(client)
            clients.remove(client)
            client.close()
            nickname = nicknames[index]
            broadcast('{} 離開了聊天!'.format(nickname).encode('utf-8'))
            nicknames.remove(nickname)
            break

# 接收來自client的訊息
def receive():
    while True:
        # 接受連線
        client, address = server.accept()
        print("偵測到連線 {}".format(str(address)))

        # 接受和儲存暱稱
        client.send('NICK'.encode('utf-8'))
        nickname = client.recv(1024).decode('utf-8')
        nicknames.append(nickname)
        clients.append(client)

        # MOTD
        print("暱稱是 {}".format(nickname))
        broadcast("{} 已加入!".format(nickname).encode('utf-8'))
        client.send('已連線至伺服器!按Enter繼續'.encode('utf-8'))

        # 開始線程
        thread = threading.Thread(target=handle, args=(client,))
        thread.start() 
receive()                       