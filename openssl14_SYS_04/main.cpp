#include <QCoreApplication>
#include<sys.cpp>

using namespace std;
void demo1();
void demo2();
void help()
{
    cout<<"Functions list\n";
    cout<<"Add User : u\n"<<"Add file :f\n"<<"File decryption :d\n";
    cout<<"Key copy : c\n"<<"Login : l\n"
       <<"Logout : o\n"<<"Show Users list : s\n"
      <<"Available file liste : a\n"<<"Owned files : w\n"<<"Exit : e\n"
     <<"Help : h\n";
}
int main(int argc, char *argv[])
{

    SYS *sys=new SYS();
    sys->startup();
    help();
    char c='0';
    while(c!='e')
    {
        cin>>c;
        if(c=='u')
        {
            cout<<"adding new user\n";
            sys->add_user();
            continue;
        }
        if(c=='f')
        {
            cout<<"adding new file\n";
            cout<<"enter file name :";
            string s;
            cin>>s;
            sys->add_file2(s);
            continue;
        }

        if(c=='d')
        {
            cout<<"decription file\n";
            cout<<"enter file name :";
            string s;
            cin>>s;
            sys->file_dec2(s);
            continue;
        }
        if(c=='c')
        {
            cout<<"Copy Key to another user\n";
            string s;
                cout<<"enter file name :";
                cin>>s;
            string sd;
                cout<<"enter destination user :";
                cin>>sd;

            sys->key_copy(s,sd);
            continue;
        }
        if(c=='t')
        {
            cout<<"delete key from another user\n";
            string s;
                cout<<"enter file name :";
                cin>>s;
            string sd;
                cout<<"enter destination user :";
                cin>>sd;

            sys->key_delete(s,sd);
            continue;
        }
        if(c=='l')
        {
            cout<<"log in\n";
            string s;
                cout<<"Enter username : ";
                cin>>s;
            sys->login(s);
            continue;
        }
        if(c=='o')
        {cout<<"logout\n";sys->logout();continue;}
        if(c=='s')
        {sys->user_list();continue;}
        if(c=='a')
        {sys->available_data_list();continue;}
        if(c=='w')
        {sys->owned_data_list();continue;}
        if(c=='h')
        {help();continue;}

    }
   sys->exit();

  //  demo2();
    return 0;
}
void demo2()
{
    SYS *sys=new SYS();
    sys->startup();


    sys->login("SYS");
    sys->add_file2("a.txt");
    cout<<"dec\n";
    sys->file_dec2("a.txt");

    sys->exit();
}

void demo3()
{
    string a="majd";
    string b="majd";
    string c="sdks";
    string d="majdas";
    SYS *sys=new SYS();

    string sign=sys->sign_create("SYS","SYS","SYS_public.pem",false);
    cout<<sys->sign_check(sign,"SYS","SYS_public.pem",false)<<endl;
}








void demo1()
{
    string usr="majd";
    string pass="password";
    string key="key----------------key";

    SRN_AES *aes=new SRN_AES();
    SRN_RSA2 *rsa=new SRN_RSA2();

    aes->aes_file_enc("a.txt",key);

   // rsa->generate_key(usr,pass);
    string v_k=rsa->public_encrypt_user(key,usr);

    string e_v_k = base64_encode(reinterpret_cast<const unsigned char*>(v_k.c_str()),v_k.length());
   // cout<<e_v_k<<endl;
    ofstream f1("a.key");
    f1<<e_v_k; f1.close();


    ifstream f2("a.key");
    string e_v_k2;
    f2>>e_v_k2; f2.close();

    string v_k2=base64_decode(e_v_k2);

    string key2=rsa->private_decrypt_user(v_k2,usr,pass);
    cout<<key2<<endl;

    aes->aes_file_dec("a.txt",key2);
}
