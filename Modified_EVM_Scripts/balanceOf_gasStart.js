// with gasstart and gas stop
const Web3 = require("web3");
var httpRpcAddr = "http://127.0.0.1:8546"
var Provider = new Web3.providers.HttpProvider(httpRpcAddr);
var web3 = new Web3(Provider);

// replace these two obtained from compilation and EVM
var contractAddress = "=";
var abi = 

var instance = web3.eth.contract(abi).at(contractAddress);

function sleep(milliseconds) {
	const date = Date.now();
	let currentDate = null;
	do {
		currentDate = Date.now();
	} while (currentDate - date < milliseconds);
}

console.log("Calling smart contract");

instance.balanceOf(web3.eth.accounts[0], function (error,result) {
	if (!error)
		console.log("Before Account 0:" + result);
	else
		console.error(error);
});

//instance.checkTotalAmount();

var promise = new Promise(function(resolve,reject) {

	instance.transfer(web3.eth.accounts[1], 10, {from: web3.eth.accounts[0],gas: 70000}, function(error,hash) 
	{
		sleep(30000);

		if (!error) {
			//console.log(result);
			//console.log("Transfer sUCCESS")
			resolve("Transfer Success!");


	    // Wait for the transaction to be mined
		    var receipt;

		    while (receipt == null||receipt==undefined) {

				receipt = web3.eth.getTransactionReceipt(hash);

		      // Simulate the sleep function
		      // Atomics.wait(new Int32Array(new SharedArrayBuffer(4)), 0, 0, 1000);
		    }
		    console.log(receipt);
		    console.log("Status: " + receipt.status);

		}
		else
		{
			console.log(error);
			reject("It broke");
		}
	});

});