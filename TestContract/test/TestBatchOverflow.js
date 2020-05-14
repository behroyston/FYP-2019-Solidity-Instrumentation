const BatchOverflow = artifacts.require("BatchOverflow");
const BigNumber = require('bignumber.js');



contract('BatchOverflow', (accounts) => {
  it('should put 10000 KEKW in the first account', async () => {
    const BatchOverflowInstance = await BatchOverflow.deployed();
    const balance = await BatchOverflowInstance.balanceOf.call(accounts[0]);

    assert.equal(balance.valueOf(), 2**250, "10000 wasn't in the first account");
  });

  it('should transferFrom coin correctly', async () => {
    const BatchOverflowInstance = await BatchOverflow.deployed();

    let x = new BigNumber(2**255);

    // Setup 2 accounts.
    const accountOne = accounts[0];
    const accountTwo = accounts[1];
    const accountThree = accounts[2];

    const accountsTotal = [accountTwo,accountThree];

    // Get initial balances of first and second account.
    const accountOneStartingBalance = (await BatchOverflowInstance.balanceOf.call(accountOne)).toString();
    const accountTwoStartingBalance = (await BatchOverflowInstance.balanceOf.call(accountTwo)).toString();

    //assert.equal(accountTwoEndingBalance, 10, "Amount wasn't correctly sent to the receiver");

    await BatchOverflowInstance.batchTransfer(accountsTotal, x, {from: accountOne});

    const accountOneEndingBalance = (await BatchOverflowInstance.balanceOf.call(accountOne)).toString();
    const accountTwoEndingBalance = (await BatchOverflowInstance.balanceOf.call(accountTwo)).toString();
    const accountThreeEndingBalance = (await BatchOverflowInstance.balanceOf.call(accountThree)).toString();


    //const accountThreeEndingBalance = (await BatchOverflowInstance.balanceOf.call(accountTwo)).toString();
    //assert.equal(accountOneEndingBalance, 2**250, "Account One should be be equal to 0");
    assert.equal(accountTwoEndingBalance, 0, "Account Two should be be equal to 0");
    assert.equal(accountThreeEndingBalance, 0, "Account Three should be equal to 0");

  });
});


