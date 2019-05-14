
#include <iostream>
#include<stdio.h>
#include<math.h>
#include <fstream>

#include <QCoreApplication>

#include <stdlib.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <iostream>
#include<string.h>
using namespace std;

 class SRN_AES
{
private:


     EVP_CIPHER_CTX* e_ctx = EVP_CIPHER_CTX_new();
     EVP_CIPHER_CTX* d_ctx = EVP_CIPHER_CTX_new();


    void p(string inputf)
    {

    }
public:

    SRN_AES(){

    }

    bool aes_file_enc(string filename,string pass)
    {
        ifstream fi( filename, ios::binary | ios::ate);
        int size= fi.tellg();
        fi.close();

        unsigned char *buffer=(unsigned char *)alloca(size);
        unsigned char *buffer2=(unsigned char *)alloca(200000);

        //int size=0;
        buffer=readf(&filename[0],size);
      //  cout<<size<<endl;

        unsigned int salt[] = {12345, 54321};
              int key_data_len = static_cast<int>(strlen(reinterpret_cast<char *>(&pass[0])));
              SRN_AES *aes=new SRN_AES();
                 aes->aes_init((unsigned char*)&pass[0],key_data_len,reinterpret_cast<unsigned char *>(&salt));

         string f=filename+".aes";


         buffer2=aes->aes_encrypt(buffer,&size);
         writef(&f[0],buffer2,size);
         free(buffer);free(buffer2);
         free(aes);
         //remove(&filename[0]);
        return (size>0);
    }
    bool aes_file_dec(string filename,string pass)
    {
        ifstream fi( filename, ios::binary | ios::ate);
        int size= fi.tellg();
        fi.close();

        unsigned char *buffer=(unsigned char *)alloca(size);
        unsigned char *buffer2=(unsigned char *)alloca(200000);

        //int size=0;
        string f=filename+".aes";
        buffer=readf(&f[0],size);
        //cout<<size<<endl;

        unsigned int salt[] = {12345, 54321};
              int key_data_len = static_cast<int>(strlen(reinterpret_cast<char *>(&pass[0])));
              SRN_AES *aes=new SRN_AES();
                 aes->aes_init((unsigned char*)&pass[0],key_data_len,reinterpret_cast<unsigned char *>(&salt));

         buffer2=aes->aes_decrypt(buffer,&size);
         writef(&filename[0],buffer2,size);
         free(buffer);free(buffer2);
         free(aes);
        // remove(&filename[0]);
        return (size>0);
    }
    unsigned char*  aes_file_dec_READ_ONLY(string filename,string pass,int &out_size)
    {
        ifstream fi( filename, ios::binary | ios::ate);
        int size= fi.tellg();
        fi.close();

        unsigned char *buffer=(unsigned char *)alloca(size);
        unsigned char *buffer2=(unsigned char *)alloca(200000);

        //int size=0;
        string f=filename+".aes";
        buffer=readf(&f[0],size);
        //cout<<size<<endl;

        unsigned int salt[] = {12345, 54321};
              int key_data_len = static_cast<int>(strlen(reinterpret_cast<char *>(&pass[0])));
              SRN_AES *aes=new SRN_AES();
                 aes->aes_init((unsigned char*)&pass[0],key_data_len,reinterpret_cast<unsigned char *>(&salt));

         buffer2=aes->aes_decrypt(buffer,&size);
         writef(&filename[0],buffer2,size);
         free(buffer);//free(buffer2);
         free(aes);
        // remove(&filename[0]);
         out_size=size;
        return buffer2;
    }

    int aes_init(string pass)
    {
        unsigned int salt[] = {12345, 54321};
       int key_data_len = static_cast<int>(strlen(reinterpret_cast<char *>(&pass[0])));
              SRN_AES *aes=new SRN_AES();
                 aes->aes_init((unsigned char*)&pass[0],pass.length(),reinterpret_cast<unsigned char *>(&salt));
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


   static unsigned char * readf(char* name,int&size )
    {
        ifstream f( name, ios::binary | ios::ate);
        size= f.tellg();
        f.close();


         char * i = new  char[size] ;
         fstream myFile (name );
         myFile.read (i, size);
         myFile.close();
        return (unsigned char*)i;
     }
    char * readf2 (const char * filename, int * read)
    {
        ifstream ifs(filename, ios::binary|ios::ate);
        ifstream::pos_type pos = ifs.tellg();
        int length = pos;
        char *pChars = new char[length];
        ifs.seekg(0, ios::beg);
        ifs.read(pChars, length);
        ifs.close();
        *read = length;
        return pChars;
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

};
