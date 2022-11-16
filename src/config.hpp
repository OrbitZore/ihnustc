#include "cardinal.hpp"
#include "logger.hpp"
#ifndef CONFIG_HPP
#define CONFIG_HPP
namespace config{
    struct account{
        string name,passwd,isp;
    };
    string portal_url,api_url,user_agent;
    int keep_alive_interval;
    map<string,account> accounts;  // account   id   to account info
    map<string,string>  interfaces;// interface name to account id
    

    void ini(const string filename){
        int line_index=0,section_index=1;
        string section;
        map<string,string> s;
        auto clear_ss=[&](){
                    if (section.empty()){
                        portal_url=s.at("portal_url");
                        api_url=s.at("api_url");
                        user_agent=s.at("user_agent");
                        keep_alive_interval=stoi(at_or(s,"keep_alive_interval","120"));
                    }else if (section=="account"){
                        account ac;
                        ac.name=move(s.at("name"));
                        ac.passwd=move(s.at("password"));
                        ac.isp=move(s.at("isp"));
                        accounts[s.at("id")]=move(ac);
                    }else if (section=="interface"){
                        interfaces[s.at("name")]=move(s.at("account_id"));
                    }
                    s.clear();
        };
        ifstream is(filename);
        try{
            while (is){
                line_index++;
                string line;
                getline(is,line);
                if (line.starts_with("[")&&line.ends_with("]")){
                    clear_ss();
                    section=line.substr(1,line.size()-2);
                    if (section!="account"&&section!="interface")
                        throw std::runtime_error("unknow section");
                    section_index=line_index;
                }else if (line.starts_with("#")){
                    continue;
                }else if (auto x=line.find("=");x!=string::npos&&x+1!=line.size()){
                    s[line.substr(0,x)]=line.substr(x+1);
                }else if (line.size()){
                    throw std::runtime_error("parse error");
                }
            }
            clear_ss();
        }catch(const out_of_range& e){
            logger::error_terminate("configuration file error on line {},section {} : cannot get essential value",section_index,section);
        }catch (const exception& e){
            logger::error_terminate("configuration file error on line {} : {}",line_index,e.what());
        }
    }
}
#endif