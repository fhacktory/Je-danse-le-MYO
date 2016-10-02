#a exécuter sur le raspberry pi
/opt/vc/bin/raspivid -t 0 -w 300 -h 300 -hf -fps 20 -o - | nc 192.168.101.103 2222

