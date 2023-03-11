#include<bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Table
{
    string name;
    vector<string> attributes,attribute_type,not_null;
    vector<string> p_key;
    vector<pair<vector<string>,string>> foreign_ref;    // some set of attributes point to a table
};

map<string,int> table_name; 
vector<Table> Tables;

int error_cnt;
bool syn_error;
void report_syntax_error(int line_no,string s="Unidentified"){
    syn_error = 1;
    error_cnt ++ ;
    cout<<"\n SYNTAX ERROR ON LINE NUMBER: "<<line_no<<" :"<<s;
    // exit(-1);
}
set<string> vartype = {"INT","VARCHAR","CHAR","DECIMAL"};
bool checktype(string type){
    string s = "";
    int i = 0;
    while(i<type.length() && type[i]>='A' && type[i]<='Z') {
        s += type[i];
        i++;
    }
    // cout<<s<<"f\n";
    if(s == "INT"){
        if(i!=type.length()) return false;
        return true;
    } 
    if(s == "VARCHAR" || s=="CHAR"){
        if(i+3>type.length() || type[i]!='(' || type[type.length()-1]!=')') return false;
        int j = i+1;
        while(j<type.length()-1) {
            if(type[j]>'9' || type[j]<'0') {
                return false;
            }
            j++;
        }
        return true;
    }
    if(s == "DECIMAL"){
        if(i+5>type.length() || type[i]!='(' || type[type.length()-1]!=')') return false;
        int j = i+1;
        int cnt = 0;
        while(j<type.length()-1) {
            if(type[j]>'9' || type[j]<'0') {
                if(type[j]==',') {
                    if(cnt) return false;
                    cnt++;
                }
                else return false;
            }
            j++;
        }
        if(cnt == 1)return true;
        return false;

    }
    return false;

}
bool exists(vector<string> v,string s){
    for(auto i:v) if(i==s) return 1;
    return 0;
}

bool TableExists(string p){
    // for(auto i:Tables) cout<<i.name<<" f "<<p<<"\n";
    for(auto i:Tables) if(i.name == p) return 1;
    return 0; 
}

