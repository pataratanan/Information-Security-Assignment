#include <iostream>
#include <string>
#include <cstring>

using namespace std;
class SDES {
public:
    int choice;
    unsigned char plainText[9] = {0};
    unsigned char cipherText[9] = {0};
    unsigned char key[11];
    unsigned char k1[9] = {0};
    unsigned char k2[9] = {0};

//using unsigned char, because the range of unsigned is 0 to 255
//unsigned char is usually used for holding binary data where 0 is valid value and still part of your data
    void XOR(unsigned char *operandL, unsigned char *operandR, unsigned char *out) {
        for (size_t i = 0; i < strlen((char *) operandL); i++) {
            out[i] = (operandL[i] ^ operandR[i]) + 48; // ^ is bitwise exclusive OR operation in c++
            //unsigned char is usually used for holding binary data where 0 is valid value and still part of your data.*/
        }
    }

    void sBox(unsigned char *in, unsigned char *out) {
        string result; // Result in string form
        int Lrow, Lcol, Rrow, Rcol;

        string binary[4] = {"00", "01", "10", "11"}; // Used to convert sbox element to binary string


        int s0[4][4] = { // Sbox1
                {1, 0, 3, 2},
                {3, 2, 1, 0},
                {0, 2, 1, 3},
                {3, 1, 3, 2}
        };

        int s1[4][4] = { // Sbox 2
                {0, 1, 2, 3},
                {2, 0, 1, 3},
                {3, 0, 1, 0},
                {2, 1, 0, 3}
        };

        if (in[0] == '1') {
            Lrow = 2 + in[3] - 48;
        } else { Lrow = in[3] - 48; }


        if (in[1] == '1') {
            Lcol = 2 + in[2] - 48;
        } else { Lcol = in[2] - 48; }

        if (in[4] == '1') {
            Rrow = 2 + in[7] - 48;
        } else { Rrow = in[7] - 48; }
        if (in[5] == '1') {
            Rcol = 2 + in[6] - 48;
        } else { Rcol = in[6] - 48; }

        // Uses string to store result since it is easier to perform concatenation with string than char arrays
        result = binary[s0[Lrow][Lcol]] + binary[s1[Rrow][Rcol]];

        strncpy((char *) out, result.c_str(), 5); // Copies result string into out char array

    }

//this function is used for permutation IP, IP-1, P10, P8, E/P, P4
    void permutation(unsigned char *in, unsigned char *out, const char *permutate) {
        //size_t is a base unsigned integer
        //unsigned int is used to store 32-bit integers
        //which only represents non-negative integers i.e. positive numbers and zero
        for (size_t i = 0; i < strlen(permutate); i++) //strlen returns the length of the given string or char
        {
            out[i] = in[permutate[i] - '0'];
        }

    }

//After Permutate P10, this function getting LS1 by take out 4 bits into 1 left shift and use it at the end.
    void leftShift(unsigned char *key1) {
        char left[5];
        char right[5];
        unsigned int t; //using unsigned int, because the value that we are storing will always be non-negative

        for (int k = 0; k < 5; k++) {
            left[k] = key1[k];
        }

        for (int k = 0; k < 5; k++) {
            right[k] = key1[k + 5];
        }

        t = left[0];
        memmove(left, left + 1, 5 - 1);
        left[4] = t;

        t = right[0];

        //copies right + 1 into right
        memmove(right, right + 1, 5 - 1); //data copied to second arguments then copied to the first
        right[4] = t;

        for (int k = 0; k < 5; k++) {
            key1[k] = left[k];
        }
        for (int k = 0; k < 5; k++) {
            key1[k + 5] = right[k];
        }
    }

//generate K1 and K2
    void generatesKey(unsigned char *key, unsigned char *key1, unsigned char *key2) {

        unsigned char kTemp[11] = {0};
        permutation(key, kTemp, "2416390875"); // P10 equivalent to 3 5 2 7 4 10 1 8 9 6 but we start at '0'

        cout << "\nP10:    " << kTemp;

        leftShift(kTemp); // LS1

        cout << "\nLS1:    " << kTemp;

        for (int i = 0; i < 8; i++) // Shrink key from 10 bits to 8
        {
            key1[i] = kTemp[i + 2];
        }

        permutation(kTemp, key1, "52637498"); // P8 (k1) equivalent to 687485109 but we start at '0'

        cout << "\nK1:   " << key1;

        leftShift(kTemp); // LS2
        leftShift(kTemp); // LeftShit again to take out 2 bits from the front and put it at the back

        cout << "\nLS2:    " << kTemp;

        permutation(kTemp, key2, "52637498"); // P8 (k2) equivalent to 687485109 but we start at '0'

        cout << "\nK2:   " << key2;
    }

//this function happened after we get Ls1, Ls2, K1, K2
    void mainDES(unsigned char *out, unsigned char *key) {
        unsigned char p4[5] = {0};
        unsigned char temp[5] = {0};
        unsigned char ipLHalf[5] = {0};
        unsigned char ipRHalf[5] = {0};

        memcpy(ipLHalf, out, 4); // Copies cipherText to IPLHalf for later use
        strncpy((char *) ipRHalf, (char *) out + 4, 5); //It copies a maximum of count characters from the string

        for (int k = 0; k < 4; k++) {
            temp[k] = out[k + 4];
        }

        permutation(temp, out, "30121230"); // EP = 41232341 but starting at 0 = 30121230

        cout << "\nEP:     " << out;

        XOR(out, key, out); // XOR K1 & K2

        cout << "\nXOR:    " << out;

        for (int k = 0; k < 4; k++) {
            temp[k] = out[k + 4];
        }

        sBox(out, temp); // S0 AND S1
        cout << "\nSBOX:   " << temp;

        permutation(temp, p4, "1320"); // P4 equivalent to 2 4 3 1 but we start at '0' E/P
        cout << "\nP4:     " << p4;

        XOR(p4, ipLHalf, p4);
        cout << "\nP4XOR:  " << p4;

        strncpy((char *) out, (char *) p4, 4); //copy the contents of p4 to out
        strncpy((char *) out + 4, (char *) ipRHalf, 4); //copy the contents of ipRHalf to out

        //cout << "\nRound:  " << out;

    }

