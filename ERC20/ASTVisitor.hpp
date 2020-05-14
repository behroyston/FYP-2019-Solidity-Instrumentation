#ifndef SIF_LIBSIF_ASTVISITOR_H_
#define SIF_LIBSIF_ASTVISITOR_H_
using namespace std;
#include "ASTNodes.hpp"

namespace Sif {

    //ASTNodePtr deepCopy(ASTNodePtr astNodePtr);
	void createAddOwner(ContractDefinitionNode* testNode);
	void createCheckTotalAmount(ContractDefinitionNode* testNode);
	void addEvent(ContractDefinitionNode* testNode);
	void checkNodeType(ASTNodePtr astNodePtr, string str);
	void instrumentTransferFunction(FunctionDefinitionNode* fd);
	void addInitialVariablesToConstructor(FunctionDefinitionNode* fd);
	void before(std::string arg);
	void visit(ASTNode* node);
	void after();
}

#endif //SIF_LIBSIF_ASTVISITOR_H_
