//#include <QCoreApplication>
//#include"sha.cpp"
#include "openssl/sha.h"
#include<iostream>
#include <fstream>
using namespace std;
class SHA
{
public:
    SHA(){;}

    std::string get_hash(std::string line)
    {
        unsigned char hash[SHA256_DIGEST_LENGTH];
            SHA256_CTX sha256;
            SHA256_Init(&sha256);
            SHA256_Update(&sha256, line.c_str(), line.length());
            SHA256_Final(hash, &sha256);
            std::string myString((char *)hash, SHA256_DIGEST_LENGTH);
         //   cout<<myString<<endl;
            return myString;
    }

    bool gleich_string_hash(std::string line,std::string hash)

    {
        return (hash==get_hash(line));
    }
    bool gleich_file_hash(std::string path,std::string hash)

    {
        return (hash==sha256_file(path));
    }

    std::string  sha256_file(std::string path)
    {
        std::ifstream file( &path[0] );
        if(!file)
        {
            cout<<"error file not found"<<endl;
            return NULL;
        }
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        string line;
        while (std::getline(file, line))
                SHA256_Update(&sha256, line.c_str(), line.length());
        SHA256_Final(hash, &sha256);

      //  cout<<hash<<endl;
        std::string hashcode((char *)hash, SHA256_DIGEST_LENGTH);

        cout<<"F sha256_file"<<endl;
        cout<<hashcode<<endl;

        return hashcode;
    }
}
;
