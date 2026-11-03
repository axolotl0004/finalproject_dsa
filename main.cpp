#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

// ======================================================
// PAUSE BEFORE RETURNING TO MENU
// ======================================================

void goBackToMenu() {
    cout << "\nPress ENTER to go back to the main menu...";
    cin.ignore();
    cin.get();
}

// ======================================================
// PRODUCT
// ======================================================

struct Product {
    int id;
    string name;
    string category;
    string brand;
    double price;
    int stock;
};

// ======================================================
// LINKED LIST - INVENTORY
// ======================================================

struct ProductNode {
    Product product;
    ProductNode* next;
};

ProductNode* head = NULL;

void addProduct(Product p) {
    ProductNode* newNode = new ProductNode;
    newNode->product = p;
    newNode->next = NULL;

    if (head == NULL) {
        head = newNode;
    } else {
        ProductNode* current = head;

        while (current->next != NULL) {
            current = current->next;
        }

        current->next = newNode;
    }
}

Product* findProduct(int id) {
    ProductNode* current = head;

    while (current != NULL) {
        if (current->product.id == id) {
            return &current->product;
        }

        current = current->next;
    }

    return NULL;
}

void showProducts() {
    ProductNode* current = head;

    cout << "\n====================================================================================================\n";
    cout << "                                      GAME INVENTORY\n";
    cout << "====================================================================================================\n";

    cout << left
         << setw(6)  << "ID"
         << setw(32) << "GAME"
         << setw(18) << "CATEGORY"
         << setw(20) << "BRAND"
         << setw(12) << "PRICE"
         << setw(8)  << "STOCK" << endl;

    cout << "----------------------------------------------------------------------------------------------------\n";

    while (current != NULL) {
        Product p = current->product;

        cout << left
             << setw(6)  << p.id
             << setw(32) << p.name
             << setw(18) << p.category
             << setw(20) << p.brand
             << "P" << setw(11) << fixed << setprecision(2) << p.price
             << setw(8)  << p.stock
             << endl;

        current = current->next;
    }

    cout << "====================================================================================================\n";
}

// ======================================================
// STACK - RECENTLY VIEWED
// ======================================================

struct StackNode {
    Product product;
    StackNode* next;
};

StackNode* top = NULL;

void push(Product p) {
    StackNode* newNode = new StackNode;
    newNode->product = p;
    newNode->next = top;
    top = newNode;
}

void showRecent() {
    if (top == NULL) {
        cout << "\nNo recently viewed games.\n";
        return;
    }

    cout << "\n============= RECENTLY VIEWED =============\n";

    StackNode* current = top;

    while (current != NULL) {
        cout << current->product.name
             << " [" << current->product.brand << "]\n";

        current = current->next;
    }
}

// ======================================================
// QUEUE - CHECKOUT
// ======================================================

struct Order {
    int productID;
    string productName;
    int quantity;
    double total;
};

struct QueueNode {
    Order order;
    QueueNode* next;
};

QueueNode* front = NULL;
QueueNode* rear = NULL;

void enqueue(Order order) {
    QueueNode* newNode = new QueueNode;
    newNode->order = order;
    newNode->next = NULL;

    if (rear == NULL) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
}

void showOrders() {
    if (front == NULL) {
        cout << "\nCheckout queue is empty.\n";
        return;
    }

    cout << "\n============= CHECKOUT QUEUE =============\n";

    QueueNode* current = front;

    while (current != NULL) {
        cout << current->order.productName
             << " | Qty: " << current->order.quantity
             << " | Total: P"
             << fixed << setprecision(2)
             << current->order.total << endl;

        current = current->next;
    }
}

void processOrder() {
    if (front == NULL) {
        cout << "\nNo orders to process.\n";
        return;
    }

    QueueNode* temp = front;

    cout << "\nProcessing: "
         << temp->order.productName << endl;

    cout << "Quantity: "
         << temp->order.quantity << endl;

    cout << "Total: P"
         << fixed << setprecision(2)
         << temp->order.total << endl;

    front = front->next;

    if (front == NULL) {
        rear = NULL;
    }

    delete temp;

    cout << "Order processed!\n";
}

// ======================================================
// BINARY SEARCH TREE - PRODUCT ID
// ======================================================

