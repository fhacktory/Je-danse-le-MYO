#a exécuter sur rpi

from BrickPi import *
import socket
import select

vitesse=200
acc=1
motArrG=PORT_A
motArrD=PORT_C
motAvG=PORT_B
motAvD=PORT_D
sock = None
client_acc = None
client_dir = None

def connectToLeap():
        global sock, client_dir

	print("En attente de connexion du Leap Motion...")
	while 1:
                try:
                        client_dir, address_dir = sock.accept()
                        break
                except:
                        None
	print("leap ok")


def connectToMyo():
        global sock, client_acc
	print("En attente de connexion du Myo...")
	while 1:
                try:
                        client_acc, address_acc = sock.accept()
                        client_acc.setblocking(0)
	                break
                except:
                        None
	print("myo ok")




	
def main():
	global vitesse, acc, sock, client_dir, client_acc
	BrickPiSetup()

	BrickPi.MotorEnable[motArrG] = 1
	BrickPi.MotorEnable[motArrD] = 1
	BrickPi.MotorEnable[motAvG] = 1
	BrickPi.MotorEnable[motAvD] = 1

	BrickPi.SensorType[PORT_4] = TYPE_SENSOR_ULTRASONIC_CONT
	BrickPi.SensorType[PORT_2] = TYPE_SENSOR_ULTRASONIC_CONT
	
	BrickPiSetupSensors()

	BrickPi.Timeout=15000
	BrickPiSetTimeout()


	
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.setblocking(0)
        
        sock.bind(('', 5001))
        sock.listen(2)


	while True:
                if client_acc is None:
                        connectToMyo()
                if client_dir is None:
                        connectToLeap()

                resp_dir = ""
                resp_acc = ""
                try:
                        resp_acc = client_acc.recv(255)
                        acc = int(resp_acc)
                except:
                        None
                try:
                        resp_dir = client_dir.recv(255)
                except:
                        None

                inp = 0
		if resp_dir.strip() != "":
                        
                        try:
                                inp = map(float, resp_dir.strip().split('\r\n'))[0]

                        except :
                                resp_dir = None
			
			print "val = {}".format(inp)
                        if resp_dir != None and inp<0:
				if abs(inp) < 0.95:
                                	BrickPi.MotorSpeed[motAvG] = acc*int(vitesse*(1.0-abs(inp)))
					BrickPi.MotorSpeed[motAvD] = acc*int(vitesse)
					BrickPi.MotorSpeed[motArrG] = -acc*int(vitesse*(1-abs(inp)))
				else:
					BrickPi.MotorSpeed[motAvG] = acc*int(vitesse*(0.7-abs(inp)))
					BrickPi.MotorSpeed[motAvD] = acc*int(0.75*vitesse)
	                                BrickPi.MotorSpeed[motArrG] = -acc*int(vitesse*(1.25-abs(inp)))
                                BrickPi.MotorSpeed[motArrD] = -acc*int(vitesse)
                        elif resp_dir != None:
				if abs(inp) < 0.9:
                                	BrickPi.MotorSpeed[motAvD] = acc*int(vitesse*(1.0-abs(inp)))
					BrickPi.MotorSpeed[motAvG] = acc*int(vitesse)
					BrickPi.MotorSpeed[motArrD] = -acc*int(vitesse*(1.0-abs(inp)))
				else:
					BrickPi.MotorSpeed[motAvD] = acc*int(vitesse*(0.7-abs(inp)))
					BrickPi.MotorSpeed[motAvG] = acc*int(0.75*vitesse)
                                BrickPi.MotorSpeed[motArrG] = -acc*int(vitesse)
                                BrickPi.MotorSpeed[motArrD] = -acc*int(vitesse*(1.25-abs(inp)))
		distActuelleG = BrickPi.Sensor[PORT_4]
		print "Capteur gauche {}".format(distActuelleG)
		distActuelleD = BrickPi.Sensor[PORT_2]
		print "Capteur droite {}".format(distActuelleD)

                                    
                BrickPiUpdateValues() 
		time.sleep(.015)

if __name__ == "__main__":
    main()

