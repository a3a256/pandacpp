#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <vector>


class DataFrame{
    public:
        std::map<std::string, std::vector<std::string>> df;

        void read_csv(std::string path, char delimeter){
            return;
        }
};