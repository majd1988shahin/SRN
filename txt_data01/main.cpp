#include <QCoreApplication>
#include<txt_data.cpp>
int main(int argc, char *argv[])
{
  //QCoreApplication a(argc, argv);

    txt_data *d =new txt_data();
    d->init();
     unsigned int salt[] = {12345, 54321};
    d->add_user("majd",salt,(unsigned char*)"passwort",
                (unsigned char*)"privat",(unsigned char*)"public");
    d->add_user("ali",salt,(unsigned char*)"sdsjfknjs",
                (unsigned char*)"sldcjsldjvfscsc",(unsigned char*)"dajsnbcsknck");
    string user="majd";
    unsigned int salt2[2] = {12345, 54321};
    string a,b,c;
    if(d->read(user,salt2,a,b,c)==0)
            cout<<salt[0]<<endl<<salt[1]<<endl<<a<<endl<<b<<endl<<c<<endl;
    return 1;
}
