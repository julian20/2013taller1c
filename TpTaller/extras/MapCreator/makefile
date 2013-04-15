CC= g++
CCFLAGS= -g -Wall -pedantic  
EXEC1=tileAreaCreator
EXEC2=entityAreaCreator

all: $(EXEC1) $(EXEC2)

$(EXEC1): $(EXEC1).cpp
	$(CC) $(CCFLAGS) $(EXEC1).cpp -o $(EXEC1)

$(EXEC2): $(EXEC2).cpp
	$(CC) $(CCFLAGS) $(EXEC2).cpp -o $(EXEC2)

clean:
	rm -f $(EXEC1) $(EXEC2)