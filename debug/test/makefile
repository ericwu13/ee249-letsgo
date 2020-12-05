CC = gcc
OBJECTS = matrix_test.o matrix_2d.o dtw.o
EXECUTABLE = run.exe

all: $(EXECUTABLE)

dtw.o: matrix_2d.o dtw.c dtw.h
	$(CC) -g -c  dtw.c -D TEST_MATRIX_2D -lm

matrix_2d.o: matrix_2d.c matrix_2d.h
	$(CC) -g -c  matrix_2d.c -D TEST_MATRIX_2D -lm

matrix_test.o: matrix_test.c matrix_2d.o dtw.o
	$(CC) -g -c  matrix_test.c -D TEST_MATRIX_2D -lm

$(EXECUTABLE): $(OBJECTS)
	$(CC) $^ -o $(EXECUTABLE) -lm

clean:
	rm -f *.o $(EXECUTABLE)