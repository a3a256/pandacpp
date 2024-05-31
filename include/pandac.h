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
#include <math.h>
#include <algorithm>

struct val_type{
    bool isnum=false;
    float num;
    std::string line = "";

    bool operator<(const val_type& other) const {
        // You need to define a meaningful comparison here.
        // This is just a simple example; adjust as per your requirements.
        // will be used in sets

        if(isnum){
            return num < other.num;
        }
        return line < other.line;
    }

    bool operator>(const val_type& other) const {
        // You need to define a meaningful comparison here.
        // This is just a simple example; adjust as per your requirements.
        // will be used in sets

        if(isnum){
            return num > other.num;
        }
        return line > other.line;
    }

    bool operator==(const val_type& other) const{
        if(isnum){
            return num == other.num;
        }
        return line == other.line;
    }
};

class Series{
    public:

        std::vector<val_type> values;
        std::vector<int> indices;
        std::string col_name;

        void to_series(std::vector<std::string> vals, std::string name = "", std::vector<int> index={});

        float mean();
        float sum();
        float median();
        val_type mode();
        void sort_values(bool ascending=true);
        void head(int n=5);
        void tail(int n=5);
        std::set<val_type> unique();
        int nunique();
        std::map<val_type, int> value_counts();

        // start testing and implementing cout overload operator for Series class
        friend std::ostream& operator<<(std::ostream& os, const Series& dt);
        Series operator+(std::string const &obj);
        Series operator+(int const &obj);
        Series operator+(float const&obj);
        Series operator+=(std::string const &obj);
        Series operator+=(int const &obj);
        Series operator+=(float const&obj);
        Series operator-(int const&obj);
        Series operator-(float const &obj);
        Series operator++();
        Series operator--();
        Series operator=(int const &obj);
        Series operator=(float const &obj);
        Series operator=(char const &obj);
        Series operator=(char* const &obj);
        Series operator=(std::string const &obj);
        // implement the following and add is_int maybe?
        Series operator*(int const &obj);
        Series operator*=(int const &obj);
        Series operator*(float const &obj);
        Series operator*=(float const &obj);

        // implement division operator
        Series operator/(int const &obj);
        Series operator/=(int const &obj);
        Series operator/(float const &obj);
        Series operator/=(float const &obj);


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

        void print_out_counts(std::map<val_type, int> mp){
            int indexes_size, col_size;
            indexes_size = INT_MIN;
            col_size = INT_MIN;
            for(auto it: mp){
                indexes_size = std::max((int)it.first.line.size(), indexes_size);
                col_size = std::max((int)std::to_string(it.second).size(), col_size);
            }
            std::string line, index, num;
            line = "";
            for(auto it: mp){
                index = it.first.line;
                while(index.size() < indexes_size){
                    index += ' ';
                }
                num = std::to_string(it.second);
                while(num.size() < col_size){
                    num = ' '+num;
                }
                line += index+num;
                line += '\n';
            }

            std::cout << line;
            return;
        }

        struct ascend_compare{
            inline bool operator() (val_type &a, val_type &b){
                return a < b;
            }
        };

        struct descend_compare{
            inline bool operator() (val_type &a, val_type &b){
                return a > b;
            }
        };
};

// IMPLEMENTATION OF OUTPUT OVERLOAD OPERATOR <<

std::ostream& operator<<(std::ostream &os, const Series &dt){
    int i, col_size;
    col_size = INT_MIN;
    for(i=0; i<dt.values.size(); i++){
        col_size = std::max((int)dt.values[i].line.size(), col_size);
    }
    std::string line = "";
    std::string val = "";
    for(i=0; i<5; i++){
        val = dt.values[i].line;
        while(val.size() < col_size){
            val = ' ' + val;
        }
        line += val + '\n';
        std::string().swap(val);
    }
    os << line;
    if(dt.values[0].isnum){
        os << "Type: numerical\n";
    }else{
        os << "Type: object\n";
    }
    os << "Name: " << dt.col_name << '\n';
    return os;
}

// IMPLEMENTATION OF ONCREMENT OVERLOAD OPERATOR

Series Series::operator+(std::string const& obj){
    int i;
    val_type t;
    for(i=0; i<values.size(); i++){
        if(values[i].isnum){
            values[i].isnum = false;
            values[i].num = -0.0f;
        }
        values[i].line += obj;
    }

    return *this;
}

