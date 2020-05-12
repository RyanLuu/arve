CC=gcc
TARGET=out

CFLAGS=-std=gnu99 -g -Wall -Werror
INCLUDES=
LFLAGS=
LIBS=
SRCS=asminst.c label.c lexer.c error.c parser.c main.c
OBJS=$(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS) $(LFLAGS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) *.o *~ $(TARGET)
