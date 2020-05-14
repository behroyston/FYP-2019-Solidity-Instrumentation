const ConvertLib = artifacts.require("ConvertLib");
const EIP20 = artifacts.require("EIP20");
const UET20 = artifacts.require("UselessEthereumToken");
const FuturXe = artifacts.require("FuturXe");
const BatchOverflow = artifacts.require("BatchOverflow")
const BigNumber = require('bignumber.js');

let test = 100000;
module.exports = function(deployer) {
  deployer.deploy(ConvertLib);
  deployer.link(ConvertLib, EIP20);
  deployer.deploy(UET20);
  // initialAmount, string _tokenName, uint8 _decimalUnits, string _tokenSymbol
  deployer.deploy(EIP20,10000,"test",1,"kekw");
  deployer.deploy(FuturXe,test,"test",1,"kekw");
  deployer.deploy(BatchOverflow);

};
