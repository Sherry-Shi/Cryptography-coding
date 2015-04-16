CC = g++
CFLAGS = -Wall -g
hw4:	hw4.o crypt.o histo.o invkey.o keygen.o solve.o
	${CC} ${CFLAGS} -o hw4 hw4.o crypt.o histo.o invkey.o keygen.o solve.o -L/home/scf-22/csci551b/openssl/lib -lcrypto -lm
keygen.o:	keygen.cpp hw4.h
	${CC} ${CFLAGS} -c keygen.cpp -I/home/scf-22/csci551b/openssl/include
crypt.o:	crypt.cpp hw4.h
	${CC} ${CFLAGS} -c crypt.cpp -I/home/scf-22/csci551b/openssl/include
histo.o:	histo.cpp hw4.h
	${CC} ${CFLAGS} -c histo.cpp -I/home/scf-22/csci551b/openssl/include
invkey.o:	invkey.cpp hw4.h
	${CC} ${CFLAGS} -c invkey.cpp -I/home/scf-22/csci551b/openssl/include
solve.o:	solve.cpp hw4.h
	${CC} ${CFLAGS} -c solve.cpp -I/home/scf-22/csci551b/openssl/include
hw4.o:	hw4.cpp hw4.h
	${CC} ${CFLAGS} -c hw4.cpp -I/home/scf-22/csci551b/openssl/include
clean:
	rm -f *.o hw4