Series Series::operator+=(std::string const& obj){
    int i;
    val_type t;
    for(i=0; i<values.size(); i++){
        if(values[i].isnum){
            values[i].isnum = false;
            values[i].num = -0.0f;
        }
        values[i].line += obj;
    }

    return *this;
}

Series Series::operator+(int const &obj){
    int i;
    val_type t;
    float val;
    for(i=0; i<values.size(); i++){
        if(values[i].isnum){
            val = values[i].num + (float)obj;
            t.isnum = true;
            t.num = val;
            t.line = std::to_string(val);
            values[i] = t;
        }else{
            values[i].line += std::to_string(obj);
        }
    }

    return *this;
}

Series Series::operator+=(int const &obj){
    int i;
    val_type t;
    float val;
    for(i=0; i<values.size(); i++){
        if(values[i].isnum){
            val = values[i].num + (float)obj;
            t.isnum = true;
            t.num = val;
            t.line = std::to_string(val);
            values[i] = t;
        }else{
            values[i].line += std::to_string(obj);
        }
    }

    return *this;
}

// IMPLEMENTATION OF ++ OVERLOAD OPERATOR

Series Series::operator++(){
    int i;
    val_type t;
    float val;
    for(i=0; i<values.size(); i++){
        if(values[i].isnum){
            val = values[i].num + 1.0f;
            t.isnum = true;
            t.num = val;
            t.line = std::to_string(val);
            values[i] = t;
        }else{
            throw std::invalid_argument("Cannot increment string by 1\n");
        }
    }

    return *this;
}

// IMPLEMENTATION OF - OVERLOAD OPERATOR SUBTRACTING INTEGER

Series Series::operator-(int const &obj){
    int i;
    val_type t;
    float val;
    for(i=0; i<values.size(); i++){
        if(values[i].isnum){
            val = values[i].num - (float)obj;
            t.isnum = true;
            t.num = val;
            t.line = std::to_string(val);
            values[i] = t;
        }else{
            throw std::invalid_argument("Cannot substract int from string\n");
        }
    }

    return *this;
}

// IMPLEMENTATION OF -- OVERLOAD OPERATOR

Series Series::operator--(){
    int i;
    val_type t;
    float val;
    for(i=0; i<values.size(); i++){
        if(values[i].isnum){
            val = values[i].num - 1.0f;
            t.isnum = true;
            t.num = val;
            t.line = std::to_string(val);
            values[i] = t;
        }else{
            throw std::invalid_argument("Cannot increment string by 1\n");
        }
    }

    return *this;
}

// IMPLEMENTATION OF ADDITION OVERLOAD OPERATORS ADDING FLOAT

Series Series::operator+(float const &obj){
    int i;
    val_type t;
    for(i=0; i<values.size(); i++){
        if(values[i].isnum){
            values[i].num += obj;
            values[i].line += std::to_string(values[i].num);
        }else{
            values[i].line += std::to_string(obj);
        }
    }

    return *this;
}

Series Series::operator+=(float const &obj){
    int i;
    val_type t;
    for(i=0; i<values.size(); i++){
        if(values[i].isnum){
            values[i].num += obj;
            values[i].line += std::to_string(values[i].num);
        }else{
            values[i].line += std::to_string(obj);
        }
    }

    return *this;
}

// IMPLEMENTATION OF ADDITION OVERLOAD OPERATORS decreasing values by FLOAT value

Series Series::operator-(float const &obj){
    int i;
    val_type t;
    float val;
    for(i=0; i<values.size(); i++){
        if(values[i].isnum){
            val = values[i].num - (float)obj;
            t.isnum = true;
            t.num = val;
            t.line = std::to_string(val);
            values[i] = t;
        }else{
            throw std::invalid_argument("Cannot substract int from string\n");
        }
    }

    return *this;
}

// IMPLEMENTATION OF ASSIGNMENT OVERLOAD OPERATORS

Series Series::operator=(int const &obj){
    int i;
    for(i=0; i<values.size(); i++){
        values[i].isnum = true;
        values[i].num = (float)obj;
        values[i].line = std::to_string(values[i].num);
    }

    return *this;
}

