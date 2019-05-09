//#include <QCoreApplication>
#include"sha.cpp"
#include "openssl/sha.h"
#include<iostream>
using namespace std;

//unsigned char[] gethash
int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);

    string line ="Yansdfsdfik";
   SHA sha;
   sha.get_hash(line);
   string s,s1;

   s=sha.get_hash(line);
   cout<<s<<endl;
   cout<<sha.gleich_string_hash(line,s)<<endl;

   s=sha.sha256_file("sha.o");
   cout<<s<<endl;
   s1=sha.sha256_file("sha.o");
   s=sha.get_hash(line);
   cout<<sha.gleich_file_hash("sha.o",s)<<endl;
    cout<<sha.gleich_file_hash("sha.o",s1)<<endl;

    return 0 ;
}
