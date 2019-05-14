#include<sha.cpp>
//#include<srn_rsa.cpp>
#include<srn_rsa2.cpp>
//#include<srn_aes.cpp>
#include<bits/stdc++.h>

//#include<txt_data.cpp>
#include<txt_data3.cpp>
#include <termios.h>
#include <unistd.h>
#include <ctime>
#include<stdio.h>
//#include<tools.cpp>
using namespace std;
class SYS
{
private:
    string active_user;
    int sys_check()
    {
        ////check files names
        ifstream f1(BASE_FILE);
        ifstream f2("SYS_public.pem");
        if(!(f1.is_open()&f2.is_open()))
            return 1;

        f1.close();f2.close();
        data->read();

        ////check existans of SYS user
        bool temp=false;
        for(txt_data3::data *p:data->users)
        {
           if(p->username=="SYS")
               temp=true;
        }
        if(temp==false)
            return 2;

        //////////
        ///check signaturs for all users
        ///
        bool error=false;
        for(txt_data3::data *p:data->users)
        {
            string s=p->username+"_public.pem";
           if(!sign_check(p->sign,p->username,s,false))
           {
               cout<<"Error in Signeture by user : "<<p->username<<endl;
               error=true;
           }
        }
        if(error)
              return 3;
        cout<<" check of Signaturs is Complet\n";
        return 0;
    }

    int init()
    {
        cout<<"System initializing\n";
        char c;
        cout<<"press \'s\' to setup or any another key to exit\n";
        cin>>c;
        if(c!='s')
        {
           cout<<"exit withuot setup\n";
           return 2;//exit without setup
        }
        cout<<"!!! Init Process !!!\n";
        cout<<"pleas add the SYS password\n";
        add_user("SYS");
        cout<<"please save the file SYS_private.pem in flash memory\n press any key when finished\n";

        data->write();
    }

public:
    txt_data3 *data=new txt_data3();
    string get_active_user(){return active_user;}

    bool startup()
    {
        int r=sys_check();
        if(r==3)
        {
            cout<<"signatuer is not correct\n";
            return false;
        }
        if(r==0)
        {
            cout<< "the system is ready\n";
            return true;
        }
        init();
        cout<<"Start up is complet\n\n";
        return true;
    }
    void add_user(string username)
    {
        if(data->check_exist(username))
        {
            cout<<"username :"<<username<<" is allready exist\n";
            return;
        }

        string pass1;
            cout<<"Please enter password:\n";
            cin>>pass1;

        SRN_RSA2 *rsa=new SRN_RSA2();
            rsa->generate_key(username,pass1);

        SHA *sha=new SHA();
        std::string hash_pass=sha->get_hash(pass1);
        std::string pk_name=username+"_public.pem";
        std::string sign=sign_create(username,pass1,pk_name,false);

        data->add_user(username,hash_pass,sign);
        if(data->check_exist(username))
            cout<<"Creating new user : "<<username<<" success\n";
        if(username!="SYS")
        {
           string s =username+"_private.pem";
           remove(&s[0]);
        }
    }
    void add_user()
    {
        string username;
        cout<<"Creating new user\nPlease enter username:\n";
        cin>>username;

        add_user(username);

    }


    void add_file2(string filename)
    {

        if(active_user.empty())
                { cout<<"Please login first\n";return;      }
                for(txt_data3::data * p:data->users)
                {
                    for(txt_data3::paar_str *f:p->owned_data)
                    {
                        if(f->name==filename)
                        {
                            cout<<"this file allready encrpted\n";
                            return;
                        }
                    }
                }
         ifstream fi(filename);
         if(!fi.is_open()){cout<<"File name :"<<filename<<" is not correct\n";   return ;}
         fi.close();


         string user_pass;
            cout<<"enter your password : "<<active_user<<endl;
             cin>>user_pass;
             if(!check_pass_for_aktive_user(user_pass))return;

        string key=generate_random_pass();
            SRN_AES *aes=new SRN_AES();
            aes->aes_file_enc(filename,key);

        SRN_RSA2 *rsa=new SRN_RSA2();
        string v_key=rsa->public_encrypt_user(key,active_user);
        string SYS_key=rsa->public_encrypt_user(key,"SYS");
        ///////////sign
        ///

            string sign=sign_create(active_user,user_pass,filename,true);
        ///////
        for(txt_data3::data *p:data->users)
        {
            if(p->username==active_user)
            {
                p->available_data.push_back(new txt_data3::paar_str(filename,v_key));
                p->owned_data.push_back(new txt_data3::paar_str(filename,sign));
            }
            if(p->username=="SYS")
            {
                p->available_data.push_back(new txt_data3::paar_str(filename,SYS_key));
            }
        }

        data->write();
        remove(&filename[0]);

    }

