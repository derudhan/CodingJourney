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

map<int, Item*> itemList;
vector<Item*> backpack;
Item* hotbar[5] = {};

bool PickUpItem(int itemId) {
    Item* pickupItem = itemList[itemId];

    if (pickupItem != nullptr) {
        backpack.push_back(pickupItem);
        cout << pickupItem->name + " has added to the backpack!" << endl;
        return true;
    }

    cout << "There is no item with id " + to_string(itemId) << endl;
    return false;
}

bool EquipToHotbar(int backpackIndex, int hotbarSlot) {
    if (backpackIndex < 0 || backpackIndex > backpack.size() - 1) {
        cout << "The size of current backpack is " + to_string(backpack.size())
             << endl;
        return false;
    }

    Item* pickUpItem = backpack[backpackIndex];

    if (hotbar[hotbarSlot] == nullptr) {
        hotbar[hotbarSlot] = pickUpItem;
        cout << pickUpItem->name + " moved from backpack index " +
                    to_string(backpackIndex) + " to hotbar slot " +
                    to_string(hotbarSlot)
             << endl;
        backpack.erase(backpack.begin() + backpackIndex);  // Erase if moved
        return true;
    } else {
        Item* temp = hotbar[hotbarSlot];
        hotbar[hotbarSlot] = pickUpItem;
        backpack[backpackIndex] = temp;
        cout << pickUpItem->name + " swapped from backpack index " +
                    to_string(backpackIndex) + " to hotbar slot " +
                    to_string(hotbarSlot)
             << endl;
        return true;
    }

    cout << "Something wrong!" << endl;
    return false;
}

void ShowInventory() {
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
    cout << endl;
}

int main() {
    Item potion = Item(0, "Potion");
    Item sword = Item(1, "Sword");
    Item peta = Item(2, "Map");

    itemList = {{potion.id, &potion}, {sword.id, &sword}, {peta.id, &peta}};
    cout << "Item List: ";
    for (auto item : itemList) {
        cout << "[" + item.second->name + "]";
    }
    cout << endl;

    PickUpItem(1);
    PickUpItem(2);

    ShowInventory();

    EquipToHotbar(0, 1);

    ShowInventory();

    EquipToHotbar(0, 1);

    ShowInventory();
}