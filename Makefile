
CFLAGS = -I . $(INCDIR) -std=c99 -Wall
EXEC_NAME = jewels
OBJS = jewels.o draw.o game.o
ALLEGRO = -lallegro_image -lallegro_primitives -lallegro_dialog -lallegro_ttf -lallegro_font -lallegro -lallegro_main -lallegro_audio -lallegro_acodec
LIBS = -lm $(ALLEGRO)
CC = gcc

all: $(EXEC_NAME)

all debug: $(EXEC_NAME)
debug: CFLAGS += -g -DIR__DEBUG__

$(EXEC_NAME): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LIBS) -o $(EXEC_NAME)

jewels.o: jewels.c
draw.o: draw.c draw.h
game.o: game.c game.h

clean:
	rm -f *.o *.bak *~

purge: clean
	rm -f $(EXEC_NAME)