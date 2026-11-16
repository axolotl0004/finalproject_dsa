# GameHub - Gaming Marketplace & Inventory System

## Project

A beginner-friendly C++ console program for a DSA final activity.

**Domain:** Gaming / Video Games  
**Marketplace name:** GameHub  
**Participating brands:** Valve, Ubisoft, Electronic Arts, Bethesda

## Why these data structures are used

### 1. Linked List
The linked list stores the game inventory.

Each node contains:
- one Product
- a pointer to the next node

### 2. Stack
The stack stores recently viewed games.

It follows **LIFO (Last In, First Out)**.

The `push()` function adds a recently viewed game.

### 3. Queue
The queue stores checkout orders.

It follows **FIFO (First In, First Out)**.

The `enqueue()` function adds an order and `processOrder()` removes the first order.

### 4. Binary Search Tree
The BST stores games using their ID.

- Smaller ID -> left
- Larger ID -> right

`searchBST()` searches for a game by ID.

### 5. Linear Search
`searchByName()` checks the inventory one product at a time to find a game name.

### 6. Bubble Sort
`sortByPrice()` uses Bubble Sort to arrange games from lowest to highest price.

### 7. Brand Filtering
`filterBrand()` displays products belonging to Valve, Ubisoft, Electronic Arts, or Bethesda.

### 8. Add New Game
Choose **11. Add New Game** from the main menu. The program asks for the game's ID, name, category, publisher/brand, price, and initial stock quantity. It checks that the ID is not already used and that the price and stock are not negative.

The new game is added to both the linked-list inventory and the Binary Search Tree, so it appears in the inventory and can also be found through BST ID search.

### 10. Remove Game
Choose **12. Remove Game** from the main menu and enter the game's ID. The program removes the matching game from the linked-list inventory. It then rebuilds the Binary Search Tree from the remaining inventory, so the removed game is no longer available through BST ID search.

### 9. Dynamic Memory
The linked list, stack, queue, and BST use dynamically allocated nodes.

The cleanup functions use `delete` to free the allocated memory.

## Running in VS Code

Open the folder in Visual Studio Code.

Open the terminal with:

Ctrl + `

Compile:

g++ main.cpp -o gamehub

Run:

.\gamehub.exe

## Suggested demo

1. Show all games.
2. Search for `Portal`.
3. Search ID `103` using the BST.
4. Filter by `Valve`, `Ubisoft`, `Electronic Arts`, or `Bethesda`.
5. Sort by price.
6. Buy game ID `102`, quantity `2`.
7. View the checkout queue.
8. Add another order.
9. Process the first order to demonstrate FIFO.
10. View recently viewed games to demonstrate the stack.
11. Show the BST in-order traversal.
12. Add a new game and enter its initial stock quantity.
13. Show the inventory again and search for the new game by ID.
14. Remove a game by ID and confirm it no longer appears in the inventory or BST search.
15. Exit.

## Important note

The Lab 1-8 manuals were not available. This version is therefore based on the requirements explicitly visible in the provided final-output document, including Lists, Stacks, Queues, BSTs, Searching, Sorting, brand filtering, checkout processing, stock updates, and memory management.

## Menu pause

After each menu operation, the program waits for:

`Press ENTER to go back to the main menu...`

This gives you time to read the product/search/order results before the menu is shown again.


## Display formatting

The inventory and brand-filter results use fixed-width columns so IDs, game names, categories, brands, prices, and stock quantities line up neatly in the console.


## Adding games

Select option `11. Add New Game`. Enter a unique positive Game ID, the game name, category, publisher/brand, price, and starting stock quantity. The new game is stored in the linked list and inserted into the BST. The inventory is held in memory while the program is running; newly added games are not saved to a file after the program closes.
