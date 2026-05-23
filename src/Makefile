CC = gcc
SRCS = server.c route.c handler.c validator.c
TARGET = server

$(TARGET): $(SRCS)
	$(CC) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)