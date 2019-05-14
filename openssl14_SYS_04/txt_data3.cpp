#include <QCoreApplication>
//#include "aes_srn.h"
#include <stdio.h>
#include <stdlib.h>
#include <openssl/aes.h>
#include <openssl/evp.h>

#include <QtDebug>
#include <iostream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>
#include <list>
#include "base64.h"
#include "tools.cpp"
using namespace std;

class txt_data3
{
public:

    class paar_str
    {
    public:
        string name;
        string v;
        paar_str(){}
        paar_str(string name,string v){this->name=name;this->v=v;}
    };
    class data
    {
    private:
        ~data(){}
    public:

        string username;
        string passhash;
        string sign;
        list<paar_str*> owned_data;
        list<paar_str*> available_data;
        data() {}
        data(string username,string passhash,string sign,
             list<paar_str*> owned_data,list<paar_str*> available_data)
        {this->username=username;this->passhash=passhash;this->sign=sign;
        this->owned_data=owned_data;this->sign=sign;
        this->available_data=available_data;}

    };


    list<data*> users;



    void show()
    {
        for ( data * user : users)
        {

            string s=user->username;
            const char* s1="\n---\n";
            s.append(s1);

            cout<<(char*)&s[0];

            s=user->passhash;   const char* s2="\n+++\n";   s.append(s2);
            cout<<(char*)&s[0];

            s=user->sign;   const char* s3="\n,,,\n";   s.append(s3);
            cout<<(char*)&s[0];

            for(paar_str * p:user->owned_data)
            {
                s=p->name;   const char* s4="\n";   s.append(s4);
                s.append(p->v); const char* s5="\n@@@\n";s.append(s5);
                cout<<(char*)&s[0];
            }
            cout<<("===\n");
            for(paar_str * p:user->available_data)
            {
                s=p->name;   const char* s4="\n";   s.append(s4);
                s.append(p->v); const char* s5="\n&&&\n";s.append(s5);
                cout<<(char*)&s[0];
            }
            cout<<("^^^\n");

        }
    }
    void write()
    {
        if(users.empty())
        {
            cout<<"No data to save\n";
            return;
        }

        ofstream f (BASE_FILE);//, ios::out | ios::ate);
        if(!f.is_open())
        {
            cout<<"failed to write data\n";
            return;
        }

        for ( data * user : users)
        {

            string s=user->username;
            std::string e = base64_encode(reinterpret_cast<const unsigned char*>(s.c_str()), s.length());
            const char* s1="\n---\n";

            f<<(char*)&e[0]<<s1;


            s=user->passhash;
            e = base64_encode(reinterpret_cast<const unsigned char*>(s.c_str()), s.length());
            const char* s2="\n+++\n";
            f<<(char*)&e[0]<<s2;

            s=user->sign;
            const char* s3="\n,,,\n";
            e = base64_encode(reinterpret_cast<const unsigned char*>(s.c_str()), s.length());
            f<<(char*)&e[0]<<s3;

            if(!user->owned_data.empty())
            for(paar_str * p:user->owned_data)
            {
                s=p->name;
               string e = base64_encode(reinterpret_cast<const unsigned char*>(s.c_str()), s.length());
                f<<(char*)&e[0]<<endl;
                s=p->v;
                e = base64_encode(reinterpret_cast<const unsigned char*>(s.c_str()), s.length());
                const char* s5="\n";
                f<<(char*)&e[0]<<s5;
            }

            f<<("===\n");
            if(!user->available_data.empty())
            for(paar_str * p:user->available_data)
            {
                s=p->name;
                e = base64_encode(reinterpret_cast<const unsigned char*>(s.c_str()), s.length());
                f<<(char*)&e[0]<<endl;
                s=p->v; const char* s6="\n";
                e = base64_encode(reinterpret_cast<const unsigned char*>(s.c_str()), s.length());
                f<<(char*)&e[0]<<s6;
            }
            f<<("***\n");

        }
    }

    bool check_exist(std::string username)
   {
        for(data *p :users)
        {
            if(p->username==username)
                return true;
        }
        return false;
   }
    int add_user(std::string username,string h_pass,string sign)
    {
        data *user=new data();
        user->username=username;
        user->passhash=h_pass;

        user->sign=sign;
        users.push_back(user);
        return 1;
    }


 /*  int read(std::string username, string &hpass,string &sign)
   {//يجب ارجاع أطوال سلاسل المحارف

           return 1;
   }
*/
   void read()
   {
       ifstream fi(BASE_FILE);
       if(!fi.is_open())
           return ;


       while(1)
       {
           string s,username,hpass,sign;

           list<paar_str*> owned_data;
           list<paar_str*> available_data;
           s="";
           username="";
           while (1) {
               if(!std::getline(fi,s))
                       goto end;
               if(s=="---")
                   break;
               else
                   username=s;
           }
           s="";
           hpass="";
           while (1) {
               if(!std::getline(fi,s))
                       goto end;
               if(s=="+++")
                   break;
               else
                   hpass=s;
           }
           s="";
           sign="";
           while (1) {
               if(!std::getline(fi,s))
                       goto end;
               if(s==",,,")
                   break;
               else
                   sign=s;
           }

           while (1)
           {string name,dsign="",s;
               if(!std::getline(fi,s))
                       goto end;
               if (s=="===")
                   break;
               else
                   name=s;

               if(!std::getline(fi,s))
                       goto end;
               dsign=s;
               std::string _name = base64_decode(name);
               std::string _dsign = base64_decode(dsign);

                   owned_data.push_back(new paar_str(_name,_dsign));
               }

           while (1)
           {string name,key="",sn;
               if(!std::getline(fi,sn))
                       goto end;
               if (sn=="***")
                   break;
               else
                name=sn;

               if(!std::getline(fi,sn))
                   goto end;
                   key=sn;
               std::string _name = base64_decode(name);
               std::string _key = base64_decode(key);
               //paar_str *p2=new paar_str(_name,_key);
               available_data.push_back(new paar_str(_name,_key));
           }

           //cout<<username<<endl;
           std::string _username = base64_decode(username);
           std::string _hpass = base64_decode(hpass);
           std::string _sign = base64_decode(sign);

           users.push_back(new data(_username,_hpass,_sign,owned_data,available_data));
       }
end:;
   }

};
