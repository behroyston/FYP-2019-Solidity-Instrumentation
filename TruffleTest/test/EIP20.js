const EIP20 = artifacts.require("EIP20");

contract('EIP20', (accounts) => {
  it('should put 10000 tokens in the first account', async () => {
    const EIP20Instance = await EIP20.deployed();
    const balance = await EIP20Instance.balanceOf.call(accounts[0]);

    assert.equal(balance.valueOf(), 10000, "10000 wasn't in the first account");
  });

  it('should transferFrom coin correctly', async () => {
    const EIP20Instance = await EIP20.deployed();

    // Setup 2 accounts.
    const accountOne = accounts[0];
    const accountTwo = accounts[1];

    // Get initial balances of first and second account.
    const accountOneStartingBalance = (await EIP20Instance.balanceOf.call(accountOne)).toNumber();
    const accountTwoStartingBalance = (await EIP20Instance.balanceOf.call(accountTwo)).toNumber();

    // Make transaction from first account to second.
    const amount = 10;
    await EIP20Instance.transfer(accountTwo, amount, { from: accountOne });

    // Get balances of first and second account after the transactions.
    const accountOneEndingBalance = (await EIP20Instance.balanceOf.call(accountOne)).toNumber();
    const accountTwoEndingBalance = (await EIP20Instance.balanceOf.call(accountTwo)).toNumber();


    assert.equal(accountOneEndingBalance, accountOneStartingBalance - amount, "Amount wasn't correctly taken from the sender");
    assert.equal(accountTwoEndingBalance, accountTwoStartingBalance + amount, "Amount wasn't correctly sent to the receiver");
  });

  it('should approve coin appropriately', async () => {
    const EIP20Instance = await EIP20.deployed();

    // Setup 2 accounts.
    const accountOne = accounts[0];
    const accountTwo = accounts[1];
    const accountThree = accounts[2];


    // Get initial allowance of first and second account.
    const accountOneAllowanceforAccountTwo = (await EIP20Instance.allowance.call(accountOne,accountTwo)).toNumber();

    const initialAmount = 0;

    assert.equal(accountOneAllowanceforAccountTwo, initialAmount, "Initial Allowance = 0 is true");

    // Make transaction from first account to second.
    const amount = 10;
    await EIP20Instance.approve(accountTwo, amount, { from: accountOne });

    // Get balances of first and second account after the transactions.
    const accountOneEndingAllowanceforAccountTwo = (await EIP20Instance.allowance.call(accountOne,accountTwo)).toNumber();

    assert.equal(accountOneEndingAllowanceforAccountTwo, amount, "Ending Allowance = 10 is true");
  
    const test2 = 10;

    await EIP20Instance.transferFrom(accountOne,accountThree,test2, { from: accountTwo });

    const accountOneEndingBalance = (await EIP20Instance.balanceOf.call(accountOne)).toNumber();
    const accountTwoEndingBalance = (await EIP20Instance.balanceOf.call(accountThree)).toNumber();

    assert.equal(accountTwoEndingBalance,  10, "10");

  });
});
