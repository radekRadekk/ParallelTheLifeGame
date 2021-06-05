CC = gcc

PTHREAD_FLAGS = -lpthread -lrt

OUTPUT_NAME = Life

main:
	$(CC) -Wall -pedantic main.c ./src/*.c ./lib/*.c -o $(OUTPUT_NAME) $(PTHREAD_FLAGS)

clean:
	rm $(OUTPUT_NAME)
	rm *.gif