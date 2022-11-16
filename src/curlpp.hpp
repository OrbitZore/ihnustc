#include "cardinal.hpp"
#include "logger.hpp"
#include "config.hpp"
#ifndef CURLPP_HPP
#define CURLPP_HPP
#include <curl/curl.h>
size_t write_string(const void *ptr, size_t size, size_t nmemb, string* stream){
    for (size_t i=0;i<size*nmemb;i++)
        (*stream)+=((const char*)ptr)[i];
    return nmemb;
}
optional<string> curl(const string& url,span<pair<CURLoption,void*>> vs){
    CURL *curl_handle = curl_easy_init();
    string res;
    if(curl_handle) {
        CURLcode ret;
        curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, &write_string);
        curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, config::user_agent.c_str());
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &res);
        ret = curl_easy_perform(curl_handle);
        if (ret) return {};
        curl_easy_cleanup(curl_handle);
    }
    return res;
}
namespace http{
    optional<string> get(const string& ifname,string url,span<pair<const char*,const char*>> args){
        char c='?';
        for (auto [i,j]:args){
            url+=exchange(c,'&');
            url+=i;
            url+="=";
            url+=j;
        }
        pair<CURLoption,void*> a[]={{CURLOPT_INTERFACE,(void*)ifname.c_str()}};
        return curl(url,a);
    }
}
#endif