CC=avr-gcc
MCU=atmega328p
CC=avr-gcc
OBJCOPY=avr-objcopy
IDIR=.
SRCS=I2CSlave.cpp ise_motor_driver_ip.cpp motordriver.cpp TI2C.cpp
CFLAGS=-std=c++11 -Wall -Os -mmcu=${MCU} -I${IDIR}
TARGET=main

all: ${SRCS} ${IDIR}/*.h
		${CC} ${CFLAGS} -o ${TARGET}.bin ${SRCS} 
		${OBJCOPY} -j .text -j .data -O ihex ${TARGET}.bin ${TARGET}.hex

flash:
		avrdude -p ${MCU} -c avrisp2 -U flash:w:${TARGET}.hex:i -F -P /dev/ttyUSB0 -B 10

clean:
		rm -f *.bin *.hex
