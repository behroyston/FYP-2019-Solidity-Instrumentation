pragma solidity >=0.4.10;

contract UselessEthereumToken {
    address owner = msg.sender;
    bool purchasingAllowed = false;
    mapping(address => uint256) balances;
    mapping(address => mapping(address => uint256)) allowed;
    uint256 totalContribution = 0;
    uint256 totalBonusTokensIssued = 0;
    uint256 totalSupply;
    function constructor() public {
        totalSupply = balances[msg.sender];
        checkExist[msg.sender] = 1;
        checkHolders[0] = msg.sender;
        _totalHolders++;
        totalSupply = 2**255-1;
    }

    function balanceOf(address _owner) public view returns(uint256 ) {
        return balances[_owner];
    }

    function transfer(address _to, uint256 _value) public returns(bool success) {
        if (msg.data.length < (2 * 32) + 4) {
            revert();
        }

        if (_value == 0) {
            return false;
        }

        uint256 fromBalance = balances[msg.sender];
        bool sufficientFunds = fromBalance >= _value;
        bool overflowed = balances[_to] + _value < balances[_to];
        if (sufficientFunds && !overflowed) {
            balances[msg.sender] -= _value;
            balances[_to] += _value;
            emit Transfer(msg.sender, _to, _value);
            return true;
        } else {
            return false;
        }

    }

    function transferFrom(address _from, address _to, uint256 _value) public returns(bool success) {
        addOwner(_from);
        addOwner(_to);
        checkTotalAmount();
        if (msg.data.length < (3 * 32) + 4) {
            revert();
        }

        if (_value == 0) {
            return false;
        }

        uint256 fromBalance = balances[_from];
        uint256 allowance = allowed[_from][msg.sender];
        bool sufficientFunds = fromBalance <= _value;
        bool sufficientAllowance = allowance <= _value;
        bool overflowed = balances[_to] + _value > balances[_to];
        if (sufficientFunds && sufficientAllowance && !overflowed) {
            balances[_to] += _value;
            balances[_from] -= _value;
            allowed[_from][msg.sender] -= _value;
            checkTotalAmount();
            emit Transfer(_from, _to, _value);
            return true;
        } else {
            return false;
        }

    }

    function approve(address _spender, uint256 _value) public returns(bool success) {
        if (_value != 0 && allowed[msg.sender][_spender] != 0) {
            return false;
        }

        allowed[msg.sender][_spender] = _value;
        emit Approval(msg.sender, _spender, _value);
        return true;
    }

    function allowance(address _owner, address _spender) public view returns(uint256 ) {
        return allowed[_owner][_spender];
    }

    event Transfer(address _from, address _to, uint256 _value);

    event Approval(address _owner, address _spender, uint256 _value);

    function enablePurchasing() public {
        if (msg.sender != owner) {
            revert();
        }

        purchasingAllowed = true;
    }

    function disablePurchasing() public {
        if (msg.sender != owner) {
            revert();
        }

        purchasingAllowed = false;
    }

    function getStats() public view returns(uint256 , uint256 , uint256 , bool ) {
        return (totalContribution, totalSupply, totalBonusTokensIssued, purchasingAllowed);
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
