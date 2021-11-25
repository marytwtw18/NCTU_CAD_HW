#ifndef FUNC_H
#define FUNC_H

#include<iostream>
using namespace std;
#include <sstream>
#include<map>
#include<vector>
#include<string>
#include "graph.h"

struct data{
    double a1;
    double a2;
    double zn;
    //need to store four look up table
    vector<double> index_outcap;
    vector<double> index_transiton_time;
    vector< vector<double> > table_cell_rise,table_cell_fall; //caculate delay
    vector< vector<double> > table_rise_transition,table_fall_transition; //calculate transtion time
};

typedef struct data libdata;
typedef pair<string,GraphNode> Pair;

/*---------------------function initialized-----------------------------------*/
void splitStr2Vec(string str, vector<string>& buf);
string RemoveSpaces(const std::string& str);
void print_lib(map<string,libdata> &lib_data);
bool compare(const Pair &p1,const Pair &p2);
double calculation(double cap1,double cap2,double in_trans1,double in_trans2,double n1,double n2,double n3,double n4,double cap,double trans);
void cal_logic(map<string,GraphNode> &circuit,string id,map<string,int> index);
void cal_delay(map<string,GraphNode> &circuit,string id,map<string,int> index, map<string,libdata> lib_data);

#endif