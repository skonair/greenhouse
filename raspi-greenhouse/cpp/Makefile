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

# -o benchmark -std=c++14 -I.. -Wall -Wextra -lpthread benchmark.cc -DCPPHTTPLIB_OPENSSL_SUPPORT -I/usr/local/opt/openssl/include -L/usr/local/opt/openssl/lib -lssl -lcrypto -DCPPHTTPLIB_ZLIB_SUPPORT -lz

${PROGRAMS}: ${SOURCES}
	g++ -std=c++14 ${CCFLAGS} -Wall -lpthread -lrf24-bcm $@.cpp -o $@

clean:
	rm -rf $(PROGRAMS)

install: all
	test -d $(prefix) || mkdir $(prefix)
	test -d $(prefix)/bin || mkdir $(prefix)/bin
	for prog in $(PROGRAMS); do \
		install -m 0755 $$prog $(prefix)/bin; \
	done

.PHONY: install
