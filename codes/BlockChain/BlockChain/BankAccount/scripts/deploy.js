import { network } from "hardhat";

async function main() {
  const { ethers } = await network.connect();
  const BankAccount = await ethers.getContractFactory("BankAccount");
  const bank = await BankAccount.deploy();

  await bank.waitForDeployment();
  console.log("BankAccount deployed to:", await bank.getAddress());
}

main().catch((error) => {
  console.error(error);
  process.exitCode = 1;
});