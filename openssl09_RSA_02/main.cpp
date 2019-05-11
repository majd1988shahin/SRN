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
using namespace std;

int main(){

SRN_RSA *a=new SRN_RSA();
a->op();


}
