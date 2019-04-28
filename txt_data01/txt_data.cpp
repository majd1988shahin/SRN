#include <QCoreApplication>
//#include "aes_srn.h"
#include <stdio.h>
#include <stdlib.h>
#include <openssl/aes.h>
#include <openssl/evp.h>

#include <QtDebug>
#include <iostream>
#include <fstream>
using namespace std;

class txt_data
{
public:
    void init()
    {
        ifstream fi("data.txt");
        if(fi.is_open())
        {
        std::string str;
        std::getline(fi, str);
        if(str=="SYS")
            return;
        }
        fi.close();
        ofstream myfile("data.txt",fstream::app);
          myfile << "SYS\n";
          myfile<<"PublicKey_sys"<<"\n\n\n\n\n";
            myfile<<"-------------------------\n";
          myfile.close();
    }
    int add_user(std::string username,unsigned int salt[] ,unsigned char* pass,unsigned char* privat,unsigned char* publicKey)
    {
        ifstream fi("data.txt");
        if(!fi.is_open())
            return 2;
        std::string str;
        int i=0;
            while (std::getline(fi, str))
            {
                if(i%7==0)
                // Process str
                if(str==username)
                    return 0;
                i++;
            }
            fi.close();

             ofstream f ("data.txt",fstream::app);//, ios::out | ios::ate);
           // f.seekg(0,ios::beg);
          f<<username<<endl<<salt[0]<<endl<<salt[1]<<endl<<pass<<endl<<privat<<endl<<publicKey<<endl;
          f<<"-------------------------\n";

          f.close();
        return 1;
    }

   int read(std::string username,unsigned int salt[2] , string &pass,
   string &privat,string &publicKey)
   {//يجب ارجاع أطوال سلاسل المحارف
       unsigned int saltt[2];
      // char a[3][1000]={{""}};

       ifstream fi("data.txt");
       if(!fi.is_open())
           return 2;
       std::string str,str1,str2,str3,str4,str5;
       int i=0;
           while (std::getline(fi, str))
           {
               if(i%7==0)
               // Process str
               if(str==username)
               {
                   std::getline(fi, str1);
                   saltt[0]=(unsigned int)std::atoi(&str1[0]);
                   std::getline(fi, str2);
                   saltt[1]=(unsigned int)std::atoi(&str2[0]);
                   salt=saltt;
                   std::getline(fi, pass);


                 //  strcpy(passt, &str[0]);
                /*   char passt[20]="";
                   char privatt[6]="";
                   char publickeyt[6]="";
                   str.copy(pass, str3.size() + 1);*/

                  // pass=&str3[0];
                 //  pass_size=str3.size();
       // unsigned char* pass = reinterpret_cast<unsigned char*>(passt);                  //  pass=(unsigned char *)passt;

                    std::getline(fi, privat);
                    //strcpy(privat, &str4[0]);
                    // privat=(unsigned char *)privatt;
                    //privat=&str4[0];
                    //privat_size=str4.size();

                     std::getline(fi, publicKey);
                    // publicKey=&str5[0];
                    // publicKey_size=str5.size();
                    //strcpy(publicKey, &str5[0]);
                    //  publicKey=(unsigned char *)publickeyt;
                   return 0;
               }
               i++;
           }
           fi.close();
           return 1;
   }

};
