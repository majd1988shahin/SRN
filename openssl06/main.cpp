#include <QCoreApplication>
#include "aes_srn.h"
#include <stdio.h>
#include <stdlib.h>
#include <openssl/aes.h>
#include <openssl/evp.h>

#include <QtDebug>

char buffer[20000000];
int bsize=0;
unsigned char* plainText;

unsigned char *cipher_text;
int cipher_text_size=0;
unsigned char * readf(char* name,int&size )
{
    ifstream f( name, ios::binary | ios::ate);
    size= f.tellg();
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
int main(int argc, char *argv[])
{
    //read write file test
    /*
unsigned char *a;
int s=0;
cout<<sizeof(unsigned char)<<endl;
a=readf("1.pdf",s);
writef("2.pdf",a,s);*/
//cout<<s;

    unsigned int salt[] = {12345, 54321};
      unsigned char key_data[] = "01234567890123456789012345678901";
      int key_data_len = static_cast<int>(strlen(reinterpret_cast<char *>(key_data)));
    AES_srn *aes=new AES_srn();
    aes->aes_init(key_data,key_data_len,reinterpret_cast<unsigned char *>(&salt));

    //algorithm test

    int csize=11;
plainText=( unsigned char*)((char *)("majd shahin"));
cout<<plainText<<endl;
    cipher_text=aes->aes_encrypt(plainText,& csize);
cout<<cipher_text<<endl;
plainText=NULL;
cout<<aes->aes_decrypt(cipher_text,&csize)<<endl;

   int size =0;
   unsigned char *data;
   data=readf("1.pdf",size);
   //cout<<"data : "<<data<<endl;
   unsigned char * cdata;
   cdata=aes->aes_encrypt(data,&size);
   writef("srn",cdata,size);
   cdata=NULL;
   size=0;

   data=NULL;
   cdata=readf("srn",size);
  // cout<<"restored data : "<<aes->aes_decrypt(cdata,&size)<<endl;

   data=aes->aes_decrypt(cdata,&size);
   writef("2.pdf",data,size);
    return 0;
}
