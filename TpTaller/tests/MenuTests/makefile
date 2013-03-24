
INCLUDES=../../includes

CC= g++
CCFLAGS= -g -Wall -pedantic -I$(INCLUDES) 
LIBS=`sdl-config --cflags --libs` -L/usr/local/lib -lSDL -lSDL_image
SRC=../../src/
LIB=Menu

VFLAGS= valgrind --leak-check=full --show-reachable=yes --track-origins=yes

EXEC=menuTests

all: $(SRC) $(EXEC)
	
$(LIB).o: 
	$(CC) $(CCFLAGS) -c $(SRC)$(LIB).cpp
	
$(EXEC): $(EXEC).cpp $(LIB).o
	$(CC) $(CCFLAGS) $(LIB).o $(EXEC).cpp -o $(EXEC) $(LIBS)
	
run: $(EXEC)
	./$(EXEC)
	
test: 
	$(VFLAGS) ./$(EXEC)
	
clean:
	rm $(EXEC) *.o
	
