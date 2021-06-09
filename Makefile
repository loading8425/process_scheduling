allocate: main.o read_txt.o process.o dual_processor.o multi_processor.o fast_processor.o
	gcc -o allocate main.o read_txt.o process.o dual_processor.o multi_processor.o fast_processor.o -lm

main.o: main.c read_txt.h process.h dual_processor.h multi_processor.h
	gcc -c main.c

read_txt.o: read_txt.c read_txt.h process.h
	gcc -c read_txt.c

process.o: process.c process.h
	gcc -c process.c

dual_processor.o: dual_processor.c dual_processor.h process.h
	gcc -c dual_processor.c

multi_processor.o: multi_processor.c multi_processor.h process.h read_txt.h
	gcc -c multi_processor.c

fast_processor.o: fast_processor.c fast_processor.h process.h read_txt.h
	gcc -c fast_processor.c

clean:
	rm -rf *.o allocate