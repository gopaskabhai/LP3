import { network } from "hardhat";

function formatFee(gasUsed, gasPrice, ethers) {
  const feeWei = gasUsed * gasPrice;
  return {
    gasUsed: gasUsed.toString(),
    gasPriceGwei: ethers.formatUnits(gasPrice, "gwei"),
    feeEth: ethers.formatEther(feeWei),
    feeGwei: ethers.formatUnits(feeWei, "gwei"),
  };
}

async function main() {
  const { ethers } = await network.connect();
  const [deployer, studentCaller] = await ethers.getSigners();

  const observations = [];

  console.log("================================================================");
  console.log("  BLOCKCHAIN ASSIGNMENT: STUDENT DATA CONTRACT DEMONSTRATION    ");
  console.log("================================================================");
  console.log(`Deployer Account : ${deployer.address}`);
  console.log(`Caller Account   : ${studentCaller.address}\n`);

  // 1. DEPLOYMENT
  console.log("----------------------------------------------------------------");
  console.log("STEP 1: DEPLOYING CONTRACT");
  console.log("----------------------------------------------------------------");
  const StudentData = await ethers.getContractFactory("StudentData", deployer);
  const contract = await StudentData.deploy();
  await contract.waitForDeployment();
  const contractAddress = await contract.getAddress();

  const deployReceipt = await contract.deploymentTransaction().wait();
  const deployStats = formatFee(deployReceipt.gasUsed, deployReceipt.gasPrice, ethers);

  observations.push({
    action: "Contract Deployment",
    gasUsed: deployStats.gasUsed,
    gasPriceGwei: deployStats.gasPriceGwei,
    feeEth: deployStats.feeEth,
  });

  console.log(`Contract Deployed At : ${contractAddress}`);
  console.log(`Tx Hash              : ${deployReceipt.hash}`);
  console.log(`Gas Used             : ${deployStats.gasUsed}`);
  console.log(`Gas Price            : ${deployStats.gasPriceGwei} Gwei`);
  console.log(`Transaction Fee      : ${deployStats.feeEth} ETH (${deployStats.feeGwei} Gwei)\n`);

  // 2. ADD STUDENT 1 (USING STRUCT & ARRAY)
  console.log("----------------------------------------------------------------");
  console.log("STEP 2: ADDING STUDENT 1 (STRUCT + ARRAY)");
  console.log("----------------------------------------------------------------");
  const tx1 = await contract.addStudent(101, "Alice Johnson", 20, "Blockchain Technology");
  const receipt1 = await tx1.wait();
  const stats1 = formatFee(receipt1.gasUsed, receipt1.gasPrice, ethers);

  observations.push({
    action: "addStudent (Alice)",
    gasUsed: stats1.gasUsed,
    gasPriceGwei: stats1.gasPriceGwei,
    feeEth: stats1.feeEth,
  });

  console.log(`Added Student        : ID 101, Alice Johnson, Age 20, Blockchain Technology`);
  console.log(`Tx Hash              : ${receipt1.hash}`);
  console.log(`Gas Used             : ${stats1.gasUsed}`);
  console.log(`Gas Price            : ${stats1.gasPriceGwei} Gwei`);
  console.log(`Transaction Fee      : ${stats1.feeEth} ETH\n`);

  // 3. ADD STUDENT 2 (USING STRUCT & ARRAY)
  console.log("----------------------------------------------------------------");
  console.log("STEP 3: ADDING STUDENT 2 (STRUCT + ARRAY)");
  console.log("----------------------------------------------------------------");
  const tx2 = await contract.addStudent(102, "Bob Smith", 21, "Computer Science");
  const receipt2 = await tx2.wait();
  const stats2 = formatFee(receipt2.gasUsed, receipt2.gasPrice, ethers);

  observations.push({
    action: "addStudent (Bob)",
    gasUsed: stats2.gasUsed,
    gasPriceGwei: stats2.gasPriceGwei,
    feeEth: stats2.feeEth,
  });

  console.log(`Added Student        : ID 102, Bob Smith, Age 21, Computer Science`);
  console.log(`Tx Hash              : ${receipt2.hash}`);
  console.log(`Gas Used             : ${stats2.gasUsed}`);
  console.log(`Gas Price            : ${stats2.gasPriceGwei} Gwei`);
  console.log(`Transaction Fee      : ${stats2.feeEth} ETH\n`);

  // 4. RETRIEVE ALL STUDENTS (ARRAY READ)
  console.log("----------------------------------------------------------------");
  console.log("STEP 4: READING STORED STUDENT DATA FROM ARRAY");
  console.log("----------------------------------------------------------------");
  const totalStudents = await contract.getStudentCount();
  console.log(`Total Students Stored: ${totalStudents}`);

  const allStudents = await contract.getAllStudents();
  allStudents.forEach((std, idx) => {
    console.log(`  [Student ${idx + 1}] ID: ${std.id} | Name: ${std.name} | Age: ${std.age} | Course: ${std.course}`);
  });
  console.log(`(Note: View functions do not consume transaction fees when called off-chain)\n`);

  // 5. TRIGGER FALLBACK FUNCTION
  console.log("----------------------------------------------------------------");
  console.log("STEP 5: TRIGGERING FALLBACK FUNCTION");
  console.log("----------------------------------------------------------------");
  console.log("Sending arbitrary data to non-existent function signature '0xabcdef12'...");
  const fallbackTx = await deployer.sendTransaction({
    to: contractAddress,
    data: "0xabcdef12",
    value: ethers.parseEther("0.01"),
  });
  const fallbackReceipt = await fallbackTx.wait();
  const fallbackStats = formatFee(fallbackReceipt.gasUsed, fallbackReceipt.gasPrice, ethers);

  observations.push({
    action: "Fallback Triggered",
    gasUsed: fallbackStats.gasUsed,
    gasPriceGwei: fallbackStats.gasPriceGwei,
    feeEth: fallbackStats.feeEth,
  });

  console.log(`Fallback Tx Hash     : ${fallbackReceipt.hash}`);
  console.log(`Gas Used             : ${fallbackStats.gasUsed}`);
  console.log(`Gas Price            : ${fallbackStats.gasPriceGwei} Gwei`);
  console.log(`Transaction Fee      : ${fallbackStats.feeEth} ETH\n`);

  // 6. TRIGGER RECEIVE FUNCTION (PLAIN ETHER TRANSFER)
  console.log("----------------------------------------------------------------");
  console.log("STEP 6: TRIGGERING RECEIVE FUNCTION (PLAIN ETHER TRANSFER)");
  console.log("----------------------------------------------------------------");
  const receiveTx = await studentCaller.sendTransaction({
    to: contractAddress,
    value: ethers.parseEther("0.05"),
  });
  const receiveReceipt = await receiveTx.wait();
  const receiveStats = formatFee(receiveReceipt.gasUsed, receiveReceipt.gasPrice, ethers);

  observations.push({
    action: "Receive (Plain ETH)",
    gasUsed: receiveStats.gasUsed,
    gasPriceGwei: receiveStats.gasPriceGwei,
    feeEth: receiveStats.feeEth,
  });

  console.log(`Receive Tx Hash      : ${receiveReceipt.hash}`);
  console.log(`Gas Used             : ${receiveStats.gasUsed}`);
  console.log(`Gas Price            : ${receiveStats.gasPriceGwei} Gwei`);
  console.log(`Transaction Fee      : ${receiveStats.feeEth} ETH\n`);

  // 7. SUMMARY TABLE OF GAS & TRANSACTION FEES
  console.log("================================================================================");
  console.log("        SUMMARY TABLE: GAS VALUES & TRANSACTION FEES OBSERVED                    ");
  console.log("================================================================================");
  console.log(
    "| " +
      "Operation".padEnd(25) +
      " | " +
      "Gas Used".padEnd(12) +
      " | " +
      "Gas Price (Gwei)".padEnd(18) +
      " | " +
      "Tx Fee (ETH)".padEnd(18) +
      " |"
  );
  console.log("|" + "-".repeat(27) + "|" + "-".repeat(14) + "|" + "-".repeat(20) + "|" + "-".repeat(20) + "|");
  observations.forEach((obs) => {
    console.log(
      "| " +
        obs.action.padEnd(25) +
        " | " +
        obs.gasUsed.padEnd(12) +
        " | " +
        obs.gasPriceGwei.padEnd(18) +
        " | " +
        obs.feeEth.padEnd(18) +
        " |"
    );
  });
  console.log("================================================================================\n");
}

main().catch((error) => {
  console.error(error);
  process.exitCode = 1;
});
