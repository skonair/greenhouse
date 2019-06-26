#############################################################################
#
# Makefile for Raspberry Pi NRF24L01/NRF24L01+ receiver
#
# Run:
#     make clean; make
#     sudo ./receiver
#############################################################################
prefix := /usr/local

# The recommended compiler flags for the Raspberry Pi
CCFLAGS=-Ofast -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s

# define all programs
PROGRAMS = receiver receive_sensor_data
SOURCES = ${PROGRAMS:=.cpp}

all: ${PROGRAMS}

${PROGRAMS}: ${SOURCES}
	g++ ${CCFLAGS} -Wall -lrf24-bcm $@.cpp -o $@

clean:
	rm -rf $(PROGRAMS)

install: all
	test -d $(prefix) || mkdir $(prefix)
	test -d $(prefix)/bin || mkdir $(prefix)/bin
	for prog in $(PROGRAMS); do \
		install -m 0755 $$prog $(prefix)/bin; \
	done

.PHONY: install