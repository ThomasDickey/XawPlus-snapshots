#
# Makefile fot the test programs
# This makefile requires libXawPlus.a in the
# parent directory !
#
# Roland Krause 2000
#

LIB_PATH =	-L/usr/X11/lib			# Libraries for X
INC_PATH =	-I/usr/X11/include		# Header for X
LIBRARIES =	-lXmu -lXext -lXt -lXpm -lX11

CC =		gcc
FLAGS=		-Wall -g
PROGS=		test_encoding test_pixmap test_scrbar test_menu

all:		prog
prog:		$(PROGS)

# --- Compile the programs

test_encoding:	test_encoding.o ../libXawPlus.a
		$(CC) -o test_encoding $(LIB_PATH) test_encoding.o ../libXawPlus.a $(LIBRARIES)

test_encoding.o:test_encoding.c
		$(CC) $(FLAGS) $(INC_PATH) -c test_encoding.c

test_pixmap:	test_pixmap.o ../libXawPlus.a
		$(CC) -o test_pixmap $(LIB_PATH) test_pixmap.o ../libXawPlus.a $(LIBRARIES)

test_pixmap.o:	test_pixmap.c
		$(CC) $(FLAGS) $(INC_PATH) -c test_pixmap.c

test_scrbar:	test_scrbar.o ../libXawPlus.a
		$(CC) -o test_scrbar $(LIB_PATH) test_scrbar.o ../libXawPlus.a $(LIBRARIES)

test_scrbar.o:	test_scrbar.c
		$(CC) $(FLAGS) $(INC_PATH) -c test_scrbar.c

test_menu:	test_menu.o ../libXawPlus.a
		$(CC) -o test_menu $(LIB_PATH) test_menu.o ../libXawPlus.a $(LIBRARIES)

test_menu.o:	test_menu.c
		$(CC) $(FLAGS) $(INC_PATH) -c test_menu.c


# --- Clean up 

clean:
		rm -f *.o $(PROGS)
