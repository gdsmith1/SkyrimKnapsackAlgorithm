/*
knapsack.cpp - Skyrim Knapsack Algorithm
Gibson Smith
gdsmith1 - https://github.com/gdsmith1
Version Completed 8/17/2023
*/

#include<iostream>
#include<sstream>
#include<fstream>
#include<string>
#include<vector>
using namespace std;

class invItem{ // read in by makeInv
    public:
        string id;
        int quantity;
};
class knapsackItem{ // converted from invItem in readCsv
    public:
        string id;
        string name;
        double weight;
        int value;
};



void makeInv(string inputfilename, vector<invItem>& items);
void readCsv(vector<knapsackItem>& inventory, vector<invItem>& items);
vector<knapsackItem> knapsack(vector<knapsackItem>& inventory, int capacity);
void backtrack(vector<knapsackItem>& inventory, vector<vector<int>>& B, int i, int j, vector<knapsackItem>& keep);



int main() {
    vector<invItem> inputs; // the inventory in the format given by player.showinventory
    vector<knapsackItem> inventory; // a detailed version of the player's inventory using data from data.csv
    string inventoryfile = "";
    double weight = 0;
    // User inputs file for inventory and character's carry limit
    cout << "Enter your inventory file:\n";
    cin >> inventoryfile;
    cout << "Enter your carry limit (integer):\n";
    cin >> weight;
    if(weight <= 0) {
        cerr << "INVALID WEIGHT VALUE.  EXITING...\n";
        exit(1);
    }

    // Reads the file given by the user and creates a list of the data it pulls
    makeInv(inventoryfile, inputs);

    // Compares the previous list with the data.csv file, creates a new list of items individually listed
    // Ignores weightless items and items not found in the database
    readCsv(inventory, inputs);

    // prints the inventory for user
    double totalweight = 0; // used for printing the total weight of inventory
    for(int i = 0; i < (int)inventory.size(); i++) {
        if(i == (int)inventory.size() - 1) {
            cout << inventory[i].name << endl;
        } else {
            cout << inventory[i].name << ", ";
        }
        totalweight += inventory[i].weight;
    }
    cout << "Total weight: " << totalweight << endl;
    cout << (int)inventory.size() << " items\n";

    vector<knapsackItem> keep = knapsack(inventory, weight);

    // Results
    cout << "Items to keep:\n"; // keep vector is in reverse order of inventory vector
    totalweight = 0;
    for(int i = (int)keep.size() - 1; i >= 0; i--) {
        if(i == 0) {
            cout << keep[i].name << endl;
        } else {
            cout << keep[i].name << ", ";
        }
        totalweight += keep[i].weight;
    }
    cout << "Total weight: " << totalweight << endl;
    cout << keep.size() << " items\n";

    return 0;
}



// Implements the recursive relation to read the entire list, map it into a value table and backtracking table,
// then calls the backtrack() function to recursively read the results
vector<knapsackItem> knapsack(vector<knapsackItem>& inventory, int capacity) {
    int size = inventory.size();
    capacity++; // this accounts for the 0th value during iteration
    vector<vector<int>> V; // value table - values represent total value of items selected
    vector<vector<int>> B; // backtrack table - values represent row of item selected

    V.resize(size);
    B.resize(size);
    for(int i = 0; i < size; i++) {
        V[i].resize(capacity);
        B[i].resize(capacity);
        for(int j = 0; j < capacity; j++) {

            if(i == 0) { // first item in knapsack
                if(inventory[0].weight > j) {
                    V[0][j] = 0;
                    B[i][j] = 0;
                } else {
                    V[0][j] = inventory[i].value;
                    B[i][j] = j - inventory[i].weight;
                }
            } else {
                if(inventory[i].weight > j) {
                    V[i][j] = V[i-1][j];
                    B[i][j] = j;
                } else {
                    if(V[i-1][j] >= V[i-1][j-inventory[i].weight] + inventory[i].value) {
                        V[i][j] = V[i-1][j];
                        B[i][j] = j;
                    } else {
                        V[i][j] = V[i-1][j-inventory[i].weight];
                        B[i][j] = j-inventory[i].weight;
                    }
                }
            }

        }
    }

    /*// Table Printing Debug
    cout << "values:\n";
    for(int i = 0; i < size; i++) {
        cout << inventory[i].value << ", ";
    }
    cout << "\nweights:\n";
    for(int i = 0; i < size; i++) {
        cout << inventory[i].weight << ", ";
    }
    cout << endl;
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < capacity; j++) {
            cout << V[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < capacity; j++) {
            cout << B[i][j] << " ";
        }
        cout << endl;
    }*/

    // Runs backtrack and returns list of items to keep
    vector<knapsackItem> keep;
    backtrack(inventory, B, size-1, capacity-1, keep);
    return keep;
}



