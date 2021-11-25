
#include "graph.h"

void print_graph(map<string,GraphNode> &circuit)
{
    for(const auto& m : circuit){
        cout << "ID:" << m.first << endl;
        cout << "ID num:" << m.second.id << endl;
        cout << "type:" <<  m.second.type<<endl;
        cout << "in wire:" ;
        
        for(int i=0;i<m.second.in_wire.size();++i)  cout << m.second.in_wire[i]<<" ";
        
        cout << endl;
        cout << "out wire:" <<  m.second.out_wire[0]<<endl;
        
        cout<<endl;
        
        if(!m.second.in_graph.empty()) cout<<"in graph:" << endl;
        for(int i=0;i<m.second.in_graph.size();++i){
            cout << m.second.in_graph[i]<<":";
            //cout << (m.second.input_pin.at(m.second.out_graph[i])).size()<<endl;
            cout << (m.second.output_pin.at(m.second.in_graph[i]));
            /*
            for(int j=0;j<(m.second.output_pin.at(m.second.in_graph[i])).size();++j){
                cout << (m.second.output_pin.at(m.second.in_graph[i])).at(j)<<" ";
            }
            */
            cout << endl;
        }
        
        
        if(!m.second.out_graph.empty()) cout<<"out graph:" << endl;
        for(int i=0;i<m.second.out_graph.size();++i){
            cout << m.second.out_graph[i]<<":";
            cout << (m.second.input_pin.at(m.second.out_graph[i]));
            //cout << (m.second.input_pin.at(m.second.out_graph[i])).size()<<endl;
            /*
            for(int j=0;j<(m.second.input_pin.at(m.second.out_graph[i])).size();++j){
                cout << (m.second.input_pin.at(m.second.out_graph[i])).at(j)<<" ";
            }
            */
            cout << endl;
        }
        
        
        cout << endl;
        cout << "output capacitance:"<< m.second.outcap << endl;
        cout << "logic result:"<< m.second.logic_result << endl<<endl;
        cout << "delay:" << m.second.delay << endl;
        cout << "total delay:" << m.second.total_delay << endl;
        cout << "transition time:" << m.second.transition_time << endl;
        //cout <<"run or not:"<< m.second.run_through <<endl;
        if( m.second.path_choose_in != " "){
            cout << "path choose in:" << m.second.path_choose_in << endl;
        }
        cout << endl<<endl;
    }
}
//return the connection situation
vector<string> search_outgraph(map<string,GraphNode> &circuit,string wire,map<string,int> &input_pin)
{
    vector<string> return_id;
    for(const auto& m : circuit){
        for(int i=0;i<m.second.in_wire.size();++i){
            if(wire == m.second.in_wire[i]){
                input_pin[m.first] = i;
                return_id.push_back(m.first);
            }
        }
    }  
    return  return_id;
}

vector<string> search_ingraph(map<string,GraphNode> &circuit,vector<string> &wire,map<string,int> &output_pin){
    vector<string> return_id;
    
    for(const auto& m : circuit){
        for(int i=0;i<m.second.out_wire.size();++i){
            if(wire[0] == m.second.out_wire[i]){
                return_id.push_back(m.first);
                output_pin[m.first] = 0;
            }
            //nand nor
            if(wire.size() > 1){
                if((wire[1] == m.second.out_wire[i])){
                    return_id.push_back(m.first);
                    output_pin[m.first] = 1;
                }
            }
        }
    }
    return  return_id;
}