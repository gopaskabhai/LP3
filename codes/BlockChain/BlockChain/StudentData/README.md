# Blockchain Technology (BT) Assignment 4: Student Data Smart Contract

## 📌 Problem Statement

> **Write a program in Solidity to create Student data. Use the following constructs:**
>
> - **Structures**
> - **Arrays**
> - **Fallback**
>
> **Deploy this as smart contract on Ethereum and Observe the transaction fee and Gas values.**

---

## 🏗️ Constructs Implemented in `contracts/StudentData.sol`

### 1. Structure (`struct Student`)

Used to group related attributes representing a single student record:

```solidity
struct Student {
    uint256 id;
    string name;
    uint8 age;
    string course;
}
```

### 2. Array (`Student[] public students`)

A dynamic array used to store multiple `Student` structures on-chain:

- Elements are appended dynamically using `students.push(newStudent)`.
- Array length and elements can be retrieved using `getStudentCount()`, `getStudent(index)`, and `getAllStudents()`.

### 3. Fallback & Receive Functions

Handles transactions sent to the contract that do not match any defined function:

```solidity
// Executed when arbitrary data or invalid function signature is called
fallback() external payable {
    emit FallbackTriggered(msg.sender, msg.value, msg.data);
}

// Executed when plain ETH is sent without calldata
receive() external payable {
    emit ReceivedEther(msg.sender, msg.value);
}
```

---

## 📊 Gas & Transaction Fee Observation Table

The interaction script calculates fees using the standard Ethereum formula:

$$
\text{Transaction Fee} = \text{Gas Used} \times \text{Gas Price}
$$

Observed values from contract execution on the local Ethereum network:

| Operation                                    | Gas Units Used   | Gas Price      | Transaction Fee (ETH) | Transaction Fee (Gwei) |
| :------------------------------------------- | :--------------- | :------------- | :-------------------- | :--------------------- |
| **Contract Deployment**                | `1,064,889`    | `1.107 Gwei` | `0.001179 ETH`      | `1,179,281 Gwei`     |
| **`addStudent` (Alice - 1st write)** | `140,512`      | `0.974 Gwei` | `0.000137 ETH`      | `136,846 Gwei`       |
| **`addStudent` (Bob - 2nd write)**   | `123,304`      | `0.853 Gwei` | `0.000105 ETH`      | `105,146 Gwei`       |
| **`Fallback` Triggered (calldata)**  | `23,882`       | `0.747 Gwei` | `0.000018 ETH`      | `17,830 Gwei`        |
| **`Receive` (Plain ETH transfer)**   | `22,625`       | `0.653 Gwei` | `0.000015 ETH`      | `14,782 Gwei`        |
| **Read (`getAllStudents`)**          | `0` *(View)* | `0 Gwei`     | `0 ETH`             | `0 Gwei`             |

### Key Observations:

1. **Deployment Cost**: Deployment is the most expensive operation because the EVM stores contract bytecode permanently on the blockchain (`~1M gas`).
2. **First vs. Subsequent Array Writes**: The first `addStudent` transaction (`140,512` gas) consumes more gas than the second (`123,304` gas) because initializing new storage slots on the EVM (SSTORE zero-to-non-zero) has a higher base cost.
3. **Fallback & Receive**: Costs only `~22k - 24k` gas, which consists of the standard `21,000` base transaction fee plus minimal execution overhead.
4. **View Calls**: Off-chain calls to `getAllStudents()` and `getStudentCount()` do not send an on-chain transaction and therefore cost `0` gas.

---

## 📋 System Requirements & Version Compatibility

This project is built to run cleanly across different operating systems (Windows, macOS, Linux) and varying environment versions:

- **Node.js**: `18.x`, `20.x`, or `22.x` (LTS recommended)
- **npm**: `9.x` or `10.x`
- **Solidity**: `0.8.28` (Hardhat downloads the required `solc` compiler automatically)

Verify your Node.js and npm versions:

```bash
node -v
npm -v
```

---

## 🚀 Quick Start (Run on Any Machine)

Open your terminal in `BlockChain/StudentData`:

### Step 1: Install Dependencies

```bash
npm install
```

### Step 2: Compile the Contract

```bash
npm run compile
```

> *Expected output: `Compiled 1 Solidity file with solc 0.8.28 (evm target: cancun)`*

### Step 3: Run the Complete Demonstration

```bash
npm run interact
```

This single command will:

1. Deploy `StudentData` and report deployment gas & fee.
2. Insert student records using `struct` and dynamic `array`.
3. Read back all students and display them.
4. Send an arbitrary transaction to trigger the `fallback()` function and measure gas.
5. Send plain ETH to trigger the `receive()` function and measure gas.
6. Print the complete **Gas & Fee Observation Summary Table**.

---

## 📁 Clean Project Structure (Minimum Files)

```text
StudentData/
├── contracts/
│   └── StudentData.sol       # Smart contract (Structs, Arrays, Fallback, Receive)
├── scripts/
│   ├── deploy.js             # Deployment script with gas tracking
│   └── interaction.js        # Full workflow & gas observation report
├── .gitignore                # Excludes node_modules, cache, artifacts
├── hardhat.config.js         # Hardhat configuration (Solidity 0.8.28)
├── package.json              # Project configuration and npm scripts
├── package-lock.json         # Pinned dependency lockfile
└── README.md                 # Complete assignment documentation
```

---

## 🌐 Alternative: Running with a Dedicated Local Blockchain Node

If you need a persistent local RPC network for live inspection or frontend integration:

**Terminal 1 (Start Node):**

```bash
npx hardhat node
```

**Terminal 2 (Deploy & Interact):**

```bash
npx hardhat run scripts/deploy.js --network localhost
npx hardhat run scripts/interaction.js --network localhost
```

---

## ❓ Common Errors & Fixes

- **`Cannot find module ...`**: Run `npm install` in the project root folder.
- **Node version errors**: Ensure Node.js is version 18 or higher (`node -v`). Update at [nodejs.org](https://nodejs.org/).
- **Port 8545 in use (`EADDRINUSE`)**: You don't need a standalone node; `npm run interact` uses Hardhat's built-in in-memory blockchain directly with zero port conflicts.
