import datetime, time, socket

my_address = '192.168.1.77'
path_to_log = r"C:\Users\thoma_000\Desktop\Desktop Items\RFID_Project\log.txt"
device_Addresses = {1:"192.168.1.231"}
device_Ports = {1:8888}

# Proper format: *ACDD...DSS<cr>, where:
#   - * -> starts the message
#   - A -> Integer address 1-9
#   - C -> Command, must be a valid uppercase letter accepted by commands
#   - DD...D -> Any data to be passed on
#   - SS -> Checksum
#   - <cr> -> Carriage return, in the form "\n"
#
# Possible Commands:
#   - L : Logs data
#   - l : Returns Log
#

def create_String(arduino, command, data):
    return bytes(("*%s%s%s%s" % (arduino, command.upper(), data, "ss")), encoding='utf8')

def write_To_Log(data):
    with open(path_to_log, 'a') as f:
        f.write(data + '\n')
        f.close()

def sendMessage(string, destination, receive=True):
    sock = socket.socket(socket.AF_INET,  # Internet
                         socket.SOCK_DGRAM)  # UDP
    sock.sendto(string, (device_Addresses.get(destination), device_Ports.get(destination)))
    data, addr = sock.recvfrom(1024)
    if receive:
        return data,addr

print('starting')
#sendMessage(create_String(1, "D", "AZDARREEEEEEECFDFSDFDS"), 1, False)
print('request sent')
while True:
    time.sleep(1)
    data, addr = sendMessage(create_String(1, "Z", "AZDARREEEEEEECFDFSDFDS"), 1, True)
    write_To_Log("FROM DEVICE 1: %s|" % datetime.datetime.now() + str(data))
    print("received message:", str(data))

