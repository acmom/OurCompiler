#include <iostream>
#include <stack>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <strstream>
#include <sstream>
using namespace std;
class Quan
{
public:
    string op;
    string arg1;
    string arg2;
    int result;
    string arg3;
};
void isRop(string r);
int pop(stack<int> *ss);
string pop(stack<string> *ss);
string pop(stack<double> *ss);
void Gen(string oper,string arg,string arg2,int result);
void Gen(string oper,string arg,string arg2,string arg3);
void semanticAction(int productionNo);
int getNo(string token);
string getSymbol(string token);
int getSymbolIndex(string sym);
void init();
void Backpatch(int tmp,int t);
int Merge(int p1,int p2);
string NewTemp();
stack<int> s_state;
stack<string> s_char;
stack<double> d_value;
stack<string> s_place;
stack<string> s_ID;
stack<int> TC;
stack<int> FC;
stack<int> T_chain;
stack<int> S_chain;
stack<int> C_chain;
vector<Quan*> AllQuan;
int NXQ = 1;
int tmpNum =1;
string rop;
double value[256];
string symbol[256];

int size_all_sym=26;
string reg_Exp[22] =
{
"S:ID = EVAL ; ",
"S:T S ",
"S:C S ",
"EVAL:@ EVAL ",
"EVAL:EVAL * EVAL ",
"EVAL:EVAL + EVAL ",
"EVAL:( EVAL ) ",
"EVAL:ID ",
"EVAL:NUMBER ",
"EB:EVAL ",
"EB:EVAL rop EVAL ",
"EB:( EB ) ",
"EB:~ EB ",
"EB:EAND EB ",
"EB:EOR EB ",
"EAND:EB && ",
"EOR:EB || ",
"rop:< ",
"rop:> ",
"rop:== ",
"C:if ( EB ) ",
"T:C S else "
};
string all_symbols[26]= 
{
"else","@","&&","$","||","*","+","(",")","NUMBER","==","if","ID",";","~",">","=","<","T","EAND","EB","S","rop","C","EVAL","EOR"
};
int parser_table[44][26] =
{
0,0,0,0,0,0,0,0,0,0,0,3,6,0,0,0,0,0,2,0,0,4,0,5,0,0,
0,0,0,0,0,0,0,0,0,0,0,3,6,0,0,0,0,0,2,0,0,7,0,5,0,0,
0,0,0,0,0,0,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,3,6,0,0,0,0,0,2,0,0,9,0,5,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,
-3,0,0,-3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,12,0,0,0,0,0,17,0,19,0,0,14,0,16,0,0,0,0,11,13,0,0,0,15,18,
20,0,0,-4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,12,0,0,0,0,0,22,0,19,0,0,14,0,0,0,0,0,0,0,0,0,0,0,21,0,
0,12,0,0,0,0,0,17,0,19,0,0,14,0,16,0,0,0,0,11,23,0,0,0,15,18,
0,12,0,0,0,0,0,22,0,19,0,0,14,0,0,0,0,0,0,0,0,0,0,0,24,0,
0,0,25,0,26,0,0,0,27,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,-9,0,-9,-9,-9,0,-9,0,-9,0,0,-9,0,-9,0,-9,0,0,0,0,0,0,0,0,
0,0,-11,0,-11,30,31,0,-11,0,28,0,0,0,0,32,0,33,0,0,0,0,29,0,0,0,
0,12,0,0,0,0,0,17,0,19,0,0,14,0,16,0,0,0,0,11,34,0,0,0,15,18,
0,12,0,0,0,0,0,17,0,19,0,0,14,0,16,0,0,0,0,11,35,0,0,0,36,18,
0,12,0,0,0,0,0,17,0,19,0,0,14,0,16,0,0,0,0,11,37,0,0,0,15,18,
0,0,-10,0,-10,-10,-10,0,-10,0,-10,0,0,-10,0,-10,0,-10,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,-23,-23,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,30,31,0,0,0,0,0,0,38,0,0,0,0,0,0,0,0,0,0,0,0,
0,12,0,0,0,0,0,22,0,19,0,0,14,0,0,0,0,0,0,0,0,0,0,0,39,0,
0,0,25,0,26,0,0,0,-15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,-5,0,-5,-5,-5,0,-5,0,-5,0,0,-5,0,-5,0,-5,0,0,0,0,0,0,0,0,
0,-17,0,0,0,0,0,-17,0,-17,0,0,-17,0,-17,0,0,0,0,0,0,0,0,0,0,0,
0,-18,0,0,0,0,0,-18,0,-18,0,0,-18,0,-18,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,-22,-22,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,-21,0,0,0,0,0,-21,0,-21,0,0,-21,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,12,0,0,0,0,0,22,0,19,0,0,14,0,0,0,0,0,0,0,0,0,0,0,40,0,
0,12,0,0,0,0,0,22,0,19,0,0,14,0,0,0,0,0,0,0,0,0,0,0,41,0,
0,12,0,0,0,0,0,22,0,19,0,0,14,0,0,0,0,0,0,0,0,0,0,0,42,0,
0,-20,0,0,0,0,0,-20,0,-20,0,0,-20,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,-19,0,0,0,0,0,-19,0,-19,0,0,-19,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,-14,0,-14,0,0,0,-14,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,25,0,26,0,0,0,43,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,-11,0,-11,30,31,0,44,0,28,0,0,0,0,32,0,33,0,0,0,0,29,0,0,0,
0,0,25,0,26,0,0,0,-16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
-2,0,0,-2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,30,31,0,44,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,-12,0,-12,30,31,0,-12,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,-6,0,-6,30,-6,0,-6,0,-6,0,0,-6,0,-6,0,-6,0,0,0,0,0,0,0,0,
0,0,-7,0,-7,30,31,0,-7,0,-7,0,0,-7,0,-7,0,-7,0,0,0,0,0,0,0,0,
0,0,-13,0,-13,0,0,0,-13,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,-8,0,-8,-8,-8,0,-8,0,-8,0,0,-8,0,-8,0,-8,0,0,0,0,0,0,0,0
};
void semanticAction(int productionNo)
{
switch(productionNo)
{
case 0:

break;
case 1:
{string sym = pop(&s_ID);string tmp1 = pop(&s_place);Gen("=",tmp1,"-",sym);}
break;
case 2:
{int tmp = Merge(pop(&T_chain),pop(&S_chain));S_chain.push(tmp);}
break;
case 3:
{int tmp = Merge(pop(&C_chain),pop(&S_chain));S_chain.push(tmp);}
break;
case 4:
{string T = NewTemp();string tmp1 = pop(&s_place);Gen("@",tmp1,"-",T);s_place.push(T);}
break;
case 5:
{string T = NewTemp();string tmp1 = pop(&s_place);string tmp2 = pop(&s_place);Gen("*",tmp1,tmp2,T);s_place.push(T);}
break;
case 6:
{string T = NewTemp();string tmp1 = pop(&s_place);string tmp2 = pop(&s_place);Gen("+",tmp1,tmp2,T);s_place.push(T);}
break;
case 7:
{ }
break;
case 8:
{string sym = pop(&s_ID);s_place.push(sym);}
break;
case 9:
{string num = pop(&d_value);s_place.push(num);}
break;
case 10:
{string tmp1 = pop(&s_place);TC.push(NXQ);FC.push(NXQ+1);Gen("jnz",tmp1,"-",0);Gen("j","-","-",0);}
break;
case 11:
{string tmp1 = pop(&s_place);string tmp2 = pop(&s_place);TC.push(NXQ);FC.push(NXQ+1);Gen("j"+rop,tmp1,tmp2,0);Gen("j","-","-",0);}
break;
case 12:
{ }
break;
case 13:
{int tmp = pop(&FC);FC.push(pop(&TC));TC.push(tmp);}
break;
case 14:
{int tmp1 = pop(&FC);int tmp2 = pop(&FC);int temp = Merge(tmp1,tmp2);FC.push(temp);}
break;
case 15:
{int tmp1 = pop(&TC);int tmp2 = pop(&TC);int temp = Merge(tmp1,tmp2);TC.push(temp);}
break;
case 16:
{int tmp1 = pop(&TC);Backpatch(tmp1,NXQ);}
break;
case 17:
{int tmp1 = pop(&FC);Backpatch(tmp1,NXQ);}
break;
case 18:
{ }
break;
case 19:
{ }
break;
case 20:
{ }
break;
case 21:
{Backpatch(pop(&TC),NXQ);C_chain.push(pop(&FC));}
break;
case 22:
{int q =NXQ;Gen("j","-","-",0);Backpatch(pop(&C_chain),NXQ);int tmp = Merge(pop(&S_chain),q);T_chain.push(tmp);}
break;
}
}
int main()
{
   init();
	ifstream infile("token");
	string temp;
	s_state.push(0);
	s_char.push("$");
	string sym;
	int index=0;
	int top_state;
	int action;
	int go_to;
	int reduction = 0;
	int id_num = -1;
	while(1)
		{
		if(reduction ==0)
		getline(infile,temp);
		sym = getSymbol(temp);
		isRop(sym);
		index = getSymbolIndex(sym);
		top_state = s_state.top();
		action = parser_table[top_state][index];
		if(action==-1)
		{
		cout<<"success!"<<endl;
		break;
		}
        else if(action==0)
        {
            cout<<"error!"<<endl;
            break;
        }
        else if(action>0)
        {
            cout<<"move:"<<sym<<endl;
            s_char.push(sym);
            s_state.push(action-1);
            reduction = 0;

            if(getNo(temp)>=0)
            {
                if(sym.compare("NUMBER")==0)
                {
                    d_value.push(value[getNo(temp)]);
                }
                if(sym.compare("ID")==0)
                {
                    s_ID.push(symbol[getNo(temp)]);
                }
            }
        }
        else if(action<0)
        {
            string exp = reg_Exp[-(action+1)-1];
            cout<<"reduce:"<<exp<<endl;
            int i;
            int num=0;
            for(i=0; i<exp.size(); i++)
            {
                if(exp.at(i)==' ')
                    num++;
            }
            while(num--)
            {
                s_char.pop();
                s_state.pop();
            }
            s_char.push(exp.substr(0,exp.find(":")));
            index = getSymbolIndex(s_char.top());
            go_to = parser_table[s_state.top()][index];
            if(go_to!=0)
            {
                s_state.push(go_to-1);
                semanticAction(-(action+1));
            }
            else
            {
                cout<<"error"<<endl;
                break;
            }
            reduction = 1;//reader don't move
        }
    }

    int max = AllQuan.size();
    int p = pop(&S_chain);
    Backpatch(p,max+1);
    ofstream fcout("quan_out");
    for(int i=0; i<AllQuan.size(); i++)
    {
        cout<<"("<<i+1<<")"<<":";
        fcout<<"("<<i+1<<")"<<":";
        Quan* p =AllQuan.at(i);
        if(p->arg3=="NULL"){
            cout<<p->op<<","<<p->arg1<<","<<p->arg2<<","<<"("<<p->result<<")"<<endl;
            fcout<<p->op<<","<<p->arg1<<","<<p->arg2<<","<<"("<<p->result<<")"<<endl;
        }
        else
        {
            cout<<p->op<<","<<p->arg1<<","<<p->arg2<<","<<p->arg3<<endl;
            fcout<<p->op<<","<<p->arg1<<","<<p->arg2<<","<<p->arg3<<endl;
        }
    }
    cout<<"end"<<endl;
	system("pause");
	return 0;
}
void init()
{
    //这个函数可以不要，在Yacc中将这些数组初始化好
    // get num table
    ifstream infile("NUMtable");
    string temp;
    while(getline(infile,temp))
    {
        double ft = atof(temp.substr(0,temp.find(" ")).c_str());
        value[atoi(temp.substr(temp.find(" ")+1,temp.length()-temp.find(",")-2).c_str())]=ft;
    }
    //get all symbol
    ifstream infile2("IDtable");
    int i=0;
    while(getline(infile2,temp))
    {
        symbol[i++]=temp.substr(0,temp.find(" "));
    }
}
//获得符号所在的列数
int getSymbolIndex(string sym)
{
    int i=0;
    //cout<<"!!"<<sym<<endl;
    for(i=0; i<size_all_sym; i++)
        if(all_symbols[i].compare(sym)==0)
            return i;
    return -1;
}
int getNo(string token) //return token's id num
{

    if(token.find(",")!=string::npos)
    {
        return atoi(token.substr(token.find(",")+1,token.length()-token.find(",")-2).c_str());
    }
    return -1;
}
//获得token序列中的符号
string getSymbol(string token) //return token's symbol
{
    //found!=std::string::npos
    if(token.find(",")==string::npos)
    {
        return token.substr(1,token.length()-2);
    }
    else
    {
        return token.substr(1,token.find(",")-1);
    }
}
//状态栈出栈操作
int pop(stack<int> *ss)
{
    if(ss->empty())
        return 0;
    else
    {
        int temp = ss->top();
        ss->pop();
        return temp;
    }
}
//符号栈出栈操作
string pop(stack<string> *ss)
{
    if(ss->empty())
    {
        return NULL;
    }
    else
    {
        string temp = ss->top();
        ss->pop();
        return temp;
    }
}
//数字栈出栈操作
string pop(stack<double> *ss)
{
    double temp = ss->top();
    char str[20];
    string t;
    sprintf(str,"%f",temp);
    t= str;
    ss->pop();
    return t;
}
//四元式生成
void Gen(string oper,string arg,string arg2,int result)
{
    Quan* q =  new Quan();
    q->op = oper;
    q->arg1 = arg;
    q->arg2 = arg2;
    q->result = result;
    q->arg3 = "NULL";
    AllQuan.push_back(q);
    NXQ++;
}
//四元式生成
void Gen(string oper,string arg,string arg2,string arg3)
{
    Quan* q =  new Quan();
    q->op = oper;
    q->arg1 = arg;
    q->arg2 = arg2;
    q->arg3 = arg3;
    AllQuan.push_back(q);
    NXQ++;
}

void Backpatch(int tmp,int t)
{
    //tmp 从 1 开始
    int Q = tmp;
    while(Q!=0)
    {
        Quan * q = AllQuan.at(Q-1);
        int r = q->result;
        q->result = t;
        if(r<0)
            r = -r;
        else
            break;
        Q = r;
    }
}
int Merge(int p1,int p2)
{
    if(p2==0)
    {
        return p1;
    }
    else
    {
        int tmp = p2;
        Quan* q = AllQuan.at(tmp-1);
        while((tmp=q->result)!=0)
        {
            q = AllQuan.at((-tmp)-1);
        }
        q->result =-p1;
        return p2;
    }
}
string NewTemp()
{
    string str;
    char t[20];
    sprintf(t, "T%d", tmpNum);
    str =t;
    tmpNum++;
    return str;
}
void isRop(string r){
    if(r=="=="||r=="<"||r==">"||r==">="||r=="<=")
        rop = r;
}
