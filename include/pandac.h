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
#include <numeric>

// change data from long to string immediately!!

struct val_type{
    bool isnum=false;
    float num;
    std::string line = "";

    bool operator<(const val_type& other) const {
        // You need to define a meaningful comparison here.
        // This is just a simple example; adjust as per your requirements.

        if(num){
            return num < other.num;
        }
        return line < other.line;
    }
};


class DataFrame{
    public:
        std::map<std::string, std::vector<val_type>> df;
        std::map<std::string, std::vector<float>> df_e;
        std::vector<std::string> columns;
        std::map<std::string, std::map<std::string, std::string>> encoder;
        std::map<std::string, std::map<std::string, std::string>> decoder;
        std::set<val_type> stk;
        std::pair<int, int> shape;

        std::vector<std::vector<std::string>> val_string;
        std::vector<std::vector<float>> val_float;

        bool converted = false;

        DataFrame(){
            return;
        }
        // RETURN
        template <typename T> void to_dataframe(std::map<std::string, std::vector<T>> df_p = {});

        void read_csv(std::string path, char delimeter, int head, std::vector<std::string> cols);

        void to_csv(std::string path);

        void to_df();

        void head(int l);

        void tail(int l);

        void drop(std::vector<int> indices={}, std::vector<std::string> names={});

        void rename_columns(std::map<std::string, std::string> dict);

        void convert_df();

        void encode_categoricals(std::vector<std::string> cols);

        void unique(std::string value);

        void nunique(std::string value);

        void sort_by(std::string column, bool ascending = true);

        std::map<std::string, int> value_counts(std::string col);


    private:

        template <typename T> std::string toString(T& t){
            return std::to_string(t);
        }

        std::string toString(char* t){
            return t;
        }

        std::string toString(std::string &t){
            return t;
        }

        void drop_col(int index=-1, std::string name = ""){
            if(index != -1){
                df.erase(columns[index]);
                if(converted){
                    df_e.erase(columns[index]);
                }
                columns.erase(columns.begin()+index);
            }

            if(name != ""){
                df.erase(name);
                if(converted){
                    df_e.erase(name);
                }
                int j;
                for(j=0; j<columns.size(); j++){
                    if(columns[j] == name){
                        columns.erase(columns.begin()+j);
                        break;
                    }
                }
            }
        }

        void internal_append_row(std::stringstream &s, std::string word, char delim){
            int col = 0;
            while(std::getline(s, word, delim)){
                val_type t;
                if(is_number(word)){
                    t.num = std::stof(word);
                    t.isnum = true;
                }
                t.line = word;
                df[columns[col]].push_back(t);
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

        // !!!!!!!!!!! temporarily freeze function 

        // std::string is_df_convertible(){
        //     std::string error_cols = "";
        //     float value;
        //     for(auto it: df){
        //         for(std::string val: it.second){ // change back to std::string
        //             try{
        //                 value = std::stof(val);
        //             }catch (const char* msg){
        //                 return it.first + " not convertible";
        //             }
        //         }
        //     }
        //     return error_cols;
        // }

        bool is_number(std::string x){
            float value;
            try{
                value = std::stof(x);
            }catch (const std::invalid_argument& ex){
                return false;
            }catch (const std::out_of_range& ex) {
                return false;  // Value is out of the range for float
            }
            return true;
        }


        // YET TO CHANGE UNIQUE FUNCTION

        void unique_vals(std::string col){
            for(int i=0; i<df[col].size(); i++){
                stk.insert(df[col][i]);
            }
        }
};

// RETURN

template <typename T> void DataFrame::to_dataframe(std::map<std::string, std::vector<T>> df_p){
    std::string line = "";
    std::vector<std::string>().swap(columns);
    for(auto it: df_p){
        columns.push_back(it.first);
        for(T i: it.second){
            val_type t;
            line = toString(i);
            if(is_number(line)){
                t.num = std::stod(line);
            }
            t.line = line;
            df[it.first].push_back(t);
        }
    }
}

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
            biggest_size = std::max((int)df[columns[i]][j].line.size(), biggest_size);
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
            diff = biggest_size - df[columns[j]][i].line.size();
            for(pad=0; pad<diff; pad++){
                line += ' ';
            }
            line += df[columns[j]][i].line + ' ';
        }
        line.pop_back();
        line += '\n';
    }
    std::cout << line;
}

