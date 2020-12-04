CC = gcc
OBJECTS = matrix_test.o matrix_2d.o
EXECUTABLE = run.exe

all: $(EXECUTABLE)

matrix_2d.o: matrix_2d.c matrix_2d.h
	$(CC) -g -c  matrix_2d.c -D TEST_MATRIX_2D

matrix_test.o: matrix_test.c matrix_2d.o
	$(CC) -g -c  matrix_test.c -D TEST_MATRIX_2D

$(EXECUTABLE): $(OBJECTS)
	$(CC) $^ -o $(EXECUTABLE) 

clean:
	rm -f *.o $(EXECUTABLE)