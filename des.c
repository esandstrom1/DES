//Ethan Sandstrom

//DES 10-bit encryptor
//Compile with gcc -Wall des.c
//Execution: ./a.out, followed by -e for encrypt or -d for decrupt, followed by 10 bit key in decimal form
//           Program will wait for input. Type string and enter

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int p_10(int initial_key);
int p_8(unsigned char ls11, unsigned char ls12);
int leftshift_1(unsigned char ls);
int leftshift_2(unsigned char ls);

int ip(unsigned char byte);
int ip_inv(unsigned char byte);

int fk(unsigned char byte, unsigned char key_8);
int F(unsigned char nibble, unsigned char key_8);
int S0(unsigned char nibble);
int S1(unsigned char nibble);
int SW(unsigned char left_4, unsigned char right_4);


int main(int argc, char* argv[])
{
    int key10 = 0b1010000010;
    unsigned char byte = 0b01110010;
    int function = 0;               //0 for encrypt. 1 for decrypt

    if(argc < 3 || argc > 3)
    {
        printf("Usage: ./a.out [-e or -d] [10-bit decimal key]\n");
        exit(1);
    }
    if((strcmp(argv[1], "-e")) != 0 && (strcmp(argv[1], "-d")) != 0)
    {
        printf("Usage: ./a.out [-e or -d] [10-bit decimal key]\n");
        exit(1);
    }
    if((strcmp(argv[1], "-d") == 0))
        function = 1;

    //Key generation
    if((key10 = atoi(argv[2])) == 0);
    //{
    //     perror("Key set to 0");
    //     exit(1);
    //}

    //Get user input
    size_t chunk_size = 1;
    char* user_input;
    if((user_input = malloc(chunk_size)) == NULL)
    {
        perror("Failed to allocate initial memory\n");
        exit(1);
    }
    if((getline(&user_input, &chunk_size, stdin)) == -1)
    {
        perror("Failed to read input\n");
        exit(1);
    }
    //printf("Input: %s\n", user_input);

    int K1 = p_8(leftshift_1(((p_10(key10)) >> 5) & 0b0000011111), leftshift_1((p_10(key10)) & 0x1F));
    int K2 = p_8(leftshift_2(leftshift_1(((p_10(key10)) >> 5) & 0b0000011111)), leftshift_2(leftshift_1((p_10(key10)) & 0x1F)));

    // int fk_result = fk(ip(byte), K1);
    // int switched = SW(((fk(ip(byte), K1) & 0b11110000) >> 4), (fk(ip(byte), K1) & 0b00001111));
    // int fk_result_2 = fk(SW(((fk(ip(byte), K1) & 0b11110000) >> 4), (fk(ip(byte), K1) & 0b00001111)), K2);

    unsigned char cipher_byte;
    unsigned char decrypted_byte;

    for(int a = 0; (byte = user_input[a]) != '\0'; a++) //Iterate through user input, encrypt byte, print
    {
        //printf("%4d ", byte);

        if(function == 0)
        {
            cipher_byte = ip_inv(fk(SW(((fk(ip(byte), K1) & 0b11110000) >> 4), (fk(ip(byte), K1) & 0b00001111)), K2));
            printf("%c", cipher_byte);
            //printf("%4d ", cipher_byte);
        }
        else if(function == 1)
        {
            decrypted_byte = ip_inv(fk(SW(((fk(ip(byte), K2) & 0b11110000) >> 4), (fk(ip(byte), K2) & 0b00001111)), K1));
            printf("%c", decrypted_byte);
        }

    }
}

int p_10(int initial_key) //3 5 2 7 4 10 1 9 8 6
{
    initial_key = initial_key & 0b1111111111; //Sanitize
    int new_key = 0b0000000000;
    new_key += (initial_key & 0b0010000000) << 2;
    new_key += (initial_key & 0b0000100000) << 3;
    new_key += (initial_key & 0b0100000000) >> 1;
    new_key += (initial_key & 0b0000001000) << 3;
    new_key += (initial_key & 0b0001000000) >> 1;
    new_key += (initial_key & 0b0000000001) << 4;
    new_key += (initial_key & 0b1000000000) >> 6;
    new_key += (initial_key & 0b0000000010) << 1;
    new_key += (initial_key & 0b0000000100) >> 1;
    new_key += (initial_key & 0b0000010000) >> 4;
    return new_key;
}
int p_8(unsigned char ls11, unsigned char ls12) //6 3 7 4 8 5 10 9
{
    ls11 = ls11 & 0b11111; //Sanitize
    ls12 = ls12 & 0b11111;
    //printf("Initial 5 bit keys: %d %d\n", ls11, ls12);
    int init_10 = ((ls11 << 5) & 0b1111100000) + ls12; //Turn 5 bits keys back into 10 bit
    //printf("Initial 10 bit key: %d\n", init_10);

    int new_key = 0b00000000;
    new_key += (init_10 & 0b0000010000) << 5;
    new_key += (init_10 & 0b0010000000) << 1;
    new_key += (init_10 & 0b0000001000) << 4;
    new_key += (init_10 & 0b0001000000) << 0;
    new_key += (init_10 & 0b0000000100) << 3;
    new_key += (init_10 & 0b0000100000) >> 1;
    new_key += (init_10 & 0b0000000001) << 3;
    new_key += (init_10 & 0b0000000010) >> 1;
    return (new_key >> 2) & 0b11111111;    //Arithmetic was performed on 10 bit #s. Convert to 8 bit
}

