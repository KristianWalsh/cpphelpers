prefix = $(DESTDIR)/usr
bindir = $(prefix)/bin
etcdir = $(DESTDIR)/etc
sharedir = $(prefix)/share/qvpncommon
httpdir = $(DESTDIR)/var/www/

CPU=$(shell uname -m | tr '_' '-')

# force c++ compilation of source files.
CC = g++
# the linker is also g++. It might be something else with other compilers.
LD = g++
# Compiler flags go here.
CFLAGS = -g -Wall -march=$(CPU) -I. -DUSE_THREADS__

# uncomment for debug
#CFLAGS = $(CFLAGS) -DDEBUG__

# Linker flags go here.
LDFLAGS = -Wl -dynamic -lstdc++ -lpthread


HELPER_CLASSES =build/obj/Configuration.o\
								build/obj/Events.o\
								build/obj/FileIODevice.o\
								build/obj/IODevice.o\
                build/obj/LockableObject.o\
                build/obj/PeriodicallyCalled.o\
                build/obj/SerialIODevice.o\
                build/obj/SerialPort.o\
                build/obj/StateMachine.o\
                build/obj/StringIODevice.o\
                build/obj/ThreadedObject.o\
                build/obj/Timer.o

EVENT_SAMPLE_OBJECTS = $(HELPER_CLASSES)\
                build/obj/samples/event-sample.o

CONFIG_SAMPLE_OBJECTS = $(HELPER_CLASSES)\
                build/obj/samples/config-sample.o

THREADS_SAMPLE_OBJECTS = $(HELPER_CLASSES)\
                build/obj/samples/threads-sample.o

STATES_SAMPLE_OBJECTS = $(HELPER_CLASSES)\
                build/obj/samples/states-sample.o


RELAY_OBJECTS = $(HELPER_CLASSES)\
                build/main.o

# use this command to erase files.
RM = /bin/rm -f

#
#
#
#

all: samples

samples: event-sample config-sample threads-sample states-sample

clean:
	rm -rf build

event-sample: $(EVENT_SAMPLE_OBJECTS)
	$(LD) -o build/$@ $(EVENT_SAMPLE_OBJECTS) $(LDFLAGS)

config-sample: $(CONFIG_SAMPLE_OBJECTS)
	$(LD) -o build/$@ $(CONFIG_SAMPLE_OBJECTS) $(LDFLAGS)

threads-sample: $(THREADS_SAMPLE_OBJECTS)
	$(LD) -o build/$@ $(THREADS_SAMPLE_OBJECTS) $(LDFLAGS)

states-sample: $(STATES_SAMPLE_OBJECTS)
	$(LD) -o build/$@ $(STATES_SAMPLE_OBJECTS) $(LDFLAGS)


tests: $(TEST_OBJECTS)	
	$(LD) -o build/runtests $(TEST_OBJECTS) $(LDFLAGS)
	mkdir -p build/test
	./build/runtests

build/tests/%.o: tests/%.cpp
	mkdir -p build/tests
	$(CC) $(CFLAGS) $(TEST_CFLAGS) -o $@ -c $<

# compile a library "CPP" source file.
build/obj/%.o: %.cpp
	mkdir -p build/obj/samples
	$(CC) $(CFLAGS) -o $@ -c $<

