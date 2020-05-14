#include "ASTVisitor.hpp"
#include <vector>
#include <string>

namespace Sif {

// counter to prevent duplicate instructions
int count = 0;
int count1 = 0;
int count2 = 0;

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
        // Initalize the rest of the global variables
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

        // create the event TotalBalance(uint256 _supply, uint256 _balance) node 
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

        // creation of our own user defined invariant functions
        createAddOwner(testNode);
        createCheckTotalAmount(testNode);
    }

    if (node->get_node_type() == NodeTypeFunctionDefinition)
    {
        FunctionDefinitionNode* fd = (FunctionDefinitionNode*)(node);

        std::string name = fd->get_name();

        if (name.compare("transferFrom") == 0) 
        {
    		if (count1 == 0)
		    	count1++;
    		else
    			return;
            //cout << "called " << "\n";

		    instrumentUETTransferFromFunction(fd);
        }

        if (name.compare("constructor") == 0) 
        {
            if (count2 == 0)
                count2++;
            else
                return;
            addInitialVariablesToConstructor(fd);
        } 
    }
}

void createAddOwner(ContractDefinitionNode* testNode) {
    FunctionDefinitionNode* fd = new FunctionDefinitionNode();

    // creation of the function call line
    // function addOwner(address _to) public returns (bool success)
    ParameterListNode* pNodes = new ParameterListNode();
    ParameterListNode* returnNodes = new ParameterListNode();
    fd->set_name("addOwner");
    fd->set_qualifier("public");

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

    BlockNodePtr pBlockPtr = (BlockNodePtr) b;
    fd->set_function_body(pBlockPtr);
    fd->set_returns(pReturnNodePtr);
    fd->set_params(pNodePtr);

    // checkExist[_to] == 1
    IndexAccessNode* iNode = new IndexAccessNode();
    IdentifierNode* identifierNode = new IdentifierNode(CHECKEXIST);
    IdentifierNode* paramBinOp = new IdentifierNode(TO);
    iNode->set_identifier((ASTNodePtr)identifierNode);
    iNode->set_index_value((ASTNodePtr)paramBinOp);
    
    LiteralNode* literalNode = new LiteralNode("1");
    string operation = "==";
    BinaryOperationNode* binNode = new BinaryOperationNode(operation,(ASTNodePtr)iNode,(ASTNodePtr)literalNode);

    // return false
    LiteralNode* lNode = new LiteralNode(BOOLEAN_FALSE);
    ReturnNode* rNode = new ReturnNode();
    rNode->set_operand((ASTNodePtr)lNode);

    // if (checkExist[_to] == 1)
    //      return false;
    IfStatementNode* ifStatement = new IfStatementNode((ASTNodePtr)binNode,(ASTNodePtr)rNode,NULL);

    b->add_statement((ASTNodePtr)ifStatement);

    // checkExist[_to] = 1
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

    //assign1->source_code(test);
    b->add_statement((ASTNodePtr)exprNode1);

    // checkHolders[_totalHolders] = _to
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

    // _totalHolders++;
    ExpressionStatementNode* exprNode3 = new ExpressionStatementNode();
    string operator3 = "++";
    IdentifierNode* unaryOperatorName = new IdentifierNode(TOTALHOLDERS);
    UnaryOperationNode* unaryOperation = new UnaryOperationNode(operator3,(ASTNodePtr)unaryOperatorName,false);
    exprNode3->set_expression((ASTNodePtr)unaryOperation);
    b->add_statement((ASTNodePtr)exprNode3);

    // return true;
    ReturnNode* rNode2 = new ReturnNode();
    LiteralNode* lNode2 = new LiteralNode(BOOLEAN_TRUE);
    rNode2->set_operand((ASTNodePtr)(lNode2));

    b->add_statement((ASTNodePtr&)(rNode2));

    testNode->add_member((ASTNodePtr)(fd));
}

