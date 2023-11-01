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

        void to_csv(std::string path);

        void to_df();

        void head(int l);

        void drop_column(int index);
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
        diff = biggest_size - columns[i].size();
        for(pad = 0; pad<diff; pad++){
            line += ' ';
        }
        line += columns[i];
        line += ' ';
    }
    line.pop_back();
    line += '\n';
    limiter = std::min(l, (int)df[columns[0]].size());
    for(i=0; i<limiter; i++){
        for(j=0; j<columns.size(); j++){
            diff = biggest_size - df[columns[j]][i].size();
            for(pad=0; pad<diff; pad++){
                line += ' ';
            }
            line += df[columns[j]][i] + ' ';
        }
        line.pop_back();
        line += '\n';
    }
    std::cout << line;
}

void DataFrame::to_csv(std::string path){
    std::fstream fout;
    fout.open(path, std::ios::out | std::ios::app);
    int i, j, len;
    for(i=0; i<columns.size(); i++){
        fout << columns[i] << ';';
    }
    fout << '\n';
    len = df[columns[0]].size();
    for(i=0; i<len-1; i++){
        for(j=0; j<columns.size()-1; j++){
            fout << df[columns[j]][i] << ';';
        }
        fout << df[columns[j]][i] << '\n';
    }
    for(j=0; j<columns.size()-1; j++){
        fout << df[columns[j]][i] << ';';
    }
    fout << df[columns[j]][i];
}

void DataFrame::drop_column(int index){
    if(index >= df.size()){
        throw std::invalid_argument("Column index is out of range\n");
    }
    df.erase(columns[index]);
    columns.erase(columns.begin()+index);
}