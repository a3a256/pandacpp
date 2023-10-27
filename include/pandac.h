#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <limits.h>


class DataFrame{
    public:
        std::map<std::string, std::vector<std::string>> df;
        std::vector<std::string> columns;

        void read_csv(std::string path, char delimeter);

        void to_df();

        void head(int l);
};

void DataFrame::read_csv(std::string path, char delimeter = ';'){
    std::ifstream fan(path);
    if(!fan.is_open()){
        throw std::invalid_argument("Path or directory does not exist\n");
    }
    std::fstream fin;
    fin.open(path, std::ios::in);
    std::string line, word;
    int index = 0, col;
    while(std::getline(fin, line)){
        std::stringstream s(line);
        if(index == 0){
            while(std::getline(s, word, delimeter)){
                columns.push_back(word);
            }
        }else{
            col = 0;
            while(std::getline(s, word, delimeter)){
                df[columns[col]].push_back(word);
                col++;
            }
        }
        index++;
    }
    return;
}

void DataFrame::head(int l = 5){
    int i, j, biggest_size, diff, pad, limiter;
    std::string line = "";
    biggest_size = INT_MIN;
    for(i=0; i<columns.size(); i++){
        biggest_size = std::max((int)columns[i].size(), biggest_size);
        for(j=0; j<df[columns[i]].size(); j++){
            biggest_size = std::max((int)df[columns[i]][j].size(), biggest_size);
        }
    }
    for(i=0; i<columns.size(); i++){
        if(columns[i].size() < biggest_size){
            diff = biggest_size - columns[i].size();
        }
        for(pad = 0; pad<diff; pad++){
            line += ' ';
        }
        line += columns[i];
        line += ' ';
    }
    line.pop_back();
    line += '\n';
    limiter = std::min(l, (int)df[columns[0]].size());
    for(i=0; i<columns.size(); i++){
        for(j=0; j<limiter; j++){
            if(df[columns[i]][j].size() < biggest_size){
                diff = biggest_size - df[columns[i]][j].size();
            }
            for(pad=0; pad<diff; pad++){
                line += ' ';
            }
            line += df[columns[i]][j] + ' ';
        }
        line.pop_back();
        line += '\n';
    }
    std::cout << line;
}