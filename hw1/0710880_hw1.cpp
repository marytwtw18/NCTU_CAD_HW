#include<iostream>
#include<algorithm>
#include<vector>
#include<fstream>
#include<sstream>
#include<string>
#include <cstdlib>
#include <set>

using namespace std;

void splitStr2Vec(string s, vector<string>& buf);

template <class T>
void print_vector(vector<vector<T>>& sepe_str);
vector<vector<int>> dec_to_binary(int var, vector<int> set);
vector<vector<int>> find_prime_implicant(vector<vector<int>> on,vector<vector<int>> dc,int var);

void Expansion(set<string> &product,int i_pos,int maxi);
string settostring(set<string> &temp);
bool sort_by_SOP(const set<string> a, const set<string> b);



/*------------------global variable---------------------------------------------*/
vector<set<string>> sum_of_product;
vector<vector<string>> product_of_sum;
set<string> s_SOP;
vector<int> SOP_cost;
set<string>::iterator iter;

//"-" more smaller
struct vec{
    int dc_count;
    vector<int> pri;
};

typedef struct vec pri_implicant;

bool mycompare(const pri_implicant p1,const  pri_implicant p2){
    return  p1.dc_count > p2.dc_count;
}


int main(int argc,char* argv[])
{
    //parameter declaration
    ifstream infile; 
    string instr;  //.i .m .d 
    vector <vector<string>> sepe_str;
    
    int input_variable = 0; //.i term
    int in_sum = 0;
    int count = 0;
    
    //.m term
    vector<vector<int>> onset;
    vector<int> on;
    
    //.d term
    vector<vector<int>> dcset;
    vector<int> dc; 
    
    //read file
    infile.open(argv[1]);
    
    if(!infile.is_open())
    {
        cout << "fail to open" << endl;
    }
    else
    {
        while(!infile.eof()){ //read in
            vector<string> new_vec;
            getline(infile,instr);
            //cout <<instr <<endl;
            splitStr2Vec(instr,new_vec);
            for(string tmp : new_vec) {
                //cout << tmp<<endl;
                
                 if((tmp==".i")||(tmp==".m")||(tmp==".m ")||(tmp==".d")||(tmp==".d "))
                 {
                     count++;
                     //cout << count << endl;
                 }
                 else if(count == 1)
                 {
                     input_variable = stoi(tmp);
                 }
                 else if(count == 2)
                 {
                     on.push_back(stoi(tmp));
                 }
                 else if(count == 3)
                 {
                     dc.push_back(stoi(tmp));
                 }            
            }
        }
    }
    
    //calculate input sum
    //cout << on.size()<<endl;
    
    
    //.m .d transform to string(eg:0010)
    onset = dec_to_binary(input_variable, on);
    //cout <<"onset:";
    //print_vector(onset);
    
    dcset = dec_to_binary(input_variable, dc);
    //cout <<endl<<"dcset:";
    //print_vector(dcset);
    
    
    
/*------------------------------------------------------------------------*/
//quine mcCluskey algo function
//grouping different number of 1
/*------------------------------------------------------------------------*/

    vector<vector<vector<int>>> grouping(input_variable+1); //var+1個 二維陣列
    vector<vector<char>> label(input_variable+1);
    
    //operator
    //vector<vector<int>>::iterator itor_g;
    //vector<char>::iterator itor_l;
   
    for(int i=0;i<onset.size();++i)
    {
        count = 0;
        for(int j=0;j<onset[i].size();++j)
        {
            if(onset[i][j]==1) count++;
        }   
        grouping[count].push_back(onset[i]);
        label[count].push_back('n');
    }
    for(int i=0;i<dcset.size();++i)
    {
        count = 0;
        for(int j=0;j<dcset[i].size();++j)
        {
            if(dcset[i][j]==1) count++;
        }   
        grouping[count].push_back(dcset[i]);
        label[count].push_back('n');
    }
    /*
    cout << endl;
    for(int i=0;i<grouping.size();++i){
        cout << i <<"'s1: "<<endl;
        for(int j=0;j<grouping[i].size();++j){
            for(int k=0;k<grouping[i][j].size();++k){
                cout << grouping[i][j][k]<<" ";
            }
            cout <<  label[i][j];
            cout << endl;
        }
    }
    */

/*------------------------------------------------------------------------*/
//pri implicant generation
/*------------------------------------------------------------------------*/
    //store don't care index
    int dcare = -1;
    int cnt = 0; //check dup
    int sizei,sizei_plus_one; //fix size
    bool break_point = true;
    bool insert = true;
    
    while(break_point){
        break_point = false;
        insert = true;
        for(int i=0;i<grouping.size()-1;++i){ //compare n-1 time
            count = 0;
            vector<int> delete_indexi,delete_indexi_plus_1;
            sizei = grouping[i].size();
            sizei_plus_one = grouping[i+1].size();
            for(int j=0;j<sizei;++j){
            
                for(int k=0;k<sizei_plus_one;++k){
                    count = 0;
                    dcare = -1;
               
                    for(int index=0;index < input_variable;++index){
                        if(grouping[i][j][index] != grouping[i+1][k][index]){
                            dcare = index;
                            count+=1;
                        }
                    }
                    if(count==1){
                        vector<int> new_vec(input_variable);
                        for(int index = 0;index < input_variable; ++index){
                            if(index == dcare){
                                new_vec[index] = -1;
                            }
                            else{
                                new_vec[index] = grouping[i][j][index];
                            }
                        }
                       
                        grouping[i].push_back(new_vec);
                        label[i].push_back('n');
                        
                        label[i][j] = '|';
                        label[i+1][k] = '|';
                    }
                }
                if( label[i][j]=='n')  label[i][j] = '*'; //cannot merge
            }
            if(i == grouping.size()-2)
            {
                for(int k=0;k<sizei_plus_one;++k){
                     if( label[i+1][k]=='n')  label[i+1][k] = '*'; //cannot merge 
                }
            }
        }
    
        /*
        cout << endl;
        for(int i=0;i<grouping.size();++i){
            cout << i <<"'s1: "<<endl;
            for(int j=0;j<grouping[i].size();++j){
                for(int k=0;k<grouping[i][j].size();++k){
                    if( grouping[i][j][k]>=0) cout << grouping[i][j][k]<<" ";
                    else cout << "_ ";
                }
                cout <<  label[i][j];
                cout << endl;
            }
        }
        */
    
        for(int i=0;i<grouping.size();++i){
            for(int j=0;j<grouping[i].size();++j){
                if(label[i][j] == '|'){
                    auto iterg = grouping[i].erase(grouping[i].begin() + j);
                    auto iterl = label[i].erase(label[i].begin() + j);
                    j--;
                }
            }
        }
    
        /*
        cout << endl;
        for(int i=0;i<grouping.size();++i){
            cout << i <<"'s1: "<<endl;
            for(int j=0;j<grouping[i].size();++j){
                for(int k=0;k<grouping[i][j].size();++k){
                    if( grouping[i][j][k]>=0) cout << grouping[i][j][k]<<" ";
                    else cout << "_ ";
                }
                cout <<  label[i][j];
                cout << endl;
            }
        }
        */
    
        //if there is n,keep runing
        for(int i=0;i<grouping.size();++i){
            for(int j=0;j<grouping[i].size();++j){
                if(label[i][j] == 'n'){  //continue run
                    break_point = true;
                }
            }
        } 
    }
    
    //delete dump
    //change compare way
    //try using set
    for(int i=0;i<grouping.size();++i){
        for(int j=0;j<grouping[i].size()-1;++j){
            for(int k=j+1;k<grouping[i].size();++k){
                cnt = 0;
                for(int index=0;index < input_variable;++index){
                    if(grouping[i][j][index]==grouping[i][k][index]) cnt++;
                }
                if(cnt == input_variable){
                     auto it = grouping[i].erase(grouping[i].begin()+k);
                     k--;
                } 
            }
        }
    }
    
    //concatenate
    vector <pri_implicant> con_gro;
    
    for(int i=0;i<grouping.size();++i){
        for(int j=0;j<grouping[i].size();++j){
            pri_implicant prii;
            prii.pri.assign(grouping[i][j].begin(),grouping[i][j].end());
            prii.dc_count = 0;
            for(int k=0;k<grouping[i][j].size();++k){
                if(grouping[i][j][k] == -1){
                    prii.dc_count++;
                }
            }
            con_gro.push_back(prii);
        }
    }
    
    sort(con_gro.begin(),con_gro.end(),mycompare);
 /*   
    cout << endl << endl;
    cout<<".p "<<con_gro.size()<<endl;
    
    if(con_gro.size() > 20) //find min literal:the first 20 smaller
    {
         //cout the first 20 pri
         for(int i=0;i<20;++i)
         {
             for(int j=0;j<con_gro[i].pri.size();++j){
                 if(con_gro[i].pri[j]>=0) cout<<con_gro[i].pri[j];
                 else if(con_gro[i].pri[j]==-1) cout<<"-";
             }
             cout <<endl;
         }   
    }
    else
    {
        for(int i=0;i<con_gro.size();++i)
        {
             for(int j=0;j<con_gro[i].pri.size();++j){
                 if(con_gro[i].pri[j]>=0) cout<<con_gro[i].pri[j];
                 else if(con_gro[i].pri[j]==-1) cout<<"-";
             }
             cout <<endl;
         }   
    }
  */  
    
/*------------------------------find min cover-----------------------------------*/ 
//using list
//vector<vector<vector<int>>> set;

cnt = 0; 

//check whether onset implicant is in prine implicant and store into column_covering
    
    //product of sum form
    for(int i=0;i<onset.size(); ++i){
        //vector<vector<int>> sum;
        vector<string> sum;
        for(int j=0;j<con_gro.size();++j){
            for(int k=0;k<input_variable;++k){
                if(onset[i][k] == con_gro[j].pri[k]) ++cnt;
                else if(con_gro[j].pri[k] == -1) ++cnt;
            }
            if(cnt == input_variable){
                string con;
                for(int k=0;k<input_variable;++k){
                    if(con_gro[j].pri[k]>= 0) con += to_string(con_gro[j].pri[k]);
                    else if(con_gro[j].pri[k] == -1) con += "-";
                }
                sum.push_back(con);
            }
            cnt = 0;
        }
        product_of_sum.push_back(sum);
    }
    
    /*
    for(int i=0;i<product_of_sum.size();++i){
        for(int j=0;j<product_of_sum[i].size(); ++j){
            cout<<product_of_sum[i][j]<<" ";
        }
        cout << endl;
    }
    */
/*-------------------------------Petrick's method--------------------------------*/  
    int sop_index = 0;
    int literal = 0;
    int min_literal = 99999,min_index;
    
    set <string> product;  
    Expansion(product,0,product_of_sum.size() );
    sort( sum_of_product.begin(), sum_of_product.end(), sort_by_SOP);
    
    for(int i=0;i<sum_of_product.size();++i)
    {
        if(sum_of_product[i].size()>sum_of_product[0].size())
        {
            sop_index = i - 1;
            break;
        }
    }
    for(int i=0;i<=sop_index;++i){
        //calculate literal
        literal = 0;
        for(iter = sum_of_product[i].begin();iter != sum_of_product[i].end(); ++iter){
            for(int j=0;j<(*iter).size();++j)
            {
                if( (*iter)[j] == '0')
                {
                    literal += 1;
                }
                else if((*iter)[j] == '1')
                {
                    literal += 1;
                }
            }
        }
        if(literal< min_literal){
            min_literal = literal;
            min_index = i;
        }
    }
    /*
    cout << endl;cout << endl;
    cout <<".mc " << sum_of_product[min_index].size()<<endl;
    
    for(iter = sum_of_product[min_index].begin(); iter != sum_of_product[min_index].end(); ++iter)
    {
        cout << *iter << endl;
    }
    cout << "literal=" << min_literal;
    cout << endl;
    */
/*-------------------------------------------------------------------------------*/    
/*------------------------------output the file----------------------------------*/ 
/*-------------------------------------------------------------------------------*/       
/* output format example
 * 	.pi 7				( there are 7 prime implicants )
 * 	0-00
 * 	-000
 *  100-
 *	10-0
 *	01--
 *	1-01
 *	-1-1
 *	.mc 3				( 3 prime implicants in minimum covering )
 *	10-0
 *	1-01
 *	01--
 *	literal=8
 */    
    ofstream outfile1,outfile2;
    outfile1.open(argv[2]);
    outfile2.open(argv[3]);
    
    outfile1<<".i "<<input_variable<<endl;
    outfile1<<".m "<<endl;
    for(int i=0;i<on.size();++i){
        outfile1<<on[i]<<" ";
    }
    outfile1<<endl;
    outfile1<<".d "<<endl;
    for(int i=0;i<dc.size();++i){
        outfile1<<dc[i]<<" ";
    } 
    outfile1<<endl;
    outfile1<<".p "<<con_gro.size()<<endl;
    if(con_gro.size() > 20) //find min literal:the first 20 smaller
    {
         //cout the first 20 pri
         for(int i=0;i<20;++i)
         {
             for(int j=0;j<con_gro[i].pri.size();++j){
                 if(con_gro[i].pri[j]>=0)  outfile1<<con_gro[i].pri[j];
                 else if(con_gro[i].pri[j]==-1)  outfile1<<"-";
             }
             outfile1 <<endl;
         }   
    }
    else
    {
        for(int i=0;i<con_gro.size();++i)
        {
             for(int j=0;j<con_gro[i].pri.size();++j){
                 if(con_gro[i].pri[j]>=0)  outfile1<<con_gro[i].pri[j];
                 else if(con_gro[i].pri[j]==-1)  outfile1<<"-";
             }
             outfile1 <<endl;
         }   
    }
    
    outfile2 <<".mc " << sum_of_product[min_index].size()<<endl;
    
    for(iter = sum_of_product[min_index].begin(); iter != sum_of_product[min_index].end(); ++iter)
    {
        outfile2 << *iter << endl;
    }
    outfile2 << "literal=" << min_literal;
    outfile2 << endl;
    
    
    outfile1.close();
    outfile2.close();
    infile.close();
    return 0;
}

