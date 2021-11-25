#include "graph.h"
#include"func.h"

void print_lib(map<string,libdata> &lib_data)
{
    for( auto m : lib_data){
        cout << endl;
        cout << "type:" << m.first<<endl;
        if( (m.first == "NOR2X1") || (m.first == "NANDX1") ){
            cout << "A1:" << m.second.a1 << endl;
            cout << "A2:" << m.second.a2 << endl;
            cout << "ZN:" << m.second.zn << endl;
        }
        else if(m.first == "INVX1"){
            cout << "I:" << m.second.a1 << endl;
            cout << "ZN:" << m.second.zn << endl;
        }
        if(!m.second.index_outcap.empty()){
            cout <<"index_outcap:"<<endl;
            for(int i=0;i<m.second.index_outcap.size();++i){
                cout << m.second.index_outcap[i]<<" ";
            }
            cout << endl;
        }
        if(!m.second.index_transiton_time.empty()){
            cout <<"index_transition time:"<<endl;
            for(int i=0;i<m.second.index_transiton_time.size();++i){
                cout << m.second.index_transiton_time[i]<<" ";
            }
            cout << endl;
        }
        if(!m.second.table_cell_rise.empty()){
            cout <<endl<<"table cell rise:"<<endl;
            for(int i=0;i<m.second.table_cell_rise.size();++i){
                for(int j=0;j<m.second.table_cell_rise[i].size();++j){
                    cout << m.second.table_cell_rise[i][j]<<" ";
                }
                cout << endl;
            }
            cout << endl;
        }
        if(!m.second.table_cell_fall.empty()){
            cout <<"table cell fall:"<<endl;
            for(int i=0;i<m.second.table_cell_fall.size();++i){
                for(int j=0;j<m.second.table_cell_fall[i].size();++j){
                    cout << m.second.table_cell_fall[i][j]<<" ";
                }
                cout << endl;
            }
            cout << endl;
        }
        if(!m.second.table_rise_transition.empty()){
            cout <<"table rise transition:"<<endl;
            for(int i=0;i<m.second.table_rise_transition.size();++i){
                for(int j=0;j<m.second.table_rise_transition[i].size();++j){
                    cout << m.second.table_rise_transition[i][j]<<" ";
                }
                cout << endl;
            }
            cout << endl;
        }
        if(!m.second.table_fall_transition.empty()){
            cout <<"table fall transition:"<<endl;
            for(int i=0;i<m.second.table_fall_transition.size();++i){
                for(int j=0;j<m.second.table_fall_transition[i].size();++j){
                    cout << m.second.table_fall_transition[i][j]<<" ";
                }
                cout << endl;
            }
            cout << endl;
        }
    }
}



bool compare(const Pair &p1,const Pair &p2){
    return p1.second.id < p2.second.id;
}

