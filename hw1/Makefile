CC = g++
CFLAGS = -Wall -g
hw1:	hw1.o hexdump.o decode_base64.o encode_base64.o
	${CC} ${CFLAGS} hw1.o hexdump.o decode_base64.o encode_base64.o -o hw1
hexdump.o:	hexdump.cpp hexdump.h
	${CC} ${CFLAGS} -c hexdump.cpp
decode_base64.o:	decode_base64.cpp decode_base64.h
	${CC} ${CFLAGS} -c decode_base64.cpp
encode_base64.o:	encode_base64.cpp encode_base64.h
	${CC} ${CFLAGS} -c encode_base64.cpp
hw1.o:	hw1.cpp hexdump.h decode_base64.h encode_base64.h
	${CC} ${CFLAGS} -c hw1.cpp
clean:
	rm -f *.o hw1
