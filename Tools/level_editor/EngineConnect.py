import socket

class EngineConnect:
    def __init__(self, host='localhost', port=2121):
        self.host = host
        self.port = port
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def connect(self):
        self.socket.connect((self.host, self.port))

    def disconnect(self):
        self.socket.close()

    def send_order(self, order : str):
        if not isinstance(order, str):
            raise ValueError("Order must be a string")
        
        self.socket.sendall(order.encode('utf-8'))

    def receive_response(self):
        return self.socket.recv(1024).decode('utf-8')