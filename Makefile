CC = gcc


brace: brace.c
	$(CC) -o brace brace.c

clean:
	rm -f brace

test: brace
	tests/run.sh
