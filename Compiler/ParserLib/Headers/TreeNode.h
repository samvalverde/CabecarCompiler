#ifndef NODE_H
#define NODE_H


#include <vector>
#include <string>
#include "CToken.h"

using namespace std;
using namespace ScannerLib;

namespace ParserLib
{
	class TreeNode {

	private:
		TreeNode *parent;
		vector <TreeNode*> children;
		int idRegla;
		string data;
		string type;
		string value;
		string dims = "";
		string scope;

	public:
		TreeNode();
		TreeNode(TreeNode * parent);

		TreeNode * GetParent();

		void SetChildren(vector<TreeNode*> children);
		vector<TreeNode*> GetChildren();
		void AddChildren(TreeNode * children);

		void SetData(int idRegla, string data);
		string GetData();
		int GetIdRegla();

		bool IsLeaf();
		bool IsInternalNode();
		bool IsRoot();

		void print(int nivel);

		void SetType(std::string type);
		string GetType();

		void SetValue(std::string value);
		string GetValue();

		void SetDims(std::string dims);
		string GetDims();

		void SetScope(std::string scope);
		string GetScope();
	};

}

#endif