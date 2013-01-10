pihat
=====
This is cloned from original pihat http://www.skagmo.com/page.php?p=projects/22_pihat

Status support is added according to http://www.picaxeforum.co.uk/showthread.php?16509-El-cheapo-mains-remote&p=147355&viewfull=1#post147355

to run it download pihat to your pi and run

chmod +x ./pihat

To pair
./pihat --repeats=50 --id=0 --channel=7 --state=1

To turn on
./pihat --id=0 --channel=7 --state=1

To turn off
./pihat --id=0 --channel=7 --state=0


id is the remote id between 0 to 65535

state takes value from 0 (off) to 1 (on). 

channel takes value from 0 to 7 with the following mappings:

0 -> ALL


7 -> button 1

6 -> button 4

5 -> button 3

3 -> button 2
