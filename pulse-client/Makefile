CC = clang++-20
CCFLAGS = -O3 -std=c++23 -pedantic -Wall -W -Werror -Wextra -c -I.
DEFINES =
LDFLAGS = -lpulse-simple
OBJ = bin/main.o common/utils.o read/pulse/pulse.o write/socket/socket.o
RUN = bin/pclient

all: $(OBJ)
	$(CC) $^ -o $(RUN) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(DEFINES) $(CCFLAGS) $< -o $@

install:
	strip $(RUN)
	cp $(RUN) /usr/bin/

clean:
	rm -f $(OBJ) $(RUN)
