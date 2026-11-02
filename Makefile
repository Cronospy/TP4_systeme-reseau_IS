CC = gcc
CFLAGS = -Wall -Wextra

client: client.c segdef.h libmylib_sem.a
	$(CC) $(CFLAGS) client.c -o client -L. -lmylib_sem

libmylib_sem.a: mylib_sem.o
	ar rcs libmylib_sem.a mylib_sem.o

mylib_sem.o: mylib_sem.c segdef.h
	$(CC) $(CFLAGS) -c mylib_sem.c -o mylib_sem.o

clean:
	rm -f client *.o libmylib_sem.a