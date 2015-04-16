/////////////////////////////////////////////////

GENERAL INFORMATION

////////////////////////////////////////////////

hw4.c    : Handle commands and invoke corresponding functions.
hw4.h    : Include all header files and function declarations.
keygen.c : Deal with keygen command.
crypt.c  : Deal with crypt command.
invkey.c : Deal with invkey command.
histo.c  : Deal with histo command.
solve.c  : Deal with solve command.
makefile : Complie the program.


HOW TO COMPILE:
--------------------------------
Makefile is included, simply enters:
    make
an executable named hw4 is created.


THE COMMANDLINE SYNTAX:
--------------------------------
    hw4 keygen -p=pphrase -t=period
    hw4 crypt -k=keyfile [file]
    hw4 invkey keyfile
    hw4 histo -t=period -i=which [file]
    hw4 solve -l=max_t file

THE MEANING of THE COMMANDLINES:
--------------------------------

keygen: Generate a keyfile with a specified period to be used by the full Vigen¨¨re cipher (with polyalphabetic substitution).
 
crypt : Encrypts (or decrypts) an input ASCII text file with the keys specified in keyfile. The output is an ASCII text file  encrypted (or decrypted) using a full Vigen¨¨re cipher (with polyalphabetic substitution).
 
invkey: Create an inverse permutation key file from keyfile.
 
histo : Produce a histogram of ciphertext character frequencies assuming the ciphertext is produced by a polyalphabetic substitution cipher with period equals period. which specifies a one-based index for the alphabets. The output must be sorted according to the frequencies of the ciphertext characters in descending order. Please do not consider any characters that is not a lowercase English character (i.e., between 'a' and 'z', inclusive).
 
solve : Apply the Method of Kasiski and use Index of Coincidence in order to determine the period of a ciphertext encrypted by a full Vigen¨¨re cipher (with polyalphabetic substitution).

Some Notice:
------------
You may have to wait for a few minutes to run the keygen commands when -len = 101.



