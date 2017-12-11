test : 2-1.o 2-5.o 2-7.o 2-14.o 2-15.o
	gcc -o test 2-1.o 2-5.o 2-7.o 2-14.o 2-15.o

2-1.o : 2-1.c
	gcc -c -o 2-1.c

2-5.o : 2-5.c
	gcc -c -o 2-5.c

2-7.o : 2-7.c
	gcc -c -o openfile newfile 2-7.c

2-14.o : 2-14.c
	gcc -c openfile 2-14.c
 
2-15.o : 2-15.c
	gcc -c openfile openfile1 openfile2 2-15.c

clean:
	rm *.o test
