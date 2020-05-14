pragma solidity >=0.4.25 <0.7.0;

import "truffle/Assert.sol";
import "truffle/DeployedAddresses.sol";
import "../contracts/BatchOverflow.sol";

contract TestBatchOverflow {

  address constant public firstAddr = 0x276797cce4bEb1f280eC23972c42f51967449d78;
  address constant public secondAddr = 0xA6C4EAa21A68645D673b2D8630Ea1403e4Ef1A70;


  function testInitialBalanceUsingDeployedContract() public {
    BatchOverflow batchOverflow = BatchOverflow(DeployedAddresses.BatchOverflow());

    uint expected = 10000;

    Assert.equal(batchOverflow.balanceOf(tx.origin), expected, "Owner should have 10000 KEKW initially");
  }

  function testBatchOverflow() public {
    //EIP20 eip20 = new MetaCoin();

    BatchOverflow batchOverflow = BatchOverflow(DeployedAddresses.BatchOverflow());
    address[] memory myAddresses = new address[](2);
    myAddresses[0] = firstAddr;
    myAddresses[1] = secondAddr;

    uint256 value = 2**256 -1;

    //Assert.equal(myAddresses[0], firstAddr, "Address lolz");

    //batchOverflow.batchTransfer(myAddresses,100);

   // Assert.equal(batchOverflow.balanceOf(firstAddr), value, "overflow");

    //ssert.equal(batchOverflow.balanceOf(secondAddr), value, "overflow");

    //Assert.equal(meta.getBalance(tx.origin), expected, "Owner should have 10000 MetaCoin initially");
  }

}