Series Series::operator=(float const &obj){
    int i;
    for(i=0; i<values.size(); i++){
        values[i].isnum = true;
        values[i].num = obj;
        values[i].line = std::to_string(values[i].num);
    }

    return *this;
}

Series Series::operator=(std::string const &obj){
    int i;
    for(i=0; i<values.size(); i++){
        values[i].isnum = false;
        values[i].line = obj;
    }

    return *this;
}

Series Series::operator=(char const &obj){
    int i;
    for(i=0; i<values.size(); i++){
        values[i].isnum = false;
        values[i].line = "";
        values[i].line += obj;
    }

    return *this;
}

Series Series::operator=(char* const &obj){
    int i, j;
    for(i=0; i<values.size(); i++){
        values[i].isnum = false;
        values[i].line = "";
        for(j=0; obj[j] != '\0'; j++){
            values[i].line += obj[j];
        }
    }

    return *this;
}

// IMPLEMENTATION OF NULTIPLICATION OVERLOADING OPERATORS
// amend values.line

Series Series::operator*(int const &obj){
    int i, j;
    val_type t;
    for(i=0; i<values.size(); i++){
        if(values[i].isnum){
            values[i].num *= obj;
            values[i].line += std::to_string(values[i].num);
        }else{
            for(j=1; j<obj; j++){
                values[i].line += values[i].line;
            }
        }
    }

    return *this;
}

Series Series::operator*=(int const &obj){
    int i, j;
    val_type t;
    for(i=0; i<values.size(); i++){
        if(values[i].isnum){
            values[i].num *= obj;
            values[i].line += std::to_string(values[i].num);
        }else{
            for(j=1; j<obj; j++){
                values[i].line += values[i].line;
            }
        }
    }

    return *this;
}

Series Series::operator*(float const &obj){
    int i, j;
    val_type t;
    for(i=0; i<values.size(); i++){
        if(values[i].isnum){
            values[i].num *= obj;
            values[i].line += std::to_string(values[i].num);
        }else{
            throw std::invalid_argument("Could not multiply float with string\n");
        }
    }

    return *this;
}

// IMPLEMENTATION OF DIVISION OVERLOAD OPERATOR

Series Series::operator/(int const &obj){
    int i, j;
    val_type t;
    for(i=0; i<values.size(); i++){
        if(values[i].isnum){
            values[i].num /= obj;
            values[i].line += std::to_string(values[i].num);
        }else{
            throw std::invalid_argument("Could not divide int by string value\n");
        }
    }

    return *this;
}

Series Series::operator/=(int const &obj){
    int i, j;
    val_type t;
    for(i=0; i<values.size(); i++){
        if(values[i].isnum){
            values[i].num /= obj;
            values[i].line = std::to_string(values[i].num);
        }else{
            throw std::invalid_argument("Could not divide int by string value\n");
        }
    }

    return *this;
}

Series Series::operator/(float const &obj){
    int i, j;
    val_type t;
    for(i=0; i<values.size(); i++){
        if(values[i].isnum){
            values[i].num /= obj;
            values[i].line = std::to_string(values[i].num);
        }else{
            throw std::invalid_argument("Could not divide float by string value\n");
        }
    }

    return *this;
}

Series Series::operator*=(float const &obj){
    int i, j;
    val_type t;
    for(i=0; i<values.size(); i++){
        if(values[i].isnum){
            values[i].num /= obj;
            values[i].line += std::to_string(values[i].num);
        }else{
            throw std::invalid_argument("Could not divide float by string value\n");
        }
    }

    return *this;
}

val_type Series::mode(){
    std::map<val_type, int> counts;
    int i, popularity;
    for(i=0; i<values.size(); i++){
        counts[values[i]]++;
    }
    popularity = INT_MIN;
    for(auto it: counts){
        popularity = std::max(popularity, it.second);
    }
    val_type t;
    for(auto it: counts){
        if(it.second == popularity){
            t = it.first;
            break;
        }
    }
    return t;
}

void Series::to_series(std::vector<std::string> vals, std::string name, std::vector<int> index){
        col_name = name;
        int i;
        for(i=0; i<vals.size(); i++){
            indices.push_back(i);
            val_type t;
            if(is_number(vals[i])){
                t.isnum = true;
                t.num = std::stof(vals[i]);
            }
            t.line = vals[i];
            values.push_back(t);
        }
        if(index.size() != 0){
            if(index.size() != values.size()){
                throw std::invalid_argument("Incorrect number of indices");
            }
            indices = index;
        }
        return;
}

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
            throw std::invalid_argument("Cannot convert 'str' to scalar value\n");
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
    print_out_counts(mp);
    return mp;
}

