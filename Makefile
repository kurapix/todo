CC = gcc
CFLAGS = -Wall -Wextra -Werror 
OBJS = main.o todolist.o
BIN = todolist

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c todolist.h
	$(CC) $(CFLAGS) -c $<

clean: 
	rm -f $(OBJS) $(BIN)