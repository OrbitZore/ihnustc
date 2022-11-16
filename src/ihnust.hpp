#include "cardinal.hpp"
#include "config.hpp"
#ifndef IHNUST_HPP
#define IHNUST_HPP
namespace ihnust{
    const string 上网登录页="\xc9\xcf\xcd\xf8\xb5\xc7\xc2\xbc\xd2\xb3",
                注销页="\xd7\xa2\xcf\xfa\xd2\xb3";
    map<string, string> ISPtoArgs = { {"school",""},{"telecom","%40telecom"},{"unicom","%40unicom"},{"cmcc","%40cmcc"} };
    class args_gen{
        const string args_user_account,args_user_password;
        char ip[16]={},v[10]={};
        void prepare_rand(){
            int rv=rand() % 9000 + 1000;char* c=v;
            do{
                *c++=rv%10+'0';
                rv/=10;
            }while (rv);
            reverse(v,c);
            *c=0;
        }
    public:
        pair<const char*,const char*> args[7];
        args_gen(const config::account &ac):
            args_user_account("%2C0%2C"+ac.name+ISPtoArgs[ac.isp]),
            args_user_password(ac.passwd),
            args{
                {"wlan_user_ip",&ip[0]},
                {"v",&v[0]},
                {"a","login"},
                {"user_account",args_user_account.c_str()},
                {"user_password",args_user_password.c_str()},
                {"c","Portal"},
                {"login_method","1"},
            }{}
        void prepare_login(const string& c){
            prepare_rand();
            memcpy(ip,c.c_str(),min((size_t)15,c.size()));
            ip[min((size_t)15,c.size())]=0;
            args[0].second=ip;
            args[1].second=v;
        }   
    };
}

#endif