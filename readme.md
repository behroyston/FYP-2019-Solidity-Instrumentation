These are the test scripts used throughout the FYP after experimentation with TruffleSuite and Solidity Instrumentation Framework(SIF)

The SIF which I utilise is credited to https://github.com/chao-peng/SIF

I prepared three separate ASTVisitor.cpp and ASTVisitor.hpp codes to test.

ERC20 - contains the script to instrument inline assembly code - gasstart and gasstop as well as the required checking UDF.
UET and BecToken - contains the script required to instrument and expose the vulnerabilities in both of these code
To Run the SIF instrumentation:
1) Replace the ASTVisitor.hpp and the ASTVisitor.cpp code
2) Run SIF -> sif -a contract_name.ast -j contract_name.json -o generated_contract.s


For the TruffleTests, they contain the tests to validate that tokens are incorrectly transferred due to integer overflow/underflow. These are mainly due to failed validation and lack of use of SafeMath which can ensure correct integer multiplication.

Lastly, using the modified Solc and EVM developed by https://github.com/franklinliu/cslprojects:
I ran a few checks using the scripts in the Modified_EVM_tests, they are similar to each other - the only difference is in the ABI and the contract address.
By compiling the contract using solc --bin --abi [contract], we can obtain the ABI.
The contract address can be obtained through running using the binary and the ABI inside the Go-EVM. 