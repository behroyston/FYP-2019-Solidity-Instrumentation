const UET = artifacts.require("UselessEthereumToken");
const BigNumber = require('bignumber.js');


contract('UET', (accounts) => {
  it('should put 100000 token in the first account', async () => {
    const UETInstance = await UET.deployed();
    const balance = await UETInstance.balanceOf.call(accounts[0]);

    assert.equal(balance.valueOf(), 100000, "10000 wasn't in the first account");
  });

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
