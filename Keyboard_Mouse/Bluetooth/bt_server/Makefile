DIR := $(CURDIR)
CC=arduino-cli
FQBN=arduino:samd:nano_33_iot
PORT=/dev/ttyACM1
PROJ=bt_server
INC=$(DIR)/../
all:
	echo $(INC)
	mv $(PROJ).c $(PROJ).ino
	$(CC) compile --fqbn $(FQBN) ../$(PROJ) --libraries $(INC)
	$(CC) upload -p $(PORT) --fqbn $(FQBN) ../$(PROJ)
	mv $(PROJ).ino $(PROJ).c

compile:
	mv $(PROJ).c $(PROJ).ino
	$(CC) compile --fqbn $(FQBN) ../$(PROJ)
	mv $(PROJ).ino $(PROJ).c

upload:
	$(CC) upload -p $(PORT) --fqbn $(FQBN) ../$(PROJ)

refresh:
	mv $(PROJ).ino $(PROJ).c

clean:
	rm -rf *~
	rm -rf *.hex
	rm -rf *.elf
	rm -rf *.bin