    void file_dec2(string filename)
    {
        if(active_user.empty())
                { cout<<"Please login first\n";return;      }
        string v_filename=filename+".aes";
        ifstream fi(v_filename);
        if(!fi.is_open()){cout<<"File name :"<<filename<<" is not correct\n";   return ;}
        fi.close();
        string v_key;
        bool can_read=false;


            for(txt_data3::data * p:data->users)
            {

                    if(p->username==active_user)
                    for(txt_data3::paar_str *f:p->available_data)
                    {
                        if(f->name==filename)
                        {
                            cout<<"You can read file : "<<filename<<"\n";
                            v_key=f->v;
                            can_read=true;
                        }
                    }
                }

            if(!can_read)
                return;

            string owner;
            string owner_sign;
            for(txt_data3::data * p:data->users)
            {

                    for(txt_data3::paar_str *f:p->owned_data)
                    {
                        if(f->name==filename)
                        {
                            if(sign_check(f->v,p->username,filename,true))
                            {
                                owner=p->username;
                                owner_sign=f->v;
                                cout<<"owner from file : "<<filename<<" is "<<owner<<endl;
                            }
                            else
                            {
                                cout<<owner<<" signiture is false\n";
                            }
                        }
                    }
                }
            if(owner.size()==0)
                cout<<"owner is unknowen\n";
        string user_pass;
        cout<<"enter your password : "<<active_user<<endl;
        cin>>user_pass;
        if(!check_pass_for_aktive_user(user_pass))
            return;


        SRN_RSA2 *rsa=new SRN_RSA2();
        string key=rsa->private_decrypt_user(v_key,active_user,user_pass);
        //cout<<"\nRestored Password\n"<<key<<endl;
      SRN_AES *aes=new SRN_AES();
      aes->aes_file_dec(filename,key);
    }


    void key_copy(string filename,string des_user)
    {
        if(active_user.empty())
        { cout<<"Please login first\n";return;      }
        string v_filename=filename+".aes";
        ifstream fi(v_filename);
        if(!fi.is_open()){cout<<"File name :"<<filename<<" is not correct\n";   return ;}
        fi.close();
        string v_key;

        for(txt_data3::data * p:data->users)
        {
            if(p->username==active_user)
            {
                bool the_file_is_mein=false;
                for(txt_data3::paar_str *f:p->owned_data)
                {
                if(f->name==filename)
                    {
                    if(sign_check(f->v,p->username,filename,true))
                    {
                        cout<<"signature "<<p->username<<" , "<<filename<<" is verified\n";
                        the_file_is_mein=true;
                    }
                    else
                    {
                        cout<<"error by Signature "<<p->username<<" , "<<filename<<endl;
                        return;
                    }
                }
                }
                if(the_file_is_mein)
                     for(txt_data3::paar_str *f:p->available_data)
                    {
                        if(f->name==filename)
                        { cout<<"You can read file : "<<filename<<"\n";    v_key=f->v;  }
                    }
                else
                {
                    cout<<"you dont own the file "<<filename<<endl;
                    return;
                }
             }

        }
        string user_pass;
        cout<<"enter your password : "<<active_user<<endl;
        cin>>user_pass;
        check_pass_for_aktive_user(user_pass);


        SRN_RSA2 *rsa=new SRN_RSA2();
        string key=rsa->private_decrypt_user(v_key,active_user,user_pass);
        SRN_RSA2 *rsa2=new SRN_RSA2();
        string neu_v_key=rsa2->public_encrypt_user(key,des_user);

        for(txt_data3::data * p:data->users)
        {
            if(p->username==des_user)
            p->available_data.push_back(new txt_data3::paar_str(filename,neu_v_key));
        }

    }

    void key_delete(string filename,string des_user)
        {
            if(active_user.empty())
            { cout<<"Please login first\n";return;      }
            string v_filename=filename+".aes";
            ifstream fi(v_filename);
            if(!fi.is_open()){cout<<"File name :"<<filename<<" is not correct\n";   return ;}
            fi.close();
            string v_key;
    bool the_file_is_mein=false;
            for(txt_data3::data * p:data->users)
            {
                if(p->username==active_user)
                {
                   // bool the_file_is_mein=false;
                    for(txt_data3::paar_str *f:p->owned_data)
                    {
                    if(f->name==filename)
                        {
                        if(sign_check(f->v,p->username,filename,true))
                        {
                            cout<<"signature "<<p->username<<" , "<<filename<<" is verified\n";
                            the_file_is_mein=true;
                        }
                        else
                        {
                            cout<<"error by Signature "<<p->username<<" , "<<filename<<endl;
                            return;
                        }
                    }
                    }

                 }

            }
            if(the_file_is_mein)
            {
                string user_pass;
                cout<<"enter your password : "<<active_user<<endl;
                cin>>user_pass;
                check_pass_for_aktive_user(user_pass);

                for(txt_data3::data * p:data->users)
                {
                    if(p->username==des_user)
                    {

                        for(txt_data3::paar_str * d:p->available_data)
                        {
                            if(d->name==filename)
                            {
                                d->name="\t";
                                d->v="\t";
                            }
                        }
                    }
                }
            }
            else
            {
                cout<<"you dont own the file "<<filename<<endl;
                return;
            }


        }

    void exit()
    {
        if(!data->users.empty())
        data->write();
    }

