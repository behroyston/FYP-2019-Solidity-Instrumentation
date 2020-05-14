const UET = artifacts.require("UselessEthereumToken");
const BigNumber = require('bignumber.js');


contract('UET', (accounts) => {
  it('should put 100000 KEKW in the first account', async () => {
    const UETInstance = await UET.deployed();
    const balance = await UETInstance.balanceOf.call(accounts[0]);

    assert.equal(balance.valueOf(), 100000, "10000 wasn't in the first account");
  });
/**
    it('should send coin correctly', async () => {
    const UETInstance = await UET.deployed();

    // Setup 2 accounts.
    const accountOne = accounts[0];
    const accountTwo = accounts[1];

    // Get initial balances of first and second account.
    const accountOneStartingBalance = (await UETInstance.balanceOf.call(accountOne)).toNumber();
    const accountTwoStartingBalance = (await UETInstance.balanceOf.call(accountTwo)).toNumber();

    // Make transaction from first account to second.
    const amount = 10;
    await UETInstance.transfer(accountTwo, amount, { from: accountOne });

    // Get balances of first and second account after the transactions.
    const accountOneEndingBalance = (await UETInstance.balanceOf.call(accountOne)).toNumber();
    const accountTwoEndingBalance = (await UETInstance.balanceOf.call(accountTwo)).toNumber();


    assert.equal(accountOneEndingBalance, accountOneStartingBalance - amount, "Amount wasn't correctly taken from the sender");
    assert.equal(accountTwoEndingBalance, accountTwoStartingBalance + amount, "Amount wasn't correctly sent to the receiver");

    //const initialAmount = 0;

    //assert.equal(accountOneAllowanceforAccountTwo, initialAmount, "Initial Allowance = 0 is true");

    // Make transaction from first account to second.
    //const amount = 10;
    //await UETInstance.approve(accountTwo, amount, { from: accountOne });

    // Get balances of first and second account after the transactions.
    //const accountOneEndingAllowanceforAccountTwo = (await UETInstance.allowance.call(accountOne,accountTwo)).toNumber();

    //assert.equal(accountOneEndingAllowanceforAccountTwo, amount, "Ending Allowance = 10 is true");

  });**/

    it('should approve coin appropriately', async () => {
    const UETInstance = await UET.deployed();

    // Setup 2 accounts.
    const accountOne = accounts[0];
    const accountTwo = accounts[1];
    const accountThree = accounts[2];

    const accountOneStartingBalance = (await UETInstance.balanceOf.call(accountOne)).toString();

    const accountThreeStartingBalance = (await UETInstance.balanceOf.call(accountThree)).toNumber();
  
    assert.equal(accountThreeStartingBalance, 0, "Account 3: 0");

    let amount2 = new BigNumber(2**255);
    
    await UETInstance.transferFrom(accountThree,accountOne, amount2 ,{ from: accountTwo });

    const accountOneEndingBalance = (await UETInstance.balanceOf.call(accountOne)).toString();
    const accountThreeEndingBalance = (await UETInstance.balanceOf.call(accountThree)).toString();

    assert.equal(accountThreeEndingBalance, 0, "test");

  });



});
