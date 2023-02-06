NAME=debugger
MAIN=cli

SRC_DIR=./src
BLD_DIR=./build
BIN_DIR=./bin

HEADERS=$(SRC_DIR)/breakpoint.h $(SRC_DIR)/debugger.h $(SRC_DIR)/inject.h $(SRC_DIR)/inspect.h
OBJECTS=$(BLD_DIR)/breakpoint.o $(BLD_DIR)/debugger.o $(BLD_DIR)/inject.o $(BLD_DIR)/inspect.o $(BLD_DIR)/$(MAIN).o
BIN=$(BIN_DIR)/$(NAME)

# --------

CC=clang
CFLAGS=-Wall -Wextra -pedantic -g -fsanitize=address

LD=$(CC)
LDFLAGS=$(CFLAGS)

# --------

all: $(BIN)

clean:
	rm $(OBJECTS) $(BIN) || true

# --------

$(BIN): $(OBJECTS)
	mkdir -p $(BIN_DIR)
	$(LD) $(LDFLAGS) -o $@ $^

$(BLD_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	mkdir -p $(BLD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<


