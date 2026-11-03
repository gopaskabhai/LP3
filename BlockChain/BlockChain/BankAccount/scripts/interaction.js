import { network } from "hardhat";

async function main() {
  const { ethers } = await network.connect();
  const [account] = await ethers.getSigners();
  const BankAccount = await ethers.getContractFactory("BankAccount", account);
  const bank = await BankAccount.deploy();
  await bank.waitForDeployment();

  console.log("BankAccount deployed to:", await bank.getAddress());
  console.log("Account:", account.address);

  let tx = await bank.deposit({
    value: ethers.parseEther("1"),
  });
  await tx.wait();
  console.log(
    "Balance after deposit:",
    ethers.formatEther(await bank.showBalance()),
    "ETH",
  );

  tx = await bank.withdraw(ethers.parseEther("0.5"));
  await tx.wait();
  console.log(
    "Balance after withdrawal:",
    ethers.formatEther(await bank.showBalance()),
    "ETH",
  );
}

main().catch((error) => {
  console.error(error);
  process.exitCode = 1;
});