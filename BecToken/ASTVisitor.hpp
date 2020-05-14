#ifndef SIF_LIBSIF_ASTVISITOR_H_
#define SIF_LIBSIF_ASTVISITOR_H_
using namespace std;
#include "ASTNodes.hpp"

namespace Sif {

    //ASTNodePtr deepCopy(ASTNodePtr astNodePtr);
	void createAddOwner(ContractDefinitionNode* testNode);
	void createCheckTotalAmount(ContractDefinitionNode* testNode);
	void addEvent(ContractDefinitionNode* testNode);
	void instrumentBatchTransferFunction(FunctionDefinitionNode* fd);
	void checkNodeType(ASTNodePtr astNodePtr, string str);
	void before(std::string arg);
	void visit(ASTNode* node);
	void after();
	void addInitialVariablesToConstructor(FunctionDefinitionNode* fd);	
}

#endif //SIF_LIBSIF_ASTVISITOR_H_
