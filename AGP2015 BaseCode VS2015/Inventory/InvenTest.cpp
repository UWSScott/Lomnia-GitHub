#include "Inventory.h"


int displayInventoryOptions();
int displayAddableItems(); 

int test()
{
	Inventory playerInventory;


	bool inventoryRunning = true;
	int playerOption, playerMenuChoice;
	string itemChoice;

	while (inventoryRunning)
	{
		cout << "*************************** INVENTORY TEST *************************************" << endl;
		cout << "********************************************************************************" << endl;
		//Show Items & Gold
		//Show Options

		playerInventory.show();
		
		playerOption = displayInventoryOptions();

		switch (playerOption)
		{
		case 1:
			//***********************************************Picking up item test
			cout << "What item would you like to pretend you picked up?" << endl; 
			playerMenuChoice = displayAddableItems();

			switch (playerMenuChoice)
			{
			case 1: playerInventory.addItem("Medkit");
				break;
			case 2: playerInventory.addItem("ManaPotion"); 
				break;
			case 3: playerInventory.addItem("Sword");
				break;
			case 4: playerInventory.addItem("Axe");
				break;
			case 5: playerInventory.addItem("Knives"); 
				break; 
			}
			break;
		case 2: 
			//***********************************************Buying item test
			cout << "What item would you like to buy?" << endl;
			playerMenuChoice = displayAddableItems();

			switch (playerMenuChoice)
			{
			case 1: playerInventory.buyItem("Medkit", 2); // price currently here, this needs to change, price should be inside class right?
				break;
			case 2: playerInventory.buyItem("ManaPotion", 4);
				break;
			case 3: playerInventory.buyItem("Sword", 15); 
				break; 
			case 4: playerInventory.buyItem("Axe", 20);
				break; 
			case 5: playerInventory.buyItem("Knives", 20);
				break;
			}
			break;
		case 3:
			//***********************************************Selling item test
		
			cout << "What item would you like to sell? Type the name (no spaces): ";
			//playerInventory.show();
			cin >> itemChoice;
			playerInventory.sellItem(itemChoice, 7);
			
			break;
		case 4:
			//***********************************************Removing item test
			cout << "What item would you like to sell? Type the name (no spaces): ";
			cin >> itemChoice;
			playerInventory.removeItem(itemChoice);
			break;
		case 5:
			cout << "Exiting inventory." << endl; 
			inventoryRunning = false; 
			break; 
		default: 
			cout << "Invalid choice." << endl; 

		}
		
	}

	return 0; 		
}


int displayInventoryOptions()
{

	int option;
	cout << "Options (Choose number choice):" << endl; 
	cout << "1. Pick Up Item" << endl;
	cout << "2. Buy Item" << endl;
	cout << "3. Sell Item" << endl;
	cout << "4. Delete Item" << endl;
	cout << "5. Exit" << endl << endl; 

	cout << "Choose option: ";
	cin >> option;

	return option;

}

int displayAddableItems()
{
	int playerMenuChoice;
	cout << "1. Medkit" << endl;
	cout << "2. ManaPotion" << endl;
	cout << "3. Sword" << endl; 
	cout << "4. Axe" << endl;
	cout << "5. Knives" << endl; 
	
	cout << "Choose number: ";
	cin >> playerMenuChoice;
	return playerMenuChoice;
}

