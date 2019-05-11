#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <stdio.h>
#include<string.h>
#include<iostream>
using namespace std;
class SRN_RSA
{

public:
    void op()
    {
        //RSA *rsa=createRSAWithFilename("majd_public.pem",1);
        unsigned char  encrypted[4098]={};
        unsigned char decrypted[4098]={};

        char plainText[2048/8] = "Hello worlddsfhdgjfdhgddfhkjhdgjgfkhg"; //key length : 2048

        string user="majd";
        generate_key(user);
       // char result[2048/8];

//int encrypted_length= public_encrypt_user((unsigned char*)plainText,strlen(plainText),user,(unsigned char*)encrypted);
public_encrypt_user((unsigned char*)plainText,strlen(plainText),user,(unsigned char*)encrypted);

int decrypted_length = private_decrypt_user((unsigned char*)encrypted,256,user, (unsigned char*)decrypted);
        cout<<plainText<<endl;
        //cout<<encrypted_length<<endl;
        cout<<decrypted<<endl;

    }
    bool generate_key(std::string userName)
        {
            int             ret = 0;
            RSA             *r = NULL;
            BIGNUM          *bne = NULL;
            BIO             *bp_public = NULL, *bp_private = NULL;

            int             bits = 2048;
            unsigned long   e = RSA_F4;
        do{
            bne = BN_new();
            ret = BN_set_word(bne,e);
            if(ret != 1){
                break;
            }

            r = RSA_new();
            ret = RSA_generate_key_ex(r, bits, bne, NULL);
            if(ret != 1){
                break;
            }


        std::string pbK_Name=userName+"_public.pem";
            bp_public = BIO_new_file(pbK_Name.c_str(), "w+");
            ret = PEM_write_bio_RSAPublicKey(bp_public, r);
            if(ret != 1){
                break;
            }

            std::string pK_Name=userName+"_private.pem";
            bp_private = BIO_new_file(pK_Name.c_str(), "w+");
            ret = PEM_write_bio_RSAPrivateKey(bp_private, r, NULL, NULL, 0, NULL, NULL);

            // 4. free
            }
            while (0)
        ;

            BIO_free_all(bp_public);
            BIO_free_all(bp_private);
            RSA_free(r);
            BN_free(bne);


            return (ret == 1);
        }

    RSA * readRSA(std::string username,int publicK)
    {
        RSA *rsa=RSA_new() ;
        FILE *f;

      //  keybio = BIO_new_mem_buf(key, -1);

        if(publicK)
        {

            std::string filename=username+"_public.pem";
            f = fopen(filename.c_str(), "rb");
            rsa = PEM_read_RSAPublicKey(f, &rsa,NULL, NULL);

        }
        else
        {
            std::string filename=username+"_private.pem";
            f = fopen(filename.c_str(), "rb");
            rsa = PEM_read_RSAPrivateKey(f, &rsa,NULL, NULL);
        }

        return rsa;
    }


    int public_encrypt_user(unsigned char * data,int data_len,string username, unsigned char *encrypted)
    {
        RSA * rsa = readRSA(username,1);
        int result = RSA_public_encrypt(data_len,data,encrypted,rsa,padding);
        return result;
    }


    int private_decrypt_user(unsigned char * enc_data,int data_len,string username, unsigned char *decrypted)
    {
        RSA * rsa = readRSA(username,0);
        int  result = RSA_private_decrypt(data_len,enc_data,decrypted,rsa,padding);
        return result;
    }

    int private_encrypt_user(unsigned char * data,int data_len,string username, unsigned char *encrypted)
    {
        RSA * rsa = readRSA(username,0);
        int result = RSA_private_encrypt(data_len,data,encrypted,rsa,padding);
        return result;
    }

    int public_decrypt_user(unsigned char * enc_data,int data_len,string username, unsigned char *decrypted)
    {
        RSA * rsa = readRSA(username,1);
        int  result = RSA_public_decrypt(data_len,enc_data,decrypted,rsa,padding);
        return result;
    }

private:

    int padding = RSA_PKCS1_PADDING;

    RSA * createRSA(unsigned char * key,int publick)
    {
        RSA *rsa= NULL;
        BIO *keybio ;
        keybio = BIO_new_mem_buf(key, -1);
        if (keybio==NULL)
        {
            printf( "Failed to create key BIO");
            return 0;
        }
        if(publick)
        {
            rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa,NULL, NULL);
        }
        else
        {
            rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa,NULL, NULL);
        }
        if(rsa == NULL)
        {
            printf( "Failed to create RSA");
        }

        return rsa;
    }
    int public_encrypt(unsigned char * data,int data_len,unsigned char * key, unsigned char *encrypted)
    {
        RSA * rsa = createRSA(key,1);
        int result = RSA_public_encrypt(data_len,data,encrypted,rsa,padding);
        return result;
    }
    int private_decrypt(unsigned char * enc_data,int data_len,unsigned char * key, unsigned char *decrypted)
    {
        RSA * rsa = createRSA(key,0);
        int  result = RSA_private_decrypt(data_len,enc_data,decrypted,rsa,padding);
        return result;
    }

    int private_encrypt(unsigned char * data,int data_len,unsigned char * key, unsigned char *encrypted)
    {
        RSA * rsa = createRSA(key,0);
        int result = RSA_private_encrypt(data_len,data,encrypted,rsa,padding);
        return result;
    }
    int public_decrypt(unsigned char * enc_data,int data_len,unsigned char * key, unsigned char *decrypted)
    {
        RSA * rsa = createRSA(key,1);
        int  result = RSA_public_decrypt(data_len,enc_data,decrypted,rsa,padding);
        return result;
    }
    void printLastError(char *msg)
    {
        char * err =(char*) malloc(130);;
        ERR_load_crypto_strings();
        ERR_error_string(ERR_get_error(), err);
        printf("%s ERROR: %s\n",msg, err);
        free(err);
    }


   };