struct TreeNode {
    Product product;
    TreeNode* left;
    TreeNode* right;
};

TreeNode* root = NULL;

void insertBST(Product p) {
    TreeNode* newNode = new TreeNode;
    newNode->product = p;
    newNode->left = NULL;
    newNode->right = NULL;

    if (root == NULL) {
        root = newNode;
        return;
    }

    TreeNode* current = root;

    while (true) {
        if (p.id < current->product.id) {
            if (current->left == NULL) {
                current->left = newNode;
                break;
            }

            current = current->left;
        } else {
            if (current->right == NULL) {
                current->right = newNode;
                break;
            }

            current = current->right;
        }
    }
}

Product* searchBST(int id) {
    TreeNode* current = root;

    while (current != NULL) {
        if (id == current->product.id) {
            return &current->product;
        }

        if (id < current->product.id) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return NULL;
}

void showBST(TreeNode* current) {
    if (current == NULL) {
        return;
    }

    showBST(current->left);

    cout << current->product.id
         << " - "
         << current->product.name
         << " [" << current->product.brand << "]\n";

    showBST(current->right);
}

// ======================================================
// ADD NEW GAME TO INVENTORY
// ======================================================

void addNewGame() {
    Product newProduct;

    cout << "\n============= ADD NEW GAME =============\n";

    cout << "Enter Game ID: ";
    cin >> newProduct.id;

    if (newProduct.id <= 0) {
        cout << "\nInvalid ID. Please use a positive number.\n";
        return;
    }

    if (findProduct(newProduct.id) != NULL) {
        cout << "\nThat Game ID already exists. Please use a different ID.\n";
        return;
    }

    cin.ignore();
    cout << "Enter Game Name: ";
    getline(cin, newProduct.name);

    if (newProduct.name.empty()) {
        cout << "\nGame name cannot be empty.\n";
        return;
    }

    cout << "Enter Category (e.g. RPG, FPS, Puzzle): ";
    getline(cin, newProduct.category);

    cout << "Enter Publisher/Brand: ";
    getline(cin, newProduct.brand);

    if (newProduct.category.empty() || newProduct.brand.empty()) {
        cout << "\nCategory and brand cannot be empty.\n";
        return;
    }

    cout << "Enter Price (P): ";
    cin >> newProduct.price;

    if (newProduct.price < 0) {
        cout << "\nPrice cannot be negative.\n";
        return;
    }

    cout << "Enter Initial Stock Quantity: ";
    cin >> newProduct.stock;

    if (newProduct.stock < 0) {
        cout << "\nStock cannot be negative.\n";
        return;
    }

    // Store the game in both the linked list and the BST.
    addProduct(newProduct);
    insertBST(newProduct);

    cout << "\nGame added successfully!\n";
    cout << "Game: " << newProduct.name << endl;
    cout << "Initial stock: " << newProduct.stock << " units\n";
}

// ======================================================
// SEARCH BY NAME - LINEAR SEARCH
// ======================================================

void searchByName() {
    string name;
    bool found = false;

    cout << "\nEnter game name: ";
    cin.ignore();
    getline(cin, name);

    ProductNode* current = head;

    while (current != NULL) {
        if (current->product.name.find(name) != string::npos) {
            cout << "\nFound: "
                 << current->product.name
                 << " | Brand: "
                 << current->product.brand
                 << " | Price: P"
                 << fixed << setprecision(2)
                 << current->product.price << endl;

            push(current->product);
            found = true;
        }

        current = current->next;
    }

    if (!found) {
        cout << "\nGame not found.\n";
    }
}

// ======================================================
// FILTER BY BRAND
// ======================================================

void filterBrand() {
    string brand;
    bool found = false;

    cout << "\nBrands: Valve, Ubisoft, Electronic Arts, Bethesda\n";
    cout << "Enter brand: ";

    cin.ignore();
    getline(cin, brand);

    ProductNode* current = head;

    cout << "\n======================================================================\n";
    cout << "                         BRAND FILTER RESULTS\n";
    cout << "======================================================================\n";

    cout << left
         << setw(6)  << "ID"
         << setw(32) << "GAME"
         << setw(12) << "PRICE"
         << setw(8)  << "STOCK" << endl;

    cout << "----------------------------------------------------------------------\n";

    while (current != NULL) {
        if (current->product.brand == brand) {
            cout << left
                 << setw(6)  << current->product.id
                 << setw(32) << current->product.name
                 << "P" << setw(11)
                 << fixed << setprecision(2)
                 << current->product.price
                 << setw(8) << current->product.stock
                 << endl;

            found = true;
        }

        current = current->next;
    }

    if (!found) {
        cout << "\nNo games found for that brand.\n";
    }

    cout << "======================================================================\n";
}

// ======================================================
// SORT BY PRICE - BUBBLE SORT
// ======================================================

void sortByPrice() {
    if (head == NULL) {
        return;
    }

    bool swapped;

    do {
        swapped = false;

        ProductNode* current = head;

        while (current->next != NULL) {
            if (current->product.price >
                current->next->product.price) {

                Product temp = current->product;
                current->product = current->next->product;
                current->next->product = temp;

                swapped = true;
            }

            current = current->next;
        }

    } while (swapped);

    cout << "\nGames sorted by price!\n";
    showProducts();
}

// ======================================================
// BUY GAME
// ======================================================

void buyGame() {
    int id;
    int quantity;

    cout << "\nEnter Game ID: ";
    cin >> id;

    Product* product = findProduct(id);

    if (product == NULL) {
        cout << "\nGame not found.\n";
        return;
    }

    cout << "Game: " << product->name << endl;
    cout << "Brand: " << product->brand << endl;
    cout << "Price: P" << fixed << setprecision(2)
         << product->price << endl;
    cout << "Stock: " << product->stock << endl;

    cout << "\nEnter quantity: ";
    cin >> quantity;

    if (quantity <= 0) {
        cout << "\nInvalid quantity.\n";
        return;
    }

    if (quantity > product->stock) {
        cout << "\nNot enough stock.\n";
        return;
    }

    Order order;

    order.productID = product->id;
    order.productName = product->name;
    order.quantity = quantity;
    order.total = product->price * quantity;

    enqueue(order);

    product->stock -= quantity;

    push(*product);

    cout << "\nAdded to checkout queue!\n";
    cout << "Total: P"
         << fixed << setprecision(2)
         << order.total << endl;

    cout << "Remaining stock: "
         << product->stock << endl;
}

// ======================================================
// SAMPLE PRODUCTS
// ======================================================

void loadProducts() {
    Product p1 = {101, "Counter-Strike 2", "FPS", "Valve", 0.00, 999};
    Product p2 = {102, "Half-Life 2", "FPS", "Valve", 499.00, 25};
    Product p3 = {103, "Portal 2", "Puzzle", "Valve", 299.00, 30};
    Product p4 = {104, "Left 4 Dead 2", "Co-op", "Valve", 399.00, 35};

    Product p5 = {201, "Assassin's Creed Mirage", "Action", "Ubisoft", 2499.00, 12};
    Product p6 = {202, "Far Cry 6", "FPS", "Ubisoft", 1999.00, 15};

    Product p7 = {301, "EA SPORTS FC 26", "Sports", "Electronic Arts", 3499.00, 10};
    Product p8 = {302, "Battlefield 2042", "FPS", "Electronic Arts", 1499.00, 8};

    Product p9 = {401, "DOOM Eternal", "FPS", "Bethesda", 1299.00, 20};
    Product p10 = {402, "The Elder Scrolls V: Skyrim", "RPG", "Bethesda", 999.00, 18};

    addProduct(p1);
    addProduct(p2);
    addProduct(p3);
    addProduct(p4);
    addProduct(p5);
    addProduct(p6);
    addProduct(p7);
    addProduct(p8);
    addProduct(p9);
    addProduct(p10);

    insertBST(p1);
    insertBST(p2);
    insertBST(p3);
    insertBST(p4);
    insertBST(p5);
    insertBST(p6);
    insertBST(p7);
    insertBST(p8);
    insertBST(p9);
    insertBST(p10);
}

// ======================================================
// CLEAN UP MEMORY
// ======================================================

void deleteInventory() {
    while (head != NULL) {
        ProductNode* temp = head;
        head = head->next;
        delete temp;
    }
}

void deleteStack() {
    while (top != NULL) {
        StackNode* temp = top;
        top = top->next;
        delete temp;
    }
}

void deleteQueue() {
    while (front != NULL) {
        QueueNode* temp = front;
        front = front->next;
        delete temp;
    }

    rear = NULL;
}

void deleteBST(TreeNode* current) {
    if (current == NULL) {
        return;
    }

    deleteBST(current->left);
    deleteBST(current->right);

    delete current;
}

// ======================================================
// REMOVE GAME FROM INVENTORY
// ======================================================

void removeGame() {
    int id;

    cout << "\n============= REMOVE GAME =============\n";
    cout << "Enter Game ID to remove: ";
    cin >> id;

    ProductNode* current = head;
    ProductNode* previous = NULL;

    // Find the game in the linked list.
    while (current != NULL && current->product.id != id) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        cout << "\nGame not found!\n";
        return;
    }

    cout << "\nGame found: " << current->product.name << endl;
    cout << "Brand: " << current->product.brand << endl;

    // Remove the game from the linked-list inventory.
    if (previous == NULL) {
        head = current->next;
    } else {
        previous->next = current->next;
    }

    delete current;

    // Rebuild the BST so the removed game cannot be found by ID.
    deleteBST(root);
    root = NULL;

    ProductNode* inventoryItem = head;
    while (inventoryItem != NULL) {
        insertBST(inventoryItem->product);
        inventoryItem = inventoryItem->next;
    }

    cout << "\nGame removed successfully!\n";
}

