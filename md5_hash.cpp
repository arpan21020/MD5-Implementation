#include<bits/stdc++.h>
// #include<bits/stdc++.h>
using namespace std;

class MD5Hash
{
private:
     // Initializing 4 word buffers
     static const uint32_t wordA = 0x67452301;
     static const uint32_t wordB = 0xefcdab89;
     static const uint32_t wordC = 0x98badcfe;
     static const uint32_t wordD = 0x10325476;

     // Current hash state
    uint32_t a, b, c, d;

    // 64-element table T[1 ... 64] constructed from the
    // sine function. Let T[i] denote the i-th element of the table, which
    // is equal to the integer part of 4294967296 times abs(sin(i)), where i
    // is in radians.
    
    uint32_t T[64] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
        0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
        0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
        0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
        0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
        0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
        0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
    };

    // S specifies the per-calculated shift amounts
    uint32_t S[64] = {
        7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
        5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
        4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
        6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
    };

    
     // Initializing 4 Auxillary  functions
       /* F(X,Y,Z) = XY v not(X) Z
          G(X,Y,Z) = XZ v Y not(Z)
          H(X,Y,Z) = X xor Y xor Z
          I(X,Y,Z) = Y xor (X v not(Z))*/

     static uint32_t F(uint32_t x, uint32_t y, uint32_t z) {
        return (x & y) | (~x & z);
    }
    
    static uint32_t G(uint32_t x, uint32_t y, uint32_t z) {
        return (x & z) | (y & ~z);
    }
    
    static uint32_t H(uint32_t x, uint32_t y, uint32_t z) {
        return x ^ y ^ z;
    }
    
    static uint32_t I(uint32_t x, uint32_t y, uint32_t z) {
        return y ^ (x | ~z);
    }
    
    static uint32_t left_rotate(uint32_t x, uint32_t n) {
        return (x << n) | (x >> (32 - n));
    }
    
    // Function to convert string to byte vector
    vector<uint8_t> string_to_bytes(string& message) {
        return vector<uint8_t>(message.begin(), message.end());
    }
    
    // Padding Function
    void pad_message(vector<uint8_t>& message) {
        uint64_t  message_len=message.size(); // Length of message 

        /*  calculating the number of bits of message, 
            since each character is represented by 8 bits, total bits will be 
            message length times 8 */

        uint64_t message_bit_length = message_len * 8; 

        // a single "1" bit is appended to the message
        message.push_back(0x80);
        
        // "0" bits are appended so that the length in bits of 
        // the padded message becomes congruent to 448, modulo 512.
        
        while ((message.size() * 8) % 512 != 448) {
            message.push_back(0x00);
        }


         // Appending original length 
        for (int i = 0; i < 8; ++i) {
            message.push_back((message_bit_length >> (i * 8)) & 0xFF);
        }

    }

    void process_block(uint8_t * block){
        uint32_t X[16];  // Array to hold 16 thirty-two bit words
        for (int i = 0; i < 16; ++i) {
            X[i] = (block[i*4 + 3] << 24) |  // Byte 3 (MSB) shifted left 24 bits
                (block[i*4 + 2] << 16) |  // Byte 2 shifted left 16 bits
                (block[i*4 + 1] << 8)  |  // Byte 1 shifted left 8 bits
                (block[i*4]);            // Byte 0 (LSB) not shifted
        }

        // Initialzing Values from 4 word buffers
        uint32_t AA=wordA;
        uint32_t BB=wordB;
        uint32_t CC=wordC;
        uint32_t DD=wordD;

        // Process the block in 4 phases of 16 operations each
        for (int phase = 0; phase < 4; ++phase) {
            // Select the function and g-calculation for this phase
            uint32_t (*round_func)(uint32_t, uint32_t, uint32_t);
            int (*g_func)(int);
            
            switch (phase) {
                case 0:
                    round_func = F;
                    g_func = [](int i) { return i; };
                    break;
                case 1:
                    round_func = G;
                    g_func = [](int i) { return (5 * i + 1) % 16; };
                    break;
                case 2:
                    round_func = H;
                    g_func = [](int i) { return (3 * i + 5) % 16; };
                    break;
                case 3:
                    round_func = I;
                    g_func = [](int i) { return (7 * i) % 16; };
                    break;
            }

            // Processing 16 operations for this phase
            for (int op = 0; op < 16; ++op) {
                int i = phase * 16 + op;  // Global operation index (0-63)
                int g = g_func(i);
                
                uint32_t F_result = round_func(BB, CC, DD) + AA + T[i] + X[g];
                AA = DD;
                DD = CC;
                CC = BB;
                BB = BB + left_rotate(F_result, S[i]);
            }
        }

        // Add this block's hash to result so far
        a += AA;
        b += BB;
        c += CC;
        d += DD;
        
    }

    // Convert hash to hex string
    string digest_to_hex() const {
        ostringstream result;

        // setfill : Ensures that if a value has fewer than two hex digits (like 0x3), 
        // it gets padded with a 0 (so it becomes "03").
        result << hex << setfill('0');
        
        uint32_t words[] = {a, b, c, d};
        for (uint32_t word : words) {
            for (int i = 0; i < 4; ++i) {
                // std::setw(2): Sets width to 2 characters — 
                // this works with setfill('0') to guarantee two-digit output.
                result << setw(2) << ((word >> (i * 8)) & 0xFF);
            }
        }

        
        return result.str();
    }


    
public:
    MD5Hash() : a(wordA),b(wordB),c(wordC), d(wordD){}


    // Computing MD5 hash of a string
    string hashing(string& message){

        a=wordA;
        b=wordB;
        c=wordC;
        d=wordD;

        // Converting string message to bytes array 
        vector<uint8_t> bytes = string_to_bytes(message);
        pad_message(bytes);

        // Dividing the bytes array in bloCks of 512 bits/64 bytes
        for (size_t i = 0; i < bytes.size(); i += 64) {
            process_block(&bytes[i]);
        }
        
        // Return final hash
        return digest_to_hex();

    }
    
};

int main() {
    MD5Hash md5;
    
    string input;
    cout << "Enter a string to hash: ";
    getline(cin, input);
    
    string hash = md5.hashing(input);
    cout << "MD5 hash: " << hash << endl;
    
    return 0;
}