void Series::sort_values(bool ascending){
    // sort by vector of pairs and resent indices
    if(ascending){
        std::sort(values.begin(), values.end(), ascend_compare());
        return;
    }
    std::sort(values.begin(), values.end(), descend_compare());
    return;
}

void Series::head(int n){
    int overall_size = (int)values.size();
    int limit = std::min(overall_size, n);
    int i, cur_val_size, whole_space, j;
    whole_space = INT_MIN;
    for(i=0; i<limit; i++){
        cur_val_size = values[i].line.size();
        whole_space = std::max(whole_space, cur_val_size);
    }
    whole_space = std::max(whole_space, (int)col_name.size());
    std::string res = "", line = "";
    cur_val_size = col_name.size();
    for(j=0; j<whole_space-cur_val_size; j++){
        line += ' ';
    }
    line += col_name;
    res += line;
    res += '\n';
    line = "";
    for(i=0; i<limit; i++){
        cur_val_size = values[i].line.size();
        for(j=0; j<whole_space-cur_val_size; j++){
            line += ' ';
        }
        line += values[i].line;
        res += line;
        res += '\n';
        line = "";
    }
    std::cout << res;
    return;
}

void Series::tail(int n){
    int overall_size = (int)values.size();
    int limit = std::min(overall_size, n);
    int i, cur_val_size, whole_space, j;
    whole_space = INT_MIN;
    for(i=values.size()-limit; i<values.size(); i++){
        cur_val_size = (int)values[i].line.size();
        whole_space = std::max(whole_space, cur_val_size);
    }
    whole_space = std::max(whole_space, (int)col_name.size());
    std::string res = "", line = "";
    cur_val_size = col_name.size();
    for(j=0; j<whole_space-cur_val_size; j++){
        line += ' ';
    }
    line += col_name;
    res += line;
    res += '\n';
    line = "";
    for(i=values.size()-limit; i<values.size(); i++){
        cur_val_size = values[i].line.size();
        for(j=0; j<whole_space-cur_val_size; j++){
            line += ' ';
        }
        line += values[i].line;
        res += line;
        res += '\n';
        line = "";
    }
    std::cout << res;
    return;
}

float Series::median(){
    std::vector<val_type> copy;
    copy = values;
    std::sort(copy.begin(), copy.end(), ascend_compare());
    int size = (int)copy.size();
    int mid = size/2;
    if(size%2 == 0){
        float mean = (copy[mid].num + copy[mid+1].num)/2.0f;
        val_type t;
        t.isnum = true;
        t.num = mean;
        t.line = std::to_string(mean);

        return t.num;
    }

    return copy[mid].num;
}


class DataFrame{
    public:
        std::map<std::string, Series> df;
        std::map<std::string, std::vector<std::string>> temp_df;
        std::vector<std::vector<val_type>> values;
        std::vector<int> index;
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

        void unique(std::vector<std::string> value);

        void nunique(std::vector<std::string> value);

        void sort_by(std::string column, bool ascending = true);

        std::map<std::string, int> value_counts(std::string col);

        void operator[](std::string name);
        void operator[](char* name);


    private:

