
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

// GAME INFORMATION
struct Product {
    int id;
    string name, category, brand;
    double price;
    int stock;
};

// INVENTORY: SINGLY LINKED LIST
struct ProductNode {
    Product data;
    ProductNode* next;
};

ProductNode* inventory = NULL;

// BINARY SEARCH TREE: SEARCH GAMES BY ID
struct BSTNode {
    int id;
    ProductNode* product;
    BSTNode* left;
    BSTNode* right;
};

BSTNode* root = NULL;

// INSERT GAME INTO BST
void insertBST(BSTNode*& node, ProductNode* product) {
    if (node == NULL) {
        node = new BSTNode{
            product->data.id, product, NULL, NULL
        };
    }
    else if (product->data.id < node->id) {
        insertBST(node->left, product);
    }
    else if (product->data.id > node->id) {
        insertBST(node->right, product);
    }
}

// SEARCH GAME IN BST
Product* searchBST(BSTNode* node, int id) {
    if (node == NULL)
        return NULL;

    if (node->id == id)
        return &node->product->data;

    if (id < node->id)
        return searchBST(node->left, id);

    return searchBST(node->right, id);
}

// FREE BST MEMORY
void clearBST(BSTNode* node) {
    if (node == NULL)
        return;

    clearBST(node->left);
    clearBST(node->right);
    delete node;
}

// REBUILD BST AFTER REMOVING A GAME
void rebuildBST() {
    clearBST(root);
    root = NULL;

    for (ProductNode* p = inventory; p != NULL; p = p->next)
        insertBST(root, p);
}

// FIND GAME USING BST
Product* findByID(int id) {
    return searchBST(root, id);
}

// ADD GAME TO INVENTORY AND BST
void insertProduct(Product item) {
    ProductNode* node = new ProductNode{item, NULL};

    if (inventory == NULL) {
        inventory = node;
    }
    else {
        ProductNode* p = inventory;

        while (p->next != NULL)
            p = p->next;

        p->next = node;
    }

    insertBST(root, node);
}

// REMOVE GAME FROM INVENTORY
bool removeProduct(int id, Product& removed) {
    ProductNode* current = inventory;
    ProductNode* previous = NULL;

    while (current != NULL) {
        if (current->data.id == id) {
            removed = current->data;

            if (previous == NULL)
                inventory = current->next;
            else
                previous->next = current->next;

            delete current;

            rebuildBST();
            return true;
        }

        previous = current;
        current = current->next;
    }

    return false;
}

// DISPLAY TABLE HEADER
void printHeader() {
    cout << "\n"
         << left << setw(7) << "ID"
         << setw(31) << "GAME"
         << setw(17) << "CATEGORY"
         << setw(19) << "BRAND"
         << setw(12) << "PRICE"
         << "STOCK\n";

    cout << string(94, '-') << "\n";
}

// DISPLAY ONE GAME
void printProduct(const Product& p) {
    cout << left << setw(7) << p.id
         << setw(31) << p.name
         << setw(17) << p.category
         << setw(19) << p.brand
         << "P" << setw(11) << fixed << setprecision(2)
         << p.price
         << p.stock << "\n";
}

// COPY INVENTORY INTO VECTOR
vector<Product> getProducts() {
    vector<Product> items;

    for (ProductNode* p = inventory; p != NULL; p = p->next)
        items.push_back(p->data);

    return items;
}

// 1. VIEW COMPLETE GAME CATALOG
void showCatalog() {
    cout << "\n========== GAMEHUB GAME CATALOG ==========\n";

    printHeader();

    for (ProductNode* p = inventory; p != NULL; p = p->next)
        printProduct(p->data);
}

// 2. FILTER GAMES BY BRAND
void filterBrand() {
    string brand;

    cout << "\nEnter game publisher/brand: ";
    cin.ignore();
    getline(cin, brand);

    bool found = false;

    printHeader();

    for (ProductNode* p = inventory; p != NULL; p = p->next) {
        if (p->data.brand == brand) {
            printProduct(p->data);
            found = true;
        }
    }

    if (!found)
        cout << "No games found for that brand.\n";
}