int main(){
    freopen("input.txt", "r", stdin);
    string line;

    int line_no = 0,table_cnt = 0;
    while(getline(cin,line)){
        line_no++;
        cout<<"Parsing Line: "<<line<<"\n";
        if(line == "") continue;  // ignore empty lines
        else if(line.substr(0,7) == "CREATE "){
            if(line.length()<16) report_syntax_error(line_no);
            if(line.substr(7,6)!="TABLE ") report_syntax_error(line_no);
            if(line[line.length()-1]!='(') report_syntax_error(line_no);
            table_name[line.substr(13,line.length()-14)] = table_cnt++;
            bool flag = 0,endline = 0;
            Table temp;
            temp.name = line.substr(13,line.length()-15);
            while(getline(cin,line)){
                line_no++;
                cout<<"Parsing Line: "<<line<<"\n";
                if(line.length()>12 && line.substr(0,13)=="PRIMARY KEY ("){
                    // if(line[line.length()-1]!=')') report_syntax_error(line_no,"Missing Closing Bracket");
                    int i = 13,n = line.length();
                    while(i<n){
                        string key = "";
                        while(i<n && line[i]==' ') i++;
                        while(i<n && (line[i]!=','  && line[i]!=')')){
                            key += line[i];
                            i++;
                        }
                        // cout<<"p op"<<key<<"\n";
                        // if(i==n) report_syntax_error(line_no,"Missing closing bracket 2");
                        if(!exists(temp.attributes,key)) report_syntax_error(line_no,"Primary Key not in Keys");
                        temp.p_key.push_back(key);
                        if(line[i]==')') break;
                        i++;
                    }
                    if(i==n) report_syntax_error(line_no,"Missing Closing Bracket");
                    if(i==n-1){
                        endline = 1;
                        continue;
                    }
                    i++;
                    while(i<n && line[i]==' ')i++;
                    if(i==n) endline = 1;
                    else if(i==n-1 && line[i]==',') continue;
                    else report_syntax_error(line_no,"Unexpected Character");
                    continue;
                }
                if(line.length()>13 && line.substr(0,13)=="FOREIGN KEY ("){
                    // cout<<"ENTER";
                    vector<string> v1,v2;
                    int i = 13,n = line.length();
                    while(true){
                        string key = "";
                        while(i<n && line[i]==' ') i++;
                        while(i<n && (line[i]!=','  && line[i]!=')')){
                            key += line[i];
                            i++;
                        }
                        // cout<<"p op"<<key<<"\n";
                        // if(i==n) report_syntax_error(line_no,"Missing closing bracket 2");
                        if(!exists(temp.attributes,key)) report_syntax_error(line_no,"Attribute does not exist");
                        v1.push_back(key);
                        if(line[i]==')') break;
                        i++;
                    }
                    if(line.length()<i+12 || line.substr(i+2,10)!="REFERENCES") report_syntax_error(line_no,"Missing Key Word REFERENCES");
                    i = i+13;
                    if(i>=n) report_syntax_error(line_no,"Missing Referenced Table");
                    string p = "";
                    while(i<n && line[i]!=','){
                        p += line[i++];
                    }
                    // cout<<p<<"\n";
                    if(!TableExists(p)) report_syntax_error(line_no,"Referenced Table Does Not Exist");
                    temp.foreign_ref.push_back({v1,p});
                    if(i==n){
                        endline = 1;
                        continue;
                    }
                    else if(line[i]==',' && i==n-1) continue;
                    else report_syntax_error(line_no,"String couldn't be parsed");


                }
                if(line == ");"){
                    flag = 1;
                    break;
                }
                if(endline) report_syntax_error(line_no,"Create Table was Supposed to end");
                if(line[line.length()-1]!=',') endline = 1;
                int i = 0,n = line.length();
                while(i<n && line[i]!=' ')i++;
                if(i==n) report_syntax_error(line_no,"Attribute Type Not Defined");
                temp.attributes.push_back(line.substr(0,i));
                // cout<<"Push :"<<line.substr(0,i)<<"\n";
                int j = i+1;
                i++;
                string type = "";
                while(i<n && line[i]!=' '){
                    type += line[i];
                    i++;
                }
                // cout<<type<<"\n";
                if(checktype(type))temp.attribute_type.push_back(type);
                else report_syntax_error(line_no,"Attribute type undefined");
                if(i==n) report_syntax_error(line_no,"bcd");
                i++;
                if(i==n) report_syntax_error(line_no,"cde");
                if(line[i]=='N'){
                    if(line.length()!=i+10) report_syntax_error(line_no,"Ambiguous NOT NULL");
                    if(line.substr(i,9)!="NOT NULL ") report_syntax_error(line_no,"Ambiguous NOT NULL2");
                    temp.not_null.push_back(temp.attributes[temp.attributes.size()-1]);
                    i = i+9;
                }
                
                if(line[i]!=',' && i!=n-1) report_syntax_error(line_no,"def");

            }
            if(flag == 0) report_syntax_error(line_no,"Create did not end correctly");
            Tables.push_back(temp);
        }
        else if(line.substr(0,7) == "SELECT "){
            int i = 7,n = line.length();
            while(i<n && line[i]==' ')i++;
            if(i==n) report_syntax_error(line_no,"SELECT STATEMENT CAN NOT BE EMPTY");\
            int j = i;
            while(j+3<n && line.substr(j,4)!="FROM")j++;
            if(j+3 == n) report_syntax_error(line_no,"Missing FROM KeyWord");
            set<string> attr;
            if(line[i]=='*'){
                i++;
                while(i<n && line[i]==' ') i++;
                if(line.substr(i,4)!="FROM") report_syntax_error(line_no,"FROM Was Expected After * ");
            }
            else{
                while(line[i]==' ')i++;
                string p = "";
                while(i<j && line.substr(i,4)!="FROM"){
                    while(i<j && line[i]==' ')i++;
                    string p = "";
                    while(i<j && line[i]!=' ' && line[i]!=',') p += line[i++];
                    while(i<j && line[i]==' ')i++;
                    if(i!=j && line[i]!=',') report_syntax_error(line_no,"Attribute Names Must Be Seperated By Comma ");
                    attr.insert(p);
                    p = "";
                    i++;
                }
            }
            set<string> avail_attr;
            i = j+4;
            while(i<n && line[i]==' ')i++;
            if(i==n) report_syntax_error(line_no,"Tables List In A Join Can Not Be Empty");
            while(i<n){
                string p = "";
                while(i<n && line[i]==' ')i++;
                while(i<n && line[i]!=' ' && line[i]!=',') p += line[i++];
                while(i<n && line[i]==' ')i++;
                if(i<n && line[i]!=',') report_syntax_error(line_no,"Table Names Must Be Seperated By Commas In A Join");
                if(!TableExists(p)) report_syntax_error(line_no,"Table "+p+" Does Not Exist");
                for(auto t:Tables[table_name[p]].attributes) avail_attr.insert(t);
                i++;
            }
            // for(auto a:avail_attr) cout<<a<<"\n";
            for(string p:attr) if(avail_attr.find(p) == avail_attr.end()) report_syntax_error(line_no,"Attribute "+p+" Does Not Exist In The Created Join");



        }
        else report_syntax_error(line_no);
        
    }

    cout<<"\n";
    if(!syn_error)cout<<"Parsing Successful, No Syntax Error Identified:)\n";
    else cout<<error_cnt<<" Syntax Errors Identified";
    // for(auto i:Tables){
    //     cout<<i.name<<"f\n";
    //     for(auto f:i.attributes) cout<<f<<" ";
    //     cout<<"f\n";
    //     for(auto f:i.attribute_type) cout<<f<<" ";
    //     cout<<"f\n";
    //     for(auto f:i.p_key) cout<<f<<" ";
    //     cout<<"f\n";
    //     for(auto j:i.foreign_ref){
    //         cout<<j.second<<" : ";
    //         for(auto k:j.first) cout<<k<<" ";
    //         cout<<"f\n";
    //     }
        // for(auto f:i.attributes) cout<<f<<" ";
        // cout<<"f\n";

    // }
    
}




