#include <mutex>
#include "cardinal.hpp"
#ifndef LOGGER_HPP
#define LOGGER_HPP
namespace logger{
    mutex m;
    template<class ...Args>
    void log(const char* type,const char *c,Args ...args){
        lock_guard lg(m);
        time_t t = time(nullptr);
        tm tm = *localtime(&t);
        cout<<"["<<put_time(&tm,"%Y/%m/%d %H:%M:%S")<<"]";
        print("[{}] ",type);
        println(c,args...);
    }
    template<class ...Args>
    void info(const char *c,Args ...args){
        log("INFO",c,args...);
    }
    template<class ...Args>
    void warn(const char *c,Args ...args){
        log("WARN",c,args...);
    }
    template<class ...Args>
    void error(const char *c,Args ...args){
        log("ERROR",c,args...);
    }
    template<class ...Args>
    void error_terminate(const char *c,Args ...args){
        log("ERROR",c,args...);
        exit(-1);
    }
}
#endif