double calculation(double cap1,double cap2,double in_trans1,double in_trans2,double n1,double n2,double n3,double n4,double cap,double trans){
    double ans1,ans2,result;
    if((cap>=cap1) && (cap <= cap2)){
        if((trans >= in_trans1) && (trans <= in_trans2)){
            ans1 =  (cap2-cap)*n1/(cap2-cap1)+(cap-cap1)*n2/(cap2-cap1);
            ans2 = (cap2-cap)*n3/(cap2-cap1)+(cap-cap1)*n4/(cap2-cap1);
            result = (trans-in_trans1)*ans2/(in_trans2-in_trans1) + (in_trans2-trans)*ans1/(in_trans2-in_trans1);
        }
        else if(trans < in_trans1){
            ans1 =  (cap2-cap)*n1/(cap2-cap1)+(cap-cap1)*n2/(cap2-cap1);
            ans2 = (cap2-cap)*n3/(cap2-cap1)+(cap-cap1)*n4/(cap2-cap1); 
            result = ans2 - (ans2-ans1)*(in_trans2-trans)/(in_trans2-in_trans1);
        }
        else if(trans > in_trans2){
            ans1 =  (cap2-cap)*n1/(cap2-cap1)+(cap-cap1)*n2/(cap2-cap1);
            ans2 = (cap2-cap)*n3/(cap2-cap1)+(cap-cap1)*n4/(cap2-cap1); 
            result = ans1 + (ans2-ans1)*(trans-in_trans1)/(in_trans2-in_trans1);
        }
    }
    else if(cap < cap1){
        if(trans < in_trans1){
            ans1 = n2-((n2-n1)*(cap2-cap)/(cap2-cap1));
            ans2 = n4-((n4-n3)*(cap2-cap)/(cap2-cap1));
            result = ans2 - (ans2-ans1)*(in_trans2-trans)/(in_trans2-in_trans1);
        }
        else if((trans >= in_trans1) && (trans <= in_trans2)){
            ans1 = n2-((n2-n1)*(cap2-cap)/(cap2-cap1));
            ans2 = n4-((n4-n3)*(cap2-cap)/(cap2-cap1));
            result = (trans-in_trans1)*ans2/(in_trans2-in_trans1) + (in_trans2-trans)*ans1/(in_trans2-in_trans1);
        }
    }  
    else if(cap > cap2){
         if(trans > in_trans2){
            ans1 = n1+(n2-n1)*(cap-cap1)/(cap2-cap1);
            ans2 = n3+(n4-n3)*(cap-cap1)/(cap2-cap1);
            result = ans1 + (ans2-ans1)*(trans-in_trans1)/(in_trans2-in_trans1);
        }
        else if((trans >= in_trans1) && (trans <= in_trans2)){
            ans1 = n1+(n2-n1)*(cap-cap1)/(cap2-cap1);
            ans2 = n3+(n4-n3)*(cap-cap1)/(cap2-cap1);
            result = (trans-in_trans1)*ans2/(in_trans2-in_trans1) + (in_trans2-trans)*ans1/(in_trans2-in_trans1);
        }
    }
    return result;
}

//deal with string
void splitStr2Vec(string str, vector<string>& buf)
{
	int current = 0; //initial position
	int next; 
	while (1)
	{
		next = str.find_first_of(" ,\";\{()\\}:" , current);
		if (next != current)
		{
			string tmp = str.substr(current, next - current);
			if (tmp.size() != 0) //忽略空字串
				buf.push_back(tmp);
		}
		if (next == string::npos) break;
		current = next + 1; //下次由 next + 1 的位置開始找起。
	}
}

string RemoveSpaces(const std::string& str) {
    string out;                 // the result
    string word;                // used to extract words from str 
    istringstream ss(str);      // create an istringstream from str
    while(ss >> word) {              // extract a word
        if(!out.empty()) out += ' '; // add a space between words
        out += word;                 // add the extracted word 
    }
    return out;
}