    void decryption(unsigned char *pText, unsigned char *cText, unsigned char *k1, unsigned char *k2) {
        unsigned char rnd[9] = {0};
        unsigned char temp[5] = {0}; // Used for swapping halves

        permutation(cText, rnd, "15203746"); //IP equivalent to 2 6 3 1 4 8 5 7 but we start at '0'
        //cout << "\nIP:     " << cText;

        //cout << "\nround 1";
        mainDES(rnd, k2);

        strncpy((char *) temp, (char *) rnd, 4); // SW copy the contents of rnd to temp
        strncpy((char *) rnd, (char *) rnd + 4, 4);
        strncpy((char *) rnd + 4, (char *) temp, 4);

        //cout << "\nSW:     " << rnd;
        //cout << "\nround 2";
        mainDES(rnd, k1);

        permutation(rnd, pText, "30246175"); // IP-1 equivalent to 4 1 3 5 7 2 8 6 but we start at '0'
        cout << "\n\nPlaintext: " << pText << "\n";
    }

    void encryption(unsigned char *plainText, unsigned char *cipherText, unsigned char *k1, unsigned char *k2) {
        unsigned char rnd[9] = {0};
        unsigned char temp[5] = {0}; // Used for swapping halves

        permutation(plainText, rnd, "15203746"); //IP equivalent to 2 6 3 1 4 8 5 7 but we start at '0'
        cout << "\nIP 8 bits: " << plainText;

        cout << "\nround 1";
        mainDES(rnd, k1);

        strncpy((char *) temp, (char *) rnd, 4); // SWAP, copy the contents of rnd to temp
        strncpy((char *) rnd, (char *) rnd + 4, 4);
        strncpy((char *) rnd + 4, (char *) temp, 4);

        cout << "\nSW: " << rnd;
        cout << "\nround 2";
        mainDES(rnd, k2);

        permutation(rnd, cipherText, "30246175"); // IP-1 equivalent to 4 1 3 5 7 2 8 6 but we start at '0'
        cout << "\n\nCiphertext:     " << cipherText << "\n";
    }
};

int main()
{
    SDES s;

    cout << "1. Encryption \n2. Decryption\n";
    cin >> s.choice;

    if (s.choice == 1)
    {

        cout << "\nEnter 8-bit Plaintext: ";
        cin >> s.plainText;

        cout << "Enter 10-bit key: ";
        cin >> s.key;

        cout << "\n\n10 bits key:  "<< s.key << ", " << "P:  "<< s.plainText << endl;
        s.generatesKey(s.key, s.k1, s.k2); // Generates key1 and key2
        s.encryption(s.plainText, s.cipherText, s.k1, s.k2);
    }
    else if (s.choice == 2)
    {
        cout << "\nEnter 8-bit Ciphertext: ";
        cin >> s.cipherText;

        cout << "Enter 10-bit key: ";
        cin >> s.key;

        s.generatesKey(s.key, s.k1, s.k2); // Generates key1 and key2
        s.decryption(s.plainText, s.cipherText, s.k1, s.k2);
    }
    else
    {
        cout << "Not in a choice!!" << endl;
        exit(1); //EXIT_FAILURE
    }

    return 0;
}