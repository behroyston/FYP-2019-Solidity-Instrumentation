#include "ASTVisitor.hpp"
#include <vector>
#include <string>

namespace Sif {


int count = 0;
int count1 = 0;

// var names
string TOTALHOLDERS = "_totalHolders";
string CHECKEXIST = "checkExist";
string CHECKHOLDERS = "checkHolders";

// function names 
string ADDOWNER = "addOwner";
string CHECKOWNER = "checkOwner";

// constant names
string ADDRESS = "address";
string UINTEGER8 = "uint8";
string UINTEGER256 = "uint256";
string BOOLEAN_CONS = "bool";
string BOOLEAN_TRUE = "true";
string BOOLEAN_FALSE = "false";

// function arguments
string TO = "_to";

void checkNodeType(ASTNodePtr astNodePtr, string str) {

    NodeType nodeType = astNodePtr->get_node_type();

    std::cout << str << " : " << nodeType << "\n";
}

void before(std::string arg) {
    return;
}

void visit(ASTNode* node) {
    // get the return value
    
    if (node->get_node_type() == NodeTypeContractDefinition)
    {
        if (count == 0)
            count++;
        else
            return;
        ContractDefinitionNode* testNode = (ContractDefinitionNode*) node;

        cout << "ContractDefinitionNode's Name: " << testNode->get_name() << "\n";

        // Initialization of Variables
        ElementaryTypeNameNode* element_key1 = new ElementaryTypeNameNode(ADDRESS);
        ElementaryTypeNameNode* element_value1 = new ElementaryTypeNameNode(UINTEGER8);
        MappingNode* mapNode1 = new MappingNode(NULL,NULL);

        mapNode1->set_key_type((ASTNodePtr)(element_key1));
        mapNode1->set_value_type((ASTNodePtr)(element_value1));
        
        VariableDeclarationNode* varDecl1 = new VariableDeclarationNode((ASTNodePtr)(mapNode1), CHECKEXIST);
        
        testNode->add_member((ASTNodePtr)(varDecl1));

        ElementaryTypeNameNode* element_key2 = new ElementaryTypeNameNode(UINTEGER8);
        ElementaryTypeNameNode* element_value2 = new ElementaryTypeNameNode(ADDRESS);
        MappingNode* mapNode2 = new MappingNode(NULL,NULL);

        mapNode2->set_key_type((ASTNodePtr)(element_key2));
        mapNode2->set_value_type((ASTNodePtr)(element_value2));

        VariableDeclarationNode* varDecl2 = new VariableDeclarationNode((ASTNodePtr)(mapNode2),CHECKHOLDERS);

        testNode->add_member((ASTNodePtr)(varDecl2));

        ElementaryTypeNameNode* ele = new ElementaryTypeNameNode(UINTEGER256);
        VariableDeclarationNode* varDecl3 = new VariableDeclarationNode((ASTNodePtr)(ele),TOTALHOLDERS);

        testNode->add_member((ASTNodePtr)(varDecl3));

        //    event TotalBalance(uint256 _supply, uint256 _balance);
        EventDefinitionNode* eventDefinitionNode = new EventDefinitionNode("TotalBalance");
        ParameterListNode* paramList = new ParameterListNode();

        string varDecl3Str = "_supply";
        string varDecl4Str = "_balance";
        ElementaryTypeNameNode* ele1 = new ElementaryTypeNameNode(UINTEGER256);
        ElementaryTypeNameNode* ele2 = new ElementaryTypeNameNode(UINTEGER256);

        VariableDeclarationNode* varDecl4 = new VariableDeclarationNode((ASTNodePtr)ele1,varDecl3Str);
        VariableDeclarationNode* varDecl5 = new VariableDeclarationNode((ASTNodePtr)ele2,varDecl4Str);

        paramList->add_parameter((ASTNodePtr)(varDecl4));
        paramList->add_parameter((ASTNodePtr)(varDecl5));
        eventDefinitionNode->set_argument_list((ParameterListNodePtr&)paramList);

        testNode->add_member((ASTNodePtr)eventDefinitionNode);

        // Creation of AddOwner
        createAddOwner(testNode);
        createCheckTotalAmount(testNode);

        // checkTotalAmount Function



       //testNode->add_member((ASTNodePtr)ifStatement);

        /**
        cout << "Num of member: " << num_members << "\n";
        for (int i = 0; i < num_members; i++)
        {
            member = testNode->get_member(i);

            checkNodeType(member,"ContractDefinitionNode's members");
        }
        **/
    }

    if (node->get_node_type() == NodeTypeFunctionDefinition)
    {
        if (count1 == 0)
            count1++;
        else
            return;

        FunctionDefinitionNode* fd = (FunctionDefinitionNode*)(node);

        std::string name = fd->get_name();

        if (name == ("transfer")) 
        {
            instrumentTransferFunction(fd);
        }
    }
}

void createAddOwner(ContractDefinitionNode* testNode) {
    FunctionDefinitionNode* fd = new FunctionDefinitionNode();

        ParameterListNode* pNodes = new ParameterListNode();
        ParameterListNode* returnNodes = new ParameterListNode();

        string _paramName1 = TO;
        ElementaryTypeNameNode* paramElement = new ElementaryTypeNameNode(ADDRESS);
        VariableDeclarationNode* param = new VariableDeclarationNode((ASTNodePtr)(paramElement),_paramName1);

        string _paramName2 = "success";
        ElementaryTypeNameNode* paramElement2 = new ElementaryTypeNameNode(BOOLEAN_CONS);
        VariableDeclarationNode* param2 = new VariableDeclarationNode((ASTNodePtr)(paramElement2),_paramName2);

        pNodes->add_parameter((ASTNodePtr)(param));
        returnNodes->add_parameter((ASTNodePtr)(param2)); 

        ParameterListNodePtr pNodePtr = (ParameterListNodePtr) pNodes;
        ParameterListNodePtr pReturnNodePtr = (ParameterListNodePtr) returnNodes;

        BlockNode* b = new BlockNode();
        fd->set_name("addOwner");
        fd->set_qualifier("public");

        BlockNodePtr pBlockPtr = (BlockNodePtr) b;
        fd->set_function_body(pBlockPtr);
        fd->set_returns(pReturnNodePtr);
        fd->set_params(pNodePtr);

        IndexAccessNode* iNode = new IndexAccessNode();
        IdentifierNode* identifierNode = new IdentifierNode(CHECKEXIST);
        IdentifierNode* paramBinOp = new IdentifierNode(TO);
        iNode->set_identifier((ASTNodePtr)identifierNode);
        iNode->set_index_value((ASTNodePtr)paramBinOp);
        
        LiteralNode* literalNode = new LiteralNode("1");
        string operation = "==";
        BinaryOperationNode* binNode = new BinaryOperationNode(operation,(ASTNodePtr)iNode,(ASTNodePtr)literalNode);

        LiteralNode* lNode = new LiteralNode(BOOLEAN_FALSE);
        ReturnNode* rNode = new ReturnNode();
        rNode->set_operand((ASTNodePtr)lNode);
        IfStatementNode* ifStatement = new IfStatementNode((ASTNodePtr)binNode,(ASTNodePtr)rNode,NULL);

        b->add_statement((ASTNodePtr)ifStatement);

        string operator2 = "=";
        LiteralNode* literalNode2 = new LiteralNode("1");
        IndexAccessNode* iNode2 = new IndexAccessNode();
        IdentifierNode* identifierNode2 = new IdentifierNode(CHECKEXIST);
        IdentifierNode* paramBinOp2 = new IdentifierNode(TO);
        iNode2->set_identifier((ASTNodePtr)identifierNode2);
        iNode2->set_index_value((ASTNodePtr)paramBinOp2);

        ExpressionStatementNode* exprNode1 = new ExpressionStatementNode();

        AssignmentNode* assign1 = new AssignmentNode(operator2);
        assign1->set_left_hand_operand((ASTNodePtr)iNode2);
        assign1->set_right_hand_operand((ASTNodePtr)literalNode2);
        exprNode1->set_expression((ASTNodePtr)assign1);

        //Indentation test(4,true,12);

        //assign1->source_code(test);
        b->add_statement((ASTNodePtr)exprNode1);

        IdentifierNode* elementNodeTo = new IdentifierNode(TO);
        IndexAccessNode* iNode3 = new IndexAccessNode();
        IdentifierNode* identifierNode3 = new IdentifierNode(CHECKHOLDERS);
        IdentifierNode* paramHolder = new IdentifierNode(TOTALHOLDERS);
        iNode3->set_identifier((ASTNodePtr)identifierNode3);
        iNode3->set_index_value((ASTNodePtr)paramHolder);

        ExpressionStatementNode* exprNode2 = new ExpressionStatementNode();

        AssignmentNode* assign2 = new AssignmentNode(operator2);
        assign2->set_left_hand_operand((ASTNodePtr)iNode3);
        assign2->set_right_hand_operand((ASTNodePtr)elementNodeTo);

        exprNode2->set_expression((ASTNodePtr)assign2);
        b->add_statement((ASTNodePtr)exprNode2);

        ExpressionStatementNode* exprNode3 = new ExpressionStatementNode();
        string operator3 = "++";
        IdentifierNode* unaryOperatorName = new IdentifierNode(TOTALHOLDERS);
        UnaryOperationNode* unaryOperation = new UnaryOperationNode(operator3,(ASTNodePtr)unaryOperatorName,false);
        exprNode3->set_expression((ASTNodePtr)unaryOperation);
        b->add_statement((ASTNodePtr)exprNode3);

        ReturnNode* rNode2 = new ReturnNode();
        LiteralNode* lNode2 = new LiteralNode(BOOLEAN_TRUE);
        rNode2->set_operand((ASTNodePtr)(lNode2));

        b->add_statement((ASTNodePtr&)(rNode2));

        testNode->add_member((ASTNodePtr)(fd));

}

void createCheckTotalAmount(ContractDefinitionNode* testNode){
        FunctionDefinitionNode* fd = new FunctionDefinitionNode();

        ParameterListNode* pNodes = new ParameterListNode();
        ParameterListNode* returnNodes = new ParameterListNode();

        string _paramName = "success";
        ElementaryTypeNameNode* paramElement = new ElementaryTypeNameNode(BOOLEAN_CONS);
        VariableDeclarationNode* param = new VariableDeclarationNode((ASTNodePtr)(paramElement),_paramName);

        returnNodes->add_parameter((ASTNodePtr)(param)); 

        ParameterListNodePtr pNodePtr = (ParameterListNodePtr) pNodes;
        ParameterListNodePtr pReturnNodePtr = (ParameterListNodePtr) returnNodes;

        BlockNode* b = new BlockNode();
        fd->set_name("checkTotalAmount");
        fd->set_qualifier("public");

        BlockNodePtr pBlockPtr = (BlockNodePtr) b;
        fd->set_function_body(pBlockPtr);
        fd->set_returns(pReturnNodePtr);
        fd->set_params(pNodePtr);
        testNode->add_member((ASTNodePtr)(fd));

        string op1 = "=";
        string sum = "sum";
        string zero = "0";
        string balance = "balance";
        
        VariableDeclarationStatementNode* assign1 = new VariableDeclarationStatementNode();
        VariableDeclarationNode* varDecl1 = new VariableDeclarationNode();
        ElementaryTypeNameNode* eleName1 = new ElementaryTypeNameNode(UINTEGER256);
        LiteralNode* lNode1 = new LiteralNode(zero);
        varDecl1->set_type((ASTNodePtr)eleName1);
        varDecl1->set_variable_name(sum);
        assign1->set_decl((VariableDeclarationNodePtr)varDecl1);
        assign1->set_value((ASTNodePtr)lNode1);
        b->add_statement((ASTNodePtr)assign1);

        VariableDeclarationStatementNode* assign2 = new VariableDeclarationStatementNode();
        VariableDeclarationNode* varDecl2 = new VariableDeclarationNode();
        ElementaryTypeNameNode* eleName2 = new ElementaryTypeNameNode(UINTEGER256);
        LiteralNode* lNode2 = new LiteralNode(zero);
        varDecl2->set_type((ASTNodePtr)eleName2);
        varDecl2->set_variable_name(balance);
        assign2->set_decl((VariableDeclarationNodePtr)varDecl2);
        assign2->set_value((ASTNodePtr)lNode2);
        b->add_statement((ASTNodePtr)assign2);

        ExpressionStatementNode* assign3 = new ExpressionStatementNode();
        VariableDeclarationNode* varDecl3 = new VariableDeclarationNode();
        ElementaryTypeNameNode* eleName3 = new ElementaryTypeNameNode(ADDRESS);
        LiteralNode* lNode3 = new LiteralNode(zero);
        varDecl3->set_type((ASTNodePtr)eleName3);
        varDecl3->set_variable_name("addr");
        assign3->set_expression((ASTNodePtr)varDecl3);
        b->add_statement((ASTNodePtr)assign3);

        // Initalization -> NodeTypeVarDeclStatement

        VariableDeclarationStatementNode* assign4 = new VariableDeclarationStatementNode();
        VariableDeclarationNode* varDecl4 = new VariableDeclarationNode();
        ElementaryTypeNameNode* eleName4 = new ElementaryTypeNameNode(UINTEGER256);
        LiteralNode* lNode4 = new LiteralNode(zero);
        varDecl4->set_type((ASTNodePtr)eleName4);
        varDecl4->set_variable_name("i");
        assign4->set_decl((VariableDeclarationNodePtr)varDecl4);
        assign4->set_value((ASTNodePtr)lNode4);

        // Condition -> NodeTypeBinaryOp
        string binOperator = "<";
        IdentifierNode* identifierNode1 = new IdentifierNode("i");
        IdentifierNode* identifierNode2 = new IdentifierNode(TOTALHOLDERS);
        BinaryOperationNode* binOp = new BinaryOperationNode(binOperator,(ASTNodePtr)(identifierNode1),(ASTNodePtr)(identifierNode2));

        ExpressionStatementNode* expr = new ExpressionStatementNode();
        IdentifierNode* identiferNode3 = new IdentifierNode("i");
        bool true_prefix = true;
        string unaryOperator = "++";
        UnaryOperationNode* unaryNode = new UnaryOperationNode(unaryOperator,(ASTNodePtr)(identiferNode3),true_prefix);
        expr->set_expression((ASTNodePtr)unaryNode);
        //VariableDeclarationNode
        //BinaryOperationNode bin = new 
        BlockNode* b2 = new BlockNode();

        ExpressionStatementNode* exprNode1 = new ExpressionStatementNode();

        AssignmentNode* assign5 = new AssignmentNode(op1);
        IdentifierNode* identifierNode3 = new IdentifierNode("addr");
        IndexAccessNode* iNode = new IndexAccessNode();
        IdentifierNode* identifierNode4 = new IdentifierNode(CHECKHOLDERS);
        IdentifierNode* elementParam = new IdentifierNode("i");
        iNode->set_identifier((ASTNodePtr)identifierNode4);
        iNode->set_index_value((ASTNodePtr)elementParam);
        assign5->set_left_hand_operand((ASTNodePtr)identifierNode3);
        assign5->set_right_hand_operand((ASTNodePtr)iNode);
        exprNode1->set_expression((ASTNodePtr)assign5);
        b2->add_statement((ASTNodePtr)exprNode1);

        ExpressionStatementNode* exprNode2 = new ExpressionStatementNode();
        AssignmentNode* assign6 = new AssignmentNode(op1);
        IdentifierNode* identifierNode5 = new IdentifierNode(balance);
        IndexAccessNode* iNode2 = new IndexAccessNode();
        IdentifierNode* identifierNode6 = new IdentifierNode("balances");
        ElementaryTypeNameNode* elementParam2 = new ElementaryTypeNameNode(ADDRESS);
        iNode2->set_identifier((ASTNodePtr)identifierNode6);
        iNode2->set_index_value((ASTNodePtr)elementParam2);
        assign6->set_left_hand_operand((ASTNodePtr)identifierNode5);
        assign6->set_right_hand_operand((ASTNodePtr)iNode2);
        exprNode2->set_expression((ASTNodePtr)assign6);
        b2->add_statement((ASTNodePtr)exprNode2);

        // require()
        ExpressionStatementNode* exprNode3 = new ExpressionStatementNode();
        IdentifierNode* requiref1 = new IdentifierNode("require");
        FunctionCallNode* f = new FunctionCallNode((ASTNodePtr)requiref1);
        string requireBinOp1 = "+";
        string requireBinOp2 = ">=";
        IdentifierNode* requireI1 = new IdentifierNode(sum);
        IdentifierNode* requireI2 = new IdentifierNode(balance);
        IdentifierNode* requireI3 = new IdentifierNode(sum);

        BinaryOperationNode* binOp1 = new BinaryOperationNode(requireBinOp1,(ASTNodePtr)requireI1,(ASTNodePtr)requireI2);
        BinaryOperationNode* binOp2 = new BinaryOperationNode(requireBinOp2,(ASTNodePtr)binOp1,(ASTNodePtr)requireI3);
        f->add_argument((ASTNodePtr)binOp2);
        exprNode3->set_expression((ASTNodePtr)f);

        ExpressionStatementNode* exprNode4 = new ExpressionStatementNode();
        string requireBinOp3 = "+=";
        IdentifierNode* identifierNode7 = new IdentifierNode(sum);
        IdentifierNode* identifierNode8 = new IdentifierNode(balance);
        BinaryOperationNode* binOp3 = new BinaryOperationNode(requireBinOp3,(ASTNodePtr)identifierNode7,(ASTNodePtr)identifierNode8);
        exprNode4->set_expression((ASTNodePtr)binOp3);

        b2->add_statement((ASTNodePtr)exprNode3);
        b2->add_statement((ASTNodePtr)exprNode4);

        IdentifierNode* identifierNode9 = new IdentifierNode("TotalBalance");
        FunctionCallNode* f2 = new FunctionCallNode((ASTNodePtr)identifierNode9);
        IdentifierNode* identifierNode10 = new IdentifierNode(sum);
        IdentifierNode* identifierNode11 = new IdentifierNode(balance);
        f2->add_argument((ASTNodePtr)identifierNode10);
        f2->add_argument((ASTNodePtr)identifierNode11);

        EmitStatementNode* e = new EmitStatementNode();
        e->set_event_call((ASTNodePtr)f2);

        b2->add_statement((ASTNodePtr)e);

        ExpressionStatementNode* exprNode5 = new ExpressionStatementNode();
        string requireBinOp4 = "==";

        IdentifierNode* identifierNode12 = new IdentifierNode(sum);
        IdentifierNode* identifierNode13 = new IdentifierNode("totalSupply");
        IdentifierNode* identifierNode14 = new IdentifierNode("require");

        BinaryOperationNode* binOp4 = new BinaryOperationNode(requireBinOp4,(ASTNodePtr)identifierNode12,(ASTNodePtr)identifierNode13);
        FunctionCallNode* f3 = new FunctionCallNode((ASTNodePtr)identifierNode14);
        f3->add_argument((ASTNodePtr)binOp4);
        exprNode5->set_expression((ASTNodePtr)f3);

        ReturnNode* returnNode = new ReturnNode();
        LiteralNode* lNode5 = new LiteralNode(BOOLEAN_TRUE);
        returnNode->set_operand((ASTNodePtr)lNode5);

        ForStatementNode* fStatement = new ForStatementNode((ASTNodePtr)assign4,(ASTNodePtr)binOp,(ASTNodePtr)unaryNode,(ASTNodePtr)b2);
        b->add_statement((ASTNodePtr)fStatement);
        b->add_statement((ASTNodePtr)exprNode5);
        b->add_statement((ASTNodePtr)returnNode);

}

void instrumentTransferFunction(FunctionDefinitionNode* fd) 
{
    BlockNodePtr blockNode = fd->get_function_body();

    vector<ASTNodePtr> v1;

    //Statement s;

    for (int i = 0; i < blockNode->num_statements(); i++) {
        
        v1.push_back((ASTNodePtr)blockNode->get_statement(i));
    }

    InlineAssemblyNode* inLineStatement1 = new InlineAssemblyNode();

    string source1 = "assembly {\n\t\tgasstop()\n\t}";
    inLineStatement1->set_source(source1);

    InlineAssemblyNode* inLineStatement2 = new InlineAssemblyNode();

    string source2 = "assembly {\n\t\tgasstart()\n\t}";
    inLineStatement2->set_source(source2);

    LiteralNode* lNode = new LiteralNode(ADDOWNER);
    LiteralNode* lNode1 = new LiteralNode(TO);

    ExpressionStatementNode* e1 = new ExpressionStatementNode();

    FunctionCallNode* f1 = new FunctionCallNode((ASTNodePtr)(lNode));
    
    f1->add_argument((ASTNodePtr)(lNode1));

    e1->set_expression((ASTNodePtr)(f1));

    string checkTotal = "checkTotalAmount";
    LiteralNode* lNode2 = new LiteralNode(checkTotal);

    ExpressionStatementNode* e2 = new ExpressionStatementNode();

    FunctionCallNode* f2 = new FunctionCallNode((ASTNodePtr)(lNode2));

    e2->set_expression((ASTNodePtr)(f2));

    v1.insert(v1.begin(),(ASTNodePtr)(inLineStatement1));
    v1.insert(v1.begin()+1,(ASTNodePtr)e1);
    v1.insert(v1.begin()+2,(ASTNodePtr)(e2));
    v1.insert(v1.begin()+3,(ASTNodePtr)(inLineStatement2));

    InlineAssemblyNode* inLineStatement3 = new InlineAssemblyNode();
    inLineStatement3->set_source(source1);
    InlineAssemblyNode* inLineStatement4 = new InlineAssemblyNode();
    inLineStatement4->set_source(source2);

    LiteralNode* lNode3 = new LiteralNode(checkTotal);

    ExpressionStatementNode* e3 = new ExpressionStatementNode();

    FunctionCallNode* f3 = new FunctionCallNode((ASTNodePtr)(lNode3));

    e3->set_expression((ASTNodePtr)(f3));

    v1.insert(v1.end()-1,(ASTNodePtr)(inLineStatement3));
    v1.insert(v1.end()-1,(ASTNodePtr)(e3));
    v1.insert(v1.end()-1,(ASTNodePtr)(inLineStatement4));



    BlockNode* newB = new BlockNode();

    for (int i = 0; i < v1.size(); i++) {
        newB->add_statement(v1.at(i));
    }

    fd->set_function_body((BlockNodePtr)newB); 


}

void after() {
    return;
}

}