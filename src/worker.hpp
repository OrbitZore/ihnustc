#include <thread>
#include "cardinal.hpp"
#include "logger.hpp"
#include "config.hpp"
#include "curlpp.hpp"
#include "ihnust.hpp"
#ifndef WORKER_HPP
#define WORKER_HPP
const char* getIP_head="v46ip='";
const char* getIP_nil="'";
class worker{
    bool status;
    const string ifname;
    const config::account ac;
    ihnust::args_gen args_gen;
public:
    worker(const string& ifname,const config::account& ac):ifname(ifname),ac(ac),args_gen(ac){}
    string getIP(){
        auto rres=http::get(ifname,config::portal_url,{});
        if (!rres){
            logger::error("interface {}: curl protal failed", ifname);
            return "";
        }
        auto& res=*rres;
        auto beg = res.find(getIP_head) + strlen(getIP_head);
        if (beg >= res.size()) return "";
        auto len = res.find(getIP_nil, beg);
        if (len >= res.size()) return "";
        return res.substr(beg, len - beg);
    }
    bool getstatus(){
        auto rres=http::get(ifname,config::portal_url,{});
        if (!rres){
            logger::error("interface {}: curl protal failed", ifname);
            return false;
        }
        auto& res=*rres;
        if (res.find(ihnust::上网登录页)!=string::npos){
            if (status!=false)
                logger::info("interface {}: offline", ifname);
            return status=false;
        }else if (res.find(ihnust::注销页)!=string::npos){
            if (status!=true)
                logger::info("interface {}: online", ifname);
            return status=true;
        }
        logger::error("interface {}: protal return invaild webpage", ifname);
        return false;
    }
    void login(){
        logger::info("interface {}: requested login", ifname);
        args_gen.prepare_login(getIP());
        http::get(ifname,config::api_url+"eportal/", args_gen.args);
    }
    void operator()(){
        while (1){
            getstatus();
            if (!status){
                login();
                getstatus();
            }
            this_thread::sleep_for( config::keep_alive_interval*1s);
        }
    }
};

#endif