        class Compare{
            public:
                Compare(int col, bool asc): col_(col), asc_(asc){}
                bool operator() (std::vector<val_type> &a, std::vector<val_type> &b){
                    if(asc_){
                        return a[col_] < b[col_];
                    }else{
                        return a[col_] > b[col_];
                    }
                }
            private:
                int col_;
                bool asc_;
        };

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
                columns.erase(columns.begin()+index);
            }

            if(name != ""){
                df.erase(name);
                int j;
                for(j=0; j<columns.size(); j++){
                    if(columns[j] == name){
                        columns.erase(columns.begin()+j);
                        break;
                    }
                }
            }
        }

        void print_out_counts(std::map<val_type, int> mp){
            int indexes_size, col_size;
            indexes_size = INT_MIN;
            col_size = INT_MIN;
            for(auto it: mp){
                indexes_size = std::max((int)it.first.line.size(), indexes_size);
                col_size = std::max((int)std::to_string(it.second).size(), col_size);
            }
            std::string line, index, num;
            line = "";
            for(auto it: mp){
                index = it.first.line;
                while(index.size() < indexes_size){
                    index += ' ';
                }
                num = std::to_string(it.second);
                while(num.size() < col_size){
                    num = ' '+num;
                }
                line += index+num;
                line += '\n';
            }

            std::cout << line;
            return;
        }

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

        void internal_append_row(std::stringstream &s, std::string word, char delim, std::vector<std::string> &cols){
            int col = 0;
            std::vector<val_type> row;
            while(std::getline(s, word, delim)){
                val_type t;
                if(is_number(word)){
                    t.isnum = true;
                    t.num = std::stof(word);
                }
                t.line = word;
                row.push_back(t);
                // temp_df[columns[col]].push_back(word);
                // col++;
            }
            if(cols.size() != 0 && columns.size() == 0){
                if(row.size() == cols.size()){
                    columns = cols;
                }else{
                    throw std::invalid_argument("The amount of entered columns does not match the amount of columns in CSV file\n");
                }
            }
            for(col=0; col<columns.size(); col++){
                temp_df[columns[col]].push_back(row[col].line);
            }
            values.push_back(row);
        }

        bool check_float(std::string &value){
            std::istringstream iss(value);
            float f;
            iss >> std::noskipws >> f;

            return iss.eof() && !iss.fail();
        }
        // maybe remove integer
        bool check_int(std::string &value){
            return !value.empty() && std::all_of(value.begin(), value.end(), ::isdigit);
        }


        // YET TO CHANGE UNIQUE FUNCTION
        // loop over Series and print out their unique results in the form of hash maps
        void unique_vals(std::string col){
            for(int i=0; i<df[col].values.size(); i++){
                stk.insert(df[col].values[i]);
            }
        }
};

// implement the following

void DataFrame::operator[](std::string name){
    if(df.find(name) == df.end()){
        continue;
    }
    return;
}

void DataFrame:: operator[](char* name){
    return;
};

