from curses import flushinp
from build.module_name import *

list = [0] * 41
print(list)


def insert_in_list(inPlace, player):
    print("player: " + str(player) + ", inPlace: " + str(inPlace))
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
        # flushinp()
        # place = float(input("Place token in column: "))
        client.setColumn(50)
        client.sendAction(0)
    elif type == 2:
        print("Wait for opponent")
    elif type == 3:
        insert_in_list(client.getInPlace(), client.getPlayer())
    elif type == 4:
        print("somebody won")
    else:
        print("type error")
