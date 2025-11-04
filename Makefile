# Makefile portable para LRU
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = bio

all: $(TARGET)

$(TARGET): bio.c
	$(CC) $(CFLAGS) -o $(TARGET) bio.c

clean:
	del /f /q $(TARGET).exe 2>nul || rm -f $(TARGET)

.PHONY: all clean
# Makefile portable para LRU
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = bio

all: $(TARGET)

$(TARGET): bio.c
	$(CC) $(CFLAGS) -o $(TARGET) bio.c

clean:
	del /f /q $(TARGET).exe 2>nul || rm -f $(TARGET)

.PHONY: all clean