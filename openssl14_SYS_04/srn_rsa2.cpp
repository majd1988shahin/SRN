#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <stdio.h>
#include<string.h>
#include<iostream>
#include <stdio.h>
#include <string.h>

#include"srn_aes.cpp"
#include "base64.h"
using namespace std;
class SRN_RSA2
{

public:



    void op2()
    {
        //RSA *rsa=createRSAWithFilename("majd_public.pem",1);
        unsigned char  encrypted[4098];
        unsigned char decrypted[4098]={};

        char plainText[2048/8] ={}; //key length : 2048

        string user="RSA_TEST";
        string pass ="test";

        string key="hi";//generate_random_pass();

        strncpy((char*)plainText,&key[0],key.length());

        string enc="";


        //generate_key(user2,pass);
       int enc_size= public_encrypt_user((unsigned char*)&key[0],key.length(),(unsigned char*)&enc[0],user);

        int decrypted_length = private_decrypt_user((unsigned char*)&enc[0],enc_size, (unsigned char*)decrypted,user,pass);
                cout<<key<<endl;

        std::string de_key(reinterpret_cast<char*>(decrypted));
                cout<<de_key<<endl;
                cout<<key.size()<<endl;
                cout<<enc_size<<endl;

                cout<<decrypted_length<<endl;


       // char result[2048/8];
/*
//int encrypted_length= public_encrypt_user((unsigned char*)plainText,strlen(plainText),user,(unsigned char*)encrypted);

*/
    }

    bool generate_key(std::string userName,string pass)
        {
            int             ret = 0;
            RSA             *r = NULL;
            BIGNUM          *bne = NULL;
            BIO             *bp_public = NULL, *bp_private = NULL;

            int             bits = 2048;
            unsigned long   e = RSA_F4;


            std::string pbK_Name=userName+"_public.pem";
            std::string pK_Name=userName+"_private.pem";
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

            bp_public = BIO_new_file(pbK_Name.c_str(), "w+");
            ret = PEM_write_bio_RSAPublicKey(bp_public, r);
            if(ret != 1){
                break;
            }


            bp_private = BIO_new_file(pK_Name.c_str(), "w+");
            ret = PEM_write_bio_RSAPrivateKey(bp_private, r, NULL, NULL, 0, NULL, NULL);


            // 4. free
            }
            while (0);

            BIO_free_all(bp_public);
            BIO_free_all(bp_private);
            RSA_free(r);
            BN_free(bne);

            aes_privatek_enc(pK_Name,pass);


            return (ret == 1);
        }


    string public_encrypt_user(string plain,string username)
    {
        unsigned char  encrypted[4098];
        RSA * rsa = readRSA_pub(username);
        int size = RSA_public_encrypt(plain.size(),(unsigned char*)&plain[0],encrypted,rsa,padding);
       // string result=(reinterpret_cast<char*>(encrypted));
        string result;
       // for(int i=0;i<size;i++)
        result.assign(( char*)encrypted,size);
        return result;
    }

    int public_encrypt_user(unsigned char * data,int data_len, unsigned char *encrypted,string username)
    {
        RSA * rsa = readRSA_pub(username);
        int result = RSA_public_encrypt(data_len,data,encrypted,rsa,padding);
        return result;
    }

    string private_decrypt_user(string enc,string username,string pass)
    {
        unsigned char  d[4098];
       RSA * rsa = readRSA_pri(username,pass);
        int size = RSA_private_decrypt(enc.size(),(unsigned char*)&enc[0],d,rsa,padding);
         string result;//=(reinterpret_cast<char*>(d));
         result.assign((char*)d,size);
        return result;
    }

    int private_decrypt_user(unsigned char * enc_data,int data_len, unsigned char *decrypted,string username,string pass)
    {
        RSA * rsa = readRSA_pri(username,pass);
        int  result = RSA_private_decrypt(data_len,enc_data,decrypted,rsa,padding);
        return result;
    }

    string private_encrypt_user(string plain,string username,string pass)
    {
        unsigned char  e[4098];
       RSA * rsa = readRSA_pri(username,pass);
        int size = RSA_private_encrypt(plain.size(),(unsigned char*)&plain[0],e,rsa,padding);
         string result;//=(reinterpret_cast<char*>(d));
         result.assign((char*)e,size);
        return result;
    }
    int private_encrypt_user(unsigned char * data,int data_len, unsigned char *encrypted,string username,string pass)
    {
        RSA * rsa = readRSA_pri(username,pass);
        int result = RSA_private_encrypt(data_len,data,encrypted,rsa,padding);
        return result;
    }

    string public_decrypt_user(string enc,string username)
    {
        unsigned char  d[4098];
       RSA * rsa = readRSA_pub(username);
        int size = RSA_public_decrypt(enc.size(),(unsigned char*)&enc[0],d,rsa,padding);
         string result;//=(reinterpret_cast<char*>(d));
         result.assign((char*)d,size);
        return result;
    }
    int public_decrypt_user(unsigned char * enc_data,int data_len, unsigned char *decrypted,string username)
    {
        RSA * rsa = readRSA_pub(username);
        int  result = RSA_public_decrypt(data_len,enc_data,decrypted,rsa,padding);
        return result;
    }

