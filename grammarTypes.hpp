#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>
#include <algorithm>
#include <sstream>
#include <string.h>
using namespace std;

class VAR_DECL{
	public:
	string type;
	string id;
	int line;
};

class idPair{
    public:
    string first;
    int second;
};

class STRING_DECL{
	public:
	string type;
	string id;
	int line;
	string value;
};

class SYMBOL_BLOCK{
	public:
	string name;
	string first = "";

	string func_name;

	int link = 1;
	int arg = 2;

	unordered_map<string, string> variable_to_dollar;

	vector<VAR_DECL*> Variables;
	vector<STRING_DECL*> StringVar;
	vector<string> CODE_BLK;

	// return T/F , vartype-> s or i or r
	pair<bool, string> findID(string Id){
		pair<bool, string> ret;
		ret.first = false;
		for (int i = 0; i < Variables.size(); i++){
			if (Id == Variables[i]->id){
				ret.first = true;

				if (Variables[i]->type == "INT"){
					ret.second = "i"; 
				} else
				{
					ret.second = "r";
				}
				
				return ret;
			}
		}

		for (int i = 0; i < StringVar.size(); i++)
		{
			if (Id == StringVar[i]->id){
				ret.first = true;

				ret.second = "s";
				
				return ret;
			}
		}
		


		return ret;
	}

	int getLink(){

		int ret = this->link;
		this->link++;
		return ret;

	}

	int getArg(){

		int ret = this->arg;
		this->arg++;
		return ret;

	}
};



class WRITE_BLOCK{
	public:
	string type;
	string code;
	string id;


};

class three_AC{
	public:
	string opcode;
	string addr1;
	bool addr1_isreg;
	string addr2;
	bool addr2_isreg;

	three_AC(){}

	string optype;
	string typeaddr1;
	string typeaddr2;

	string code(){
		string code = opcode;

		if (addr1 != "")
		{
			code += " ";
			code += addr1;
		}

		if (addr2 != "")
		{
			code += " ";
			code += addr2;
		}
		
		return code;
	}
};

class AST_Node{
	public:
	AST_Node* parent;
	vector<AST_Node*> children;

	string exprReg;
	string expr_prefix_op;

	string add_op_type;
	string mul_op_type;

	string goto_ins;

	string outlabel;
	string label;

	string factor_op;

	string type; //vardecl
	string id;
	int line;
	string value;

	string first; // identifier
    int second;

	vector<three_AC> three_address_codes;
	vector<string> code;
	vector<AST_Node*> expr;
	AST_Node(){
		this->parent = NULL;
		this->exprReg = "";
	}
};


class BasicBlock{
	public:
	vector<three_AC*>  three_address_codes;
	vector<string> code;

	vector<BasicBlock*> children;
	vector<BasicBlock*> parents;
};

class ControlFlowGraph{
	public:
	BasicBlock* root;
};

class AST{

	AST_Node* currNode;

	int regno = 0;
	int labelno = 1;
	public:
	AST_Node* Root;
	vector<int> adder;
	vector<int> mul;

	vector<string> final_code;
	unordered_map<string, string> RegisterDecl;

	int reg[4];


	AST(){

	}

	void generateCode(){

	}

	void doubleMemFix(three_AC* temp, string&code){
		string modcode;
		bool modbit = false;
		string code1, code2, code3;
		// if (temp->opcode.substr(0, 3) == "add"){
		// 	cout << "substr working " << "\n";
		// }
		if ((temp->opcode.substr(0, 3) == "add" || temp->opcode.substr(0, 3) == "sub" || temp->opcode.substr(0, 3) == "mul" || temp->opcode.substr(0, 3) == "div" || temp->opcode.substr(0, 3) == "cmp") && !temp->addr1_isreg && !temp->addr2_isreg)
		{
			string reg = this->getNewReg();
			code1 = "move " + temp->addr2 + " " + reg;
			string prevaddr = temp->addr2;
			temp->addr2 = reg;
			// cout << "hit error" << "\n";
			code2 = temp->code();

			// code3 = "move " + reg + " " + prevaddr;
			modbit = true;
		}


		if (modbit)
		{	
			modcode += code1; modcode += "\n";
			modcode += code2; modcode += "\n";
			// modcode += code3; modcode += "\n";

			code = modcode;
		}

	}

	void fixCode(){
		for (auto& code : this->Root->code){
			char* tok = strtok((char*)code.c_str(), "\n");
			three_AC* temp = splitCode(string(tok));
			doubleMemFix(temp, code);
			// __alloc_reg(temp);
			// this->final_code.push_back(temp->code());
			delete temp;
			while (tok=strtok(NULL, "\n"))
			{
				temp = splitCode(string(tok));
				doubleMemFix(temp, code);
				// __alloc_reg(temp);
				// this->final_code.push_back(temp->code());
				delete temp;
			}

		}
	}

