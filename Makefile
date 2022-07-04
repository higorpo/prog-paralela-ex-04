compile:
	gcc -o reproduz_texto reproduz_texto.c -pthread
exec:
	./reproduz_texto $(tamp)