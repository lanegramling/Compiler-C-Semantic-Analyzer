#ifndef LILC_AST_HPP
#define LILC_AST_HPP

#include <ostream>
#include <list>
#include "symbols.hpp"

//Here is a suggestion for all the different kinds of AST nodes
// and what kinds
// of children they have. All of these kinds of AST nodes are
// subclasses of ASTNode.
// Indentation indicates further subclassing:
//
//     Subclass		Children
//     --------		------
//     ProgramNode	DeclListNode
//     DeclListNode	linked list of DeclNode
//     DeclNode
//       VarDeclNode	TypeNode, IdNode, int
//       FnDeclNode	TypeNode, IdNode, FormalsListNode, FnBodyNode
//       FormalDeclNode    TypeNode, IdNode
//       StructDeclNode    IdNode, DeclListNode
//
//     FormalsListNode     linked list of FormalDeclNode
//     FnBodyNode          DeclListNode, StmtListNode
//     StmtListNode        linked list of StmtNode
//     ExpListNode         linked list of ExpNode
//
//     TypeNode:
//       IntNode           -- none --
//       BoolNode          -- none --
//       VoidNode          -- none --
//       StructNode        IdNode
//
//     StmtNode:
//       AssignStmtNode      AssignNode
//       PostIncStmtNode     ExpNode
//       PostDecStmtNode     ExpNode
//       ReadStmtNode        ExpNode
//       WriteStmtNode       ExpNode
//       IfStmtNode          ExpNode, DeclListNode, StmtListNode
//       IfElseStmtNode      ExpNode, DeclListNode, StmtListNode,
//                                    DeclListNode, StmtListNode
//       WhileStmtNode       ExpNode, DeclListNode, StmtListNode
//       CallStmtNode        CallExpNode
//       ReturnStmtNode      ExpNode
//
//     ExpNode:
//       IntLitNode          -- none --
//       StrLitNode          -- none --
//       TrueNode            -- none --
//       FalseNode           -- none --
//       IdNode              -- none --
//       DotAccessNode       ExpNode, IdNode
//       AssignNode          ExpNode, ExpNode
//       CallExpNode         IdNode, ExpListNode
//       UnaryExpNode        ExpNode
//         UnaryMinusNode
//         NotNode
//       BinaryExpNode       ExpNode ExpNode
//         PlusNode
//         MinusNode
//         TimesNode
//         DivideNode
//         AndNode
//         OrNode
//         EqualsNode
//         NotEqualsNode
//         LessNode
//         GreaterNode
//         LessEqNode
//         GreaterEqNode
//
//
// Here are the different kinds of AST nodes again, organized according to
// whether they are leaves, internal nodes with linked lists of kids, or
// internal nodes with a fixed number of kids:
//
// (1) Leaf nodes:
//        IntNode,   BoolNode,  VoidNode,  IntLitNode,  StrLitNode,
//        TrueNode,  FalseNode, IdNode
//
// (2) Internal nodes with (possibly empty) linked lists of children:
//        DeclListNode, FormalsListNode, StmtListNode, ExpListNode
//
// (3) Internal nodes with fixed numbers of kids:
//        ProgramNode,     VarDeclNode,     FnDeclNode,     FormalDeclNode,
//        StructDeclNode,  FnBodyNode,      StructNode,     AssignStmtNode,
//        PostIncStmtNode, PostDecStmtNode, ReadStmtNode,   WriteStmtNode
//        IfStmtNode,      IfElseStmtNode,  WhileStmtNode,  CallStmtNode
//        ReturnStmtNode,  DotAccessNode,   CallExpNode,
//        UnaryExpNode,    BinaryExpNode,   UnaryMinusNode, NotNode,
//        PlusNode,        MinusNode,       TimesNode,      DivideNode,
//        AndNode,         OrNode,          EqualsNode,     NotEqualsNode,
//        LessNode,        GreaterNode,     LessEqNode,     GreaterEqNode
//
// **********************************************************************

// **********************************************************************
// ASTnode class (base class for all other kinds of nodes)
// **********************************************************************