void DataFrame::tail(int l = 5){
    int i, j, biggest_size, diff, pad, limiter;
    std::string line = "";
    biggest_size = INT_MIN;
    for(i=0; i<columns.size(); i++){
        biggest_size = std::max((int)columns[i].size(), biggest_size);
        for(j=0; j<df[columns[i]].size(); j++){
                biggest_size = std::max((int)df[columns[i]][j].line.size(), biggest_size);
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
    for(i=df[columns[0]].size()-limiter; i<df[columns[0]].size(); i++){
        for(j=0; j<columns.size(); j++){
            diff = biggest_size - df[columns[j]][i].line.size();
            for(pad=0; pad<diff; pad++){
                line += ' ';
            }
            line += df[columns[j]][i].line + ' ';
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
    for(i=0; i<columns.size()-1; i++){
        fout << columns[i] << ';';
    }
    fout << columns[i] << "\n";
    len = df[columns[0]].size();
    for(i=0; i<len-1; i++){
        for(j=0; j<columns.size()-1; j++){
            fout << df[columns[j]][i].line << ';';
        }
        fout << df[columns[j]][i].line << '\n';
    }
    for(j=0; j<columns.size()-1; j++){
        fout << df[columns[j]][i].line << ';';
    }
    fout << df[columns[j]][i].line;
}

void DataFrame::drop(std::vector<int> indices, std::vector<std::string> names){
    if(indices.size() > columns.size() || names.size() > columns.size()){
        throw std::invalid_argument("Number of columns to drop is more than the amount of columns\n");
    }
    if(indices.size() != 0){
        for(int j: indices){
            if(j >= columns.size()){
                throw std::invalid_argument("Column index is out of range\n");
            }
        }

        for(int j: indices){
            drop_col(j);
        }

        return;
    }

    if(names.size() != 0){
        int j;
        for(j=0; j<names.size(); j++){
            if(df.find(names[j]) == df.end()){
                throw std::invalid_argument("Column not found\n");
            }
        }
        for(j = 0; j<names.size(); j++){
            drop_col(-1, names[j]);
        }
        return;
    }
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
            drop_col(index+1);
        }else{
            error_line += it.first + " not found in range\n";
            throw std::invalid_argument(error_line);
        }
    }
}

// void DataFrame::convert_df(){
//     std::string convertible = is_df_convertible();
//     if(convertible.size() != 0){
//         throw std::invalid_argument(convertible);
//     }
//     for(auto it: df){
//         for(std::string val: it.second){
//             df_e[it.first].push_back(std::stof(val));
//         }
//     }

//     converted = true;
// }



// YET TO CHANGE UNIQUE FUNCTION

void DataFrame::unique(std::string value){
    if(df.find(value) == df.end()){
        throw std::invalid_argument("Column not found in scope\n");
    }
    std::set<val_type>().swap(stk);
    unique_vals(value);
    for(auto it: stk){
        std::cout << it.line << " ";
    }
    std::set<val_type>().swap(stk);
    std::cout << "\n";
}

void DataFrame::nunique(std::string value){
    std::set<val_type>().swap(stk);
    unique_vals(value);
    std::cout << stk.size() << "\n";
    std::set<val_type>().swap(stk);
}

void DataFrame::encode_categoricals(std::vector<std::string> cols){
    int i, index;
    std::map<std::string, std::set<std::string>> ecn;
    for(i=0; i<cols.size(); i++){
        if(df.find(cols[i]) != df.end()){
            for(val_type s: df[cols[i]]){
                ecn[cols[i]].insert(s.line);
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
            df[it.first][i].line = encoder[it.first][df[it.first][i].line];
            df[it.first][i].num = std::stof(df[it.first][i].line);
        }
    }
}

void DataFrame::sort_by(std::string column, bool ascending){
    if(df.find(column) == df.end()){
        std::string error_line = column + " was not found in the range\n";
        throw std::invalid_argument(error_line);
    }

    std::vector<std::vector<val_type>> vals_extracted;
    std::vector<val_type> temp;
    std::vector<std::vector<float>> floats_extracted;
    std::vector<float> ftemp;
    int i, j, col_index = 0;
    for(i=0; i<df[columns[0]].size(); i++){
        for(j=0; j<columns.size(); j++){
            if(columns[j] == column){
                col_index = j;            
            }
            temp.push_back(df[columns[j]][i]);
            if(converted){
                ftemp.push_back(df_e[columns[j]][i]);
            }
        }
        vals_extracted.push_back(temp);
        std::vector<val_type>().swap(temp);
    }

    bool sorted = false;
    while(!sorted){
        sorted = true;
        for(i=1; i<vals_extracted.size(); i++){
            if(vals_extracted[i][col_index].isnum){
                if(ascending){
                    if(vals_extracted[i][col_index].num < vals_extracted[i-1][col_index].num){
                        sorted = false;
                        temp = vals_extracted[i];
                        vals_extracted[i] = vals_extracted[i-1];
                        vals_extracted[i-1] = temp;
                    }
                }else{
                    if(vals_extracted[i][col_index].num > vals_extracted[i-1][col_index].num){
                        sorted = false;
                        temp = vals_extracted[i];
                        vals_extracted[i] = vals_extracted[i-1];
                        vals_extracted[i-1] = temp;
                    }
                }
            }else{
                if(vals_extracted[i][col_index].line < vals_extracted[i-1][col_index].line){
                    sorted = false;
                    temp = vals_extracted[i];
                    vals_extracted[i] = vals_extracted[i-1];
                    vals_extracted[i-1] = temp;
                }
            }
        }
    }
    std::vector<val_type>().swap(temp);
    std::vector<float>().swap(ftemp);
    for(i=0; i<df[columns[0]].size(); i++){
        for(j=0; j<columns.size(); j++){
            df[columns[j]][i] = vals_extracted[i][j];
            if(converted){
                df_e[columns[j]][i] = floats_extracted[i][j];
            }
        }
    }
}

std::map<std::string, int> DataFrame::value_counts(std::string col){
    std::map<std::string, int> counts;
    int i, space;
    for(i=0; i<df[col].size(); i++){
        counts[df[col][i].line]++;
    }
    space = 0;
    for(auto it: counts){
        space = std::max((int)it.first.size(), space);
    }
    std::string head = "";
    std::string num = "";
    for(i=0; i<space; i++){
        head += ' ';
    }
    head += col;
    head += '\n';
    for(auto it: counts){
        head += it.first;
        for(i=0; i<space - it.first.size(); i++){
            head += ' ';
        }
        num = std::to_string(it.second);
        for(i=0; i<col.size()-num.size(); i++){
            head += ' ';
        }
        head += num;
        head += '\n';
    }
    head.pop_back();
    std::cout << head << '\n';
    return counts;
}


class Series{
    public:

        std::vector<val_type> values;

        Series(std::vector<std::string> vals, std::string name = ""){
            int i;
            for(i=0; i<vals.size(); i++){
                val_type t;
                if(is_number(vals[i])){
                    t.isnum = true;
                    t.num = std::stof(vals[i]);
                }
            }
            return;
        }

        float mean();
        float sum();
        std::set<val_type> unique();
        int nunique();
        std::map<val_type, int> value_counts();


    private:
        bool is_number(std::string x){
            float value;
            try{
                value = std::stof(x);
            }catch (const std::invalid_argument& ex){
                return false;
            }catch (const std::out_of_range& ex) {
                return false;  // Value is out of the range for float
            }
            return true;
        }
};

float Series::mean(){
    float _sum;
    try{
        _sum = sum();
    }catch (const std::invalid_argument& ex){
        throw ex;
    }
    return (float)(_sum/(float)values.size());
}

float Series::sum(){
    float _sum = 0.0f;
    int i;
    for(i=0; i<values.size(); i++){
        if(!values[i].isnum){
            throw std::invalid_argument("Cannpt convert 'str' to scalar value\n");
        }
        _sum += values[i].num;
    }
    return _sum;
}

std::set<val_type> Series::unique(){
    std::set<val_type> stk;
    int i;
    for(i=0; i<values.size(); i++){
        stk.insert(values[i]);
    }
    return stk;
}

int Series::nunique(){
    std::set<val_type> stk;
    stk = unique();
    return (int)stk.size();
}

std::map<val_type, int> Series::value_counts(){
    std::map<val_type, int> mp;
    int i;
    for(i=0; i<values.size(); i++){
        mp[values[i]]++;
    }
    return mp;
}