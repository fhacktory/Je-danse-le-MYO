## GoPro Streaming script from Sonof8Bits for HERO4 Session running 2.00 Firmware

import sys
import socket
import urllib
import subprocess
import ffmpy
from time import sleep

def get_command_msg(id):
	return "_GPHD_:%u:%u:%d:%1lf\n" % (0, 0, 2, 0)
	
UDP_IP = "10.5.5.9"
UDP_PORT = 8554
KEEP_ALIVE_PERIOD = 2500
KEEP_ALIVE_CMD = 2
MESSAGE = get_command_msg(KEEP_ALIVE_CMD)

##
## HTTP GETs the URL that tells the GoPro to start streaming.
##
urllib.urlopen("http://10.5.5.9/gp/gpControl/execute?p1=gpStream&a1=proto_v2&c1=restart").read()

for i in range(5):
	urllib.urlopen("http://10.5.5.9/gp/gpControl/status").read()

print("UDP target IP:", UDP_IP)
print("UDP target port:", UDP_PORT)
print("message:", MESSAGE)
print("Press ctrl+C to quit this application.\n")

##
## Opens the stream over udp in ffplay. This is a known working configuration by Reddit user hoppjerka:
## https://www.reddit.com/r/gopro/comments/2md8hm/how_to_livestream_from_a_gopro_hero4/cr1b193
##
subprocess.Popen("ffplay -fflags nobuffer -f:v mpegts -probesize 8192 udp://:8554", shell=True)

if sys.version_info.major >= 3:
	MESSAGE = bytes(MESSAGE, "utf-8")

while True:
	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))
	sleep(KEEP_ALIVE_PERIOD/1000)
