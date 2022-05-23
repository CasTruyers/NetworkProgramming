from build.module_name import *

print(PyConnectFourClient)

client = PyConnectFourClient()
client.join()
client.waitForOpponent()

while client.handleNetworkEvent():
    pass
