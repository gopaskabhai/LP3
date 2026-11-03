# BankAccount Smart Contract: Complete Theory, Code Breakdown & Viva Guide

> **Target Audience**: Anyone with **zero prior knowledge** of Blockchain, Solidity, or Hardhat.  
> This guide covers all foundational concepts, a line-by-line code explanation, terminal commands, and common questions asked by external examiners.

---

## 📑 Table of Contents
1. [Foundational Concepts (Zero-Knowledge Primer)](#1-foundational-concepts-zero-knowledge-primer)
2. [What This Project Does](#2-what-this-project-does)
3. [Line-by-Line Code Breakdown: `contracts/BankAccount.sol`](#3-line-by-line-code-breakdown-contractsbankaccountsol)
4. [Line-by-Line Breakdown of Scripts (`deploy.js` & `interaction.js`)](#4-line-by-line-breakdown-of-scripts)
5. [Hardhat & Terminal Commands: What Happens Behind the Scenes?](#5-hardhat--terminal-commands-what-happens-behind-the-scenes)
6. [Top 15 External Examiner Viva Questions & Model Answers](#6-top-15-external-examiner-viva-questions--model-answers)

---

## 1. Foundational Concepts (Zero-Knowledge Primer)

### What is a Blockchain?
A blockchain is a decentralized, distributed, and append-only digital ledger. Instead of one central bank or server controlling the database, thousands of computers (nodes) across the network maintain an identical copy of the database and agree on new data using consensus mechanisms.

### What is Ethereum?
While Bitcoin was built as digital money, **Ethereum** is a programmable blockchain. It includes a built-in virtual computer called the **Ethereum Virtual Machine (EVM)** that can run code.

### What is a Smart Contract?
A **Smart Contract** is a program stored on the Ethereum blockchain that runs automatically when predetermined conditions are met. 
- **Immutable**: Once deployed to the blockchain, its code can never be modified or tampered with.
- **Trustless**: It executes code without depending on a human intermediary (like a bank manager).
- **Code is Law**: Rules written in the contract are strictly enforced by mathematics and cryptography.

### What is Solidity?
**Solidity** is the object-oriented, high-level programming language used to write smart contracts for Ethereum. Its syntax is similar to JavaScript and C++.

### What is Hardhat?
**Hardhat** is a development environment for Ethereum software. It allows developers to:
- Compile Solidity code.
- Run a simulated, local Ethereum blockchain on your computer for fast testing.
- Deploy smart contracts and execute JavaScript scripts against them.

### What is Ethers.js?
**Ethers.js** is a JavaScript library that allows our JavaScript/Node.js scripts to connect to and communicate with the Ethereum blockchain (e.g., signing transactions, sending ETH, calling contract functions).

---

## 2. What This Project Does

In a traditional bank:
- You open an account with an account number.
- You give cash to the teller to deposit.
- You request a withdrawal, and the bank subtracts from your ledger.

In this **`BankAccount` Smart Contract**:
- The contract itself acts as the automated bank.
- Anyone with an Ethereum wallet address can call:
  1. `deposit()`: Sends real or simulated Ether (ETH) to the contract. The contract increases its tracked balance.
  2. `withdraw(amount)`: Checks if the contract has enough funds, subtracts `amount`, and transfers ETH back to the caller's wallet.
  3. `showBalance()`: Reads and returns the current balance tracked by the contract.

---

## 3. Line-by-Line Code Breakdown: `contracts/BankAccount.sol`

Let's inspect every single line of the contract:

```solidity
1: // SPDX-License-Identifier: MIT
```
- **What it is**: Machine-readable open-source license comment.
- **Why we use it**: Solidity compiler requires a license identifier (here, the permissive MIT license) to prevent compiler warnings and clarify licensing.

```solidity
2: pragma solidity ^0.8.0;
```
- **What it is**: The `pragma` directive.
- **Why we use it**: It tells the Solidity compiler which version to use. `^0.8.0` means any compiler version from `0.8.0` up to (but not including) `0.9.0`. 
- **Key Viva Point**: In Solidity `0.8.0` and above, **arithmetic overflow and underflow checks are built-in automatically**. Older versions needed the `SafeMath` library.

```solidity
4: contract BankAccount {
```
- **What it is**: Defines the contract named `BankAccount`.
- **Analogy**: Similar to `class BankAccount` in Java or C++.

```solidity
5:     uint256 public balance;
```
- **What it is**: A state variable.
- **`uint256`**: Unsigned integer of 256 bits (values from $0$ to $2^{256} - 1$, cannot be negative).
- **`public`**: Solidity automatically creates a free "getter" function behind the scenes so anyone can read `balance()`.
- **Where is it stored?**: It is stored permanently on the blockchain in **Storage** (the permanent persistent memory of the contract).

```solidity
7:     function deposit() public payable {
```
- **`public`**: Can be called internally by this contract or externally by any user/contract.
- **`payable`**: **CRITICAL KEYWORD**. In Solidity, by default, functions **reject** Ether sent to them. Adding `payable` allows this function to receive native Ether (ETH) alongside the transaction. If a function is not marked `payable`, any transaction attempting to send ETH with it will instantly fail and revert.

```solidity
8:         require(msg.value > 0, "Amount must be greater than zero");
```
- **`require(condition, errorMessage)`**: Used for input validation and access control. If the condition is `true`, execution continues. If `false`, the transaction immediately **reverts**:
  1. All state changes made in the transaction are rolled back as if nothing happened.
  2. Any unused gas is refunded to the sender.
  3. The error string `"Amount must be greater than zero"` is returned.
- **`msg.value`**: A global variable provided by the EVM. It contains the exact amount of Ether (in **Wei**) sent with the transaction. ($1 \text{ ETH} = 10^{18} \text{ Wei}$).

```solidity
9:         balance += msg.value;
```
- Updates the state variable `balance` by adding the incoming `msg.value`.

```solidity
12:    function withdraw(uint256 amount) public {
```
- A function that takes one input parameter: `amount` (the amount of Wei to withdraw). It does not need `payable` because it is not *receiving* ETH; it is *sending* ETH out.

```solidity
13:        require(amount > 0, "Invalid amount");
14:        require(amount <= balance, "Insufficient balance");
```
- Validation guards:
  - Ensures the user cannot withdraw $0$ or negative amounts.
  - Ensures the contract doesn't allow withdrawals exceeding its current balance.

```solidity
16:        balance -= amount;
17:        payable(msg.sender).transfer(amount);
```
- **Why line 16 comes before line 17**: **The Checks-Effects-Interactions Pattern**.  
  We update our internal state (`balance -= amount`) **before** calling the external transfer (`payable(msg.sender).transfer(amount)`). This is a primary security defense against **Reentrancy Attacks**.
- **`msg.sender`**: A global EVM variable representing the Ethereum address of whoever initiated/called this function.
- **`payable(msg.sender)`**: Typecasting. In Solidity, standard addresses cannot receive ETH. We cast `msg.sender` to an `address payable` so it can receive funds.
- **`.transfer(amount)`**: Sends `amount` in Wei to the caller. It forwards a stipend of 2300 gas and automatically reverts if the transfer fails.

```solidity
20:    function showBalance() public view returns (uint256) {
21:        return balance;
22:    }
```
- **`view`**: Indicates that this function **only reads** from the blockchain storage and **does not modify** any state.
- **Why this matters**: Because it does not modify the blockchain, calling `showBalance()` from off-chain (e.g. from a script, website, or wallet) is **completely free and costs 0 Gas**!

---

## 4. Line-by-Line Breakdown of Scripts

### Script 1: `scripts/deploy.js`
This script deploys the smart contract onto the Ethereum blockchain:

```javascript
import { network } from "hardhat";

async function main() {
  // 1. Connect to the active network (e.g. local in-memory or localhost:8545)
  const { ethers } = await network.connect();

  // 2. Fetch the contract abstraction (Bytecode + ABI)
  const BankAccount = await ethers.getContractFactory("BankAccount");

  // 3. Send deployment transaction to the blockchain
  const bank = await BankAccount.deploy();

  // 4. Wait for the transaction to be mined into a block
  await bank.waitForDeployment();

  // 5. Print the newly generated contract address
  console.log("BankAccount deployed to:", await bank.getAddress());
}
```

### Script 2: `scripts/interaction.js`
This script demonstrates the complete life-cycle of the contract:

```javascript
// 1. Get test accounts provided by Hardhat
const [account] = await ethers.getSigners();

// 2. Deploy fresh instance
const BankAccount = await ethers.getContractFactory("BankAccount", account);
const bank = await BankAccount.deploy();
await bank.waitForDeployment();

// 3. Deposit 1 ETH into the contract
// ethers.parseEther("1") converts 1.0 to 1,000,000,000,000,000,000 Wei
let tx = await bank.deposit({ value: ethers.parseEther("1") });
await tx.wait(); // Wait for transaction confirmation

// 4. Read balance using showBalance()
// ethers.formatEther() converts Wei back into human-readable ETH string
console.log("Balance after deposit:", ethers.formatEther(await bank.showBalance()), "ETH");

// 5. Withdraw 0.5 ETH
tx = await bank.withdraw(ethers.parseEther("0.5"));
await tx.wait();

// 6. Read updated balance (should be 0.5 ETH)
console.log("Balance after withdrawal:", ethers.formatEther(await bank.showBalance()), "ETH");
```

---

## 5. Hardhat & Terminal Commands: What Happens Behind the Scenes?

### 1. `npm install`
- **What it does**: Reads `package.json` and `package-lock.json`, downloads all required JavaScript libraries (`hardhat`, `@nomicfoundation/hardhat-ethers`, `ethers`), and installs them locally in `node_modules`.

### 2. `npx hardhat compile` (or `npm run compile`)
- **What it does**: Invokes the Solidity compiler (`solc 0.8.28`).
- **What it produces**:
  - **Bytecode**: The compiled binary instructions executed by the Ethereum Virtual Machine (EVM).
  - **ABI (Application Binary Interface)**: A JSON file that lists all contract functions, parameters, and return types so external scripts know how to communicate with the contract.

### 3. `npx hardhat node`
- **What it does**: Starts an independent, persistent local Ethereum blockchain node running on your local machine at `http://127.0.0.1:8545`.
- **What it gives you**: Generates 20 dummy test accounts, each pre-funded with 10,000 fake ETH, along with their private keys.
- **Why keep it open**: It keeps the blockchain running continuously, just like a real public network.

### 4. `npx hardhat run scripts/deploy.js --network localhost`
- **What `--network localhost` means**:
  - By default, Hardhat spins up an **in-memory temporary blockchain**, executes the script, and immediately destroys it when the script finishes.
  - When you pass `--network localhost`, you tell Hardhat: *"Do not create a temporary blockchain. Instead, connect to the persistent blockchain node currently running on `http://127.0.0.1:8545`."*
  - The deployed contract remains alive and accessible on that local node as long as `npx hardhat node` is running.

### 5. `npm run interact` (or `npx hardhat run scripts/interaction.js`)
- Runs the interaction script on Hardhat's built-in fast network, compiles automatically if needed, deploys the contract, tests deposits and withdrawals, and prints the output without requiring two separate terminal windows.

---

## 6. Top 15 External Examiner Viva Questions & Model Answers

### Q1: What is a Smart Contract, and why is it called "immutable"?
> **Answer**: A smart contract is a computer program executed on the Ethereum Virtual Machine (EVM). Once deployed onto the blockchain, its code cannot be modified, deleted, or patched by anyone (including the creator), making it immutable and trustless.

---

### Q2: What is the difference between `msg.sender` and `msg.value`?
> **Answer**:
> - `msg.sender` is the Ethereum address (`address`) of the person or contract calling the function.
> - `msg.value` is the amount of native cryptocurrency (Ether in **Wei**) sent along with the transaction call.

---

### Q3: Why is `deposit()` marked as `payable`, but `withdraw()` is not?
> **Answer**: In Solidity, any function intended to receive Ether must explicitly be declared with the `payable` modifier. `deposit()` receives Ether from the user, so it must be `payable`. `withdraw()` does not receive Ether; it sends Ether *out* to the user, so it must not be `payable`.

---

### Q4: What happens if `require()` fails in a transaction?
> **Answer**: If the condition inside `require()` evaluates to `false`:
> 1. The transaction immediately reverts.
> 2. All state changes made in that transaction are undone.
> 3. Any remaining, unconsumed gas is refunded to the caller.
> 4. The provided error string is returned.

---

### Q5: What is the difference between `view`, `pure`, and regular functions?
> **Answer**:
> - **Regular (State-changing)**: Modifies storage variables on the blockchain. Requires a transaction and costs Gas.
> - **`view`**: Reads state variables from storage but does not modify them. Free when called off-chain.
> - **`pure`**: Does not read nor modify state variables (e.g. mathematical calculations like `add(a, b)`). Free when called off-chain.

---

### Q6: What is the difference between Wei, Gwei, and Ether?
> **Answer**: They are denominations of Ether:
> - $1 \text{ Ether} = 10^9 \text{ Gwei} = 10^{18} \text{ Wei}$
> - **Wei** is the smallest indivisible unit in Ethereum (like cents to dollars, but with 18 decimal places). Solidity handles all financial calculations in Wei to prevent floating-point precision loss.

---

### Q7: Why did we put `balance -= amount` before `payable(msg.sender).transfer(amount)`?
> **Answer**: This implements the **Checks-Effects-Interactions pattern**. We update our internal state (*Effect*) before calling an external address (*Interaction*). If we transferred first, a malicious contract caller could trigger a recursive fallback function to repeatedly drain funds before `balance` was updated (a **Reentrancy Attack**, famously responsible for the 2016 DAO hack).

---

### Q8: What is the difference between `transfer()`, `send()`, and `call()` for sending ETH?
> **Answer**:
> - `transfer()`: Forwards 2300 gas; automatically reverts if the transfer fails.
> - `send()`: Forwards 2300 gas; returns a boolean (`true`/`false`) instead of reverting.
> - `call{value: amount}("")`: Recommended modern approach in Solidity 0.8+ for sending ETH; forwards all available gas (or a specified amount) and returns a boolean success flag.

---

### Q9: What is an ABI and what is Bytecode?
> **Answer**:
> - **Bytecode**: Low-level machine code that the EVM executes.
> - **ABI (Application Binary Interface)**: A JSON specification describing all the functions, inputs, and outputs of the smart contract. It enables JavaScript or other programming languages to encode function calls and decode return values from the contract.

---

### Q10: Where are variables stored in Solidity? (Storage vs Memory vs Calldata)
> **Answer**:
> - **Storage**: Persistent storage located permanently on the blockchain ledger (most expensive in gas). State variables live here.
> - **Memory**: Temporary workspace available only during function execution. Cleared when execution finishes.
> - **Calldata**: Non-modifiable, temporary area where function arguments passed from outside are stored.

---

### Q11: What is Gas and why does Ethereum use it?
> **Answer**: Gas is a unit that measures the amount of computational effort required to execute an operation on the Ethereum network. It exists to:
> 1. Prevent infinite loops (solves the Halting Problem in computer science).
> 2. Prevent spam or Denial of Service (DoS) attacks on nodes.
> 3. Compensate validators/miners for computing and securing transactions.

---

### Q12: What is the difference between Gas Limit and Gas Price?
> **Answer**:
> - **Gas Limit**: The maximum number of gas units the sender is willing to consume on a transaction.
> - **Gas Price**: The amount of Ether (in Gwei) the sender is willing to pay per unit of gas.
> - $\text{Total Transaction Fee} = \text{Gas Used} \times \text{Gas Price}$.

---

### Q13: Does calling `showBalance()` cost gas?
> **Answer**: It depends on who calls it:
> - If called **externally by a user or script** (off-chain), it is executed locally on the node and is **completely free (0 Gas)**.
> - If called **internally by another smart contract during an on-chain transaction**, it consumes a small amount of gas as part of that transaction's execution.

---

### Q14: What does the `--network localhost` flag do when executing a script?
> **Answer**: It directs Hardhat to connect to an already active JSON-RPC node running locally at `http://127.0.0.1:8545` instead of using the default, temporary in-memory Hardhat network.

---

### Q15: Why is arithmetic overflow/underflow safe in Solidity 0.8.0+?
> **Answer**: Prior to Solidity 0.8.0, if an integer exceeded its maximum value (e.g. $255 + 1$ in `uint8`), it would wrap around to $0$ without error unless developers used OpenZeppelin's `SafeMath`. In Solidity 0.8.0 and above, the compiler automatically includes overflow and underflow checks that revert the transaction if an overflow occurs.
