#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>
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
	string addr2;
	string addr3;

	three_AC(){}

	string optype;
	string typeaddr1;
	string typeaddr2;
	string typeaddr3;

	string code(){
		string code = opcode + " ";

		if (addr1 != "")
		{
			code += addr1;
			code += " ";
		}

		if (addr2 != "")
		{
			code += addr2;
			code += " ";
		}

		if (addr3 != "")
		{
			code += addr3;
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


class AST{

	AST_Node* currNode;

	int regno = 0;
	int labelno = 1;
	public:
	AST_Node* Root;
	vector<int> adder;
	vector<int> mul;


	AST(){

	}

	void generateCode(){

	}

	void printCode(){
		for(auto code : this->Root->code){
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