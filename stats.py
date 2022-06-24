import zmq

counter1 = 0
counter2 = 0
counter3 = 0
counter4 = 0

context = zmq.Context()
socket = context.socket(zmq.SUB)
socket.connect("tcp://benternet.pxl-ea-ict.be:24042")
socket.setsockopt_string(zmq.SUBSCRIBE, "connectFourStat>", encoding='utf-8')

while True:
    message = socket.recv()
    print(f"received message: {message}")

    print("p1 wint zonder hold te gebruiken: ", (int(message[16]) - 48))
    print("p2 wint zonder hold te gebruiken: ", (int(message[18]) - 48))
    print("p1 wint net na hold te gebruiken: ", (int(message[20]) - 48))
    print("p2 wint net na hold te gebruiken: ", (int(message[22]) - 48))