template <typename T> void DataFrame::to_dataframe(std::map<std::string, std::vector<T>> df_p){
    std::string line = "";
    std::vector<std::string>().swap(columns);
    std::set<int> sizes;
    for(auto it: df_p){
        sizes.insert((int)it.second.size());
    }
    if(sizes.size() > 1){
        throw std::invalid_argument("All columns must have the same number of values");
    }
    std::set<int>().swap(sizes);
    for(auto it: df_p){
        columns.push_back(it.first);
        Series s;
        s.to_series(it.second);
        df[it.first] = s;
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
    int row_num = 0;
    while(std::getline(fin, line)){
        std::stringstream s(line);
        if(row_num >= head){
            std::cout << "row number " << row_num << '\n';
            index.push_back(row_num-head);
            internal_append_row(s, word, delimeter, cols);
        }
        row_num++;
    }
    row_num -= head+1;
    shape.first = row_num;
    shape.second = columns.size();
    for(auto it: temp_df){
        Series col;
        col.to_series(it.second, it.first);
        df[it.first] = col;
    }
    return;
}

void DataFrame::head(int l = 5){
    int i, j, biggest_size, diff, pad, limiter, vv;
    std::string line = "";
    biggest_size = INT_MIN;
    for(i=0; i<columns.size(); i++){
        biggest_size = std::max((int)columns[i].size(), biggest_size);
        for(j=0; j<df[columns[i]].values.size(); j++){
            vv = (int)df[columns[i]].values[j].line.size();
            biggest_size = std::max(vv, biggest_size);
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
    vv = (int)df[columns[0]].values.size();
    limiter = std::min(l, vv);
    for(i=0; i<limiter; i++){
        for(j=0; j<columns.size(); j++){
            diff = biggest_size - df[columns[j]].values[i].line.size();
            for(pad=0; pad<diff; pad++){
                line += ' ';
            }
            line += df[columns[j]].values[i].line + ' ';
        }
        line.pop_back();
        line += '\n';
    }
    std::cout << line;
}

void DataFrame::tail(int l = 5){
    int i, j, biggest_size, diff, pad, limiter, vv;
    std::string line = "";
    biggest_size = INT_MIN;
    for(i=0; i<columns.size(); i++){
        biggest_size = std::max((int)columns[i].size(), biggest_size);
        for(j=0; j<df[columns[i]].values.size(); j++){
            vv = (int)df[columns[i]].values[j].line.size();
            biggest_size = std::max(vv, biggest_size);
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
    vv = (int)df[columns[0]].values.size();
    limiter = std::min(l, vv);
    for(i=df[columns[0]].values.size()-limiter; i<df[columns[0]].values.size(); i++){
        for(j=0; j<columns.size(); j++){
            diff = biggest_size - df[columns[j]].values[i].line.size();
            for(pad=0; pad<diff; pad++){
                line += ' ';
            }
            line += df[columns[j]].values[i].line + ' ';
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
    len = df[columns[0]].values.size();
    for(i=0; i<len-1; i++){
        for(j=0; j<columns.size()-1; j++){
            fout << df[columns[j]].values[i].line << ';';
        }
        fout << df[columns[j]].values[i].line << '\n';
    }
    for(j=0; j<columns.size()-1; j++){
        fout << df[columns[j]].values[i].line << ';';
    }
    fout << df[columns[j]].values[i].line;
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

void DataFrame::unique(std::vector<std::string> cols){
    // YET TO REMOVE THIS PART
    // std::string name;
    // int i;
    // for(i=0; i<cols.size(); i++){
    //     if(df.find(cols[i]) == df.end()){
    //         name = "Column " + cols[i] + " not found in scope\n";
    //         throw std::invalid_argument(name);
    //     }
    // }
    // std::set<val_type>().swap(stk);
    // unique_vals(value);
    // for(auto it: stk){
    //     std::cout << it.line << " ";
    // }
    // std::set<val_type>().swap(stk);
    // std::cout << "\n";
}

void DataFrame::nunique(std::vector<std::string> cols){
    if(cols.size() > columns.size()){
        throw std::invalid_argument("Number of columns is more than the actual amount of columns\n");
    }
    int i;
    for(i=0; i<cols.size(); i++){
        if(df.find(cols[i]) == df.end()){
            std::string name = "Column " + cols[i] + " not found in scope\n";
            throw std::invalid_argument(name);
        }
    }
    std::map<std::string, int> mp;
    for(i=0; i<cols.size(); i++){
        mp[cols[i]] = df[cols[i]].nunique();
    }
    int indexes_size, col_size;
    indexes_size = INT_MIN;
    col_size = INT_MIN;
    for(auto it: mp){
        indexes_size = std::max((int)it.first.size(), indexes_size);
        col_size = std::max((int)std::to_string(it.second).size(), col_size);
    }
    std::string line, index, num;
    line = "";
    for(auto it: mp){
        index = it.first;
        while(index.size() < indexes_size){
            index += ' ';
        }
        num = std::to_string(it.second);
        while(num.size() < col_size){
            num = ' '+num;
        }
        line += index + num;
        line += '\n';
    }
    std::cout << line;
}

void DataFrame::encode_categoricals(std::vector<std::string> cols){
    int i, index;
    std::map<std::string, std::set<std::string>> ecn;
    for(i=0; i<cols.size(); i++){
        if(df.find(cols[i]) != df.end()){
            for(val_type s: df[cols[i]].values){
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
        for(i=0; i<df[it.first].values.size(); i++){
            df[it.first].values[i].line = encoder[it.first][df[it.first].values[i].line];
            df[it.first].values[i].num = std::stof(df[it.first].values[i].line);
        }
    }
}

void DataFrame::sort_by(std::string column, bool ascending){
    if(df.find(column) == df.end()){
        std::string error_line = column + " was not found in the range\n";
        throw std::invalid_argument(error_line);
    }
    int i, j, col_index = 0;
    for(i=0; i<columns.size(); i++){
        if(columns[i] == column){
            col_index = i;
            break;
        }
    }
    std::sort(values.begin(), values.end(), Compare(col_index, ascending));
    for(i=0; i<df[columns[0]].values.size(); i++){
        for(j=0; j<columns.size(); j++){
            df[columns[j]].values[i] = values[i][j];
        }
    }
}

std::map<std::string, int> DataFrame::value_counts(std::string col){
    std::map<std::string, int> counts;
    int i, space;
    for(i=0; i<df[col].values.size(); i++){
        counts[df[col].values[i].line]++;
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