// 3. SEARCH GAMES BY NAME (LINEAR SEARCH)
void searchKeyword() {
    string keyword;

    cout << "\nEnter game name or keyword: ";
    cin.ignore();
    getline(cin, keyword);

    bool found = false;

    for (ProductNode* p = inventory; p != NULL; p = p->next) {
        if (p->data.name.find(keyword) != string::npos) {
            cout << "\nFound Game: " << p->data.name
                 << "\nID: " << p->data.id
                 << "\nBrand: " << p->data.brand
                 << "\nPrice: P" << fixed << setprecision(2)
                 << p->data.price << "\n";

            found = true;
        }
    }

    if (!found)
        cout << "No matching game found.\n";
}

// 4. SEARCH GAMES BY PRICE RANGE (BINARY SEARCH)
void filterPriceRange() {
    double low, high;

    cout << "\nMinimum game price: P";
    cin >> low;

    cout << "Maximum game price: P";
    cin >> high;

    if (low > high || low < 0) {
        cout << "Invalid price range.\n";
        return;
    }

    vector<Product> items = getProducts();

    sort(items.begin(), items.end(),
         [](const Product& a, const Product& b) {
             return a.price < b.price;
         });

    int left = 0;
    int right = (int)items.size();

    // FIND FIRST PRICE GREATER THAN OR EQUAL TO LOW
    while (left < right) {
        int mid = left + (right - left) / 2;

        if (items[mid].price < low)
            left = mid + 1;
        else
            right = mid;
    }

    int first = left;

    left = 0;
    right = (int)items.size();

    // FIND FIRST PRICE GREATER THAN HIGH
    while (left < right) {
        int mid = left + (right - left) / 2;

        if (items[mid].price <= high)
            left = mid + 1;
        else
            right = mid;
    }

    int last = left;

    if (first == last) {
        cout << "No games found in that price range.\n";
        return;
    }

    printHeader();

    for (int i = first; i < last; i++)
        printProduct(items[i]);
}

// SHOPPING CART: DOUBLY LINKED LIST
struct CartNode {
    Product item;
    int quantity;
    CartNode* next;
    CartNode* prev;
};

CartNode* cartHead = NULL;
CartNode* cartTail = NULL;

// 5. ADD GAME TO SHOPPING CART
void addToCart() {
    int id, quantity;

    cout << "\nEnter Game ID: ";
    cin >> id;

    Product* item = findByID(id);

    if (item == NULL) {
        cout << "Game not found.\n";
        return;
    }

    cout << "Enter quantity: ";
    cin >> quantity;

    if (quantity <= 0 || quantity > item->stock) {
        cout << "Invalid quantity or insufficient stock.\n";
        return;
    }

    CartNode* node = new CartNode{
        *item, quantity, NULL, cartTail
    };

    if (cartHead == NULL)
        cartHead = node;
    else
        cartTail->next = node;

    cartTail = node;

    cout << item->name << " added to shopping cart.\n";
}

// NAVIGATE SHOPPING CART
void browseCart() {
    if (cartHead == NULL) {
        cout << "\nYour shopping cart is empty.\n";
        return;
    }

    CartNode* current = cartHead;
    char direction;

    do {
        cout << "\nGame: " << current->item.name
             << "\nQuantity: " << current->quantity
             << "\nSubtotal: P" << fixed << setprecision(2)
             << current->item.price * current->quantity
             << "\n";

        cout << "\n[N]ext, [P]revious, [Q]uit browsing: ";
        cin >> direction;

        if ((direction == 'N' || direction == 'n')
            && current->next != NULL) {
            current = current->next;
        }
        else if ((direction == 'P' || direction == 'p')
                 && current->prev != NULL) {
            current = current->prev;
        }
        else if (direction != 'Q' && direction != 'q') {
            cout << "No item in that direction.\n";
        }

    } while (direction != 'Q' && direction != 'q');
}

