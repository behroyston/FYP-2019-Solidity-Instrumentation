pragma solidity >=0.4.25 <0.7.0;

import "truffle/Assert.sol";
import "truffle/DeployedAddresses.sol";
import "../contracts/EIP20.sol";

contract TestEIP20 {

  function testInitialBalanceUsingDeployedContract() public {
    EIP20 eip20 = EIP20(DeployedAddresses.EIP20());

    uint expected = 10000;

    Assert.equal(eip20.balanceOf(tx.origin), expected, "Owner should have 10000 KEKW initially");
  }

  /**function testInitialBalanceWithNewEIP20Coin() public {
    //EIP20 eip20 = new MetaCoin();

    uint expected = 10000;

    Assert.equal(meta.getBalance(tx.origin), expected, "Owner should have 10000 MetaCoin initially");
  }**/

}
