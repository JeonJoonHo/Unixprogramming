test: main.o read.o write.o
	gcc -o test main.o read.o write.o

main.o: io.h main.c
	gcc -c main.c

read.o: io.h read.c
	gcc -c read.c

write.o: io.h write.c
	gcc -c write.c

clean:
	rm *.o