    bool login(string username)
    {
        string pass;
        cout<<" enter your password\n";
        cin>>pass;
        if(!data->check_exist(username))
        {
            cout<<"user: "<<username<<" is not existed\n";
            return false;
        }
        for(txt_data3::data * p: data->users)
        {
            if(p->username==username)
            {
                SHA *sha=new SHA();
                 if(sha->gleich_string_hash(pass,p->passhash))
                 {
                     cout<<"Login success\n";
                     active_user=username;
                     return true;
                 }
                 else
                 {
                     cout<<"Login faild\n";
                     return false;
                 }
            }
        }
        cout<<"Login faild\n";
        return false;
    }
    bool check_pass_for_aktive_user(string pass)
    {
        for(txt_data3::data * p: data->users)
        {
            if(p->username==active_user)
            {
                SHA *sha=new SHA();
                 if(sha->gleich_string_hash(pass,p->passhash))
                     return true;
                 else
                 {
                     cout<<"password faild\n";
                     return false;
                 }
            }
        }
        cout<<"password faild\n";
        return false;
    }

    void logout(){active_user="";}
    void user_list()
    {
        cout<<"Users list :\n";
        for(txt_data3::data *p:data->users)
        {
            cout<<p->username<<endl;
        }
        cout<<"---------------\n";
    }

    void available_data_list()
    {
        if(active_user.size()==0)
        {
            cout<<"Please login\n";
            return;
        }
        cout<<"Files list you are allowed to read :\n";
        for(txt_data3::data *p:data->users)
        {
            if(p->username==active_user)
            {
                for(txt_data3::paar_str *f:p->available_data)
                {
                    cout<<f->name<<endl;
                }
            }
        }
        cout<<"---------------\n";
    }

    void owned_data_list()
    {
        if(active_user.size()==0)
        {
            cout<<"Please login\n";
            return;
        }
        cout<<"Files list you have :\n";
        for(txt_data3::data *p:data->users)
        {
            if(p->username==active_user)
            {
                for(txt_data3::paar_str *f:p->owned_data)
                {
                    cout<<f->name<<endl;
                }
            }
        }
        cout<<"---------------\n";
    }
    void add_file(string filename)
    {
        if(active_user.empty())
        { cout<<"Please login first\n";return;      }
        for(txt_data3::data * p:data->users)
        {
            for(txt_data3::paar_str *f:p->owned_data)
            {
                if(f->name==filename)
                {
                    cout<<"this file allready encrpted\n";
                    return;
                }
            }
        }
        ifstream fi(filename);
        if(!fi.is_open()){cout<<"File name :"<<filename<<" is not correct\n";   return ;}
        fi.close();

        string user_pass;
        cout<<"enter your password : "<<active_user<<endl;
        cin>>user_pass;
        if(!check_pass_for_aktive_user(user_pass))return;

        ////////////////
        string key=generate_random_pass();
        cout<<"key :\n"<<key;
        SRN_AES *aes=new SRN_AES();
        aes->aes_file_enc(filename,key);


        ///////////////
        unsigned char  encrypted[4098]={};
        char plainText[2048/8]={} ;
        strncpy((char*)plainText, &key[0], PASSWORD_LINGTH);
        SRN_RSA2 *rsa=new SRN_RSA2();
        rsa->public_encrypt_user((unsigned char*)plainText,strlen(plainText),
                                 (unsigned char*)encrypted,active_user);
        string v_key=(char*)encrypted;
        ///2
        v_key=rsa->public_encrypt_user(key,active_user);

        /////////////////
        string hash_v_data;     string v_filename=filename+".aes";
        SHA *sha;


        hash_v_data=sha->sha256_file(v_filename);
        unsigned char  encrypted2[4098]={};
        char plainText2[2048/8]={} ;
        strncpy(plainText2, &hash_v_data[0], hash_v_data.length());
        SRN_RSA2 *rsa2=new SRN_RSA2();
        rsa2->private_encrypt_user((unsigned char*)plainText2,strlen(plainText2),
                                   (unsigned char*)encrypted2,active_user,user_pass);
        string sign=(char*)encrypted2;

        //////////////////2
        ///
        sign=rsa2->private_encrypt_user(hash_v_data,active_user,user_pass);

        for(txt_data3::data *p:data->users)
        {
            if(p->username==active_user)
            {
                p->available_data.push_back(new txt_data3::paar_str(filename,v_key));
                p->owned_data.push_back(new txt_data3::paar_str(filename,sign));
            }
        }

        data->write();

    }

    string sign_create(string user_name,string pass,string file_name,bool file_is_encr)
    {
        SHA *sha=new SHA();
        string s=sha->hash_user_file(user_name,file_name,file_is_encr);
        SRN_RSA2 *rsa=new SRN_RSA2();
        return rsa->private_encrypt_user(s,user_name,pass);
    }
    bool sign_check(string sign,string user_name,string file_name,bool file_is_enc)
    {
        SHA *sha=new SHA();
        string s1=sha->hash_user_file(user_name,file_name,file_is_enc);

        SRN_RSA2 *rsa=new SRN_RSA2();
        string s2=rsa->public_decrypt_user(sign,user_name);
        return string_check(s1,s2);

    }

};
