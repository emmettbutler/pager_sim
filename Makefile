all:
	gcc -pthread -Wall pagers.c -o test_pagers

clean:
	rm -f a.out