void cal_logic(map<string,GraphNode> &circuit,string id,map<string,int> index){
    //cout << id <<":"<<endl;
    
    if(circuit[id].type == "INVX1"){
        if(!circuit[id].in_graph.empty()){
            circuit[id].logic_result = !( circuit[circuit[id].in_graph[0]].logic_result);
            //cout <<itr->first<<":"<< itr->second.logic_result << endl;
        }
        else{  //read in graph
            circuit[id].logic_result = !( index[circuit[id].in_wire[0]]);
            
        }
    }
    else if(circuit[id].type == "NANDX1"){
        //one or two
        if(circuit[id].in_graph.empty()){
            circuit[id].logic_result = !(index[circuit[id].in_wire[0]] * index[circuit[id].in_wire[1]]);
        }
        else if(circuit[id].in_graph.size() == 2){ 
            circuit[id].logic_result = !(circuit[circuit[id].in_graph[0]].logic_result * circuit[circuit[id].in_graph[1]].logic_result);
            //cout <<itr->first<<":"<< itr->second.logic_result << endl;
        }
        //only one input graph
        else{
            if(circuit[circuit[id].in_graph[0]].out_wire[0]==circuit[id].in_wire[0]){
                circuit[id].logic_result = !(circuit[circuit[id].in_graph[0]].logic_result * index[circuit[id].in_wire[1]]);
            }
            else{
                circuit[id].logic_result = !(circuit[circuit[id].in_graph[0]].logic_result * index[circuit[id].in_wire[0]]);
            }
        }
    }
    else if(circuit[id].type == "NOR2X1"){
         //one or two
        if(circuit[id].in_graph.empty()){
            circuit[id].logic_result = !(index[circuit[id].in_wire[0]] + index[circuit[id].in_wire[1]]);
        }
        else if(circuit[id].in_graph.size() == 2){ 
            circuit[id].logic_result = !(circuit[circuit[id].in_graph[0]].logic_result + circuit[circuit[id].in_graph[1]].logic_result);
            //cout <<itr->first<<":"<< itr->second.logic_result << endl;
        }
        //only one input graph
        else{
            if(circuit[circuit[id].in_graph[0]].out_wire[0]==circuit[id].in_wire[0]){
                circuit[id].logic_result = !(circuit[circuit[id].in_graph[0]].logic_result + index[circuit[id].in_wire[1]]);
            }
            else{
                circuit[id].logic_result = !(circuit[circuit[id].in_graph[0]].logic_result + index[circuit[id].in_wire[0]]);
            }
        }
    }
}


