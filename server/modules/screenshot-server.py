import os
import socket
from datetime import datetime

# Create the 'screenshots' folder if it doesn't exist
if not os.path.exists('screenshots'):
    os.makedirs('screenshots')

# Set up the server IP address and port
SERVER_ADDRESS = '192.168.0.20'
SERVER_PORT = 9040

# Create a socket object
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind the socket to the server address and port
server_socket.bind((SERVER_ADDRESS, SERVER_PORT))

# Listen for incoming connections
server_socket.listen(1)
print('Server listening on {}:{}'.format(SERVER_ADDRESS, SERVER_PORT))

# Accept a client connection
client_socket, client_address = server_socket.accept()
print('Accepted connection from {}:{}'.format(client_address[0], client_address[1]))

# Receive and store the screenshot file
current_time = datetime.now().strftime('%Y-%m-%d_%H-%M-%S')
screenshot_filename = '{}-{}.bmp'.format(client_address[0], current_time)
screenshot_path = os.path.join('screenshots', screenshot_filename)
with open(screenshot_path, 'wb') as file:
    while True:
        data = client_socket.recv(4096)
        if not data:
            break
        file.write(data)

print('Screenshot received and saved as {}'.format(screenshot_path))

# Close the client socket and server socket
client_socket.close()
server_socket.close()
