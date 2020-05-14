/*
Implements EIP20 token standard: https://github.com/ethereum/EIPs/blob/master/EIPS/eip-20.md
.*/


pragma solidity ^0.4.21;

contract EIP20 {

    // total amount of tokens
    uint256 public totalSupply;

    // solhint-disable-next-line no-simple-event-func-name
    uint256 constant private MAX_UINT256 = 2**256 - 1;
    mapping (address => uint256) public balances;
    mapping (address => mapping (address => uint256)) public allowed;

    uint256 _totalHolders;
    mapping (address => uint8) public checkExist;
    mapping (uint256 => address) public checkHolders;
    // solhint-disable-next-line no-simple-event-func-name
    event Transfer(address indexed _from, address indexed _to, uint256 _value);
    event Approval(address indexed _owner, address indexed _spender, uint256 _value);
    /*
    NOTE:
    The following variables are OPTIONAL vanities. One does not have to include them.
    They allow one to customise the token contract & in no way influences the core functionality.
    Some wallets/interfaces might not even bother to look at this information.
    */
    string public name;                   //fancy name: eg Simon Bucks
    uint8 public decimals;                //How many decimals to show.
    string public symbol;                 //An identifier: eg SBX

    function EIP20(
        uint256 _initialAmount,
        string _tokenName,
        uint8 _decimalUnits,
        string _tokenSymbol
    ) public {
        balances[msg.sender] = _initialAmount;               // Give the creator all initial tokens
        totalSupply = _initialAmount;                        // Update total supply
        name = _tokenName;                                   // Set the name for display purposes
        decimals = _decimalUnits;                            // Amount of decimals for display purposes
        symbol = _tokenSymbol;                               // Set the symbol for display purposes
    }

    function transfer(address _to, uint256 _value) public returns (bool success) 
    {
        
        addOwner(_to);
        checkTotalAmount();

        require(balances[msg.sender] >= _value);
        balances[msg.sender] -= _value;
        balances[_to] += _value;

        emit Transfer(msg.sender, _to, _value); //solhint-disable-line indent, no-unused-vars
        return true;
    }

    // keep track of the total owners in the pool
    function addOwner(address _to) public returns (bool success)
    {
        if (checkExist[_to] == 1)
            return false;
        checkExist[_to] = 1;
        checkHolders[_totalHolders] = _to;
        _totalHolders++;
        return true;
    }

    // use the pool to keep track of the total amount of tokens
    function checkTotalAmount() public returns (bool success)
    {

        uint256 sum = 0;
        uint256 balance = 0;
        address addr = 0;
        for (uint256 i = 0; i < _totalHolders; i++) 
        {
            addr = checkHolders[i];
            balance = balances[addr];
            require(sum + balance >= sum);
            sum += balance;
        }
        require(sum == totalSupply);
    }

    function transferFrom(address _from, address _to, uint256 _value) public returns (bool success) {
        uint256 allowance = allowed[_from][msg.sender];
        //require(balances[_from] >= _value && allowance >= _value);
        balances[_to] += _value;
        balances[_from] -= _value;
        if (allowance < MAX_UINT256) {
            allowed[_from][msg.sender] -= _value;
        }
        //emit Transfer(_from, _to, _value); //solhint-disable-line indent, no-unused-vars
        return true;
    }

    function balanceOf(address _owner) public view returns (uint256 balance) {
        return balances[_owner];
    }

    function approve(address _spender, uint256 _value) public returns (bool success) {
        allowed[msg.sender][_spender] = _value;
        //emit Approval(msg.sender, _spender, _value); //solhint-disable-line indent, no-unused-vars
        return true;
    }

    function allowance(address _owner, address _spender) public view returns (uint256 remaining) {
        return allowed[_owner][_spender];
    }
}