// ======================================================
// MENU
// ======================================================

void showMenu() {
    cout << "\n\n============================================\n";
    cout << "                 GAMEHUB\n";
    cout << "============================================\n";
    cout << "      Gaming Marketplace & Inventory System\n";
    cout << "      Brands: Valve | Ubisoft | EA | Bethesda\n";
    cout << "============================================\n";
    cout << "1. Show All Games\n";
    cout << "2. Search Game by Name\n";
    cout << "3. Search Game by ID (BST)\n";
    cout << "4. Filter by Brand\n";
    cout << "5. Sort by Price\n";
    cout << "6. Buy Game\n";
    cout << "7. View Checkout Queue\n";
    cout << "8. Process Order\n";
    cout << "9. Recently Viewed Games\n";
    cout << "10. Show BST In-Order\n";
    cout << "11. Add New Game\n";
    cout << "12. Remove Game\n";
    cout << "0. Exit\n";
    cout << "============================================\n";
    cout << "Choose: ";
}

// ======================================================
// MAIN
// ======================================================

int main() {
    loadProducts();

    int choice;

    do {
        showMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                showProducts();
                goBackToMenu();
                break;

            case 2:
                searchByName();
                goBackToMenu();
                break;

            case 3: {
                int id;

                cout << "\nEnter Game ID: ";
                cin >> id;

                Product* result = searchBST(id);

                if (result != NULL) {
                    cout << "\nGame found: "
                         << result->name
                         << " | "
                         << result->brand
                         << " | P"
                         << fixed << setprecision(2)
                         << result->price << endl;

                    push(*result);
                } else {
                    cout << "\nGame not found.\n";
                }

                goBackToMenu();
                break;
            }

            case 4:
                filterBrand();
                goBackToMenu();
                break;

            case 5:
                sortByPrice();
                goBackToMenu();
                break;

            case 6:
                buyGame();
                goBackToMenu();
                break;

            case 7:
                showOrders();
                goBackToMenu();
                break;

            case 8:
                processOrder();
                goBackToMenu();
                break;

            case 9:
                showRecent();
                goBackToMenu();
                break;

            case 10:
                cout << "\n============= BST IN-ORDER =============\n";
                showBST(root);
                goBackToMenu();
                break;

            case 11:
                addNewGame();
                goBackToMenu();
                break;

            case 12:
                removeGame();
                goBackToMenu();
                break;

            case 0:
                cout << "\nThank you for using GameHub!\n";
                break;

            default:
                cout << "\nInvalid choice.\n";
                goBackToMenu();
        }

    } while (choice != 0);

    deleteInventory();
    deleteStack();
    deleteQueue();
    deleteBST(root);

    return 0;
}
