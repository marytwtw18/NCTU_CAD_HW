#include<iostream>
#include<algorithm>
#include<vector>
#include<fstream>
#include <sstream>
#include<string>
#include <deque>
#include<map>
#include<stack>

#include "graph.h"
#include "func.h"

using namespace std;

int main(int argc,char* argv [])
{
    ifstream infile,infile2,infile3;
    ofstream outfile,outfile2,outfile3;
    vector<string> input_node,output_node,wire_node;
    string instr;

    //combinational circuit     
    map<string,GraphNode> circuit;   
    
    //lib data
    map<string,libdata> lib_data;  
    
    deque<string> undo_queue; //to calculate delay
    
    infile.open(argv[1]);
    infile2.open(argv[3]);
    infile3.open(argv[5]);
    
    string new_argv = argv[1];
    
    string sub_str = new_argv.substr(0,new_argv.size()-2);
    
    outfile.open("0710880_"+sub_str+"_load.txt");
    outfile2.open("0710880_"+sub_str+"_delay.txt");
    outfile3.open("0710880_"+sub_str+"_path.txt");

/*-----------------------read in lib file---------------------*/
    if(!infile3.is_open())
    {
        cout << "fail to open" << endl;
    }
    else
    {    
        string cell_type;
        string pin;
        double cap;
        libdata newnode;
        
        bool timing = false;
        bool cell_rise =false;
        bool cell_fall =false;
        bool rise_transition = false;
        bool fall_transition = false;
        
        //read lib data and build look up table
        while(!infile3.eof()){
            vector<string> new_vec;
            getline(infile3,instr);
            instr = RemoveSpaces(instr);
            //cout <<instr << endl;
            splitStr2Vec(instr,new_vec);
            /*
            for(int i=0;i<new_vec.size();++i){
                cout <<new_vec[i]<<" ";
            }
            cout << endl;
            */
            if(new_vec.size())
            {
                if(new_vec[0] =="cell"){
                    if(cell_type!="\0"){
                        lib_data[cell_type] = newnode;
                        newnode.table_cell_rise.clear();
                        newnode.table_cell_fall.clear();
                        newnode.table_rise_transition.clear();
                        newnode.table_fall_transition.clear();
                    }
                    cell_type = new_vec[1];
                    
                    timing = false;
                    cell_rise =false;
                    cell_fall =false;
                    rise_transition = false;
                    fall_transition = false;
                    //cout <<endl<< cell_type <<endl;
                }
                else if(new_vec[0] == "index_1"){
                    for(int i=1;i<new_vec.size();++i){
                        newnode.index_outcap.push_back(stod(new_vec[i]));
                    }
                }
                else if(new_vec[0] == "index_2"){
                    for(int i=1;i<new_vec.size();++i){
                        newnode.index_transiton_time.push_back(stod(new_vec[i]));
                    }
                }
                else if(new_vec[0] =="pin"){
                    pin = new_vec[1];
                    //cout << pin <<endl;
                }
                else if(new_vec[0] =="capacitance"){
                    cap = stod(new_vec[1]);
                    if( (pin=="A1") || (pin=="I")){
                        newnode.a1 = cap;
                    }
                    else if(pin=="A2"){
                        newnode.a2 = cap;
                    }
                    else if(pin=="ZN"){
                        newnode.zn = cap;
                    }
                    //cout << cap <<endl;
                }
                else if(new_vec[0] == "timing"){
                    timing = true;
                    fall_transition = false;
                    rise_transition = false;
                    cell_rise = false;
                    cell_fall = false;
                }
                else if(new_vec[0] == "cell_rise"){
                    cell_rise = true;
                }
                else if(new_vec[0] == "cell_fall"){
                    cell_fall = true;
                    cell_rise = false;
                }
                else if(new_vec[0] == "rise_transition"){
                    rise_transition = true;
                    cell_fall = false;
                }
                else if(new_vec[0] == "fall_transition"){
                    fall_transition = true;
                    rise_transition = false;
                }
                else if(new_vec[0] == "values" && timing ){
                    
                    vector<double> cell;
                    for(int i=1;i<new_vec.size();++i){
                        cell.push_back(stod(new_vec[i]));
                    }
                    
                    if(cell_rise){
                        newnode.table_cell_rise.push_back(cell);
                    }
                    else if(cell_fall){
                        newnode.table_cell_fall.push_back(cell);
                    }
                    else if(rise_transition){
                        newnode.table_rise_transition.push_back(cell);
                    }
                    else if(fall_transition){
                        newnode.table_fall_transition.push_back(cell);
                    }
                }
                else{
                    vector<double> cell;
                    if(timing){
                        for(int i=0;i<new_vec.size();++i){
                            cell.push_back(stod(new_vec[i]));
                        }
                        if(cell_rise){
                            newnode.table_cell_rise.push_back(cell);
                        }
                        else if(cell_fall){
                            newnode.table_cell_fall.push_back(cell);
                        }
                        else if(rise_transition){
                            newnode.table_rise_transition.push_back(cell);
                        }
                        else if(fall_transition){
                            newnode.table_fall_transition.push_back(cell);
                        }
                    }
                }
            }
            //cout << endl;
        }
        
        lib_data[cell_type] = newnode;
    }
    
    //read verilog data
    if(!infile.is_open())
    {
        cout << "fail to open" << endl;
    }
    else
    {    
        while(!infile.eof()){
            vector<string> new_vec;
            getline(infile,instr);
            instr = RemoveSpaces(instr);
            splitStr2Vec(instr,new_vec);
            
            //for(int i=0;i<new_vec.size();++i)  cout << new_vec[i]<<" ";
            //cout << endl;
            
            if(!new_vec.empty()){
                if(new_vec[0] == "input"){
                    for(int i=1;i<new_vec.size();++i){
                        input_node.push_back( new_vec[i]);
                        //cout << new_vec[i]<<" ";
                    }
                    //cout << endl;
                }
                else if(new_vec[0] == "output"){
                    for(int i=1;i<new_vec.size();++i){
                        output_node.push_back( new_vec[i]);
                        //cout << new_vec[i]<<" ";
                    }
                    //cout << endl;
                }
                else if(new_vec[0] == "wire"){
                    for(int i=1;i<new_vec.size();++i){
                        wire_node.push_back( new_vec[i]);
                        //cout << new_vec[i]<<" ";
                    }
                    //cout << endl;
                }
                else if(new_vec[0] == "INVX1"){
                    GraphNode new_node;
                    string str;
                    str = str.assign(new_vec[1],1,new_vec[1].size()-1);
                    new_node.id = stoi(str);
                    new_node.type = "INVX1";
                    for(int i=0;i<new_vec.size();++i){
                        if(new_vec[i] == ".I"){
                            new_node.in_wire.push_back(new_vec[i+1]);
                        }
                        else if(new_vec[i] == ".ZN"){
                            new_node.out_wire.push_back(new_vec[i+1]);
                        }
                    }
                    new_node.run_through = false;
                    circuit[new_vec[1]] = new_node;
                }
                else if(new_vec[0] == "NOR2X1") {
                    GraphNode new_node;
                    string str;
                    str = str.assign(new_vec[1],1,new_vec[1].size()-1);
                    new_node.id = stoi(str);
                    new_node.type = "NOR2X1";
                    
                    for(int i=0;i<new_vec.size();++i){
                        if((new_vec[i] == ".A1") || (new_vec[i] == ".A2")){
                            new_node.in_wire.push_back(new_vec[i+1]);
                        }
                        else if(new_vec[i] == ".ZN"){
                            new_node.out_wire.push_back(new_vec[i+1]);
                        }
                    }
                    new_node.run_through = false;
                    circuit[new_vec[1]] = new_node;
                }   
                else if(new_vec[0] == "NANDX1"){
                    GraphNode new_node;
                    string str;
                    str = str.assign(new_vec[1],1,new_vec[1].size()-1);
                    new_node.id = stoi(str);
                    new_node.type = "NANDX1";
                    
                    for(int i=0;i<new_vec.size();++i){
                        if((new_vec[i] == ".A1") || (new_vec[i] == ".A2")){
                            new_node.in_wire.push_back(new_vec[i+1]);
                        }
                        else if(new_vec[i] == ".ZN"){
                            new_node.out_wire.push_back(new_vec[i+1]);
                        }
                    }
                    new_node.run_through = false;
                    circuit[new_vec[1]] = new_node;
                }
                //cout <<instr << endl;
            }
        }
    }
    
    //input imformation:read input array set
    vector<map<string,int>> input_data;
    
    if(!infile2.is_open())
    {
        cout << "fail to open" << endl;
    }
    else
    {    
        while(!infile2.eof()){
            map<string,int> data_in;
            vector<string> new_vec;
            getline(infile2,instr);
            instr = RemoveSpaces(instr);
            splitStr2Vec(instr,new_vec);
            
            /*
            for(int i=0;i<new_vec.size();++i)  cout << new_vec[i]<<" ";
            cout <<endl;
            */
            
            if((new_vec[0]=="0") || (new_vec[0]=="1")){
                for(int i=0; i < input_node.size() ;++i){
                    data_in[input_node[i]] = stoi(new_vec[i]);
                }
                input_data.push_back(data_in);
            }
            //cout << endl;
            //cout <<instr << endl;           
        }
    }
    
    //store out node
    vector<string> end_node;
    for(auto itr = circuit.begin();itr != circuit.end(); ++itr){
        for(int i=0;i<output_node.size();++i){
            if(itr->second.out_wire[0] == output_node[i]){
                end_node.push_back(itr->first);
            }
        }
    }
    
    
/*go through the graph(n2) and build the adjancy list*/    
    for(auto itr=circuit.begin();itr != circuit.end() ;++itr){
        itr->second.out_graph = search_outgraph(circuit,itr->second.out_wire[0],itr->second.input_pin);    
        itr->second.in_graph = search_ingraph(circuit,itr->second.in_wire,itr->second.output_pin);
        if(itr->second.in_graph.empty()){
            undo_queue.push_back(itr->first);
            //cout <<undo_queue.back()<<" ";
        }
    }
    
    for(auto itr=circuit.begin();itr != circuit.end() ;++itr){
        for(int i=0;i<itr->second.out_graph.size();++i){
            if(circuit[itr->second.out_graph[i]].type == "NOR2X1"){
                if(itr->second.input_pin.at(itr->second.out_graph[i]) == 0){
                    itr->second.outcap  += lib_data["NOR2X1"].a1;
                }
                else if(itr->second.input_pin.at(itr->second.out_graph[i]) == 1){
                    itr->second.outcap  += lib_data["NOR2X1"].a2;
                }
                //cout << itr->second.outcap <<endl;
            }
            else if(circuit[itr->second.out_graph[i]].type == "INVX1"){
                if(itr->second.input_pin.at(itr->second.out_graph[i]) == 0){
                    itr->second.outcap  += lib_data["INVX1"].a1;
                }
            }
            else if(circuit[itr->second.out_graph[i]].type == "NANDX1"){
                if(itr->second.input_pin.at(itr->second.out_graph[i]) == 0){
                    itr->second.outcap  += lib_data["NANDX1"].a1;
                }
                else if(itr->second.input_pin.at(itr->second.out_graph[i]) == 1){
                    itr->second.outcap  += lib_data["NANDX1"].a2;
                }
            }     
        }
        for(int i=0;i<output_node.size();++i){
            if(itr->second.out_wire[0] == output_node[i]){
                itr->second.outcap += 0.03;
                break;
            }
        }
        //if(itr->second.out_graph.empty())  itr->second.outcap = 0.03;
    }
    
    //order in number
    vector<Pair> vec;
    
    for(auto itr = circuit.begin();itr != circuit.end(); ++itr){
        vec.push_back(make_pair(itr->first,itr->second));
        if(itr !=  circuit.begin())  outfile <<endl;
        outfile << itr->first <<" "<<itr->second.outcap;
    }
    
    sort(vec.begin(),vec.end(),compare);
    
    for(auto itr = vec.begin();itr != vec.end(); ++itr){
        //cout << "ID:" << itr->first << endl;   
    }

    //cal delay and logic:using deque  
    
    for(int i=0;i<input_data.size();++i){
        
        //initialize
        for( auto& cir : circuit){
            cir.second.run_through = false;
            cir.second.logic_result = -1;
            cir.second.total_delay = 0;
            cir.second.delay = 0;
            cir.second.transition_time = 0;
            cir.second.path_choose_in = " ";
        }
        
        //deal with queue
        deque<string> undo_queue2 = undo_queue;
        bool in_queue = false;
        
        //queue end when run through all nodes
        while(!undo_queue2.empty()){
            
            //if is still input node at front,re push
            if(circuit[undo_queue2.front()].in_graph.empty()){
                cal_logic(circuit,undo_queue2.front(),input_data[i]);
                cal_delay(circuit,undo_queue2.front(),input_data[i],lib_data);
                
                circuit[undo_queue2.front()].run_through = true;
                for(int j=0;j<circuit[undo_queue2.front()].out_graph.size();++j){
                    //prevent redundant add
                    for(int k=0;k<undo_queue2.size();++k){
                        if(undo_queue2[k] == circuit[undo_queue2.front()].out_graph[j]){
                            in_queue = true;
                        }
                    }
                    if(!in_queue)  undo_queue2.push_back(circuit[undo_queue2.front()].out_graph[j]);
                    in_queue = false;    
                }
                undo_queue2.pop_front();
            }
            else{
                //front to back
                //改成 跳過忽略pop後面的 不燃會deadlock
                
                int pop_index = 0; //erase 的node
                deque<string>::iterator itr;
                itr = undo_queue2.begin();
                
                while(1){
                    if(circuit[undo_queue2[pop_index]].in_graph.size() == 1){
                        if(!circuit[circuit[undo_queue2[pop_index]].in_graph[0]].run_through){
                            ++pop_index;
                            ++itr;
                        }
                        else{
                            break;
                        }
                    }
                    else if(circuit[undo_queue2[pop_index]].in_graph.size() == 2){
                        if((circuit[circuit[undo_queue2[pop_index]].in_graph[0]].run_through) && (circuit[circuit[undo_queue2[pop_index]].in_graph[1]].run_through)){
                            break;
                        }
                        else{
                            ++pop_index;
                            ++itr;
                        }
                    }
                }
                cal_logic(circuit,undo_queue2[pop_index],input_data[i]);
                cal_delay(circuit,undo_queue2[pop_index],input_data[i],lib_data);
                
                circuit[undo_queue2[pop_index]].run_through = true;
                
                
                
                for(int j=0;j<circuit[undo_queue2[pop_index]].out_graph.size();++j){
                    for(int k=0;k<undo_queue2.size();++k){
                        if(undo_queue2[k] == circuit[undo_queue2[pop_index]].out_graph[j]){
                            in_queue = true;
                        }
                    }
                    if(!in_queue)  undo_queue2.push_back(circuit[undo_queue2[pop_index]].out_graph[j]);
                    in_queue = false;       
                }
                undo_queue2.erase(itr);
            }        
        }
        
        //cout <<endl;   
        //print_graph(circuit);
        for(auto itr = circuit.begin();itr != circuit.end(); ++itr){
            if((i != 0) && (itr ==  circuit.begin()))  outfile2 <<endl<<endl;
            if(itr !=  circuit.begin())  outfile2 <<endl;
            outfile2 << itr->first <<" "<<itr->second.logic_result<<" "
            << itr->second.delay<<" "<<itr->second.transition_time;
        }
        
        
        stack<string> path;
        double max_delay=0;
        string max_node;
        string path_node;
        
        for(int i=0;i<end_node.size();++i){
            if( max_delay < circuit[end_node[i]].total_delay){
                 max_delay = circuit[end_node[i]].total_delay;
                 max_node = end_node[i];
            }
        }
        //cout<<"Longest delay = "<<max_delay<<",the node is "<<max_node<<endl;
        if((i != 0) && (i != input_data.size())) outfile3 << endl; 
        outfile3 <<"Longest delay = "<<max_delay;
        outfile3 << ", the path is: ";
        
        path.push(circuit[max_node].out_wire[0]);
        
        path_node = circuit[max_node].path_choose_in;
        
        while(circuit[path_node].path_choose_in != " "){
            path.push(circuit[path_node].out_wire[0]);
            path_node = circuit[path_node].path_choose_in;
        }
        path.push(circuit[path_node].out_wire[0]);
        
        if(circuit[path_node].type == "INVX1"){
            path.push(circuit[path_node].in_wire[0]);
        }
        else if(circuit[path_node].type == "NANDX1"){
            if(input_data[i][circuit[path_node].in_wire[0]]==0){
                path.push(circuit[path_node].in_wire[0]);
            }
            else if(input_data[i][circuit[path_node].in_wire[1]]==0){
                path.push(circuit[path_node].in_wire[1]);
            }
            else{
                path.push(circuit[path_node].in_wire[0]);
            }
        }
        else if(circuit[path_node].type == "NOR2X1"){
            if(input_data[i][circuit[path_node].in_wire[0]]==1){
                path.push(circuit[path_node].in_wire[0]);
            }
            else if(input_data[i][circuit[path_node].in_wire[1]]==1){
                path.push(circuit[path_node].in_wire[1]);
            }
            else{
                path.push(circuit[path_node].in_wire[0]);
            }
        }
        
        while(!path.empty()){
            if(path.size()>1){
                outfile3 << path.top()<<" -> ";
            }
            else{
                outfile3 << path.top();
            }
            path.pop();
        }
    }

    return 0;
}