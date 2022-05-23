from build.module_name import *

list = [0] * 41
print(list)


def insert_in_list(inPlace, player):
    if player:
        list[inPlace - 1] = 2
    else:
        list[inPlace - 1] = 1


client = PyConnectFourClient()
client.join()
client.waitForOpponent()

while True:
    type = client.handleNetworkEvent()
    if type == 0:
        break
    elif type == 1:
        place = int(input("Place token in column: "))
        client.setColumn(place)
        client.sendAction(0)
    elif type == 2:
        print("Wait for opponent")
    elif type == 3:
        insert_in_list(client.inPlace)
    elif type == 4:
        print("somebody won")
    else:
        print("type error")
