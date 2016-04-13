/**
 * Copyright 2016 Alexander Scheel
 *
 * inventory stacking test cases
**/


#include <iostream>
#include "../src/object.h"

using namespace std;


int main(int argc, char* argv[])
{
    object_t* inventory = NULL;

    object_t* item_1 = new object_t();
    item_1->display = true;
    item_1->name = "Item 1";
    item_1->description = "Item in player's inventory.";
    item_1->type = "magical";
    item_1->representation = '1';

    item_1->convert_to_stack();

    inventory = item_1;

    cout << "Converted item_1 to stack:" << endl;
    cout << inventory->name << endl;
    cout << inventory->type << endl;
    cout << inventory->stack_size << endl;

    object_t* item_2 = new object_t();
    item_2->display = true;
    item_2->name = "Item 2";
    item_2->description = "Item in player's inventory.";
    item_2->type = "magical";
    item_2->representation = '2';

    object_t* item_3 = new object_t();
    item_3->display = true;
    item_3->name = "Item 3";
    item_3->description = "Item in player's inventory.";
    item_3->type = "magical";
    item_3->representation = '3';

    object_t* item_4 = new object_t();
    item_4->display = true;
    item_4->name = "Item 4";
    item_4->description = "Item in player's inventory.";
    item_4->type = "magical";
    item_4->representation = '4';

    object_t* item_5 = new object_t();
    item_5->display = true;
    item_5->name = "Item 5";
    item_5->description = "Item in player's inventory.";
    item_5->type = "magical";
    item_5->representation = '5';

    object_t* item_6 = new object_t();
    item_6->display = true;
    item_6->name = "Item 6";
    item_6->description = "Item in player's inventory.";
    item_6->type = "magical";
    item_6->representation = '6';

    cout << true << " " << false << endl;

    cout << "Adding item 2: " << inventory->add_to_stack(item_2) << endl;
    cout << "Adding item 3: " << inventory->add_to_stack(item_3) << endl;

    object_t* removed = inventory->pick_from_top_of_stack();
    cout << "Removing one item: " << removed->name << endl;
    delete removed;

    object_t* removed_2 = inventory->pick_from_top_of_stack();
    cout << "Removing second item: " << removed_2->name << endl;
    delete removed_2;

    cout << "Adding item 4: " << inventory->add_to_stack(item_4) << endl;

    object_t* removed_3 = inventory->pick_from_top_of_stack();
    cout << "Removing third item: " << removed_3->name << endl;
    delete removed_3;

    object_t* removed_4 = inventory->pick_from_top_of_stack();
    cout << "Removing fourth item: " << removed_4->name << endl;
    delete removed_4;

    cout << "Adding item 5: " << inventory->add_to_stack(item_5) << endl;
    cout << "Adding item 6: " << inventory->add_to_stack(item_6) << endl;

    object_t* removed_5 = inventory->pick_from_top_of_stack();
    cout << "Removing fifth item: " << removed_5->name << endl;
    delete removed_5;

    cout << "Converting last item from stack:" << endl;

    cout << "pre-Inventory stack_size: " << inventory->stack_size << endl;
    cout << inventory->convert_from_stack() << endl;
    cout << "Item name: " << inventory->name << endl;
    cout << "Item type: " << inventory->type << endl;
    cout << "Item stack_size: " << inventory->stack_size << endl;

    delete inventory;

    return 0;
}
