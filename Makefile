CC = clang
CFLAGS = -Wall -Wextra -std=c99 -g
LFLAGS = 
LIBS = -lm
SOURCES = myshell.c utility.c
OBJECTS = $(subst .c,.o,$(SOURCES))
BIN = myshell
.PHONY: clean help

myshell: $(OBJECTS)
ifeq ($(OS),Windows_NT)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) -o $@.exe
else
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) -o $@	
endif

%.o : %.c
	$(CC) $(CFLAGS) -c $< 

all : $(BIN)

clean:
ifeq ($(OS),Windows_NT)
	rm -f $(OBJECTS) $(BIN).exe *~
else
	rm -f $(OBJECTS) $(BIN) *~
endif

help:
	@echo "Valid targets:"
	@echo "  clean:  removes .o and binary files"
