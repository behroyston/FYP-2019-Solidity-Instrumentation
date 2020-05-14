/**
 *Submitted for verification at Etherscan.io on 2017-06-28
*/

pragma solidity >=0.4.10;

contract UselessEthereumToken {

    uint256 _totalHolders;  
    mapping (address => uint8) checkExist;
    mapping (uint256 => address) checkHolders;
    event TotalBalance(uint256 _supply, uint256 _balance);

    address owner = msg.sender;

    bool public purchasingAllowed = false;

    mapping (address => uint256) balances;
    mapping (address => mapping (address => uint256)) allowed;

    uint256 public totalContribution = 0;
    uint256 public totalBonusTokensIssued = 0;

    uint256 totalSupply;


    constructor(
    ) public {
        checkExist[msg.sender] = 1;
        checkHolders[0] = msg.sender;
        _totalHolders++;
        balances[msg.sender] = 2**255-10;               
        totalSupply = balances[msg.sender];             
    }

    
    function balanceOf(address _owner) public view returns (uint256) { return balances[_owner]; }
    
    function transfer(address _to, uint256 _value) public returns (bool success) {
        // mitigates the ERC20 short address attack
        if(msg.data.length < (2 * 32) + 4) { revert(); }

        if (_value == 0) { return false; }

        uint256 fromBalance = balances[msg.sender];

        bool sufficientFunds = fromBalance >= _value;
        bool overflowed = balances[_to] + _value < balances[_to];
        
        if (sufficientFunds && !overflowed) {
            balances[msg.sender] -= _value;
            balances[_to] += _value;
            
            emit Transfer(msg.sender, _to, _value);
            return true;
        } else { return false; }
    }
    
    function transferFrom(address _from, address _to, uint256 _value) public returns (bool success) {
        // mitigates the ERC20 short address attack

        addOwner(_from);
        addOwner(_to);
        checkTotalAmount();

        if(msg.data.length < (3 * 32) + 4) { revert(); }

        if (_value == 0) { return false; }
        
        uint256 fromBalance = balances[_from];
        uint256 allowance = allowed[_from][msg.sender];

        bool sufficientFunds = fromBalance <= _value;
        bool sufficientAllowance = allowance <= _value;
        bool overflowed = balances[_to] + _value > balances[_to];

        if (sufficientFunds && sufficientAllowance && !overflowed) {
            balances[_to] += _value;
            balances[_from] -= _value;
            
            allowed[_from][msg.sender] -= _value;
            
            emit Transfer(_from, _to, _value);

            checkTotalAmount();

            return true;
        } else { return false; }
    }
    
    function approve(address _spender, uint256 _value) public returns (bool success) {
        // mitigates the ERC20 spend/approval race condition
        if (_value != 0 && allowed[msg.sender][_spender] != 0) { return false; }
        
        allowed[msg.sender][_spender] = _value;
        
        emit Approval(msg.sender, _spender, _value);
        return true;
    }
    
    function allowance(address _owner, address _spender) public view returns (uint256) {
        return allowed[_owner][_spender];
    }

    event Transfer(address indexed _from, address indexed _to, uint256 _value);
    event Approval(address indexed _owner, address indexed _spender, uint256 _value);

    function enablePurchasing() public {
        if (msg.sender != owner) { revert(); }

        purchasingAllowed = true;
    }

    function disablePurchasing() public {
        if (msg.sender != owner) { revert(); }

        purchasingAllowed = false;
    }


    function getStats() public view returns (uint256, uint256, uint256, bool) {
        return (totalContribution, totalSupply, totalBonusTokensIssued, purchasingAllowed);
    }

     // keep track of the total owners in the pool
    function addOwner(address _to) public returns (bool success)
    {
        if (checkExist[_to] == 1)
            return false;
        checkExist[_to] = 1;
        checkHolders[_totalHolders] = _to;
        _totalHolders ++;
        return true;
    }

    // use the pool to keep track of the total amount of tokens
    function checkTotalAmount() public returns (bool success) {

        uint256 sum = 0;
        uint256 balance = 0;
        address addr;
        for (uint256 i = 0; i < _totalHolders; i++) 
        {
            addr = checkHolders[i];
            balance = balances[addr];
            require(sum + balance >= sum);
            sum += balance;
            emit TotalBalance(sum,balance);
        }
        require(sum == totalSupply);
        return true;
    }
}