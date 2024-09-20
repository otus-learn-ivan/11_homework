#include <fstream>
#include <iostream>
#include <memory>
#include <chrono>
#include <vector>
#include "async.h"
#include <boost/asio.hpp>
#include <sstream>
#include <map>
#include <algorithm>
#include <iterator>
#include <utility>
#include <set>
using namespace std;

//---------------------------------------------
std::vector<string>  str_to_vector(std::string cmd){
    std::vector<std::string> out_vector;
    istringstream iss(cmd);
    while(iss){
        string s;
        iss >> s;
        out_vector.push_back(s);
    }
    return out_vector;
}


using Ttable = std::map<std::string,std::string>;
using Ttable_answer = std::map<std::string,std::pair<std::string,std::string>>;

Ttable tableA;
Ttable tableB;

struct Tinserter_table{
    std::string operator()(std::vector<string>& v_str){
        auto& table =  v_str[1]=="A"?tableA:tableB;
        if(table.count(v_str[2])){
            return "ERR duplicate " + v_str[2];
        }
        table[v_str[2]] = v_str[3];
        // for(const auto& out : table){
        //     std::cout << out.first << " " << out.second << std::endl;
        // }
        return "OK\n";
    }
};
struct Ttruncate_table{
    std::string operator()(std::vector<string>& v_str){
        auto& table =  v_str[1]=="A"?tableA:tableB;
        table.clear();
        return "OK\n";
    }
};


template<class Tset,class Tpair >
class MyInsertIterator : public std::insert_iterator<Tset> {
public:
    using std::insert_iterator<Tset>::insert_iterator;

    MyInsertIterator(std::insert_iterator<Tset> it):std::insert_iterator<Tset>(it){}
    MyInsertIterator& operator=(const Tpair& pair) {
        std::insert_iterator<Tset>::operator*()= pair.first;
        return *this;
    }
    MyInsertIterator& operator*(){
        return *this;
    }
};

struct Tintersection{//пересечение
    std::string operator()(std::vector<string>& v_str){
        v_str.clear();
        std::set<string> intersection_key;
        MyInsertIterator<std::set<string>,pair<string,string>> It { std::inserter(intersection_key, intersection_key.begin())};
        std::set_intersection(tableA.begin(), tableA.end(), tableB.begin(), tableB.end(),It,
                                  [](auto& a, auto& b){return a.first < b.first;}
        );
        Ttable_answer intersection;
        for (const auto& key : intersection_key) {
            intersection.insert({key,{tableA[key],tableB[key]}});
        }
        std::stringstream  answer;
        for(const auto& out : intersection){
            answer << out.first << "," << out.second.first <<","<<out.second.second << "\n";
        }
        return answer.str();
    }
};
struct Tsymmetric_difference{//разность
    std::string operator()(std::vector<string>& v_str){
        v_str.clear();
        std::set<string> difference_key;
        MyInsertIterator<std::set<string>,pair<string,string>> It { std::inserter(difference_key, difference_key.begin())};
        std::set_difference(tableA.begin(), tableA.end(), tableB.begin(), tableB.end(),It,
                                  [](auto& a, auto& b){return a.first < b.first;}
        );
        std::set_difference(tableB.begin(), tableB.end(), tableA.begin(), tableA.end(),It,
                                  [](auto& a, auto& b){return a.first < b.first;}
        );
        Ttable_answer difference;
        for (const auto& key : difference_key) {
            tableA.count(key)>0?difference.insert({key,{tableA[key],""}}):difference.insert({key,{"",tableB[key]}});
        }
        std::stringstream  answer;
        for(const auto& out : difference){
            answer<< out.first << "," << out.second.first <<","<<out.second.second << std::endl;
        }
        return answer.str();
    }
};

std::map<std::string,std::function <std::string(std::vector<string>&)>> Comands{
    {"INSERT", Tinserter_table()},
    {"TRUNCATE",Ttruncate_table()},
    {"INTERSECTION",Tintersection()},
    {"SYMMETRIC_DIFFERENCE",Tsymmetric_difference()}
};

std::string parser_join(std::string cmd){
    std::cout << cmd << std::endl;
    auto vk_str = str_to_vector(cmd);
    return Comands.at(vk_str[0])(vk_str);
}

void signal_handler(int signal);
int main_client_server(const unsigned short g_port_num_);
int main(int argc, char* argv[])
{
    std::signal(SIGINT, signal_handler);
    if(argc == 1){return 0;}
    std::cout << "Hello world " << argv[1] << " " <<std::endl;
    main_client_server(atoi(argv[1]));
    return 0;
}
