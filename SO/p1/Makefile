CC = gcc

CFLAGS = -g -std=c17 -D_POSIX_C_SOURCE=200809L \
         -Wall -Werror -Wextra \
         -Wcast-align -Wconversion -Wfloat-equal -Wformat -Wnull-dereference -Wshadow -Wsign-conversion -Wswitch-enum -Wundef -Wunreachable-code -Wunused \
         -pedantic \
		 -pthread \
		 -fsanitize=address -fsanitize=undefined 
         

SRC = main.c parser.c eventlist.c \
      processing/processing.c \
      operations.c \
      utils/utils.c

ifneq ($(shell uname -s),Darwin) # if not MacOS
	CFLAGS += -fmax-errors=5
endif

all: ems

ems: $(SRC)
	$(CC) $(CFLAGS) $(SLEEP) -o ems $(SRC)

run: ems
	@./ems

clean:
	rm -f *.o ems

format:
	@which clang-format >/dev/null 2>&1 || echo "Please install clang-format to run this command"
	clang-format -i *.c *.h