0. CLI - Create/Load Wallet
mandatory
At startup, your CLI must either create a wallet (EC key pair), or load it from a folder.

Implement the command wallet_load

Usage: wallet_load <path>, where:
<path> is the path to the folder from which to load the key pair (string)
Description: Load wallet (EC key pair)
Please review your task manually with the following checklist
[Prerequisites] The program compile when running make

0/4 pts
1. CLI - Save Wallet
mandatory
Implement the command wallet_save

Usage: wallet_save <path>, where:
<path> is the path to the folder in which to save the key pair (string)
Description: Save wallet (EC key pair)
Please review your task manually with the following checklist
[Send command] Try and type the command wallet_save <path>

the current wallet must be serialized at the given path
If the folder already exists, it must be overridden
Keys must be stored in the PEM format
0/1 pt
2. CLI - send
mandatory
Implement the command send

Usage: send <amount> <address>, where:
<amount> is the number of coins to send (Integer)
<address> is the EC public key of the receiver
Description: Send coins
Create a new transaction
Verify the transaction validity
Add transaction to a local list (transaction pool)
Do not update list of unspent
Please review your task manually with the following checklist
[Prerequisites] The program compile when running make

0/4 pts
3. CLI - mine
mandatory
Implement the command mine

Usage: mine
Description: Mine a block
Create a new Block using the Blockchain API
IF transactions are available in the local transaction pool, include the transactions 1 by 1 in the Block
Verify the transaction using the list of unspent outputs.
If the transaction is not valid, do not include it in the Block, and delete it
Update the list of unspent outputs after a transaction is processed
Set the difficulty of the Block using the difficulty adjustment method
Inject a coinbase transaction as the first transaction in the Block
Mine the Block (proof of work)
Verify Block validity
Add the Block to the Blockchain
Please review your task manually with the following checklist
[Mine command] Try and type the command mine (without having any transaction in the local transaction pool)

A Block containing only a coinbase transaction should be mined and added to the Blockchain (2 points)
0/9 pts
4. CLI - info
mandatory
Implement the command info

Usage: info
Description: Display information about the Blockchain, at least the following:
Display the number of Blocks in the Blockchain
Display the number of unspent transaction output
Display the number of pending transactions in the local transaction pool
Please review your task manually with the following checklist
[Info command] Try and type the command info

The number of Blocks in the Blockchain, or the content of the Blockchain is displayed (1 point)
The number of unspent transaction outputs, or the list of unspent transaction outputs is displayed (1 point)
The number of transactions, or the list of transactions in the local transaction pool is displayed (1 point)
0/9 pts
5. CLI - load
mandatory
Implement the command load

Usage: load <path>
Description: Load a Blockchain from a file
Override the local Blockchain
Please review your task manually with the following checklist
[Load command] Try and type the command load <path> with <path> pointing to a valid serialized Blockchain. Then run the command info

The Blockchain should be correctly loaded (3 points)
0/4 pts
6. CLI - save
mandatory
Implement the command save

Usage: save <path>
Description: Save the local Blockchain into a file
If the file exists, override it
Please review your task manually with the following checklist
[Save command] After mining one or several Blocks using the send and the mine commands, try and run save ./save.hblk

The local Blockchain must be correctly saved in the file save.hblk