string settostring(set<string> &temp)
{
    stringstream ss;
    for(iter=temp.begin();iter!=temp.end();++iter) ss << *iter;
    string str;
    ss >> str;
    return str;
}

void Expansion(set<string> &product,int i_pos,int maxi)
{
    if(i_pos == maxi){
        string str = settostring(product); //expansion
        if( s_SOP.find(str) == s_SOP.end()){
             s_SOP.insert(str);
             sum_of_product.push_back(product);
        }
    }
    
    for(int i=0; i< product_of_sum[i_pos].size(); ++i){
        if( product.find(product_of_sum[i_pos][i]) == product.end()){
            product.insert(product_of_sum[i_pos][i]);
            Expansion(product, i_pos+1, maxi);
            product.erase(product_of_sum[i_pos][i]);
        }
        else{
            Expansion(product, i_pos+1, maxi);
        }
    }
}


bool sort_by_SOP(const set<string> a, const set<string> b) {
	return ( a.size() < b.size() ) ;
}


void splitStr2Vec(string str, vector<string>& buf)
{
	int current = 0; //initial position
	int next;
 
	while (1)
	{
		next = str.find_first_of(" ", current);
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

template <class T>
void print_vector(vector<vector<T>>& sepe_str)
{
    cout << endl;
    for(int i=0;i<sepe_str.size();++i)
    {
        for(int j=0;j<sepe_str[i].size();++j)
        {
            cout<<sepe_str[i][j]<<" ";
        }
        cout << endl;
    }
}

//.m .d transform to string(eg:0010)
vector<vector<int>> dec_to_binary(int var, vector<int> set)
{
    vector<vector<int>> dtob;
    
    for(int i=0; i < set.size(); ++i){
        vector<int> trans_set(var);
        
        for(int j=0;j<var;++j){ //initialize
            trans_set[j] = 0;    
        }
        
        for(int j=var-1; j>=0 ;--j){
            trans_set[j] = set[i] % 2;
            set[i] = set[i] /2 ;
        }
        dtob.push_back(trans_set);
    }
    return dtob;
}

vector<vector<int>> find_prime_implicant(vector<vector<int>> on,vector<vector<int>> dc,int var)
{
    int count;
    vector<vector<vector<int>>> grouping(var); //var's 2D vector
    for(int i=0;i<on.size();++i)
    {
        count = 0;
        for(int j=0;j<on[i].size();++j)
        {
            if(on[i][j]==1) count++;
        }   
        grouping[count].push_back(on[i]);
    }
    for(int i=0;i<dc.size();++i)
    {
        count = 0;
        for(int j=0;j<dc[i].size();++j)
        {
            if(dc[i][j]==1) count++;
        }   
        grouping[count].push_back(dc[i]);
    }
}