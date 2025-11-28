#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

struct Item {
    int id;
    string name;

    Item(int args0, string args1) { id = args0, name = args1; }
};

int main() {
    // Items
    Item potion = Item(0, "Potion");
    Item sword = Item(1, "Sword");
    Item peta = Item(2, "Map");

    // Init
    map<int, Item*> item_list = {
        {potion.id, &potion}, {sword.id, &sword}, {peta.id, &peta}};
    vector<Item*> backpack;
    Item* hotbar[5] = {};

    // Simulation
    cout << "Item List: ";
    for (auto item : item_list) {
        cout << "[" + item.second->name + "]";
    }
    cout << endl;

    backpack.push_back(item_list[1]);  // Add Sword
    backpack.push_back(item_list[2]);  // Add Map
    cout << "Backpack List: ";
    for (auto item : backpack) {
        cout << "[" + item->name + "]";
    }
    cout << endl;

    hotbar[1] = backpack.back();
    backpack.pop_back();
    cout << "Backpack List: ";
    for (auto item : backpack) {
        cout << "[" + item->name + "]";
    }
    cout << endl;

    int index = 0;
    cout << "Hotbar List: ";
    for (auto item : hotbar) {
        if (item != nullptr) {
            cout << "[" + to_string(index) + ", " + item->name + "]";
        } else {
            cout << "[" + to_string(index) + ", " + "Empty]";
        }
        index++;
    }
}