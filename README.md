S-DES Encryptor / Decryptor
This is an implementation of the Simplified DES encryption algorithm, which uses a 10-bit key to encrypt and decrypt messages. Messages can be input via terminal or input file and likewise can be output to terminal or output file. 

Instructions:
Compile with
gcc -Wall des.c -o des
Execute with -e for encrypt or -d for decrypt and a 10-bit key in decimal form
./des [-e of -d] [key]
Type message (plaintext or ciphertext) or input file with redirect

E.g. ./des -e 250 < message.txt > ciphertext.txt
This will encrypt the text contained within message.txt and output the resulting ciphertext to ciphertext.txt
