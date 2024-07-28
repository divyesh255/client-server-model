CC = gcc
SRC_DIR = ../src
BIN_DIR = ../bin

SRCS = $(SRC_DIR)/client.c $(SRC_DIR)/server.c

# Build the target executable
TARGET = $(BIN_DIR)/client $(BIN_DIR)/server

all : $(TARGET) 

$(BIN_DIR)/client: $(SRC_DIR)/client.c
	@mkdir -p $(BIN_DIR)
	@$(CC) $(CFLAGS) -o $@ $< -lpthread

$(BIN_DIR)/server: $(SRC_DIR)/server.c
	@$(CC) $(CFLAGS) -o $@ $< -lpthread

#Clean files which is generated.	
clean:
	@rm -rf $(BIN_DIR)
