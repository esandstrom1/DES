S-DES Encryptor / Decryptor
This is an implementation of the Simplified DES encryption algorithm for text encryption.
It uses a 10-bit key to encrypt and decrypt messages.

Instructions:
Compile with
gcc -Wall des.c -o des  
Execute with -e for encrypt or -d for decrypt, a 10-bit key in decimal form, and the input file name
./des [-e of -d] [key] [input file]

E.g. ./des -e 250 message.txt > ciphertext.txt  
This will encrypt the text contained within message.txt and output the resulting ciphertext to ciphertext.txt