void createCheckTotalAmount(ContractDefinitionNode* testNode){
    FunctionDefinitionNode* fd = new FunctionDefinitionNode();

    // creation of the function call line
    // function checkTotalAmount() public returns (bool success)
    ParameterListNode* pNodes = new ParameterListNode();
    ParameterListNode* returnNodes = new ParameterListNode();
    fd->set_name("checkTotalAmount");
    fd->set_qualifier("public");

    // bool variable's name -> success
    string _paramName = "success";
    ElementaryTypeNameNode* paramElement = new ElementaryTypeNameNode(BOOLEAN_CONS);
    VariableDeclarationNode* param = new VariableDeclarationNode((ASTNodePtr)(paramElement),_paramName);

    returnNodes->add_parameter((ASTNodePtr)(param)); 

    ParameterListNodePtr pNodePtr = (ParameterListNodePtr) pNodes;
    ParameterListNodePtr pReturnNodePtr = (ParameterListNodePtr) returnNodes;

    // creation of what is inside our function
    BlockNode* b = new BlockNode();
    BlockNodePtr pBlockPtr = (BlockNodePtr) b;
    fd->set_function_body(pBlockPtr);
    fd->set_returns(pReturnNodePtr);
    fd->set_params(pNodePtr);
    testNode->add_member((ASTNodePtr)(fd));

    string op1 = "=";
    string sum = "sum";
    string zero = "0";
    string balance = "balance";
    
    // uint256 sum = 0
    VariableDeclarationStatementNode* assign1 = new VariableDeclarationStatementNode();
    VariableDeclarationNode* varDecl1 = new VariableDeclarationNode();
    ElementaryTypeNameNode* eleName1 = new ElementaryTypeNameNode(UINTEGER256);
    LiteralNode* lNode1 = new LiteralNode(zero);
    varDecl1->set_type((ASTNodePtr)eleName1);
    varDecl1->set_variable_name(sum);
    assign1->set_decl((VariableDeclarationNodePtr)varDecl1);
    assign1->set_value((ASTNodePtr)lNode1);
    b->add_statement((ASTNodePtr)assign1);

    // uint256 balance = 0
    VariableDeclarationStatementNode* assign2 = new VariableDeclarationStatementNode();
    VariableDeclarationNode* varDecl2 = new VariableDeclarationNode();
    ElementaryTypeNameNode* eleName2 = new ElementaryTypeNameNode(UINTEGER256);
    LiteralNode* lNode2 = new LiteralNode(zero);
    varDecl2->set_type((ASTNodePtr)eleName2);
    varDecl2->set_variable_name(balance);
    assign2->set_decl((VariableDeclarationNodePtr)varDecl2);
    assign2->set_value((ASTNodePtr)lNode2);
    b->add_statement((ASTNodePtr)assign2);

    // address addr;
    ExpressionStatementNode* assign3 = new ExpressionStatementNode();
    VariableDeclarationNode* varDecl3 = new VariableDeclarationNode();
    ElementaryTypeNameNode* eleName3 = new ElementaryTypeNameNode(ADDRESS);
    LiteralNode* lNode3 = new LiteralNode(zero);
    varDecl3->set_type((ASTNodePtr)eleName3);
    varDecl3->set_variable_name("addr");
    assign3->set_expression((ASTNodePtr)varDecl3);
    b->add_statement((ASTNodePtr)assign3);

    // creation of the first line for our for loop: for (uint256 i = 0; i < _totalHolders; i++)
    // uint256 i = 0
    VariableDeclarationStatementNode* assign4 = new VariableDeclarationStatementNode();
    VariableDeclarationNode* varDecl4 = new VariableDeclarationNode();
    ElementaryTypeNameNode* eleName4 = new ElementaryTypeNameNode(UINTEGER256);
    LiteralNode* lNode4 = new LiteralNode(zero);
    varDecl4->set_type((ASTNodePtr)eleName4);
    varDecl4->set_variable_name("i");
    assign4->set_decl((VariableDeclarationNodePtr)varDecl4);
    assign4->set_value((ASTNodePtr)lNode4);

    // Condition -> NodeTypeBinaryOp 
    // i < _totalHolders
    string binOperator = "<";
    IdentifierNode* identifierNode1 = new IdentifierNode("i");
    IdentifierNode* identifierNode2 = new IdentifierNode(TOTALHOLDERS);
    BinaryOperationNode* binOp = new BinaryOperationNode(binOperator,(ASTNodePtr)(identifierNode1),(ASTNodePtr)(identifierNode2));

    // i++
    ExpressionStatementNode* expr = new ExpressionStatementNode();
    IdentifierNode* identiferNode3 = new IdentifierNode("i");
    bool true_prefix = true;
    string unaryOperator = "++";
    UnaryOperationNode* unaryNode = new UnaryOperationNode(unaryOperator,(ASTNodePtr)(identiferNode3),true_prefix);
    expr->set_expression((ASTNodePtr)unaryNode);

    // creation of what is inside the inner block of our for loop
    BlockNode* b2 = new BlockNode();
    ExpressionStatementNode* exprNode1 = new ExpressionStatementNode();

    // addr = checkHolders[i]
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

    // balance = balances[addr]
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

    // require(sum + balance >= sum)
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
    b2->add_statement((ASTNodePtr)exprNode3);

    // sum += balance
    ExpressionStatementNode* exprNode4 = new ExpressionStatementNode();
    string requireBinOp3 = "+=";
    IdentifierNode* identifierNode7 = new IdentifierNode(sum);
    IdentifierNode* identifierNode8 = new IdentifierNode(balance);
    BinaryOperationNode* binOp3 = new BinaryOperationNode(requireBinOp3,(ASTNodePtr)identifierNode7,(ASTNodePtr)identifierNode8);
    exprNode4->set_expression((ASTNodePtr)binOp3);
    b2->add_statement((ASTNodePtr)exprNode4);

    // emit TotalBalance(sum,balance)
    IdentifierNode* identifierNode9 = new IdentifierNode("TotalBalance");
    FunctionCallNode* f2 = new FunctionCallNode((ASTNodePtr)identifierNode9);
    IdentifierNode* identifierNode10 = new IdentifierNode(sum);
    IdentifierNode* identifierNode11 = new IdentifierNode(balance);
    f2->add_argument((ASTNodePtr)identifierNode10);
    f2->add_argument((ASTNodePtr)identifierNode11);
    EmitStatementNode* e = new EmitStatementNode();
    e->set_event_call((ASTNodePtr)f2);
    b2->add_statement((ASTNodePtr)e);

    // require(sum == totalSupply)
    ExpressionStatementNode* exprNode5 = new ExpressionStatementNode();
    string requireBinOp4 = "==";
    IdentifierNode* identifierNode12 = new IdentifierNode(sum);
    IdentifierNode* identifierNode13 = new IdentifierNode("totalSupply");
    IdentifierNode* identifierNode14 = new IdentifierNode("require");
    BinaryOperationNode* binOp4 = new BinaryOperationNode(requireBinOp4,(ASTNodePtr)identifierNode12,(ASTNodePtr)identifierNode13);
    FunctionCallNode* f3 = new FunctionCallNode((ASTNodePtr)identifierNode14);
    f3->add_argument((ASTNodePtr)binOp4);
    exprNode5->set_expression((ASTNodePtr)f3);

    // return true;
    ReturnNode* returnNode = new ReturnNode();
    LiteralNode* lNode5 = new LiteralNode(BOOLEAN_TRUE);
    returnNode->set_operand((ASTNodePtr)lNode5);

    ForStatementNode* fStatement = new ForStatementNode((ASTNodePtr)assign4,(ASTNodePtr)binOp,(ASTNodePtr)unaryNode,(ASTNodePtr)b2);
    b->add_statement((ASTNodePtr)fStatement);
    b->add_statement((ASTNodePtr)exprNode5);
    b->add_statement((ASTNodePtr)returnNode);

}

