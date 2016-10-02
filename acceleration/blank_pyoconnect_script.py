
''' ============================== '''
from PyoConnect import *
myo = Myo(sys.argv[1] if len(sys.argv) >= 2 else None) 
''' ============================== '''

import socket

hote = "192.168.101.103"
port = 5001

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((hote, port))
print "Connection on {}".format(port)

def onUnlock():
	myo.unlock("hold")

def onPoseEdge(pose, edge):
	print("onPoseEdge: "+pose+", "+edge)
	if(pose == "fist" ):
		if(edge == "on"):
			sock.send("1")


		if(edge == "off"):
			sock.send("0")

	if(pose == "waveOut" ):
		if(edge == "on"):
			sock.send("-1")

		if(edge == "off"):
			sock.send("0")




myo.onUnlock = onUnlock
myo.onPoseEdge = onPoseEdge

myo.connect()
while True:
	myo.run()
	myo.tick()

