CC = g++
CFLAGS = -Wall -g
hw5:	hw5.o tablecheck.o crypt_DES.o
	${CC} ${CFLAGS} -o hw5 hw5.o tablecheck.o crypt_DES.o
tablecheck.o:	tablecheck.cpp hw5.h
	${CC} ${CFLAGS} -c tablecheck.cpp
crypt_DES.o:	crypt_DES.cpp hw5.h
	${CC} ${CFLAGS} -c crypt_DES.cpp
hw5.o:	hw5.cpp hw5.h
	${CC} ${CFLAGS} -c hw5.cpp
clean:
	rm -f *.o hw5