void instrumentUETTransferFromFunction(FunctionDefinitionNode* fd)
{
    BlockNodePtr blockNode = fd->get_function_body();
    vector<ASTNodePtr> v1;

    ASTNodePtr ifNode;

    //Statement s;
    for (int i = 0; i < blockNode->num_statements(); i++) {
        
        ASTNodePtr stmt = (ASTNodePtr)blockNode->get_statement(i);
        v1.push_back(stmt);
    }

    // addOwner(_from)
    LiteralNode* lNode1 = new LiteralNode(ADDOWNER);
    string from = "_from";
    LiteralNode* lNode2 = new LiteralNode(from);
    ExpressionStatementNode* e1 = new ExpressionStatementNode();
    FunctionCallNode* f1 = new FunctionCallNode((ASTNodePtr)(lNode1));
    f1->add_argument((ASTNodePtr)(lNode2));
    e1->set_expression((ASTNodePtr)(f1));

    // addOwner(_to)
    LiteralNode* lNode3 = new LiteralNode(ADDOWNER);
    string to = "_to";
    LiteralNode* lNode4 = new LiteralNode(to);
    ExpressionStatementNode* e2 = new ExpressionStatementNode();
    FunctionCallNode* f2 = new FunctionCallNode((ASTNodePtr)(lNode3));
    f2->add_argument((ASTNodePtr)(lNode4));
    e2->set_expression((ASTNodePtr)(f2));

    // checkTotalAmount() in the first line of the code
    string checkTotal = "checkTotalAmount";
    LiteralNode* lNode5 = new LiteralNode(checkTotal);
    ExpressionStatementNode* e3 = new ExpressionStatementNode();
    FunctionCallNode* f3 = new FunctionCallNode((ASTNodePtr)(lNode5));
    e3->set_expression((ASTNodePtr)(f3));

    v1.insert(v1.begin(),(ASTNodePtr)(e3));
    v1.insert(v1.begin(),(ASTNodePtr)(e2));
    v1.insert(v1.begin(),(ASTNodePtr)(e1));

    // get the last ifStatement and add in our custom instruction
    shared_ptr<IfStatementNode> ifStatement = std::dynamic_pointer_cast<IfStatementNode>(v1.back());
    ASTNodePtr a2 = (ASTNodePtr)(ifStatement->get_then());
    shared_ptr<BlockNode> b2 = std::dynamic_pointer_cast<BlockNode>(a2);

    // adding our own custom instruction into the for block loop
    vector<ASTNodePtr> v2;
    for (int i = 0; i < b2->num_statements(); i++) {
        ASTNodePtr stmt = (ASTNodePtr)(b2->get_statement(i));
        v2.push_back(stmt);
    }

    LiteralNode* lNode6 = new LiteralNode(checkTotal);
    ExpressionStatementNode* e4 = new ExpressionStatementNode();
    FunctionCallNode* f4 = new FunctionCallNode((ASTNodePtr)(lNode6));
    e4->set_expression((ASTNodePtr)(f4));

    v2.insert(v2.end()-2,(ASTNodePtr)(e4));

    BlockNode* newB1 = new BlockNode();

    for (int i = 0; i < v2.size(); i++) {
        newB1->add_statement(v2.at(i));
    }

    ifStatement->set_then((ASTNodePtr)newB1);

    // add in our addOwner instruction for the for loop

    BlockNode* newB = new BlockNode();

    for (int i = 0; i < v1.size(); i++) {
        newB->add_statement(v1.at(i));
    }

    // now we replace the for statement with our modified one
    int no_of_statements = newB->num_statements();

    fd->set_function_body((BlockNodePtr)newB); 
}