	void printCode(){
		for(auto code : this->Root->code){
			cout << code << "\n";
		}
	}

	bool isNum(string s){
		for (int i = 0; i < s.length(); i++)
		{
			if (s.substr(i,1) != "0" && s.substr(i,1) != "1" && s.substr(i,1) != "2" && s.substr(i,1) != "3" && s.substr(i,1) != "4" && s.substr(i,1) != "5" && s.substr(i,1) != "6" && s.substr(i,1) != "7" && s.substr(i,1) != "8" && s.substr(i,1) != "9"){
				return false;
			}
		}
		return true;
	}

	three_AC* splitCode(string code){
		three_AC* temp = new three_AC();

		stringstream ss(code);
    	string word;

		int num = 1;
		string lastaddr;
		string type;
    	while (ss >> word) {
			
			if (num<3)
			{
	       		// cout << word << "\n";
				if (num==1)
				{
					temp->opcode = word;

				}

				if (num==2)	{
					temp->addr1 = word;
					if (word.substr(0,1) == "r" && isNum(word.substr(1, word.length()-1)))
					{
						temp->addr1_isreg = true;
						// cout << word << "\n";
					} else{
						temp->addr1_isreg = false;
					}
				}
				
			} else {
				lastaddr += word;
				lastaddr += " ";
			}
			num++;
    	}

		lastaddr = lastaddr.substr(0, lastaddr.length()-1);
		if (num>=3)
		{
			temp->addr2 = lastaddr;
			if (lastaddr.substr(0,1) == "r" && isNum(lastaddr.substr(1, lastaddr.length()-1))){
				temp->addr2_isreg = true;
				// cout << lastaddr << "\n";
			} else{
				temp->addr2_isreg = false;
			}
		}
		

		return temp;
	}

	void __alloc_reg(three_AC* temp){

		string ac1 = "";
		string ac2 = "";
		if (temp->addr1_isreg && temp->addr1 != ""){
			// cout << temp->addr1.substr(1, temp->addr1.length()) << "\n";
			string varno = temp->addr1.substr(1, temp->addr1.length());
			string precode = "move T200010021Var" + varno + " " + "r0";
			temp->addr1 = "r0";
			ac1 = "move r0 T200010021Var" + varno;


			this->final_code.push_back(precode);
			// this->final_code.push_back(temp->code());
			// this->final_code.push_back(aftercode); 
		}

		if (temp->addr2_isreg && temp->addr2 != ""){
			// cout << temp->addr2.substr(1, temp->addr2.length()) << "\n";
			string varno = temp->addr2.substr(1, temp->addr2.length());
			string precode = "move T200010021Var" + varno + " " + "r1";
			temp->addr2 = "r1";
			ac2 = "move r1 T200010021Var" + varno;

			this->final_code.push_back(precode);
		}
		
		this->final_code.push_back(temp->code());

		if (ac1 != "")
			this->final_code.push_back(ac1);

		if (ac2 != "")
			this->final_code.push_back(ac2);
		
	}

	void registerAllocation(){

		for (int i = 0; i <= this->regno+1; i++){
			this->final_code.push_back("var T200010021Var" + to_string(i));
		}

		for (auto code : this->Root->code){
			char* tok = strtok((char*)code.c_str(), "\n");
			three_AC* temp = splitCode(string(tok));
			__alloc_reg(temp);
			// this->final_code.push_back(temp->code());

			while (tok=strtok(NULL, "\n"))
			{
				temp = splitCode(string(tok));
				__alloc_reg(temp);
				// this->final_code.push_back(temp->code());
			}
			
		}
	}

	void printfinalcode(){
		for(auto code : this->final_code){
			cout << code << "\n";
		}
	}

	string getNewReg(){
		string res = "r";
		res += to_string(regno);
		regno++;
		return res;
	}

	string getNewLabel(){
		std::string res = "label";
		res += to_string(labelno);
		labelno++;
		return res;
	}

	void passCodeUp(AST_Node *parent, AST_Node *child){

		if(child&&parent){
			for(auto code : child->code){
				parent->code.push_back(code);
			}
		}
	}

	void postordertraversal(AST_Node *root, vector<string> &final_code){
		
		for(auto child : root->children){
			postordertraversal(child, final_code);
		}
		
		
		if(root->children.size() == 0){

			for(auto code : root->three_address_codes){
				final_code.push_back(code.code());
			}
		}
	}

};

vector<SYMBOL_BLOCK*> SYMBOL_TABLE;
vector<unordered_map<string, string>> scope_manager;
unordered_map<int, unordered_map<string, string>> all_scopes;

SYMBOL_BLOCK* currBlock = NULL;
string currVarType ;

bool isDecl;
string currDeclType;
int blockCounter = 1;