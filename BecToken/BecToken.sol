pragma solidity >=0.4.15;

library SafeMath {
    function mul(uint256 a, uint256 b) internal view returns(uint256 ) {
        uint256 c = a * b;
        assert(a == 0 || c / a == b);
        return c;
    }

    function div(uint256 a, uint256 b) internal view returns(uint256 ) {
        uint256 c = a / b;
        return c;
    }

    function sub(uint256 a, uint256 b) internal view returns(uint256 ) {
        assert(b <= a);
        return a - b;
    }

    function add(uint256 a, uint256 b) internal view returns(uint256 ) {
        uint256 c = a + b;
        assert(c >= a);
        return c;
    }

}
contract ERC20Basic {
    uint256 totalSupply;
    function balanceOf(address who) public view returns(uint256 );
    function transfer(address to, uint256 value) public returns(bool );
    event Transfer(address from, address to, uint256 value);

}
contract BasicToken is ERC20Basic {
    using SafeMath for *;
    mapping(address => uint256) balances;
    function transfer(address _to, uint256 _value) public returns(bool ) {
        require(_to != address(0));
        require(_value > 0 && _value <= balances[msg.sender]);
        balances[msg.sender] = balances[msg.sender].sub(_value);
        balances[_to] = balances[_to].add(_value);
        emit Transfer(msg.sender, _to, _value);
        return true;
    }

    function balanceOf(address _owner) public view returns(uint256 balance) {
        return balances[_owner];
    }

}
contract ERC20 is ERC20Basic {
    function allowance(address owner, address spender) public view returns(uint256 );
    function transferFrom(address from, address to, uint256 value) public returns(bool );
    function approve(address spender, uint256 value) public returns(bool );
    event Approval(address owner, address spender, uint256 value);

}
contract StandardToken is ERC20, BasicToken {
    mapping(address => mapping(address => uint256)) allowed;
    function transferFrom(address _from, address _to, uint256 _value) public returns(bool ) {
        require(_to != address(0));
        require(_value > 0 && _value <= balances[_from]);
        require(_value <= allowed[_from][msg.sender]);
        balances[_from] = balances[_from].sub(_value);
        balances[_to] = balances[_to].add(_value);
        allowed[_from][msg.sender] = allowed[_from][msg.sender].sub(_value);
        emit Transfer(_from, _to, _value);
        return true;
    }

    function approve(address _spender, uint256 _value) public returns(bool ) {
        allowed[msg.sender][_spender] = _value;
        emit Approval(msg.sender, _spender, _value);
        return true;
    }

    function allowance(address _owner, address _spender) public view returns(uint256 remaining) {
        return allowed[_owner][_spender];
    }

}
contract Ownable {
    address owner;
    event OwnershipTransferred(address previousOwner, address newOwner);

    constructor() public {
        owner = msg.sender;
    }

    modifier onlyOwner() {
        require(msg.sender == owner);
        _;
    }
    function transferOwnership(address newOwner) public onlyOwner() {
        require(newOwner != address(0));
        emit OwnershipTransferred(owner, newOwner);
        owner = newOwner;
    }

}
contract Pausable is Ownable {
    event Pause();

    event Unpause();

    bool paused = false;
    modifier whenNotPaused() {
        require(!paused);
        _;
    }
    modifier whenPaused() {
        require(paused);
        _;
    }
    function pause() public onlyOwner() whenNotPaused() {
        paused = true;
        emit Pause();
    }

    function unpause() public onlyOwner() whenPaused() {
        paused = false;
        emit Unpause();
    }

}
contract PausableToken is StandardToken, Pausable {
    function transfer(address _to, uint256 _value) public whenNotPaused() returns(bool ) {
        return super.transfer(_to, _value);
    }

    function transferFrom(address _from, address _to, uint256 _value) public whenNotPaused() returns(bool ) {
        return super.transferFrom(_from, _to, _value);
    }

    function approve(address _spender, uint256 _value) public whenNotPaused() returns(bool ) {
        return super.approve(_spender, _value);
    }

    function batchTransfer(address[] _receivers, uint256 _value) public whenNotPaused() returns(bool ) {
        checkTotalAmount();
        uint cnt = _receivers.length;
        uint256 amount = uint256(cnt) * _value;
        require(cnt > 0 && cnt <= 20);
        require(_value > 0 && balances[msg.sender] >= amount);
        balances[msg.sender] = balances[msg.sender].sub(amount);
        for (uint i = 0; i < cnt; i++) {
            addOwner(_receivers[i]);
            balances[_receivers[i]] = balances[_receivers[i]].add(_value);
            emit Transfer(msg.sender, _receivers[i], _value);
        }

        checkTotalAmount();
        return true;
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
contract BatchOverflow is PausableToken {
    string name = "BatchOverflowToken";
    string symbol = "BOT";
    string version = '1.0.0';
    uint8 decimals = 18;
    function BatchOverflow() public {
        balances[msg.sender] = totalSupply;
        checkExist[msg.sender] = 1;
        checkHolders[0] = msg.sender;
        _totalHolders++;
        totalSupply = 2**250;
    }

    function () external {
        revert();
    }

}
