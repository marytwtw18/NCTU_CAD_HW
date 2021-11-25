#ifndef GRAPH_H
#define GRAPH_H

using namespace std;
#include<map>
#include<vector>
#include<string>
#include<iostream>
/*---------------------function initialized-----------------------------------*/


struct GraphNode{
    int id;
    string type;
    vector<string> in_wire;
    vector<string> out_wire;
    vector<string> in_graph;
    map<string,int> input_pin; //0:a1,1:a2
    vector<string> out_graph;
    map<string,int> output_pin; //0:a1,1:a2
    
    double outcap;
    
    int logic_result;
    double total_delay,delay,transition_time;
    bool run_through;
    string path_choose_in;
};
typedef struct GraphNode GraphNode;

void print_graph(map<string,GraphNode> &circuit);
vector<string> search_outgraph(map<string,GraphNode> &circuit,string wire,map<string,int> &input_pin);
vector<string> search_ingraph(map<string,GraphNode> &circuit,vector<string> &wire,map<string,int> &output_pin);

#endif