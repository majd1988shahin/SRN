
#include <iostream>
#include<stdio.h>
#include<math.h>
#include <fstream>

#include <QCoreApplication>

#include <stdlib.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <iostream>
using namespace std;
 class AES_srn
{
private:


     EVP_CIPHER_CTX* e_ctx = EVP_CIPHER_CTX_new();
     EVP_CIPHER_CTX* d_ctx = EVP_CIPHER_CTX_new();


    void p(string inputf)
    {

    }
public:

    AES_srn(){

    }

    int aes_init(unsigned char *key_data,
                 int key_data_len, unsigned char *salt)
    {
        int i, nrounds = 5;
        unsigned char key[32], iv[32];

        i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), salt, key_data, key_data_len, nrounds, key, iv);
        if (i != 32)
        {
            printf("Key size is %d bits - should be 256 bits\n", i);
            exit(-1);
        }

        EVP_CIPHER_CTX_init(e_ctx);
        EVP_EncryptInit_ex(e_ctx, EVP_aes_256_cbc(), nullptr, key, iv);
        EVP_CIPHER_CTX_init(d_ctx);
        EVP_DecryptInit_ex(d_ctx, EVP_aes_256_cbc(), nullptr, key, iv);

        return 0;
    }

    unsigned char *aes_encrypt( unsigned char *plaintext, int *len)
    {
        int c_len = *len + AES_BLOCK_SIZE, f_len = 0;
        unsigned char *ciphertext = reinterpret_cast<unsigned char*>(malloc(static_cast<size_t>(c_len)));

        EVP_EncryptInit_ex(e_ctx, nullptr, nullptr, nullptr, nullptr);
        EVP_EncryptUpdate(e_ctx, ciphertext, &c_len, plaintext, *len);
        EVP_EncryptFinal_ex(e_ctx, ciphertext+c_len, &f_len);

        *len = c_len + f_len;
        return ciphertext;
    }

    unsigned char *aes_decrypt(unsigned char *ciphertext, int *len)
    {
        int p_len = *len, f_len = 0;
        unsigned char *plaintext = reinterpret_cast<unsigned char*>(malloc(static_cast<size_t>(p_len + AES_BLOCK_SIZE)));

        EVP_DecryptInit_ex(d_ctx, nullptr, nullptr, nullptr, nullptr);
        EVP_DecryptUpdate(d_ctx, plaintext, &p_len, ciphertext, *len);
        EVP_DecryptFinal_ex(d_ctx, plaintext+p_len, &f_len);

        *len = p_len + f_len;
        return plaintext;
    }


};

