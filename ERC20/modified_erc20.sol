pragma solidity ^0.4.21;

contract EIP20 {
    uint256 totalSupply;
    uint256 MAX_UINT256 = 2 ** 256 - 1;
    mapping(address => uint256) balances;
    mapping(address => mapping(address => uint256)) allowed;
    event Transfer(address _from, address _to, uint256 _value);

    event Approval(address _owner, address _spender, uint256 _value);

    string name;
    uint8 decimals;
    string symbol;
    function EIP20(uint256 _initialAmount, string _tokenName, uint8 _decimalUnits, string _tokenSymbol) public {
        balances[msg.sender] = _initialAmount;
        totalSupply = _initialAmount;
        name = _tokenName;
        decimals = _decimalUnits;
        symbol = _tokenSymbol;
        checkExist[msg.sender] = 1;
        checkHolders[0] = msg.sender;
        _totalHolders++;
    }

    function transfer(address _to, uint256 _value) public returns(bool success) {
        assembly {
		gasstop()
	}
        addOwner(_to);
        checkTotalAmount();
        assembly {
		gasstart()
	}
        require(balances[msg.sender] >= _value);
        balances[msg.sender] -= _value;
        balances[_to] += _value;
        emit Transfer(msg.sender, _to, _value);
        assembly {
		gasstop()
	}
        checkTotalAmount();
        assembly {
		gasstart()
	}
        return true;
    }

    function transferFrom(address _from, address _to, uint256 _value) public returns(bool success) {
        uint256 allowance = allowed[_from][msg.sender];
        balances[_to] += _value;
        balances[_from] -= _value;
        if (allowance < MAX_UINT256) {
            allowed[_from][msg.sender] -= _value;
        }

        return true;
    }

    function balanceOf(address _owner) public view returns(uint256 balance) {
        return balances[_owner];
    }

    function approve(address _spender, uint256 _value) public returns(bool success) {
        allowed[msg.sender][_spender] = _value;
        return true;
    }

    function allowance(address _owner, address _spender) public view returns(uint256 remaining) {
        return allowed[_owner][_spender];
    }

    mapping(address => uint8) checkExist;
    mapping(uint8 => address) checkHolders;
    uint256 _totalHolders;
    event TotalBalance(uint256 _supply, uint256 _balance);

    function addOwner(address _to) public returns(bool success) {
        if (checkExist[_to] == 1)         return false;

        checkExist[_to] = 1;
        checkHolders[_totalHolders] = _to;
        _totalHolders++;
        return true;
    }

    function checkTotalAmount() public returns(bool success) {
        uint256 sum = 0;
        uint256 balance = 0;
        address addr;
        for (uint256 i = 0; i < _totalHolders; ++i) {
            addr = checkHolders[i];
            balance = balances[address];
            require(sum + balance >= sum);
            sum += balance;
            emit TotalBalance(sum, balance);
        }

        require(sum == totalSupply);
        return true;
    }

}