// CHECKOUT: FIFO QUEUE
struct Order {
    Product item;
    int quantity;
};

struct OrderNode {
    Order data;
    OrderNode* next;
};

OrderNode* orderFront = NULL;
OrderNode* orderRear = NULL;

// 6. PROCESS CHECKOUT
void checkoutCart() {
    if (cartHead == NULL) {
        cout << "\nShopping cart is empty.\n";
        return;
    }

    // CHECK STOCK BEFORE CHECKOUT
    for (CartNode* c = cartHead; c != NULL; c = c->next) {
        Product* live = findByID(c->item.id);

        if (live == NULL || c->quantity > live->stock) {
            cout << "Checkout stopped: stock changed for "
                 << c->item.name << ".\n";
            return;
        }
    }

    CartNode* c = cartHead;

    while (c != NULL) {
        Product* live = findByID(c->item.id);

        live->stock -= c->quantity;

        OrderNode* node = new OrderNode{
            {c->item, c->quantity}, NULL
        };

        if (orderRear == NULL) {
            orderFront = orderRear = node;
        }
        else {
            orderRear->next = node;
            orderRear = node;
        }

        CartNode* old = c;
        c = c->next;
        delete old;
    }

    cartHead = cartTail = NULL;

    cout << "\nCheckout added to order queue.\n";
    cout << "Game stock has been updated.\n";
}

// PROCESS THE FIRST CHECKOUT ORDER
void processCheckout() {
    if (orderFront == NULL) {
        cout << "\nThere are no checkout orders.\n";
        return;
    }

    OrderNode* old = orderFront;

    cout << "\nProcessing Order: "
         << old->data.item.name
         << "\nQuantity: " << old->data.quantity
         << "\nTotal: P" << fixed << setprecision(2)
         << old->data.item.price * old->data.quantity
         << "\n";

    orderFront = orderFront->next;

    if (orderFront == NULL)
        orderRear = NULL;

    delete old;

    cout << "Order processed successfully (FIFO).\n";
}

// ADMIN UNDO: STACK
struct AdminAction {
    char type;
    Product item;
};

struct AdminNode {
    AdminAction action;
    AdminNode* next;
};

AdminNode* adminTop = NULL;

// PUSH ACTION INTO STACK
void pushAdmin(char type, Product item) {
    adminTop = new AdminNode{
        {type, item}, adminTop
    };
}

// ADMIN COMMAND: ADD NEW GAME
void addGameAdmin() {
    Product p;

    cout << "\n========== ADD NEW GAME ==========\n";

    cout << "New Game ID: ";
    cin >> p.id;

    if (p.id <= 0 || findByID(p.id) != NULL) {
        cout << "Invalid or duplicate Game ID.\n";
        return;
    }

    cin.ignore();

    cout << "Game name: ";
    getline(cin, p.name);

    cout << "Category: ";
    getline(cin, p.category);

    cout << "Publisher/brand: ";
    getline(cin, p.brand);

    cout << "Price: P";
    cin >> p.price;

    cout << "Initial stock: ";
    cin >> p.stock;

    if (p.name.empty() || p.category.empty()
        || p.brand.empty() || p.price < 0 || p.stock < 0) {
        cout << "Invalid game details. Game was not added.\n";
        return;
    }

    insertProduct(p);

    // SAVE ACTION TO UNDO STACK
    pushAdmin('A', p);

    cout << "\nGame successfully added to GameHub!\n";
}

// ADMIN COMMAND: REMOVE GAME
void removeGameAdmin() {
    int id;
    Product removed;

    cout << "\n========== REMOVE GAME ==========\n";

    cout << "Enter Game ID to remove: ";
    cin >> id;

    if (!removeProduct(id, removed)) {
        cout << "Game not found.\n";
        return;
    }

    // SAVE ACTION TO UNDO STACK
    pushAdmin('R', removed);

    cout << removed.name << " has been removed.\n";
}

