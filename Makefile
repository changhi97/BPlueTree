BPTree_exe : BPTree.o File.o FileHashMap.o main.o
	gcc -o BPTree_exe BPTree.o File.o FileHashMap.o -lm main.o
BPTree.o : BPTree.c
	gcc -c -o BPTree.o BPTree.c
File.o : File.c
	gcc -c -o File.o File.c
FileHashMap.o : FileHashMap.c
	gcc -c -o FileHashMap.o FileHashMap.c
main.o : main.c
	gcc -c -o main.o main.c
clean :
	rm *.o BPTree_exe
