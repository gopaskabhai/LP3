# Blockchain Technology (BT) Assignment: BankAccount Smart Contract

A cross-platform Solidity smart contract project built with Hardhat that simulates a decentralized bank account with deposit, withdrawal, and balance checking capabilities.

---

## 📋 System Requirements & Version Compatibility

This project is configured to run on **any operating system** (Windows, macOS, Linux) with zero manual environment configuration.

- **Node.js**: Version `18.x`, `20.x`, or `22.x` (LTS recommended).
- **npm**: Version `9.x` or `10.x` (bundled with Node.js).
- **Solidity Compiler**: `0.8.28` (downloaded automatically by Hardhat on first compile).

Check your installed versions by running:

```bash
node -v
npm -v
```

---

## 🚀 Quick Start (Run on Any Machine)

Clone or extract the repository, open your terminal (PowerShell, Command Prompt, macOS Terminal, or Linux Bash) in the project folder, and run:

### 1. Install Dependencies

Installs Hardhat and ethers locally:

```bash
npm install
```

### 2. Compile the Smart Contract

Compiles `contracts/BankAccount.sol`:

```bash
npm run compile
```

> *Expected output: `Compiled 1 Solidity file with solc 0.8.28`*

### 3. Run the Demonstration (One Command)

Deploys the contract, deposits `1.0 ETH`, withdraws `0.5 ETH`, and prints balance checks:

```bash
npm run interact
```

### ✅ Expected Output

```text
BankAccount deployed to: 0x5FbDB2315678afecb367f032d93F642f64180aa3
Account: 0xf39Fd6e51aad88F6F4ce6aB8827279cffFb92266
Balance after deposit: 1.0 ETH
Balance after withdrawal: 0.5 ETH
```

---

## 🛠 Project Structure

```text
BankAccount/
├── contracts/
│   └── BankAccount.sol       # Solidity smart contract code
├── scripts/
│   ├── deploy.js             # Contract deployment script
│   └── interaction.js        # Full demo: deploy -> deposit -> withdraw -> balance
├── .gitignore                # Excludes node_modules, artifacts, cache
├── hardhat.config.js         # Hardhat configuration (Solidity 0.8.28)
├── package.json              # Project scripts and dependencies
├── package-lock.json         # Pinned reproducible dependency versions
└── README.md                 # Assignment documentation and execution guide
```

---

## 📜 Smart Contract Specification (`BankAccount.sol`)

| Function                     | Type                 | Description                                                                     |
| :--------------------------- | :------------------- | :------------------------------------------------------------------------------ |
| `deposit()`                | `external payable` | Accepts ETH and credits it to the contract balance. Rejects `0 ETH` deposits. |
| `withdraw(uint256 amount)` | `external`         | Validates that balance is sufficient and transfers ETH back to caller.          |
| `showBalance()`            | `external view`    | Returns the current ETH balance held in the contract.                           |

---

## 🌐 Alternative: Running with a Dedicated Local Node

If you want to run a separate local JSON-RPC blockchain node for live inspection:

**Terminal 1 (Start Local Node):**

```bash
npx hardhat node
```

*(Leaves a local blockchain node running on `http://127.0.0.1:8545`)*

**Terminal 2 (Deploy and Interact):**

```bash
npx hardhat run scripts/deploy.js --network localhost
npx hardhat run scripts/interaction.js --network localhost
```

---

## ❓ Troubleshooting

- **Error: `Cannot find module ...`**
  - Fix: Run `npm install` in the project root folder.
- **Node version mismatch / syntax errors:**
  - Fix: Ensure your Node.js is version 18 or above (`node -v`). Download the latest LTS from [nodejs.org](https://nodejs.org/).
- **Port `8545` already in use (`EADDRINUSE`):**
  - Fix: You do not need to run `npx hardhat node`. Simply run `npm run interact`, which runs directly on Hardhat's built-in in-memory network without binding port 8545.
