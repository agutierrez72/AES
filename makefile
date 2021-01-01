aes: AES.o
	g++ AES.o -o aes

AES.o: AES.cpp
	g++ -c AES.cpp

run: aes
	clear
	./aes


clean:
	rm *.o aes
	clear