import { network } from "hardhat";

async function main() {
  const { ethers } = await network.connect();
  const [deployer] = await ethers.getSigners();

  console.log("==================================================");
  console.log("   STUDENT DATA SMART CONTRACT DEPLOYMENT");
  console.log("==================================================");
  console.log(`Deployer Address : ${deployer.address}`);

  const StudentDataFactory = await ethers.getContractFactory("StudentData", deployer);
  const studentData = await StudentDataFactory.deploy();
  await studentData.waitForDeployment();

  const contractAddress = await studentData.getAddress();
  const deployTx = studentData.deploymentTransaction();
  const receipt = await deployTx.wait();

  const gasUsed = receipt.gasUsed;
  const gasPrice = receipt.gasPrice;
  const txFeeWei = gasUsed * gasPrice;

  console.log(`Contract Address : ${contractAddress}`);
  console.log(`Transaction Hash : ${receipt.hash}`);
  console.log("--------------------------------------------------");
  console.log("GAS & TRANSACTION FEE OBSERVATION (DEPLOYMENT):");
  console.log(`  Gas Units Used : ${gasUsed.toString()}`);
  console.log(`  Gas Price      : ${ethers.formatUnits(gasPrice, "gwei")} Gwei (${gasPrice.toString()} Wei)`);
  console.log(`  Transaction Fee: ${ethers.formatUnits(txFeeWei, "gwei")} Gwei (${ethers.formatEther(txFeeWei)} ETH)`);
  console.log("==================================================\n");
}

main().catch((error) => {
  console.error(error);
  process.exitCode = 1;
});
