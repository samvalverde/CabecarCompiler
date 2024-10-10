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

	};

}

#endif