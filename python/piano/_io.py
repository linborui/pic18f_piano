import serial

def get_port():
    # Get the serial port name from the user
    port = input('Enter the serial port name: ')
    return port

def get_baudrate():
    # Get the baudrate from the user
    baudrate = input('Enter the baudrate: ')
    return baudrate

class Serial:
    def __init__(self, port=None, baudrate=None, timeout=None):
        if not port:
            port = 'com3'
        if not baudrate:
            baudrate = 9600
        if not timeout:
            timeout = 1
        self.ser = serial.Serial(port, baudrate=baudrate, timeout=timeout)
        self.ser.flushInput()

    def write(self, data):
        self.ser.write(data.encode())

    def read(self):
        return self.ser.readline().decode('utf-8').rstrip()

    def close(self):
        self.ser.close()