int leftshift_1(unsigned char ls) //Wrap leftmost bit around for 5 bit numbers
{
    /*
    ls = ls & 0b11111; //Sanitize
    if(ls / 16 >= 1) //MSb is a 1
        return ((ls << 1) & 0b11110) + 1;  //Shift left by 1, save only 1st 4 bits, add 1 (wrap around bit)
    else             //MSb is a 0
        return ((ls << 1) & 0b11110);      //Shift left by 1, save only 1st 4 bits, add 0
    */

    ls = ls & 0b11111; //Sanitize
    int wrap = ((ls & 0b10000) >> 4) & 0b00001;
    return ((ls << 1) & 0b11110) + wrap;
}
int leftshift_2(unsigned char ls) //Wrap around leftmost 2 bits
{
    ls = ls & 0b11111; //Sanitize
    int wrap = ((ls & 0b11000) >> 3) & 0b00011; //Get the 2 most significant bits that will wrap around
    return ((ls << 2) & 0b11100) + wrap;
}

int ip(unsigned char byte) //2 6 3 1 4 8 5 7
{
    byte = byte & 0b11111111; //Sanitize
    int permuted_byte = 0b00000000;
    permuted_byte += (byte & 0b01000000) << 1;
    permuted_byte += (byte & 0b00000100) << 4;
    permuted_byte += (byte & 0b00100000);
    permuted_byte += (byte & 0b10000000) >> 3;
    permuted_byte += (byte & 0b00010000) >> 1;
    permuted_byte += (byte & 0b00000001) << 2;
    permuted_byte += (byte & 0b00001000) >> 2;
    permuted_byte += (byte & 0b00000010) >> 1;
    return permuted_byte;
}
int ip_inv(unsigned char byte) //4 1 3 5 7 2 8 6
{
    byte = byte & 0b11111111; //Sanitize
    int permuted_byte = 0b00000000;
    permuted_byte += (byte & 0b00010000) << 3;
    permuted_byte += (byte & 0b10000000) >> 1;
    permuted_byte += (byte & 0b00100000);
    permuted_byte += (byte & 0b00001000) << 1;
    permuted_byte += (byte & 0b00000010) << 2;
    permuted_byte += (byte & 0b01000000) >> 4;
    permuted_byte += (byte & 0b00000001) << 1;
    permuted_byte += (byte & 0b00000100) >> 2;
    return permuted_byte;
}

int fk(unsigned char byte, unsigned char key_8) 
{
    byte = byte & 0b11111111;
    int left = (byte >> 4) & 0b1111; //Get left half of input byte
    int right = byte & 0b1111;       //Get right half of input byte

    int output = 0b00000000;
    output = ((left ^ F(right, key_8)) << 4) & 0b11110000; //Set left 4 bits of output
    output += right;                                       //Set right 4 bits of output
    return output;
}

int F(unsigned char nibble, unsigned char key_8) //Sub-function of fk(). Takes right half of byte (4 bits), 8 bit key. Returns 4 bits
{
    nibble = nibble & 0b1111;
    //printf("input nibble %d\n", nibble);
    //EP (Expansion/Permutation) step: turn 4 bits into permuted 8 bits
    int byte = 0b00000000;
    byte += (nibble & 0b0001) << 7;
    byte += (nibble & 0b1000) << 3;
    byte += (nibble & 0b0100) << 3;
    byte += (nibble & 0b0010) << 3;
    byte += (nibble & 0b0100) << 1;
    byte += (nibble & 0b0010) << 1;
    byte += (nibble & 0b0001) << 1;
    byte += (nibble & 0b1000) >> 3;

    int new_byte = byte ^ key_8; //XOR the byte generated from E/P with the 8 bit key
    int left = (new_byte & 0b11110000) >> 4;
    int right = new_byte & 0b00001111;

    int temp_nibble = 0b0000;
    int nibble_out = 0b0000;
    temp_nibble = (S0(left) << 2) + (S1(right));

    nibble_out += (temp_nibble & 0b0100) << 1;
    nibble_out += (temp_nibble & 0b0001) << 2;
    nibble_out += (temp_nibble & 0b0010);
    nibble_out += (temp_nibble & 0b1000) >> 3;


    //printf("%d\n", nibble_out);
    return nibble_out;
}

int S0(unsigned char nibble) //Get 4 bits. Use bits as row and col coordinates in a matrix. Return number (2 bits) at intersection
{
    int row_coord = ((nibble & 0b1000) >> 2) + (nibble & 0b0001); //Add bits 1 and 4 to get row coordinate
    int col_coord = ((nibble & 0b0100) >> 1) + ((nibble & 0b0010) >> 1); //Add bits 2 and 3 to get col coordinate
    
    int matrix[4][4] = {{1, 0, 3, 2},
                        {3, 2, 1, 0},
                        {0, 2, 1, 3},
                        {3, 1, 3, 2}};
    //printf("S0 intersection (%d, %d) = %d\n", row_coord, col_coord, matrix[row_coord][col_coord]);
    return(matrix[row_coord][col_coord]);
}
int S1(unsigned char nibble)
{
    int row_coord = ((nibble & 0b1000) >> 2) + (nibble & 0b0001); //Add bits 1 and 4 to get row coordinate
    int col_coord = ((nibble & 0b0100) >> 1) + ((nibble & 0b0010) >> 1); //Add bits 2 and 3 to get col coordinate
    
    int matrix[4][4] = {{0, 1, 2, 3},
                        {2, 0, 1, 3},
                        {3, 0, 1, 0},
                        {2, 1, 0, 3}};
    //printf("S1 ntersection (%d, %d) = %d\n", row_coord, col_coord, matrix[row_coord][col_coord]);
    return(matrix[row_coord][col_coord]);
}

int SW(unsigned char left_4, unsigned char right_4) //Swap 2 nibbles, return a byte
{
    left_4 = left_4 & 0b1111;
    right_4 = right_4 & 0b1111;
    int out_byte = 0b00000000;
    out_byte = (right_4 << 4) + (left_4);
    return out_byte;
}
