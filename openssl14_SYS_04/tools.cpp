//#include<srn_aes.cpp>
#include<bits/stdc++.h>

#include <termios.h>
#include <unistd.h>
#include <ctime>
using namespace std;

const static int PASSWORD_LINGTH=50;
const static string BASE_FILE="data.txt";

    static bool string_check(string s1,string s2)
    {
        if(s1.length()!=s2.length())
            return false;
        for(int i=0;i<s1.length();i++)
            if(s1[i]!=s2[i])
                return false;
        return true;
    }
    static string generate_random_pass()
    {srand (time(NULL));
        string result;
        for(int i=0;i<PASSWORD_LINGTH;i++)
           result.push_back((char)(rand()%255));
        cout<<"random password :\n"<<result<<endl;
        return result;
    }