private:

    int padding = RSA_PKCS1_PADDING;

    unsigned char* aes_privatek_read(string username,string pass,int &size)
    {
        unsigned int salt[] = {12345, 54321};
       int key_data_len = static_cast<int>(strlen(reinterpret_cast<char *>(&pass[0])));
              SRN_AES *aes=new SRN_AES();
                 aes->aes_init((unsigned char*)&pass[0],pass.length(),reinterpret_cast<unsigned char *>(&salt));

         string f=username+"_private.pem";


         //buffer2=aes->aes_decrypt(buffer,&size);

         //writef(&f[0],buffer2,size);
      //   free(buffer);//free(buffer2);

         unsigned char * result =aes->aes_file_dec_READ_ONLY(f,pass,size);
        return result ;
    }
    unsigned char * readf(char* name,int&size )
    {
        ifstream f( name, ios::binary | ios::ate);
        size= f.tellg();
        //cout<<size<<endl;
        f.close();

        unsigned char * in = (unsigned char *) malloc(size);
         fstream myFile (name, ios::in  | ios::binary);
         myFile.read ((char*)in, size);
         myFile.close();
        return in;
     }
    void writef(char* name , unsigned char * data, int size)
    {
        std::ofstream f;
        f.open(name);
        //f<<data;

    f.write ((char*)data, size); f.close();
      /*  fstream myFile (name, ios::out | ios::binary);
        myFile.write ((char*)&data[0], sizeof (unsigned char)*size);
        myFile.close();*/
    }
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

    void printLastError(char *msg)
    {
        char * err =(char*) malloc(130);;
        ERR_load_crypto_strings();
        ERR_error_string(ERR_get_error(), err);
        printf("%s ERROR: %s\n",msg, err);
        free(err);
    }

    RSA * readRSA_pub(std::string username)
    {
        RSA *rsa=RSA_new() ;
        FILE *f;


            std::string filename=username+"_public.pem";
            f = fopen(filename.c_str(), "rb");
            rsa = PEM_read_RSAPublicKey(f, &rsa,NULL, NULL);

        return rsa;
    }

    RSA * readRSA_pri(std::string username,string pass)
    {
     /*   RSA *rsa=RSA_new() ;
        FILE *f;


            std::string filename=username+"_private.pem";
            f = fopen(filename.c_str(), "rb");
            rsa = PEM_read_RSAPrivateKey(f, &rsa,NULL, NULL);

        return rsa;*/

        RSA *rsa= NULL;
        BIO *keybio ;
        int size=0;
        unsigned char* key =aes_privatek_read(username,pass,size);
        keybio = BIO_new_mem_buf(key, -1);
        if (keybio==NULL)
        {
            printf( "Failed to create key BIO");
            return 0;
        }

        rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa,NULL, NULL);
        if(rsa == NULL)
        {
            printf( "Failed to create RSA");
        }


        return rsa;
    }

    bool aes_privatek_enc(string filename,string pass)
    {
        unsigned int salt[] = {12345, 54321};
              int key_data_len = static_cast<int>(strlen(reinterpret_cast<char *>(&pass[0])));
              SRN_AES *aes=new SRN_AES();
                 aes->aes_init((unsigned char*)&pass[0],key_data_len,reinterpret_cast<unsigned char *>(&salt));

       aes->aes_file_enc(filename,pass);
        free(aes);
       //  remove(&filename[0]);
        return true;
    }
   };
