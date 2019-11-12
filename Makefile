CC = g++
CFLAGS = -I.
DEPS = token.h scanner.h parser.h testTree.h
OBJ = main.o token.o scanner.o parser.o testTree.o
TARGET = frontEnd

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o parser frontEnd
