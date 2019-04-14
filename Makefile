CC := gcc
CFLAGS := -lncurses
TARGET := ./bin/texter
TARGET_DEB := ./bin/texter_deb

SRCS := $(wildcard ./src/*.c ./src/**/*.c)
OBJS := $(patsubst ./src/**/%.c, %.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

debug: $(TARGET_DEB)

$(TARGET_DEB): $(OBJS)
	$(CC) $(CFLAGS) -g -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(TARGET) *.o

install: all
	sudo cp $(TARGET) /usr/bin/texter

uninstall:
	sudo rm -r /usr/bin/texter
	
.PHONY: all clean