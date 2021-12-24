#ifndef TREE_H
#define TREE_H
#include <vector>
#include <string>
#include <stack>
#include <new>
#include"Scanner.h"
using namespace std;
class node
{
public:
	vector<node*> children;
	node* right_node;
	bool is_statement;
	bool is_root;
	bool is_repeat;
	bool is_else_part;
	bool is_if;
	bool is_first_statement;
	int num_children;
	Token nodeToken;
	node(Token token);
	node();
	void addChild(node* child);
	vector<node*> getChild();
	
	
	friend class Tree;
};
class Tree
{
private:
	node* root;
	Scanner* scanner;
public:
	Tree(string filename);
	void delete_tree(node* current_node);
	~Tree();
	void setRoot(node* ptr);
	node* getRoot();
	void factor(node* parent);
	void term(node* parent);
	void simple_exp(node* parent);
	void exp(node* parent);
	void repeat_statement(node* parent);
	void if_statement(node* parent);
	node* statement(node* parent);
	void statement_sequance(node* parent);
	void parse();
	void print_syntax_tree(node* parent,int level);
};
#endif