void cal_delay(map<string,GraphNode> &circuit,string id,map<string,int> index, map<string,libdata> lib_data){
    //calculate every iteration circuit's delay 
    
    double in_trans = 0;
    int indexo1,indexo2,indexi1,indexi2;
    indexo1 = -1;indexo2 = -1; indexi1 = -1; indexi2 = -1;
    
    if(circuit[id].type == "INVX1"){
        if(circuit[id].in_graph.empty()){  //no vertex before the node
            in_trans = 0;
            indexi1 = 0; 
            indexi2 = 1;
        }
        else{
            in_trans = circuit[circuit[id].in_graph[0]].transition_time;
            circuit[id].total_delay +=  circuit[circuit[id].in_graph[0]].total_delay;
            for(int i=0;i<lib_data["INVX1"].index_transiton_time.size();++i){
                if(in_trans < lib_data["INVX1"].index_transiton_time[i]){
                    indexi1 = i-1;
                    indexi2 = i;
                    if(i==0){
                        ++indexi1;
                        ++indexi2;
                    }
                    break; 
                }
            }
            
            if(in_trans > lib_data["INVX1"].index_transiton_time.back()){
                indexi1 = lib_data["INVX1"].index_transiton_time.size()-2;
                indexi2 = lib_data["INVX1"].index_transiton_time.size()-1;
            }
        
        }
        if(!circuit[id].in_graph.empty()){
            circuit[id].path_choose_in = circuit[id].in_graph[0];
        }
        //calculate index
        for(int i=0;i < lib_data["INVX1"].index_outcap.size();++i){
            if(circuit[id].outcap < lib_data["INVX1"].index_outcap[i]){
                indexo1 = i-1;
                indexo2 = i;
                if(i == 0){
                    ++indexo1;
                    ++indexo2;
                }
                break;
            }
        }
        //bigger than largest data in outcap
        if(circuit[id].outcap > lib_data["INVX1"].index_outcap.back()){
            indexo1 = lib_data["INVX1"].index_outcap.size()-2;
            indexo2 = lib_data["INVX1"].index_outcap.size()-1;
        }
            
        //rising
        if(circuit[id].logic_result){
            circuit[id].transition_time =  calculation(lib_data["INVX1"].index_outcap[indexo1],lib_data["INVX1"].index_outcap[indexo2],lib_data["INVX1"].index_transiton_time[indexi1],lib_data["INVX1"].index_transiton_time[indexi2],lib_data["INVX1"].table_rise_transition[indexi1][indexo1],lib_data["INVX1"].table_rise_transition[indexi1][indexo2],lib_data["INVX1"].table_rise_transition[indexi2][indexo1],lib_data["INVX1"].table_rise_transition[indexi2][indexo2],circuit[id].outcap,in_trans);
            circuit[id].delay = calculation(lib_data["INVX1"].index_outcap[indexo1],lib_data["INVX1"].index_outcap[indexo2],lib_data["INVX1"].index_transiton_time[indexi1],lib_data["INVX1"].index_transiton_time[indexi2],lib_data["INVX1"].table_cell_rise[indexi1][indexo1],lib_data["INVX1"].table_cell_rise[indexi1][indexo2],lib_data["INVX1"].table_cell_rise[indexi2][indexo1],lib_data["INVX1"].table_cell_rise[indexi2][indexo2],circuit[id].outcap,in_trans);
        }
        else{
            circuit[id].transition_time = calculation(lib_data["INVX1"].index_outcap[indexo1],lib_data["INVX1"].index_outcap[indexo2],lib_data["INVX1"].index_transiton_time[indexi1],lib_data["INVX1"].index_transiton_time[indexi2],lib_data["INVX1"].table_fall_transition[indexi1][indexo1],lib_data["INVX1"].table_fall_transition[indexi1][indexo2],lib_data["INVX1"].table_fall_transition[indexi2][indexo1],lib_data["INVX1"].table_fall_transition[indexi2][indexo2],circuit[id].outcap,in_trans);
            circuit[id].delay = calculation(lib_data["INVX1"].index_outcap[indexo1],lib_data["INVX1"].index_outcap[indexo2],lib_data["INVX1"].index_transiton_time[indexi1],lib_data["INVX1"].index_transiton_time[indexi2],lib_data["INVX1"].table_cell_fall[indexi1][indexo1],lib_data["INVX1"].table_cell_fall[indexi1][indexo2],lib_data["INVX1"].table_cell_fall[indexi2][indexo1],lib_data["INVX1"].table_cell_fall[indexi2][indexo2],circuit[id].outcap,in_trans);
        }
        circuit[id].total_delay +=  circuit[id].delay;
        //cout << circuit[id].delay << endl;
        //cout << circuit[id].transition_time << endl;       
    }
    else if(circuit[id].type == "NANDX1"){
        if(circuit[id].in_graph.empty()){  //no vertex before the node
            in_trans = 0;
            indexi1 = 0; 
            indexi2 = 1;
        }
        else{
            //in_trans = circuit[circuit[id].in_graph[0]].transition_time;
            //circuit[id].total_delay +=  circuit[circuit[id].in_graph[0]].total_delay;
            if(circuit[id].in_graph.size()==1){
                if(circuit[circuit[id].in_graph[0]].out_wire[0]==circuit[id].in_wire[0]){
                    if(!(circuit[circuit[id].in_graph[0]].logic_result) && !(index[circuit[id].in_wire[1]])){
                        //total delay min
                        in_trans = 0;
                    }
                    else if(!(circuit[circuit[id].in_graph[0]].logic_result)){
                        in_trans = circuit[circuit[id].in_graph[0]].transition_time;
                        circuit[id].total_delay +=  circuit[circuit[id].in_graph[0]].total_delay;
                        circuit[id].path_choose_in = circuit[id].in_graph[0];
                    }
                    else if(!(index[circuit[id].in_wire[1]])){
                        in_trans = 0;
                    }
                    else{ //total delay:max
                        in_trans = circuit[circuit[id].in_graph[0]].transition_time;
                        circuit[id].total_delay +=  circuit[circuit[id].in_graph[0]].total_delay;
                        circuit[id].path_choose_in = circuit[id].in_graph[0];
                    }
                }
                else{
                    if(!(circuit[circuit[id].in_graph[0]].logic_result) && !(index[circuit[id].in_wire[0]])){
                        in_trans = 0;
                    }
                    else if(!(circuit[circuit[id].in_graph[0]].logic_result)){
                        in_trans = circuit[circuit[id].in_graph[0]].transition_time;
                        circuit[id].total_delay +=  circuit[circuit[id].in_graph[0]].total_delay;
                        circuit[id].path_choose_in = circuit[id].in_graph[0];
                    }
                    else if(!(index[circuit[id].in_wire[0]])){
                        in_trans = 0;
                    }
                    else{
                        in_trans = circuit[circuit[id].in_graph[0]].transition_time;
                        circuit[id].total_delay +=  circuit[circuit[id].in_graph[0]].total_delay;
                        circuit[id].path_choose_in = circuit[id].in_graph[0];
                    }
                }
            }
            //two input
            else{
                if(!(circuit[circuit[id].in_graph[0]].logic_result) && !(circuit[circuit[id].in_graph[1]].logic_result)){
                    if(circuit[circuit[id].in_graph[0]].total_delay < circuit[circuit[id].in_graph[1]].total_delay){
                        in_trans = circuit[circuit[id].in_graph[0]].transition_time;
                        circuit[id].total_delay +=  circuit[circuit[id].in_graph[0]].total_delay;
                        circuit[id].path_choose_in = circuit[id].in_graph[0];
                    }
                    else{
                        in_trans = circuit[circuit[id].in_graph[1]].transition_time;
                        circuit[id].total_delay +=  circuit[circuit[id].in_graph[1]].total_delay;
                        circuit[id].path_choose_in = circuit[id].in_graph[1];
                    }
                }
                else if(!(circuit[circuit[id].in_graph[0]].logic_result)){
                    in_trans = circuit[circuit[id].in_graph[0]].transition_time;
                    circuit[id].total_delay +=  circuit[circuit[id].in_graph[0]].total_delay;
                    circuit[id].path_choose_in = circuit[id].in_graph[0];
                }
                else if(!(circuit[circuit[id].in_graph[1]].logic_result)){
                    in_trans = circuit[circuit[id].in_graph[1]].transition_time;
                    circuit[id].total_delay +=  circuit[circuit[id].in_graph[1]].total_delay;
                    circuit[id].path_choose_in = circuit[id].in_graph[1];
                }
                else{
                    //longest
                    if(circuit[circuit[id].in_graph[0]].total_delay < circuit[circuit[id].in_graph[1]].total_delay){
                        in_trans = circuit[circuit[id].in_graph[1]].transition_time;
                        circuit[id].total_delay +=  circuit[circuit[id].in_graph[1]].total_delay;
                        circuit[id].path_choose_in = circuit[id].in_graph[1];
                    }
                    else{
                        in_trans = circuit[circuit[id].in_graph[0]].transition_time;
                        circuit[id].total_delay +=  circuit[circuit[id].in_graph[0]].total_delay;
                        circuit[id].path_choose_in = circuit[id].in_graph[0];
                    }
                }
            }
            
            for(int i=0;i<lib_data["NANDX1"].index_transiton_time.size();++i){
                if(in_trans < lib_data["NANDX1"].index_transiton_time[i]){
                    indexi1 = i-1;
                    indexi2 = i;
                    if(i==0){
                        ++indexi1;
                        ++indexi2;
                    }
                    break; 
                }
            }
            
            if(in_trans > lib_data["NANDX1"].index_transiton_time.back()){
                indexi1 = lib_data["NANDX1"].index_transiton_time.size()-2;
                indexi2 = lib_data["NANDX1"].index_transiton_time.size()-1;
            }
        }
        
        //calculate index
        for(int i=0;i < lib_data["NANDX1"].index_outcap.size();++i){
            if(circuit[id].outcap < lib_data["NANDX1"].index_outcap[i]){
                indexo1 = i-1;
                indexo2 = i;
                if(i == 0){
                    ++indexo1;
                    ++indexo2;
                }
                break;
            }
        }
        //bigger than largest data in outcap
        if(circuit[id].outcap > lib_data["NANDX1"].index_outcap.back()){
            indexo1 = lib_data["NANDX1"].index_outcap.size()-2;
            indexo2 = lib_data["NANDX1"].index_outcap.size()-1;
        }
        //rising
        if(circuit[id].logic_result){
            circuit[id].transition_time = calculation(lib_data["NANDX1"].index_outcap[indexo1],lib_data["NANDX1"].index_outcap[indexo2],lib_data["NANDX1"].index_transiton_time[indexi1],lib_data["NANDX1"].index_transiton_time[indexi2],lib_data["NANDX1"].table_rise_transition[indexi1][indexo1],lib_data["NANDX1"].table_rise_transition[indexi1][indexo2],lib_data["NANDX1"].table_rise_transition[indexi2][indexo1],lib_data["NANDX1"].table_rise_transition[indexi2][indexo2],circuit[id].outcap,in_trans);
            circuit[id].delay = calculation(lib_data["NANDX1"].index_outcap[indexo1],lib_data["NANDX1"].index_outcap[indexo2],lib_data["NANDX1"].index_transiton_time[indexi1],lib_data["NANDX1"].index_transiton_time[indexi2],lib_data["NANDX1"].table_cell_rise[indexi1][indexo1],lib_data["NANDX1"].table_cell_rise[indexi1][indexo2],lib_data["NANDX1"].table_cell_rise[indexi2][indexo1],lib_data["NANDX1"].table_cell_rise[indexi2][indexo2],circuit[id].outcap,in_trans);
            
        }
        else{
            circuit[id].transition_time = calculation(lib_data["NANDX1"].index_outcap[indexo1],lib_data["NANDX1"].index_outcap[indexo2],lib_data["NANDX1"].index_transiton_time[indexi1],lib_data["NANDX1"].index_transiton_time[indexi2],lib_data["NANDX1"].table_fall_transition[indexi1][indexo1],lib_data["NANDX1"].table_fall_transition[indexi1][indexo2],lib_data["NANDX1"].table_fall_transition[indexi2][indexo1],lib_data["NANDX1"].table_fall_transition[indexi2][indexo2],circuit[id].outcap,in_trans);
            circuit[id].delay = calculation(lib_data["NANDX1"].index_outcap[indexo1],lib_data["NANDX1"].index_outcap[indexo2],lib_data["NANDX1"].index_transiton_time[indexi1],lib_data["NANDX1"].index_transiton_time[indexi2],lib_data["NANDX1"].table_cell_fall[indexi1][indexo1],lib_data["NANDX1"].table_cell_fall[indexi1][indexo2],lib_data["NANDX1"].table_cell_fall[indexi2][indexo1],lib_data["NANDX1"].table_cell_fall[indexi2][indexo2],circuit[id].outcap,in_trans);
        }
        
        circuit[id].total_delay +=  circuit[id].delay;
        //cout << circuit[id].delay << endl;
        //cout << circuit[id].transition_time << endl;   
    }
    else if(circuit[id].type == "NOR2X1"){
        
        if(circuit[id].in_graph.empty()){  //no vertex before the node
            in_trans = 0;
            indexi1 = 0; 
            indexi2 = 1;
        }
        else{
            //in_trans = circuit[circuit[id].in_graph[0]].transition_time;
            //circuit[id].total_delay +=  circuit[circuit[id].in_graph[0]].total_delay;
            if(circuit[id].in_graph.size()==1){
                if(circuit[circuit[id].in_graph[0]].out_wire[0]==circuit[id].in_wire[0]){
                    if( circuit[circuit[id].in_graph[0]].logic_result && (index[circuit[id].in_wire[1]])){
                        //total delay min
                        in_trans = 0;
                    }
                    else if((circuit[circuit[id].in_graph[0]].logic_result)){
                        in_trans = circuit[circuit[id].in_graph[0]].transition_time;
                        circuit[id].total_delay +=  circuit[circuit[id].in_graph[0]].total_delay;
                        circuit[id].path_choose_in = circuit[id].in_graph[0];
                    }
                    else if((index[circuit[id].in_wire[1]])){
                        in_trans = 0;
                    }
                    else{ //total delay:max
                        in_trans = circuit[circuit[id].in_graph[0]].transition_time;
                        circuit[id].total_delay +=  circuit[circuit[id].in_graph[0]].total_delay;
                        circuit[id].path_choose_in = circuit[id].in_graph[0];
                    }
                }
                else{
                    if((circuit[circuit[id].in_graph[0]].logic_result) && (index[circuit[id].in_wire[0]])){
                        in_trans = 0;
                    }
                    else if((circuit[circuit[id].in_graph[0]].logic_result)){
                        in_trans = circuit[circuit[id].in_graph[0]].transition_time;
                        circuit[id].total_delay +=  circuit[circuit[id].in_graph[0]].total_delay;
                        circuit[id].path_choose_in = circuit[id].in_graph[0];
                    }
                    else if((index[circuit[id].in_wire[0]])){
                        in_trans = 0;
                    }
                    else{
                        in_trans = circuit[circuit[id].in_graph[0]].transition_time;
                        circuit[id].total_delay +=  circuit[circuit[id].in_graph[0]].total_delay;
                        circuit[id].path_choose_in = circuit[id].in_graph[0];
                    }
                }
            }
            //two input
            else{
                if((circuit[circuit[id].in_graph[0]].logic_result) && (circuit[circuit[id].in_graph[1]].logic_result)){
                    if(circuit[circuit[id].in_graph[0]].total_delay < circuit[circuit[id].in_graph[1]].total_delay){
                        in_trans = circuit[circuit[id].in_graph[0]].transition_time;
                        circuit[id].total_delay +=  circuit[circuit[id].in_graph[0]].total_delay;
                        circuit[id].path_choose_in = circuit[id].in_graph[0];
                    }
                    else{
                        in_trans = circuit[circuit[id].in_graph[1]].transition_time;
                        circuit[id].total_delay +=  circuit[circuit[id].in_graph[1]].total_delay;
                        circuit[id].path_choose_in = circuit[id].in_graph[1];
                    }
                }
                else if((circuit[circuit[id].in_graph[0]].logic_result)){
                    in_trans = circuit[circuit[id].in_graph[0]].transition_time;
                    circuit[id].total_delay +=  circuit[circuit[id].in_graph[0]].total_delay;
                    circuit[id].path_choose_in = circuit[id].in_graph[0];
                }
                else if((circuit[circuit[id].in_graph[1]].logic_result)){
                    in_trans = circuit[circuit[id].in_graph[1]].transition_time;
                    circuit[id].total_delay +=  circuit[circuit[id].in_graph[1]].total_delay;
                    circuit[id].path_choose_in = circuit[id].in_graph[1];
                }
                else{
                    //longest
                    if(circuit[circuit[id].in_graph[0]].total_delay < circuit[circuit[id].in_graph[1]].total_delay){
                        in_trans = circuit[circuit[id].in_graph[1]].transition_time;
                        circuit[id].total_delay +=  circuit[circuit[id].in_graph[1]].total_delay;
                        circuit[id].path_choose_in = circuit[id].in_graph[1];
                    }
                    else{
                        in_trans = circuit[circuit[id].in_graph[0]].transition_time;
                        circuit[id].total_delay +=  circuit[circuit[id].in_graph[0]].total_delay;
                        circuit[id].path_choose_in = circuit[id].in_graph[0];
                    }
                }
            }
            
            for(int i=0;i<lib_data["NOR2X1"].index_transiton_time.size();++i){
                if(in_trans < lib_data["NOR2X1"].index_transiton_time[i]){
                    indexi1 = i-1;
                    indexi2 = i;
                    if(i==0){
                        ++indexi1;
                        ++indexi2;
                    }
                    break; 
                }
            }
            
            if(in_trans > lib_data["NOR2X1"].index_transiton_time.back()){
                indexi1 = lib_data["NOR2X1"].index_transiton_time.size()-2;
                indexi2 = lib_data["NOR2X1"].index_transiton_time.size()-1;
            }
        }
        
        //calculate index
        for(int i=0;i < lib_data["NOR2X1"].index_outcap.size();++i){
            if(circuit[id].outcap < lib_data["NOR2X1"].index_outcap[i]){
                indexo1 = i-1;
                indexo2 = i;
                if(i == 0){
                    ++indexo1;
                    ++indexo2;
                }
                break;
            }
        }
        //bigger than largest data in outcap
        if(circuit[id].outcap > lib_data["NOR2X1"].index_outcap.back()){
            indexo1 = lib_data["NOR2X1"].index_outcap.size()-2;
            indexo2 = lib_data["NOR2X1"].index_outcap.size()-1;
        }
        //rising
        if(circuit[id].logic_result){
            circuit[id].transition_time = calculation(lib_data["NOR2X1"].index_outcap[indexo1],lib_data["NOR2X1"].index_outcap[indexo2],lib_data["NOR2X1"].index_transiton_time[indexi1],lib_data["NOR2X1"].index_transiton_time[indexi2],lib_data["NOR2X1"].table_rise_transition[indexi1][indexo1],lib_data["NOR2X1"].table_rise_transition[indexi1][indexo2],lib_data["NOR2X1"].table_rise_transition[indexi2][indexo1],lib_data["NOR2X1"].table_rise_transition[indexi2][indexo2],circuit[id].outcap,in_trans);
            circuit[id].delay = calculation(lib_data["NOR2X1"].index_outcap[indexo1],lib_data["NOR2X1"].index_outcap[indexo2],lib_data["NOR2X1"].index_transiton_time[indexi1],lib_data["NOR2X1"].index_transiton_time[indexi2],lib_data["NOR2X1"].table_cell_rise[indexi1][indexo1],lib_data["NOR2X1"].table_cell_rise[indexi1][indexo2],lib_data["NOR2X1"].table_cell_rise[indexi2][indexo1],lib_data["NOR2X1"].table_cell_rise[indexi2][indexo2],circuit[id].outcap,in_trans);
            
        }
        else{
            circuit[id].transition_time = calculation(lib_data["NOR2X1"].index_outcap[indexo1],lib_data["NOR2X1"].index_outcap[indexo2],lib_data["NOR2X1"].index_transiton_time[indexi1],lib_data["NOR2X1"].index_transiton_time[indexi2],lib_data["NOR2X1"].table_fall_transition[indexi1][indexo1],lib_data["NOR2X1"].table_fall_transition[indexi1][indexo2],lib_data["NOR2X1"].table_fall_transition[indexi2][indexo1],lib_data["NOR2X1"].table_fall_transition[indexi2][indexo2],circuit[id].outcap,in_trans);
            circuit[id].delay = calculation(lib_data["NOR2X1"].index_outcap[indexo1],lib_data["NOR2X1"].index_outcap[indexo2],lib_data["NOR2X1"].index_transiton_time[indexi1],lib_data["NOR2X1"].index_transiton_time[indexi2],lib_data["NOR2X1"].table_cell_fall[indexi1][indexo1],lib_data["NOR2X1"].table_cell_fall[indexi1][indexo2],lib_data["NOR2X1"].table_cell_fall[indexi2][indexo1],lib_data["NOR2X1"].table_cell_fall[indexi2][indexo2],circuit[id].outcap,in_trans);
        }
          
        circuit[id].total_delay +=  circuit[id].delay;
        //cout << circuit[id].delay << endl;
        //cout << circuit[id].transition_time << endl;  
    }
}
