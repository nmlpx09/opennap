CC = clang++-20
CCFLAGS = -O3 -std=c++23 -pedantic -Wall -W -Werror -Wextra -c -I.
DEFINES =
LDFLAGS = -lasound
OBJ = bin/main.o common/utils.o read/socket/socket.o write/alsa/alsa.o
RUN = bin/aserver

all: $(OBJ)
	$(CC) $^ -o $(RUN) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(DEFINES) $(CCFLAGS) $< -o $@

install:
	strip $(RUN)
	cp $(RUN) /usr/bin/

clean:
	rm -f $(OBJ) $(RUN)