namespace LILC{

class SymSymbol;
class DeclListNode;
class DeclNode;
class TypeNode;
class IdNode;
class StructDeclNode;

class ASTNode{
public:
	virtual void unparse(std::ostream& out, int indent) = 0;
	void doIndent(std::ostream& out, int indent){
		for (int k = 0 ; k < indent; k++){ out << " "; }
	}
};

class ProgramNode : public ASTNode{
public:
	ProgramNode(DeclListNode * L) : ASTNode(){
		myDeclList = L;
	}
	void unparse(std::ostream& out, int indent);
private:
	DeclListNode * myDeclList;

};

class DeclListNode : public ASTNode{
public:
	DeclListNode(std::list<DeclNode *> * decls) : ASTNode(){
		myDecls = *decls;
	}
	void unparse(std::ostream& out, int indent);
private:
	std::list<DeclNode *> myDecls;
};

class FormalsListNode : public ASTNode {
public:
	FormalsListNode(std::list<DeclNode *> * decls) : ASTNode() {
		myDecls = *decls;
	}
	void unparse(std::ostream& out, int indent);
private:
	std::list<DeclNode *> myDecls;
};

class DeclNode : public ASTNode{
public:
	virtual void unparse(std::ostream& out, int indent) = 0;
};

class VarDeclNode : public DeclNode{
public:
	VarDeclNode(TypeNode * type, IdNode * id, int size) : DeclNode(){
		myType = type;
		myId = id;
		mySize = size;
	}
	void unparse(std::ostream& out, int indent);
	static const int NOT_STRUCT = -1; //Use this value for mySize
					  // if this is not a struct type
private:
	TypeNode * myType;
	IdNode * myId;
	int mySize;
};

class FormalDeclNode : public DeclNode {
public:
	FormalDeclNode(TypeNode * type, IdNode * id, int size) : DeclNode() {
		myType = type;
		myId = id;
		mySize = size;
	}
	void unparse(std::ostream& out, int indent);
private:
	TypeNode * myType;
	IdNode * myId;
	int mySize;
};

class StmtNode : public ASTNode {
public:
	StmtNode() : ASTNode(){}
	virtual void unparse(std::ostream& out, int indent) = 0;
};

class StmtListNode : public ASTNode {
public:
	StmtListNode(std::list<StmtNode *> * list) : ASTNode() {
		myList = *list;
	}
	void unparse(std::ostream& out, int indent);
private:
	std::list<StmtNode *> myList;
};

class FnBodyNode : public ASTNode {
public:
	FnBodyNode(std::list<DeclNode *> * varDeclList, StmtListNode * stmtList) : ASTNode() {
		myDeclList = *varDeclList;
		myStmtList = stmtList;
	}
	void unparse(std::ostream& out, int indent);
private:
	std::list<DeclNode *> myDeclList;
	StmtListNode * myStmtList;
};

class FnDeclNode : public DeclNode {
public:
	FnDeclNode(TypeNode * type, IdNode * id, FormalsListNode * formals, FnBodyNode * body, int size) : DeclNode() {
		myType = type;
		myId = id;
		myFormals = formals;
		myBody = body;
		mySize = size;	}
	void unparse(std::ostream& out, int indent);

private:
	TypeNode * myType;
	IdNode * myId;
	FormalsListNode * myFormals;
	FnBodyNode * myBody;
	int mySize;
};

/*  NOTE Need formals stuff  */

class VarDeclListNode : public ASTNode{
public:
	VarDeclListNode(std::list<VarDeclNode *> * varDecls) : ASTNode(){
		myVarDecls = *varDecls;
	}
	void unparse(std::ostream& out, int indent);
private:
	std::list<VarDeclNode *> myVarDecls;
};

class StructDeclNode : public DeclNode {
public:
	StructDeclNode(IdNode * id, std::list<DeclNode *> * varDecls, int size) : DeclNode(){
		myId = id;
		mySize = size;
		myDecls = *varDecls;
	}
	void unparse(std::ostream& out, int indent);
private:
	IdNode * myId;
	int mySize;
	std::list<DeclNode *> myDecls;
};

class ExpNode : public ASTNode {
public:
	ExpNode() : ASTNode() {}
	virtual void unparse(std::ostream& out, int indent) = 0;
		IdNode * myId;
		int mySize;
		std::list<DeclNode *> myDecls;
};

class TypeNode : public ASTNode{
public:
	TypeNode() : ASTNode(){
	}
	virtual void unparse(std::ostream& out, int indent) = 0;
};

class IdNode : public TypeNode{
public:
	IdNode(IDToken * token) : TypeNode(){
		myStrVal = token->value();
	}
	void unparse(std::ostream& out, int indent);
private:
	std::string myStrVal;
};

class IntNode : public TypeNode{
public:
	IntNode(): TypeNode(){
	}
	void unparse(std::ostream& out, int indent);
};

class BoolNode : public TypeNode{
public:
	BoolNode(): TypeNode(){
	}
	void unparse(std::ostream& out, int indent);
};

class VoidNode : public TypeNode{
public:
	VoidNode(): TypeNode(){
	}
	void unparse(std::ostream& out, int indent);
};

} //End namespace LIL' C

#endif
