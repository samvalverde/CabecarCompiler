#include <iostream>
#include <string>

#include "TreeNode.h"
#include "CToken.h"

using namespace std; 
using namespace ScannerLib;

namespace ParserLib
{
	TreeNode::TreeNode() {
		this->parent = NULL;
		this->children;
	}

	TreeNode::TreeNode(TreeNode * parent) {
		this->parent = parent;
	}

	TreeNode* TreeNode::GetParent() {
		return this->parent;
	}

	void TreeNode::SetChildren(vector<TreeNode*> children) {
		this->children = children;
	}

	vector<TreeNode*>  TreeNode::GetChildren() {
		return (this->children);
	}

	void TreeNode::AddChildren(TreeNode * children) {
		this->children.insert(this->children.begin(), children);
	}

	void TreeNode::SetData(int idRegla, string data) {
		this->data = data;
		this->idRegla = idRegla;
	}

	string TreeNode::GetData() {
		return this->data;
	}

	int TreeNode::GetIdRegla() {
		return this->idRegla;
	}

	void TreeNode::print(int nivel) {
		string strSpace = "";
		for (int i = 0; i < nivel; i++) {
			strSpace += "  ";
		}
		cout << strSpace << this->data << "\n";
		for (unsigned i = 0; i < this->children.size(); i++) {
			this->children[i]->print(nivel + 1);
		}
	}

	void TreeNode::SetType(std::string type) {
		this->type = type;
	}
	string TreeNode::GetType() {
		return this->type;
	}

	void TreeNode::SetValue(std::string value) {
		this->value = value;
	}
	string TreeNode::GetValue() {
		return this->value;
	}

	void TreeNode::SetDims(std::string dims) {
		this->dims = dims;
	}
	string TreeNode::GetDims() {
		return this->dims;
	}

	void TreeNode::SetScope(std::string scope) {
		this->scope = scope;
	}
	string TreeNode::GetScope() {
		return this->scope;
	}

}

