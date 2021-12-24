#include "Tree.h"
#include<iostream>
using namespace std;
node::node(Token token) {
	this->num_children = 0;
	this->is_root = false;
	this->is_else_part = false;
	this->is_first_statement = true;
	this->is_repeat = false;
	this-> is_if = false;
	this->right_node = NULL;
	this->is_statement = false;
	this->nodeToken = token;
}
node::node() {
	this->num_children = 0;
	this->right_node = NULL;
	this->is_else_part = false;
	this->is_root = false;
	this->is_repeat = false;
	this->is_first_statement = true;
	this->is_if = false;
	this->is_statement = false;
}
void node::addChild(node* child) {
	this->children.push_back(child);
	num_children++;
}
std::vector<node*> node::getChild()
{
	return this->children;
}

void Tree::factor(node* parent) {
	Token token = this->scanner->get_token(false);
	if (token.tokenType == "IDENTIFIER"|| token.tokenType == "number") {
		parent->addChild(new node(token));
	}
	else if (token.tokenType=="OPEN_PARENTHESIS") {
		exp(parent);
		token = this->scanner->get_token(false);
		if (token.tokenType != "CLOSE_PARENTHESIS") {
			//ERROR CODE
		}
	}
	else {
		//ERROR CODE
	}
}
void Tree::term(node* parent) {
	node* n = new node();
	factor(n);
	Token token = this->scanner->get_token(true);
	if (token.tokenType == "MULTIPLY"|| token.tokenType == "DIVISION") {
		this->scanner->match(token);
		n->nodeToken = token;
		parent->addChild(n);
		term(n);
	}
	else {
		parent->addChild(n->children[0]);
		delete n;
	}

}
void Tree::simple_exp(node* parent) {
	node* n = new node();
	term(n);
	Token token = this->scanner->get_token(true);
	if (token.tokenType == "PLUS" || token.tokenType == "MINUS") {
		this->scanner->match(token);
		n->nodeToken = token;
		parent->addChild(n);
		simple_exp(n);
	}
	else {
		parent->addChild(n->children[0]);
		delete n;
	}
}
void Tree::exp(node* parent) {
	node* n = new node();
	simple_exp(n);
	Token token = this->scanner->get_token(true);
	if (token.tokenType =="SMALLER"|| token.tokenType == "EQUAL") {
		this->scanner->match(token);
		n->nodeToken = token;
		parent->addChild(n);
		simple_exp(n);
		int y;
	}
	else {
		parent->addChild(n->children[0]);
		delete n;
	}

}

void Tree::repeat_statement(node* parent) {
	statement_sequance(parent);
	Token token = this->scanner->get_token(false);
	if (token.tokenType!="UNTIL") {
		//ERROR UNTIL IS MISSING
	}
	exp(parent);
}
void Tree::if_statement(node* parent) {
	exp(parent);
	Token token = this->scanner->get_token(false);
	if (token.tokenType=="THEN") {
		statement_sequance(parent);
	}
	else {
		//ERROR missing then
	}
	token = this->scanner->get_token(false);

	if (token.tokenType == "ELSE") {
		parent->is_else_part = true;
		statement_sequance(parent);
		token = this->scanner->get_token(false);
		if (token.tokenType != "END") {
			//end of else is missing
		}
	}
	else if (token.tokenType != "END") {
		//ERROR CODE END MISSING
	}
}
node* Tree::statement(node* parent) {
	Token token = this->scanner->get_token(false);
	node* n = new node(token);
	n->is_statement = true;
	if (token.tokenType == "IF") {
		n->is_if = true;
		if_statement(n);
	}
	else if (token.tokenType == "REPEAT") {
		n->is_repeat = true;
		repeat_statement(n);
	}
	else if (token.tokenType == "READ") {
		token = this->scanner->get_token(false);
		if (token.tokenType=="IDENTIFIER") {
			n->addChild(new node(token));
		}
	}
	else if (token.tokenType == "WRITE") {
		exp(n);
	}
	else if (token.tokenType == "IDENTIFIER") {
		token = this->scanner->get_token(false);

		if (token.tokenType != "Assign") {
			//error code := not found;
		}
		exp(n);
	}
	return n;
}
void Tree::statement_sequance(node* parent) {
	node* n = statement(parent);
	if (parent->is_root ) {
		parent->addChild(n);
	}
	else if ( ( (parent->is_repeat || parent->is_if) && parent->is_first_statement ) || parent->is_else_part ) {
		parent->is_first_statement = false;
		parent->addChild(n);
	}
	else {
		parent->right_node = n;
	}
	Token token = this->scanner->get_token(true);
	if (token.tokenType=="SEMICOLON") {
		this->scanner->match(token);
		statement_sequance(n);
	}
}
Tree::~Tree()
{
}

Tree::Tree(string filename) {
	this->root = NULL;
	ifstream infile(filename);
	string str="";
	str = "";

	for (string line = ""; getline(infile, line); )
	{
		str += line + "\n";
	}
	this->scanner = new Scanner(str);
	cout<<scanner->get_all_tokens();
	parse();
}
void Tree::setRoot(node* ptr) {
	this->root = ptr;
}
node* Tree::getRoot(){
	return this->root;
}

void Tree::parse() {
	this-> root = new node();
	this->root->is_root = true;
	statement_sequance(this->root);
	int x;
}
void Tree::print_syntax_tree(node* parent,int level) {
	if (parent->is_root) {
		print_syntax_tree(parent->children[0],level);
	}
	if (parent->children.size()==0) {
		cout << "Level=" << level << " child == >" <<"Token value ==> "<< parent->nodeToken.tokenValue <<" token type ==> "<< parent->nodeToken.tokenType << endl;
		return;
	}
	else if(parent->is_statement){
		cout <<"Level="<<level<< " STATEMENT == >"<< parent->nodeToken.tokenValue<< endl;
		if (parent->right_node!=NULL) {
			print_syntax_tree(parent->right_node, level);
		}
		
		for (auto n : parent->children) {
			print_syntax_tree(n, level + 1);
			cout << "Level=" << level << " child == >" << parent->nodeToken.tokenValue << endl;
		}
		
	}
}