// Recursively backtracks through the B table to fill the list of items to keep
void backtrack(vector<knapsackItem>& inventory, vector<vector<int>>& B, int i, int j, vector<knapsackItem>& keep) {
    if(B[i][j] != j) { // took item
        keep.push_back(inventory[i]);
    }
    if(i == 0) {
        return;
    }
    backtrack(inventory, B, i-1, B[i][j], keep);
}




// Reads a list of item ids and compares them to the data.csv file to import them into the inventory list
// Ignores irrelevant data such as objects that have no weight (and could be kept regardless) and incomplete entries
// data.csv assumes load order is default, IE no extra mods before dlcs
// data is read in format: "ID,Name,Weight,Value"
void readCsv(vector<knapsackItem>& inventory, vector<invItem>& items) {
    int size = items.size();
    for(int i = 0; i < size; i++) {
        ifstream file;
        string word;
        file.open("data.csv");
        string line;
        string id;
        string name;
        int value;
        int weight;
        knapsackItem conversion;
        if(file.is_open()){
            id = "";
            while(getline(file, line)) {
                if(id != "") {
                    break;
                }
                id = line.substr(0, 8); // item ids are 8 characters
                for(int j = 0; j < 8; j++) { //string compare that converts items[i].id to lowercase
                    if(id[j] != tolower(items[i].id[j])) {
                        id = "";
                        break;
                    }
                    if(j==7) { // all characters match
                        //cout << "found item " << id << "!\n    Quantity - " << items[i].quantity << endl; // debug
                        istringstream cur(line); // converts current line to a stream
                        for(int k = 0; k < 4; k++) { // reads line as a stream and assigns variables
                            getline(cur, word, ',');
                            if(k == 1) {
                                name = word;
                            } else if(k == 2) {
                                weight = stoi(word);
                            } else if(k == 3) {
                                value = stoi(word);
                            }
                        }
                        for(int k = 0; k < items[i].quantity; k++) {
                            if(weight == 0) { // item was found, but not necessary
                                id = "a";
                                break;
                            }
                            conversion.id = id;
                            conversion.name = name;
                            conversion.value = value;
                            conversion.weight = weight;
                            inventory.push_back(conversion);
                        }
                    }
                }
            }
            if(id == "") { // id was not in the databse
                cout << "item " << items[i].id << " not found\n"; // debug
            }
            file.close();
        } else {
            cerr << "DATA FILE MISSING.  EXITING...\n";
            exit(1);
        }
    }
    return;
}



// Reads output from the console command: "player.showinventory" and catelogs items
// expected format: "Quantity - Name (ID) (Status) Condition"
void makeInv(string inputfilename, vector<invItem>& items) {
    ifstream file;
    file.open(inputfilename);
    string line; // holds current line
    string word; // holds current word
    int quantity;
    string id;
    invItem import; // invItem class read from input
    if(file.is_open()){
        while(getline(file, line)) { // reads the next line
            istringstream cur(line); // converts current line to a stream
            getline(cur, word, ' ');
            if(word[0] > 47 && word[0] < 58) { //only reads usable data (first character is a number)
                //cout << line << endl; // debug
                quantity = stoi(word);
                while(getline(cur, word, ' ')) {
                    if(word[0] == 40) { // finds "(", the start of id
                        id = word.substr(1, word.size()-2);
                        //cout << "QUANTITY: " << quantity << "    ID: " << id << endl; // debug
                        import.id = id;
                        import.quantity = quantity;
                        items.push_back(import);
                        break;
                    }
                }
            }
        }
        file.close();
    } else {
        cerr << "INVENTORY FILE (" << inputfilename << ") DOES NOT EXIST.  EXITING...\n";
        exit(1);
    }
    return;
}