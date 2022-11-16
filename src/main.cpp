#include <thread>
#include "cardinal.hpp"
#include "logger.hpp"
#include "config.hpp"
#include "curlpp.hpp"
#include "worker.hpp"
using namespace std;

int main(int argc, char** argv)
{
    string ConfigFilename=argc>1?argv[1]:"config.ini";
    config::ini(ConfigFilename);
    list<worker> workers;
    vector<jthread> threads;
    for (auto [ifname,ac_name]:config::interfaces){
        workers.emplace_back(ifname,config::accounts[ac_name]);
        threads.emplace_back(workers.back());
    }
    return 0;
}
