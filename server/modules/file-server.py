import os
import socket
from datetime import datetime

SERVER_IP = '0.0.0.0'  # Listen on all available IP addresses
SERVER_PORT = 9030
BUFFER_SIZE = 1024

def main():
    # Create the "transfers" directory if it doesn't exist
    if not os.path.exists('transfers'):
        os.makedirs('transfers')

    # Create a socket object
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Bind the socket to all available IP addresses and the specified port
    server_socket.bind((SERVER_IP, SERVER_PORT))

    # Listen for incoming connections
    server_socket.listen(1)

    print(f"Server listening on {SERVER_IP}:{SERVER_PORT}")

    current_time = datetime.now().strftime('%Y-%m-%d_%H-%M-%S')

    while True:
        # Accept a client connection
        client_socket, address = server_socket.accept()
        print(f"Accepted connection from {address[0]}:{address[1]}")

        # Receive the file data
        file_path = os.path.join('transfers', address[0]+"-"+current_time)
        with open(file_path, 'wb') as file:
            while True:
                data = client_socket.recv(BUFFER_SIZE)
                if not data:
                    break
                file.write(data)

        print("File received successfully.")

        # Close the client socket
        client_socket.close()

if __name__ == '__main__':
    main()
