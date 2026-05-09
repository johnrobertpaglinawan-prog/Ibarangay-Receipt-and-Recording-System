BARANGAY RECEIPT AND RECORDING SYSTEM

The Ibarangay Database Management System is a modern, terminal-based application written in C. It is designed to act as a lightweight, highly efficient Local Government Unit (LGU) management tool. Its primary purpose is to allow barangay administrators to register citizens, securely manage demographic data, process legal document requests, and generate official transaction receipts.

Data Structures and Algorithms

This system abandons static arrays in favor of Dynamic Memory Allocation, utilizing several core Data Structures and Algorithms to power its features.

Data Structures

Singly Linked List: Acts as the primary database storage (Resident nodes). It allows the system to scale infinitely without wasting unused memory. Supports: Add New Resident, View Database, and Delete Record.

Queue (FIFO): Manages the logical flow of citizens waiting for document processing. Supports: The Transaction Engine (First-Come, First-Served logic).

Stack (LIFO): Stores generated official receipts. Supports: The Receipt Generation module, ensuring the most recent transaction is always placed at the top of the history stack.

Algorithms

Linear Search: Traverses the linked list node-by-node to match integer queries. Supports Query by ID, locating target nodes for Deletion, and verifying IDs for Transactions.

Bubble Sort: An in-place sorting mechanism that compares adjacent node data and swaps them to achieve alphabetical order. Supports the View Indexed Database feature (alphabetizing the directory).

Head Insertion: Instantly binds new memory allocations to the front of the database. Supports the Add New Resident feature.