void addInitialVariablesToConstructor(FunctionDefinitionNode* fd) 
{
    //checkExist[msg.sender] = 1;
    string operator1 = "=";
    LiteralNode* literalNode1 = new LiteralNode("1");
    IndexAccessNode* iNode1 = new IndexAccessNode();
    IdentifierNode* identifierNode1 = new IdentifierNode(CHECKEXIST);
    string msg_sender = "msg.sender";
    IdentifierNode* paramBinOp1 = new IdentifierNode(msg_sender);
    iNode1->set_identifier((ASTNodePtr)identifierNode1);
    iNode1->set_index_value((ASTNodePtr)paramBinOp1);

    ExpressionStatementNode* exprNode1 = new ExpressionStatementNode();

    AssignmentNode* assign1 = new AssignmentNode(operator1);
    assign1->set_left_hand_operand((ASTNodePtr)iNode1);
    assign1->set_right_hand_operand((ASTNodePtr)literalNode1);
    exprNode1->set_expression((ASTNodePtr)assign1);
    
    //checkHolders[0] = msg.sender;
    IndexAccessNode* iNode2 = new IndexAccessNode();
    IdentifierNode* identifierNode2 = new IdentifierNode(CHECKHOLDERS);
    IdentifierNode* literalNode2 = new IdentifierNode(msg_sender);
    IdentifierNode* paramBinOp2 = new IdentifierNode("0");
    iNode2->set_identifier((ASTNodePtr)identifierNode2);
    iNode2->set_index_value((ASTNodePtr)paramBinOp2);

    ExpressionStatementNode* exprNode2 = new ExpressionStatementNode();

    AssignmentNode* assign2 = new AssignmentNode(operator1);
    assign2->set_left_hand_operand((ASTNodePtr)iNode2);
    assign2->set_right_hand_operand((ASTNodePtr)literalNode2);
    exprNode2->set_expression((ASTNodePtr)assign2);
    
    //_totalHolders++;
    ExpressionStatementNode* exprNode3 = new ExpressionStatementNode();
    IdentifierNode* identiferNode3 = new IdentifierNode(TOTALHOLDERS);
    bool false_prefix = false;
    string unaryOperator = "++";
    UnaryOperationNode* unaryNode = new UnaryOperationNode(unaryOperator,(ASTNodePtr)(identiferNode3),false_prefix);
    exprNode3->set_expression((ASTNodePtr)unaryNode);
    
    //totalSupply = 2**250;
    IdentifierNode* identifierNode4 = new IdentifierNode("totalSupply");
    LiteralNode* literalNode3 = new LiteralNode("2**255-1");

    ExpressionStatementNode* exprNode4 = new ExpressionStatementNode();

    AssignmentNode* assign3 = new AssignmentNode(operator1);
    assign3->set_left_hand_operand((ASTNodePtr)identifierNode4);
    assign3->set_right_hand_operand((ASTNodePtr)literalNode3);
    exprNode4->set_expression((ASTNodePtr)assign3);

    BlockNodePtr b = fd->get_function_body();
    
    b->add_statement((ASTNodePtr)exprNode1);
    b->add_statement((ASTNodePtr)exprNode2);
    b->add_statement((ASTNodePtr)exprNode3);
    b->add_statement((ASTNodePtr)exprNode4);
}   

void after() {
    return;
}

}
