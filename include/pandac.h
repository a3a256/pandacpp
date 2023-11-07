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
#include <ios>
#include <stdlib.h>
#include <set>
#include <cstdlib>
#include <utility>


class DataFrame{
    public:
        std::map<std::string, std::vector<std::string>> df;
        std::map<std::string, std::vector<float>> df_e;
        std::vector<std::string> columns;
        std::map<std::string, std::map<std::string, std::string>> encoder;
        std::map<std::string, std::map<std::string, std::string>> decoder;

        std::pair<int, int> shape;

        void read_csv(std::string path, char delimeter, int head, std::vector<std::string> cols);

        void to_csv(std::string path);

        void to_df();

        void head(int l);

        void drop_column(int index);

        void rename_columns(std::map<std::string, std::string> dict);

        void convert_df();

        void encode_categoricals(std::vector<std::string> cols);

        void unique_vals(std::string value);


    private:

        void internal_append_row(std::stringstream &s, std::string word, char delim){
            int col = 0;
            while(std::getline(s, word, delim)){
                df[columns[col]].push_back(word);
                col++;
            }
        }

        bool check_float(std::string &value){
            std::istringstream iss(value);
            float f;
            iss >> std::noskipws >> f;

            return iss.eof() && !iss.fail();
        }

        bool check_int(std::string &value){
            // char *p;
            // long converted = std::strtol(value, &p, 10);
            // if(*p){
            //     return false;
            // }
            // return true;
            return !value.empty() && std::all_of(value.begin(), value.end(), ::isdigit);
        }

        std::string is_df_convertible(){
            std::string error_cols = "";
            for(auto it: df){
                for(std::string val: it.second){
                    if(!check_float(val)){
                        if(!(check_int(val))){
                            error_cols += it.first + " ";
                            break;
                        }
                    }
                }
            }
            return error_cols;
        }
};

void DataFrame::read_csv(std::string path, char delimeter = ';', int head = 0, std::vector<std::string> cols = {}){
    std::ifstream fan(path);
    if(!fan.is_open()){
        throw std::invalid_argument("Path or directory does not exist\n");
    }
    std::fstream fin;
    fin.open(path, std::ios::in);
    std::string line, word;
    int index = 0;
    while(std::getline(fin, line)){
        std::stringstream s(line);
        if(index == head){
            while(std::getline(s, word, delimeter)){
                columns.push_back(word);
            }
            if(cols.size() != 0){
                if(cols.size() == columns.size()){
                    std::vector<std::string>().swap(columns);
                    for(std::string col: cols){
                        columns.push_back(col);
                    }
                    internal_append_row(s, word, delimeter);
                }else{
                    throw std::invalid_argument("The amount of entered columns does not match the amount of columns in CSV file\n");
                }
            }
        }else if(index > head){
            internal_append_row(s, word, delimeter);
        }
        index++;
    }
    index -= head;
    index --;
    shape.first = index;
    shape.second = columns.size();
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

void DataFrame::rename_columns(std::map<std::string, std::string> dict){
    int index, i;
    std::string error_line = "Column ";
    for(auto it: dict){
        if(df.find(it.first) != df.end()){
            df[it.second] = df[it.first];
            for(i=0; i<columns.size(); i++){
                if(columns[i] == it.first){
                    index = i;
                }
            }
            columns.insert(columns.begin()+index, it.second);
            drop_column(index+1);
        }else{
            error_line += it.first + " not found in range\n";
            throw std::invalid_argument(error_line);
        }
    }
}

void DataFrame::convert_df(){
    std::string convertible = is_df_convertible();
    if(convertible.size() != 0){
        throw std::invalid_argument(convertible);
    }
    for(auto it: df){
        for(std::string val: it.second){
            df_e[it.first].push_back(std::stof(val));
        }
    }
}

void DataFrame::unique_vals(std::string value){
    if(df.find(value) == df.end()){
        throw std::invalid_argument("Column not found in scope\n");
    }
    std::set<std::string> stk;
    for(std::string s: df[value]){
        stk.insert(s);
    }
    for(auto it: stk){
        std::cout << it << " ";
    }
    std::cout << "\n";
    std::set<std::string>().swap(stk);
}

void DataFrame::encode_categoricals(std::vector<std::string> cols){
    int i, index;
    std::map<std::string, std::set<std::string>> ecn;
    for(i=0; i<cols.size(); i++){
        if(df.find(cols[i]) != df.end()){
            for(std::string s: df[cols[i]]){
                ecn[cols[i]].insert(s);
            }
        }else{
            std::string line = "Column " + cols[i] + " not found in scope\n";
            throw std::invalid_argument(line);
        }
    }

    for(auto it: ecn){
        index = 0;
        for(auto s: it.second){
            encoder[it.first][s] = std::to_string(index);
            decoder[it.first][std::to_string(index)] = s;
            index++;
        }
    }

    for(auto it: encoder){
        for(i=0; i<df[it.first].size(); i++){
            df[it.first][i] = encoder[it.first][df[it.first][i]];
        }
    }
}