// ADMIN COMMAND: UNDO LAST ACTION
void undoAdminAction() {
    if (adminTop == NULL) {
        cout << "\nNo admin action to undo.\n";
        return;
    }

    AdminNode* old = adminTop;
    AdminAction action = old->action;

    adminTop = adminTop->next;

    if (action.type == 'A') {
        Product ignored;
        removeProduct(action.item.id, ignored);

        cout << "Undo complete: added game was removed.\n";
    }
    else {
        insertProduct(action.item);

        cout << "Undo complete: removed game was restored.\n";
    }

    delete old;
}

// 7. ADMIN COMMANDS MENU
void adminMenu() {
    int choice;

    do {
        cout << "\n====================================\n";
        cout << "           ADMIN COMMANDS\n";
        cout << "====================================\n";
        cout << "1. Add New Game\n";
        cout << "2. Remove Game\n";
        cout << "3. Undo Last Admin Action (Stack Pop)\n";
        cout << "0. Return to Main Menu\n";
        cout << "====================================\n";
        cout << "Choose: ";

        cin >> choice;

        switch (choice) {
            case 1:
                addGameAdmin();
                break;

            case 2:
                removeGameAdmin();
                break;

            case 3:
                undoAdminAction();
                break;

            case 0:
                cout << "Returning to GameHub menu...\n";
                break;

            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 0);
}

// 8. PARTNER BRANDS: CIRCULAR LINKED LIST
struct BrandNode {
    string name;
    BrandNode* next;
};

BrandNode* brandHead = NULL;

// ADD PARTNER BRAND
void addBrand(string name) {
    BrandNode* node = new BrandNode{name, NULL};

    if (brandHead == NULL) {
        brandHead = node;
        node->next = node;
    }
    else {
        BrandNode* p = brandHead;

        while (p->next != brandHead)
            p = p->next;

        p->next = node;
        node->next = brandHead;
    }
}

// DISPLAY PARTNER BRANDS
void showBrandBanners() {
    if (brandHead == NULL)
        return;

    cout << "\n========== GAMEHUB PARTNER BRANDS ==========\n";

    BrandNode* p = brandHead;

    do {
        cout << "[ " << p->name << " ]  ";
        p = p->next;
    } while (p != brandHead);

    cout << "\nReturned to the first brand (Circular List).\n";
}

// 9. SORT GAME INVENTORY
void sortInventory() {
    vector<Product> items = getProducts();

    if (items.empty()) {
        cout << "Inventory is empty.\n";
        return;
    }

    int choice;

    cout << "\n========== SORT GAME INVENTORY ==========\n";
    cout << "1. Sort by Price\n";
    cout << "2. Sort by Stock\n";
    cout << "3. Sort by Brand\n";
    cout << "Choose: ";

    cin >> choice;

    if (choice == 1) {
        sort(items.begin(), items.end(),
             [](const Product& a, const Product& b) {
                 return a.price < b.price;
             });
    }
    else if (choice == 2) {
        sort(items.begin(), items.end(),
             [](const Product& a, const Product& b) {
                 return a.stock < b.stock;
             });
    }
    else if (choice == 3) {
        sort(items.begin(), items.end(),
             [](const Product& a, const Product& b) {
                 return a.brand < b.brand;
             });
    }
    else {
        cout << "Invalid sorting choice.\n";
        return;
    }

    printHeader();

    for (const Product& p : items)
        printProduct(p);
}

// LOAD SAMPLE GAMES
void loadSampleGames() {
    insertProduct({101, "Counter-Strike 2", "FPS", "Valve", 0.00, 50});
    insertProduct({102, "Half-Life 2", "Action", "Valve", 299.00, 20});
    insertProduct({103, "Portal 2", "Puzzle", "Valve", 249.00, 18});
    insertProduct({104, "Left 4 Dead 2", "FPS", "Valve", 199.00, 25});

    insertProduct({201, "Assassin's Creed Mirage", "Action", "Ubisoft", 1799.00, 12});
    insertProduct({202, "Far Cry 6", "FPS", "Ubisoft", 1499.00, 10});

    insertProduct({301, "EA SPORTS FC 26", "Sports", "Electronic Arts", 2999.00, 15});
    insertProduct({302, "Battlefield 2042", "FPS", "Electronic Arts", 999.00, 8});

    insertProduct({401, "DOOM Eternal", "FPS", "Bethesda", 899.00, 14});
    insertProduct({402, "The Elder Scrolls V: Skyrim", "RPG", "Bethesda", 799.00, 16});

    addBrand("Valve");
    addBrand("Ubisoft");
    addBrand("Electronic Arts");
    addBrand("Bethesda");
}

// PAUSE BEFORE RETURNING TO MENU
void pauseMenu() {
    cout << "\nPress ENTER to return to the main menu...";
    cin.ignore();
    cin.get();
}

// MAIN MENU
void showMenu() {
    cout << "\n===============================================================\n";
    cout << "                 GAMEHUB - GAME MARKETPLACE\n";
    cout << "===============================================================\n";

    cout << "1. View Complete Game Catalog\n";
    cout << "2. Filter Games by Brand\n";
    cout << "3. Search Games by Name (Linear Search)\n";
    cout << "4. Search Games by Price Range (Binary Search)\n";
    cout << "5. Manage Shopping Cart (Doubly Linked List)\n";
    cout << "6. Process Checkout (FIFO Queue)\n";
    cout << "7. Admin Commands / Undo Action (LIFO Stack)\n";
    cout << "8. Display Partner Brands (Circular Linked List)\n";
    cout << "9. Sort Game Inventory by Price, Stock, or Brand\n";
    cout << "10. Exit\n";

    cout << "===============================================================\n";
    cout << "Choose: ";
}

// FREE DYNAMIC MEMORY
void cleanUp() {
    // FREE BST FIRST, BEFORE DELETING INVENTORY NODES
    clearBST(root);
    root = NULL;

    while (inventory != NULL) {
        ProductNode* old = inventory;
        inventory = inventory->next;
        delete old;
    }

    while (cartHead != NULL) {
        CartNode* old = cartHead;
        cartHead = cartHead->next;
        delete old;
    }

    while (orderFront != NULL) {
        OrderNode* old = orderFront;
        orderFront = orderFront->next;
        delete old;
    }

    while (adminTop != NULL) {
        AdminNode* old = adminTop;
        adminTop = adminTop->next;
        delete old;
    }

    if (brandHead != NULL) {
        BrandNode* p = brandHead->next;

        while (p != brandHead) {
            BrandNode* old = p;
            p = p->next;
            delete old;
        }

        delete brandHead;
        brandHead = NULL;
    }
}

// MAIN PROGRAM
int main() {
    loadSampleGames();

    int choice;

    do {
        showMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                showCatalog();
                pauseMenu();
                break;

            case 2:
                filterBrand();
                pauseMenu();
                break;

            case 3:
                searchKeyword();
                pauseMenu();
                break;

            case 4:
                filterPriceRange();
                pauseMenu();
                break;

            case 5:
                addToCart();
                browseCart();
                pauseMenu();
                break;

            case 6:
                checkoutCart();
                processCheckout();
                pauseMenu();
                break;

            case 7:
                adminMenu();
                break;

            case 8:
                showBrandBanners();
                pauseMenu();
                break;

            case 9:
                sortInventory();
                pauseMenu();
                break;

            case 10:
                cout << "\nThank you for using GameHub!\n";
                break;

            default:
                cout << "Invalid choice.\n";
                pauseMenu();
        }

    } while (choice != 10);

    cleanUp();

    return 0;
}
