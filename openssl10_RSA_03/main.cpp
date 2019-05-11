#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <stdio.h>
#include<string.h>
#include<iostream>

#include <srn_rsa.cpp>
#include<string.h>

using namespace std;

static char * ReadAllBytes(const char * filename, int * read)
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
int main(){

    SRN_RSA *a=new SRN_RSA();
    a->op();



/*unsigned char * e=b->readf("majd.txt",c);
cout<<e<<endl;
unsigned char * ee=b->readf("majd.txt",c);
cout<<ee<<endl;
char * f=b->readf2("majd.txt",&c);
cout<<f<<endl;
unsigned char * d=(unsigned char * )ReadAllBytes("majd.txt",&c);
cout<<d<<endl;
*/

//a->op();
return 0;

}
