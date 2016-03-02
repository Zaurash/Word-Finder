SRC=./src/WordFinder.C
OBJ=$(SRC:.C=.o)

prog: $(OBJ)
	g++ $(OBJ) -o WordFinder

.C.o: $<
	g++  -g -I. -c $<
	mv *.o ./src
clean:
	rm ./src/*.o WordFinder ./*.txt