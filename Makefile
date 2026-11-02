CC = gcc
CFLAGS = -Wall -Wextra

# Cible finale : créer l'exécutable "client"
client: client.c segdef.h libseg.a
	$(CC) $(CFLAGS) client.c -o client -L. -lseg

# Pour nettoyer les fichiers compilés (en tapant "make clean")
clean:
	rm -f client *.o