/******************************************************************
This is to certify that this project is my own work, based on my personal efforts in studying and applying
the concepts learned. I have constructed the functions and their respective algorithms
and corresponding code by myself. The program was run, tested, and debugged by my own efforts. I
further certify that I have not copied in part or whole or otherwise plagiarized the work of other students
and/or persons.
Brett Harley A. Mider, DLSU ID# 12117601
******************************************************************/

/**
 * Description: The game of "Secret Potions of the Geffen Witches", a text-based and menu-driven fishing game where
 *              the player has to catch and sell fish for money, buy materials, and craft items to achieve their
 *              ultimate goal of obtaining Ymir's Avatar Potion.
 * Programmed by: Brett Harley A. Mider (S28A)
 * Last modified: February 6, 2022
 * Version: 1.0
 * Acknowledgements: StackOverflow
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// arbitrary amount; accounts for null terminator
#define MAX_STR_LEN 51

#define MAX_LOCATIONS_AMT 6
#define MAX_INV_SLOTS 17
#define MAX_CRAFTABLE_ITEMS 5
#define MAX_NEEDED_ITEMS 4
#define MAX_BUYABLE_ITEMS 4
#define MAX_SELLABLE_ITEMS 8
#define MAX_PROMPT_OPTNS 8

struct neededItem {
    char name[MAX_STR_LEN];
    char location[MAX_STR_LEN];
    int qty;
};

struct craftableItem {
    char name[MAX_STR_LEN];
    char selector[MAX_STR_LEN];
    struct neededItem prerequisites[MAX_NEEDED_ITEMS];
};

struct storeItem {
    char name[MAX_STR_LEN];
    char selector[MAX_STR_LEN];
    float price;
};

struct promptOptn {
    char selector[MAX_STR_LEN];
    char optnName[MAX_STR_LEN];
};

struct prompt {
    char title[MAX_STR_LEN];
    struct promptOptn optns[MAX_PROMPT_OPTNS];
    int promptOptnAmt;
};

struct invSlot {
    char itemName[MAX_STR_LEN];
    int qty;
};

struct playerStats {
    struct invSlot bagSlots[MAX_INV_SLOTS];
    float coins;
};

struct fishingLoc {
    char name[MAX_STR_LEN];
    char commonFish[MAX_STR_LEN];
    char rareFish[MAX_STR_LEN];
};

/**
 * Checks if two given strings are the same.
 * Precondition: None
 * @param str1 is the first string to be compared.
 * @param str2 is the second string to be compared.
 * @return 1 (true) if str1 and str2 are the same, and returns 0 (false) otherwise.
 */
int
strIsSame(char str1[], char str2[]) {
    int isStrSame = 0;
    if (!strcmp(str1, str2)) {
        isStrSame = 1;
    }
    return isStrSame;
}

/**
 * Initializes the player's beginning statistics. It first creates a temporary list of item names, then copies them
 * to each inventory slot. Each slot's quantity is then set to 0, and the player's initial money is set to 0 as well.
 * Precondition: None
 * @param pPlayer points to the actual variable containing the player's statistics.
 * @return None.
 */
void
initializePlayerStats(struct playerStats * pPlayer) {
    char itemList[MAX_INV_SLOTS][MAX_STR_LEN] = {
        "Ymir's Avatar Potion",
        "Fire Chakra Potion",
        "Water Chakra Potion",
        "Earth Chakra Potion",
        "Air Chakra Potion",
        "Scaleless Blackfish",
        "Mariana Snailfish",
        "Mudskipper",
        "Hillstream Loach",
        "Gold",
        "Majestic Water",
        "Wondrous Vinegar",
        "Magical Bait",
        "Tilapia",
        "Sardine",
        "Bangus",
        "Tuna"
    };

    for (int i = 0; i < MAX_INV_SLOTS; i++) {
        strncpy(pPlayer->bagSlots[i].itemName, itemList[i], MAX_STR_LEN);
        pPlayer->bagSlots[i].qty = 0;
    }

    pPlayer->coins = 0.0; // not really needed since all bag values are initialized to zero
                          // at the beginning of the program, but this is more explicit
                          // and allows easy changing of initial coin amount
}

/**
 * Displays the player's bag's contents, specifically item names and their quantities. If the item's quantity is 0,
 * then the item name and its quantity is not displayed.
 * Precondition: None
 * @param pPlayer points to the actual variable containing the player's statistics.
 * @return None.
 */
void
displayBag(struct playerStats * pPlayer) {
    printf("Bag\n\n");
    for (int i = 0; i < MAX_INV_SLOTS; i++) {
        if (pPlayer->bagSlots[i].qty > 0) {
            printf("%-30s x %d\n", pPlayer->bagSlots[i].itemName,
                                   pPlayer->bagSlots[i].qty);
        }
    }
    printf("\n\n");
}

/**
 * Displays the amount of coins the player currently has.
 * Precondition: None
 * @param pPlayer points to the actual variable containing the player's statistics.
 * @return None.
 */
void
displayCoins(struct playerStats * pPlayer) {
    printf("Coins: %.2f\n\n", pPlayer->coins);
}

/**
 * Initializes the craftable items. It first creates a temporary list of the craftable items, which contain their
 * name, their selectors in the crafting menu, and their prerequisite items' names, method of obtaining, and
 * required quantity. It then copies these to the actual list of craftable items.
 * Precondition: None
 * @param craftables is the actual array containing the craftable items, to which the temporary data is copied to.
 * @return None.
 */
void
initializeCraftables(struct craftableItem craftables[]) {
    struct craftableItem tempCraftables[MAX_CRAFTABLE_ITEMS] = {
        // Item name           Selector  Prerequisite items, location, and amount needed
        {"Ymir's Avatar Potion", "1", {{"Fire Chakra Potion", "Crafting", 1},
                                       {"Water Chakra Potion", "Crafting", 1},
                                       {"Earth Chakra Potion", "Crafting", 1},
                                       {"Air Chakra Potion", "Crafting", 1}}
        },
        {"Fire Chakra Potion", "2", {{"Scaleless Blackfish", "Taal Lake", 1},
                                     {"Gold", "Holgrehenn Store", 1},
                                     {"Majestic Water", "Holgrehenn Store", 1},
                                     {"Wondrous Vinegar", "Holgrehenn Store", 1}}
        },
        {"Water Chakra Potion", "3", {{"Mariana Snailfish", "Galathea Deep", 1},
                                     {"Gold", "Holgrehenn Store", 1},
                                     {"Majestic Water", "Holgrehenn Store", 1},
                                     {"Wondrous Vinegar", "Holgrehenn Store", 1}}
        },
        {"Earth Chakra Potion", "4", {{"Mudskipper", "Dagupan Mangrove Forests", 1},
                                     {"Gold", "Holgrehenn Store", 1},
                                     {"Majestic Water", "Holgrehenn Store", 1},
                                     {"Wondrous Vinegar", "Holgrehenn Store", 1}}
        },
        {"Air Chakra Potion", "5", {{"Hillstream Loach", "Mindanao Current", 1},
                                     {"Gold", "Holgrehenn Store", 1},
                                     {"Majestic Water", "Holgrehenn Store", 1},
                                     {"Wondrous Vinegar", "Holgrehenn Store", 1}}
        }
    };

    for (int i = 0; i < MAX_CRAFTABLE_ITEMS; i++) {
        strncpy(craftables[i].name, tempCraftables[i].name, MAX_STR_LEN);
        strncpy(craftables[i].selector, tempCraftables[i].selector, MAX_STR_LEN);

        for (int j = 0; j < MAX_NEEDED_ITEMS; j++) {
            strncpy(craftables[i].prerequisites[j].name, tempCraftables[i].prerequisites[j].name, MAX_STR_LEN);
            strncpy(craftables[i].prerequisites[j].location, tempCraftables[i].prerequisites[j].location, MAX_STR_LEN);
            craftables[i].prerequisites[j].qty = tempCraftables[i].prerequisites[j].qty;
        }
    }
}

/**
 * Displays the craftable items, which include their names, selectors in the crafting menu, and their prerequisite
 * items' names, method of obtaining, and required quantity.
 * Precondition: None.
 * @param craftables is an array containing the list of craftable items, which has their names, selectors, and their
 *        prerequisite items' names, method of obtaining, and required quantity.
 * @return None.
 */
void
displayCraftableItems(struct craftableItem craftables[]) {
    for (int i = 0; i < MAX_CRAFTABLE_ITEMS; i++) {
        printf("(%s) %s\n", craftables[i].selector, craftables[i].name);
        printf("\n\tPrerequisites:\n\n");
        for (int j = 0; j < MAX_NEEDED_ITEMS; j++) {
            printf("\t%-20s x %d (%s)\n", craftables[i].prerequisites[j].name,
                                          craftables[i].prerequisites[j].qty,
                                          craftables[i].prerequisites[j].location);
        }
        printf("\n");
    }
}

/**
 * Initializes the buyable items list. It first creates a temporary list containing the buyable items' names, their
 * selectors, and prices, then copies them to the actual list.
 * Precondition: None
 * @param buyItemList is the actual array containing the buyable items, to which the temporary data is copied to.
 * @return None.
 */
void
initializeBuyItemList(struct storeItem buyItemList[]) {
    struct storeItem tempBuyItemList[MAX_BUYABLE_ITEMS] = {
        // Item name, selector, and price
        {"Gold", "1", 750.00},
        {"Majestic Water", "2", 100.00},
        {"Wondrous Vinegar", "3", 150.00},
        {"Magical Bait", "4", 300.00}
    };

    for (int i = 0; i < MAX_BUYABLE_ITEMS; i++) {
        strncpy(buyItemList[i].name, tempBuyItemList[i].name, MAX_STR_LEN);
        strncpy(buyItemList[i].selector, tempBuyItemList[i].selector, MAX_STR_LEN);
        buyItemList[i].price = tempBuyItemList[i].price;
    }
}

/**
 * Initializes the sellable item list. It first creates a temporary list containing the sellable items' names, their
 * selectors, and prices, then copies them to the actual list.
 * Precondition: None
 * @param sellItemList is the actual array containing the sellable items, to which the temporary data is copied to.
 * @return None.
 */
void
initializeSellItemList(struct storeItem sellItemList[]) {
    struct storeItem tempSellItemList[] = {
        // Item name, selector, and price
        {"Scaleless Blackfish", "1", 1000.00},
        {"Mariana Snailfish", "2", 1000.00},
        {"Mudskipper", "3", 1000.00},
        {"Hillstream Loach", "4", 1000.00},
        {"Tilapia", "5", 35.00},
        {"Sardine", "6", 40.00},
        {"Bangus", "7", 50.00},
        {"Tuna", "8", 45.00}
    };

    for (int i = 0; i < MAX_SELLABLE_ITEMS; i++) {
        strncpy(sellItemList[i].name, tempSellItemList[i].name, MAX_STR_LEN);
        strncpy(sellItemList[i].selector, tempSellItemList[i].selector, MAX_STR_LEN);
        sellItemList[i].price = tempSellItemList[i].price;
    }
}

/**
 * Displays the list of buyable items, which include their names, selectors, and prices.
 * Precondition: None
 * @param buyItemList is the array containing the buyable items' names, selectors, and prices.
 * @return None.
 */
void
showBuyItemList(struct storeItem buyItemList[]) {
    for (int i = 0; i < MAX_BUYABLE_ITEMS; i++) {
        printf("(%s) %-20s - %-4.2f Ymir\n", buyItemList[i].selector, buyItemList[i].name, buyItemList[i].price);
    }
    printf("\n\n");
}

/**
 * Displays the list of sellable items, which include their names, selectors, and prices.
 * Precondition: None
 * @param sellItemList is the array containing the sellable items' names, selectors, and prices.
 * @return None.
 */
void
showSellItemList(struct storeItem sellItemList[]) {
    for (int i = 0; i < MAX_SELLABLE_ITEMS; i++) {
        printf("(%s) %-20s - %-4.2f Ymir\n", sellItemList[i].selector, sellItemList[i].name, sellItemList[i].price);
    }
    printf("\n");
}

/**
 * Displays the opening message of the game. It first creates a temporary string containing the message and then
 * actually prints it. This is because it is more readable for both the user and programmer, and more convenient
 * to edit and format for the latter as well.
 * Precondition: None
 * @return None.
 */
void
showOpeningMessage(void) {

    char openingMessage[] =
        "Welcome, traveler, to the town of Geffen!\n\n"

        "Here, witches from all over the country have gathered to discover the secret behind Ymir's Avatar Potion,\n"
        "the only potion that can help cultivate the Avatar's powers. I have heard that the Avatar Potion can only\n"
        "be crafted here in Geffen, and it requires four materials: the Fire Chakra Potion, the Water Chakra Potion,\n"
        "the Earth Chakra Potion, and the Air Chakra Potion.\n\n"

        "Those potions can also be crafted here in Geffen, and the secret behind these potions are the extremely\n"
        "rare fishes that can be gathered from four fishing sites not far from here: Taal Lake, Galathea Deep,\n"
        "Dagupan Mangrove Forests, and Mindanao Current. These locations house the scaleless blackfish, the\n"
        "marian snailfish, the mudskipper, and the hillstream loach, respectively.\n\n"

        "According to people around here, the chances of catching these extremely rare fish from their respective\n"
        "habitats are only 10 percent. This chance can be increased to 30 percent if you used the Magical Bait which\n"
        "can be bought from the Holgrehenn Store.\n\n"

        "The Holgrehenn Store also sells materials that you will need to craft those potions: some gold, majestic\n"
        "water, and wondrous vinegar. I should mention that the currency used here in Geffen is Ymir gold coins.\n"
        "If you wish to gain some coins, you could fish from the fishing spots and sell your loot to the store\n"
        "as they buy these as well.\n\n"

        "Oh, right, since you're new, here is a magical fishing rod that you can use to fish. Also, do not worry\n"
        "about your living expenses, as Geffen is a prosperous town. Housing and food is provided for free to all\n"
        "citizens of Geffen.\n\n"
        
        "So, I hope you enjoy your stay, and best of luck to ya!\n"
        ;
    
    printf("%s\n", openingMessage);
}

/**
 * Finds the earliest instance of a newline character in a string and then replaces it with the null terminator.
 * Otherwise, does nothing. Mainly meant to be used to remove the newline character that fgets() places at the
 * end of the string when getting input from the user.
 * Precondition: None
 * @param buf is the string to be searched for a newline character.
 * @return None.
 */
void
removeNewline(char buf[]) {
    buf[strcspn(buf, "\n")] = 0;
}

/**
 * Removes excess characters from the input stream, if there are any, so the next function that gets input from
 * the user would not read those characters instead. Meant to be used with fgets().
 * Precondition: None
 * @param buf is the string used for storing the user's input.
 * @param bufLenLimit is the maximum amount of characters that the string can store. Must account for the null
 *        terminator.
 * @return None.
 */
void
clearInputBuffer(char buf[], int bufLenLimit) {
    // if buf's length is equal to or greater than bufLenLimit, that means there are extra characters left on the
    // input stream, in which case, getchar() will read and discard them until it finally reads and discards
    // a newline character, which is the last character placed on the input stream after the user enters their input
    if (strlen(buf) >= bufLenLimit - 1)
        while (getchar() != '\n');
}

/**
 * Gets a string input from the user which length is limited by a specified value to prevent buffer overflow.
 * It is a combination of three functions: fgets(), for reading user input; removeNewline(), for removing the extra
 * newline character placed by fgets() at the end of the string; and clearInputBuffer() to remove extra characters
 * from the input stream, if there are any.
 * Precondition: None
 * @param buf is the string used for storing the user's input.
 * @param bufLen is the maximum amount of characters that the string can store. Must account for the null terminator.
 * @return None.
 */
void
getStrUserInput(char buf[], int bufLen) {
    printf("Input > ");
    fgets(buf, bufLen, stdin);
    removeNewline(buf);
    clearInputBuffer(buf, bufLen);
}

/**
 * Displays the prompt title, options, and selectors.
 * Precondition: None
 * @param givenPrompt contains the prompt title, options, and selectors.
 * @return None.
 */
void
showPrompt(struct prompt givenPrompt) {
    printf("%s\n\n", givenPrompt.title);
    for (int i = 0; i < givenPrompt.promptOptnAmt; i++) {
        printf("(%s) %s\n", givenPrompt.optns[i].selector, givenPrompt.optns[i].optnName);
    }
    printf("\n");
}

/**
 * Validates the user's input to a prompt by checking if it is on the option/selector list of the prompt.
 * Precondition: None
 * @param input is the string user input to be compared against the prompt's selectors.
 * @param givenPrompt contains the prompt title, options, and selectors.
 * @return 1 (true) if input is valid and returns 0 (false) otherwise.
 */
int
validatePromptInput(char input[], struct prompt givenPrompt) {
    int givenInputIsValid = 0;
    for (int i = 0; i < givenPrompt.promptOptnAmt && givenInputIsValid == 0; i++) {
        if (strIsSame(input, givenPrompt.optns[i].selector)) {
            givenInputIsValid = 1;
        }
    }
    return givenInputIsValid;
}

/**
 * Sets the corresponding action/option name to the input string given a selector from a prompt.
 * Precondition: None
 * @param input is the string used to initially store a selector. It is then used to store the corresponding
 *        action/option name.
 * @param givenPrompt contains the prompt title, options, and selectors.
 * @return None.
 */
void
promptInputToAction(char input[], struct prompt givenPrompt) {
    int conversionIsDone = 0;
    for (int i = 0; i < givenPrompt.promptOptnAmt && conversionIsDone == 0; i++) {
        if (strIsSame(input, givenPrompt.optns[i].selector)) {
            strncpy(input, givenPrompt.optns[i].optnName, MAX_STR_LEN);
            conversionIsDone = 1;
        }
    }
}

/**
 * Asks user for their string input, then validates it by comparing it to the prompt's selectors. It keeps on
 * asking the user for their input until the user enters a valid one. It then converts the input, which is a selector,
 * to the corresponding action/option name.
 * Precondition: None
 * @param input is the string used to initially store a selector. It is then used to store the corresponding
 *        action/option name.
 * @param givenPrompt contains the prompt title, options, and selectors.
 * @return None.
 */
void
getPromptInput(char input[], struct prompt givenPrompt) {
    int inputIsValid = 0;
    do {
        getStrUserInput(input, MAX_STR_LEN);
        inputIsValid = validatePromptInput(input, givenPrompt);
    } while(inputIsValid == 0);

    promptInputToAction(input, givenPrompt);
}

/**
 * Gets user confirmation to prevent accidental input execution. Meant for options that have significant effects.
 * A prompt is shown asking for user input with only two options: "Yes" and "Cancel". User input is then obtained and
 * validated. "Yes" means that user approves of the action, and "Cancel" means the user wants to terminate the action.
 * Precondition: None
 * @return 1 (true) if user is sure or answers "Yes", and 0 (false) if user is unsure or answers "Cancel".
 */
int
getUserConfirmation(void) {
    char userConfirmationInput[MAX_STR_LEN] = "";
    int userIsSure = 0;
    struct prompt userConfirmation = {
        "Are you sure?", // prompt title
        { // prompt selectors and options
            {"1", "Yes"},
            {"2", "Cancel"}
        },
        2 // amount of prompt options
    };

    printf("\n");
    showPrompt(userConfirmation);
    getPromptInput(userConfirmationInput, userConfirmation);

    if (strIsSame(userConfirmationInput, "Yes")) {
        userIsSure = 1;
    }

    else {
        userIsSure = 0;
    }

    return userIsSure;
}

/**
 * One of main menu options. Asks if the user wishes to exit the program or if they want to go back to the main menu.
 * If the user chooses the first choice, "Exit program", the user is asked to confirm their decision. If the user
 * confirms, the program terminates. Otherwise, the program goes back to the main menu. If the user chose the
 * second option, "Cancel", the program also goes back to the main menu.
 * Precondition: None
 * @return 1 (true) if the user wishes to exit the program, or 0 (false) otherwise (go back to the main menu).
 */
int
exitProg(void) {
    char exitProgInput[MAX_STR_LEN] = "";
    int exitProg = 0;
    struct prompt exitProgPrompt = {
        "Exit Program", // prompt title
        { // prompt selectors and options
            {"1", "Exit program"},
            {"2", "Cancel"}
        },
        2 // amount of prompt options
    };

    system("cls");
    showPrompt(exitProgPrompt);
    getPromptInput(exitProgInput, exitProgPrompt);

    if (strIsSame(exitProgInput, "Exit program")) {
        exitProg = getUserConfirmation();
    }
    else {
        exitProg = 0;
    }

    return exitProg;
}

/**
 * Displays the user's bag's contents and amount of coins they currently have. If the user enters the first (and only)
 * option, they go back to the main menu.
 * Precondition: None
 * @param pPlayer is used to point to the actual variable containing the player's statistics.
 * @return None.
 */
void
checkBag(struct playerStats * pPlayer) {
    char checkBagInput[MAX_STR_LEN] = "";
    struct prompt checkBagPrompt = {
        "Bag Options", // prompt name
        { // prompt selector and action
            {"1", "Back to Main Menu"}
        },
        1 // amount of prompt options
    };

    system("cls");
    displayCoins(pPlayer);
    displayBag(pPlayer);

    showPrompt(checkBagPrompt);
    getPromptInput(checkBagInput, checkBagPrompt);
    // does nothing afterwards since there is only one option with one valid input,
    // and there is no further action required aside from letting this function call end
    // in extending functionality, put code here
}

/**
 * Validates the user's input in the Craft Potion menu. It checks the user's input against the valid selectors
 * of the prompt.
 * Precondition: None
 * @param craftPotionInput is used to store the string input of the user for the Craft Potion prompt/menu.
 * @param craftables contains the list of craftable items' names, selectors, and prerequisite items' details.
 * @return 1 (true) if the input is valid or it has matched with a selector, and 0 (false) otherwise.
 */
int
validateCraftPotionInput(char craftPotionInput[], struct craftableItem craftables[]) {
    int inputIsValid = 0;
    if (strIsSame(craftPotionInput, "0")) { // default cancel option
        inputIsValid = 1;
    }

    for (int i = 0; i < MAX_CRAFTABLE_ITEMS && inputIsValid == 0; i++) {
        if (strIsSame(craftPotionInput, craftables[i].selector)) {
            inputIsValid = 1;
        }
    }

    return inputIsValid;
}

/**
 * Gets the user's input in the Craft Potion menu. It first prompts the user to enter a string input, and then
 * validates it. It continues to prompt the user to enter a string input until the user enters a valid input.
 * Precondition: None
 * @param craftPotionInput is used to store the user's string input in the Craft Potion menu.
 * @param craftables contains the list of craftable items' names, selectors, and prerequisite items' details.
 * @return None.
 */
void
getCraftPotionInput(char craftPotionInput[], struct craftableItem craftables[]) {
    int inputIsValid = 0;

    printf("Select potion to craft or type '0' to cancel crafting.\n\n");
    do {
        getStrUserInput(craftPotionInput, MAX_STR_LEN);
        inputIsValid = validateCraftPotionInput(craftPotionInput, craftables);
    } while (inputIsValid == 0);
}

/**
 * Gets the potion name from the user's input in the Craft Potion menu. It compares the user's input, a selector,
 * with the selectors of the potions. If there is a match, the potion's name is copied to a string.
 * Precondition: input is valid (it matches with a selector in craftables).
 * @param input is used to store the user's string input in the Craft Potion menu. It contains a valid selector.
 * @param potionName is used to store the potion's name.
 * @param craftables contains the list of craftable items' names, selectors, and prerequisite items' details.
 * @return None.
 */
void
getPotionNameFromInput(char input[], char potionName[], struct craftableItem craftables[]) {
    int itemHasBeenFound = 0;
    for (int i = 0; i < MAX_CRAFTABLE_ITEMS && itemHasBeenFound == 0; i++) {
        if (strIsSame(input, craftables[i].selector)) {
            strncpy(potionName, craftables[i].name, MAX_STR_LEN);
            itemHasBeenFound = 1;
        }
    }
}

/**
 * Obtains the index number of an item in the player's inventory (bag) using the item name. It compares the given
 * item name with the item names in the player's inventory (bag). It then returns the index number of the item
 * once it has been found in the inventory.
 * Precondition: itemName contains a valid item name.
 * @param itemName contains the item's name. It is compared with the item names currently in the player's inventory.
 * @param pPlayer is a pointer to the actual variable containing the player's statistics, including the inventory/bag.
 * @return the index number of an item in the player's inventory (bag).
 */
int
getItemIndexFromInv(char itemName[], struct playerStats * pPlayer) {
    int itemHasBeenFound = 0;
    int itemIndex = 0;

    for (int i = 0; i < MAX_INV_SLOTS && itemHasBeenFound == 0; i++) {
        if (strIsSame(itemName, pPlayer->bagSlots[i].itemName)) {
            itemIndex = i;
            itemHasBeenFound = 1;
        }
    }

    return itemIndex;
}

/**
 * Obtains the quantity of an item in the player's inventory (bag) using the item name. It first locates the item
 * in the inventory, and once it has been found, its quantity is accessed and returned.
 * Precondition: itemName contains a valid item name.
 * @param itemName contains the item's name. It will be used to locate the item in the inventory.
 * @param pPlayer is a pointer to the actual variable containing the player's statistics, including the inventory/bag.
 * @return the quantity of the given item in the player's inventory.
 */
int
getItemQtyFromInv(char itemName[], struct playerStats * pPlayer) {
    int itemIndex = getItemIndexFromInv(itemName, pPlayer);
    return pPlayer->bagSlots[itemIndex].qty;
}

/**
 * Obtains a craftable item's index in the craftable items list. It first locates the item in the list and once it
 * has been found, its index is returned.
 * Precondition: itemName contains a valid craftable item's name.
 * @param itemName contains the craftable item's name. It will be used to locate the item in the craftable items list.
 * @param craftables contains the list of craftable items' names, selectors, and prerequisite items' details.
 * @return the index of the craftable item in the craftable items list.
 */
int
getCraftableItemIndex(char itemName[], struct craftableItem craftables[]) {
    int itemIndex = 0;
    int itemHasBeenFound = 0;

    for (int i = 0; i < MAX_CRAFTABLE_ITEMS && itemHasBeenFound == 0; i++) {
        if (strIsSame(itemName, craftables[i].name)) {
            itemIndex = i;
            itemHasBeenFound = 1;
        }
    }

    return itemIndex;
}

/**
 * Checks the player's inventory if it has all the prerequisite items of a craftable item. It compares the quantity of
 * the craftable item's prerequisite items in the inventory to the required amount.
 * Precondition: itemName contains a valid craftable item's name.
 * @param itemName contains the craftable item's name.
 * @param craftables contains the list of craftable items' names, selectors, and prerequisite items' details.
 * @param pPlayer is a pointer to the actual variable containing the player's statistics, including the inventory/bag.
 * @return 1 (true) if the player has all the prerequisite items needed to craft the given craftable item,
 *         and 0 (false) if not.
 */
int
checkMaterials(char itemName[], struct craftableItem craftables[], struct playerStats * pPlayer) {
    // finds the craftable item in the list so its prerequisite items can be accessed easily
    int craftableItemIndex = getCraftableItemIndex(itemName, craftables);
    int allReqsAreMet = 1; // assume that all requirements are met

    // loop will stop immediately once allReqsAreMet becomes 0 (if at least one prerequisite item's quantity in the
    // player's inventory/bag is not greater than or equal to the required amount)
    for (int i = 0; i < MAX_NEEDED_ITEMS && allReqsAreMet == 1; i++) {
        allReqsAreMet = getItemQtyFromInv(craftables[craftableItemIndex].prerequisites[i].name, pPlayer) >=
                        craftables[craftableItemIndex].prerequisites[i].qty;
    }

    return allReqsAreMet;
}

/**
 * Adds a specified quantity of an item to the player's inventory/bag.
 * Precondition: itemName contains a valid item name, and itemQty contains a non-negative integer.
 * @param itemName contains the name of the item to be added to the player's inventory/bag.
 * @param itemQty contains the amount of the item to be added to the player's inventory/bag.
 * @param pPlayer is a pointer to the actual variable containing the player's statistics, including the inventory/bag.
 * @return None.
 */
void
addItem(char itemName[], int itemQty, struct playerStats * pPlayer) {
    int itemIndex = getItemIndexFromInv(itemName, pPlayer); // find item index from player inventory using item name
    pPlayer->bagSlots[itemIndex].qty += itemQty; // use item index to locate item in inventory
}

/**
 * Deducts a specified quantity of an item from the player's inventory/bag.
 * Precondition: itemName contains a valid item name, and itemQty contains a non-negative integer.
 * @param itemName contains the name of the item to be deducted from the player's inventory/bag.
 * @param itemQty contains the amount of the item to be deducted from the player's inventory/bag.
 * @param pPlayer is a pointer to the actual variable containing the player's statistics, including the inventory/bag.
 * @return None.
 */
void
deductItem(char itemName[], int itemQty, struct playerStats * pPlayer) {
    int itemIndex = getItemIndexFromInv(itemName, pPlayer);
    pPlayer->bagSlots[itemIndex].qty -= itemQty;
}

/**
 * Deducts prerequisite items from the player's inventory/bag.
 * Precondition: itemName contains a valid craftable item name, and multiplier is a non-negative integer.
 * @param itemName contains the name of the craftable item.
 * @param craftables contains the list of craftable items' names, selectors, and prerequisite items' details.
 * @param multiplier contains the amount of crafted items.
 * @param pPlayer is a pointer to the actual variable containing the player's statistics, including the inventory/bag.
 * @return None.
 */
void
deductItemPrerequisites(char itemName[], struct craftableItem craftables[], int multiplier, struct playerStats * pPlayer) {
    // gets the craftable item's index from the craftable items list so its prerequisite items can be easily accessed
    int craftableItemIndex = getCraftableItemIndex(itemName, craftables);
    for (int i = 0; i < MAX_NEEDED_ITEMS; i++) {
        deductItem(craftables[craftableItemIndex].prerequisites[i].name, // deducts what item
                   craftables[craftableItemIndex].prerequisites[i].qty * multiplier, // deducts how much of the item
                   pPlayer);
    }
}

/**
 * Crafts an item. If the user has enough materials, they are asked to confirm crafting. If they confirm, the item is
 * crafted, meaning the prerequisite items are deducted from the inventory/bag, and the crafted item is added to the
 * inventory. If they do not confirm, nothing is crafted. If they do not have enough materials, nothing is crafted and
 * they are told that they have insufficient materials.
 * Precondition: itemName is a valid craftable item name.
 * @param itemName contains the name of the craftable item.
 * @param craftables contains the list of craftable items' names, selectors, and prerequisite items' details.
 * @param pPlayer is a pointer to the actual variable containing the player's statistics, including the inventory/bag.
 * @return None.
 */
void
craftItem(char itemName[], struct craftableItem craftables[], struct playerStats * pPlayer) {
    int hasAllMaterials = checkMaterials(itemName, craftables, pPlayer);

    if (hasAllMaterials == 1) {

        int userIsSure = getUserConfirmation();
        if (userIsSure == 1) {
            addItem(itemName, 1, pPlayer);
            deductItemPrerequisites(itemName, craftables, 1, pPlayer);
            printf("%s successfully crafted. It has been placed in the bag.\n", itemName);
        }

        else if (userIsSure == 0) {
            printf("Did not craft anything.\n");
        }
    }

    else {
        printf("Not enough materials!\n");
    }
    printf("\n");
}

/**
 * Brings up the Craft Potion menu. The list of craftable items is shown, and the player's inventory/bag is shown for
 * convenience as well. The user is then asked for their input which can only be two options: to cancel crafting, or
 * to craft a potion. If they cancel crafting, the function terminates immediately. Otherwise, the potion's name is
 * obtained from the input, a selector. The player's inventory is then checked for the item's prerequisites. If the
 * player has sufficient materials, they are asked to confirm the crafting. If they do, the item is crafted and its
 * prerequisites are removed from the inventory. Otherwise, nothing is crafted. If they have insufficient materials,
 * nothing is crafted and they are notified of having insufficient materials.
 * Precondition: None
 * @param pPlayer is a pointer to the actual variable containing the player's statistics, including the inventory/bag.
 * @param craftables contains the list of craftable items' names, selectors, and prerequisite items' details.
 * @return None.
 */
void
craftPotion(struct playerStats * pPlayer, struct craftableItem craftables[]) {
    char craftPotionInput[MAX_STR_LEN] = "";
    char potionName[MAX_STR_LEN] = "";

    system("cls");
    displayCraftableItems(craftables);
    displayBag(pPlayer);
    getCraftPotionInput(craftPotionInput, craftables);

    if (!strIsSame(craftPotionInput, "0")) { // cancel crafting
        getPotionNameFromInput(craftPotionInput, potionName, craftables);
        craftItem(potionName, craftables, pPlayer);
        system("pause");
    }
}

/**
 * Teleports to Geffen. The user is prompted between two choices: to craft a potion or to go back to the main menu.
 * If they choose to craft a potion, they are shown the Craft Potion menu where they could craft a potion or cancel
 * crafting. If they choose the second option, this function terminates and the user goes back to the main menu.
 * Precondition: None
 * @param pPlayer is a pointer to the actual variable containing the player's statistics, including the inventory/bag.
 * @param craftables contains the list of craftable items' names, selectors, and prerequisite items' details.
 * @return None.
 */
void
tpToGeffen(struct playerStats * pPlayer, struct craftableItem craftables[]) {
    char geffenInput[MAX_STR_LEN] = "";
    int goBackToMainMenu = 0;
    struct prompt geffenPrompt = {
        "Geffen", // prompt title
        { // prompt selector and action
            {"1", "Craft Potion"},
            {"2", "Back to Main Menu"}
        },
        2 // amount of prompt options
    };

    do {
        system("cls");
        showPrompt(geffenPrompt);
        getPromptInput(geffenInput, geffenPrompt);

        if (strIsSame(geffenInput, "Craft Potion")) {
            craftPotion(pPlayer, craftables);
        }

        else if (strIsSame(geffenInput, "Back to Main Menu")) {
            goBackToMainMenu = 1;
        }

    } while (goBackToMainMenu == 0);
}

/**
 * Validates the user input in the Buy Items and Sell Items menus. It compares the input against the selectors of the
 * buyable or sellable items.
 * Precondition: listLen is a positive integer.
 * @param storeInput stores the user's string input in the Buy Items and Sell Items menus.
 * @param cancelSelector is a string that is used to cancel buying and selling items in the store.
 * @param itemList is a list of items that can either be bought or sold.
 * @param listLen contains how many items are in the itemList.
 * @return 1 (true) if the input is valid, and 0 (false) otherwise.
 */
int
validateStoreInput(char storeInput[], char cancelSelector[], struct storeItem itemList[], int listLen) {
    int inputIsValid = 0;

    if (strIsSame(storeInput, cancelSelector)) {
        inputIsValid = 1;
    }

    for (int i = 0; i < listLen && inputIsValid == 0; i++) {
        if (strIsSame(storeInput, itemList[i].selector)) {
            inputIsValid = 1;
        }
    }

    return inputIsValid;
}

/**
 * Gets the user input in the Buy Items menu. The user is asked to choose between buying an item or cancelling buying.
 * Their string input is obtained and validated. The user is asked for their input until they enter a valid one.
 * Precondition: None
 * @param buyItemInput is used to store the user's input in the Buy Items menu.
 * @param buyItemList contains the list of buyable items, including their names, prices, and selectors.
 * @return None.
 */
void
getBuyItemsInput(char buyItemInput[], struct storeItem buyItemList[]) {
    int inputIsValid = 0;

    printf("Select item to buy or type '0' to cancel buying.\n\n");
    do {
        getStrUserInput(buyItemInput, MAX_STR_LEN);
        inputIsValid = validateStoreInput(buyItemInput, "0", buyItemList, MAX_BUYABLE_ITEMS);
    } while (inputIsValid == 0);
}

/**
 * Gets the buyable or sellable item name using the user's input. It compares the user's input against the items'
 * selectors in the buyable or sellable item list. It stores the item name in storeItem.
 * Precondition: input contains a valid selector for a buyable or sellable item, and listLen is a positive integer.
 * @param input contains an item selector in the Buy Items or Sell Items menu.
 * @param storeItem is used to store the item name.
 * @param itemList is the buyable or sellable item list.
 * @param listLen is how many items are in the buyable or sellable item list.
 * @return None.
 */
void
getStoreItemName(char input[], char storeItem[], struct storeItem itemList[], int listLen) {
    for (int i = 0; i < listLen; i++) {
        if (strIsSame(input, itemList[i].selector)) {
            strncpy(storeItem, itemList[i].name, MAX_STR_LEN);
        }
    }
}

/**
 * Gets how much of an item the user wants to buy or sell. It gets a string input from the user, then converts it to
 * a non-negative integer. It converts the numbers in the initial part of the string until it encounters a
 * non-numerical character. The user is asked for their input until they enter a non-negative integer.
 * Precondition: None
 * @return the amount of items the user wants to buy or sell. 
 */
int
getStoreItemQty(void) {
    char strItemQty[MAX_STR_LEN] = "";
    int itemQty = 0;

    printf("\nHow much? (Default is 0)\n");
    do {
        getStrUserInput(strItemQty, MAX_STR_LEN);
        itemQty = (int)strtol(strItemQty, NULL, 10);
    } while (!(itemQty >= 0));

    printf("Quantity was set to %d.\n", itemQty);

    return itemQty;
}

/**
 * Gets the given buyable or sellable item's price. It compares the given item name with the items in either the
 * buyable or sellable item list. If there is a match, the price is accessed and returned.
 * Precondition: storeItem contains a valid item name, and listLen is a positive integer.
 * @param storeItem contains a buyable or sellable item name which price is to be found.
 * @param itemList is the buyable or sellable item list.
 * @param listLen contains how many items are in the itemList.
 * @return the price of the given buyable or sellable item.
 */
float
getStoreItemPrice(char storeItem[], struct storeItem itemList[], int listLen) {
    int priceHasBeenFound = 0;
    float price = 0;

    for (int i = 0; i < listLen && priceHasBeenFound == 0; i++) {
        if (strIsSame(storeItem, itemList[i].name)) {
            price = itemList[i].price;
            priceHasBeenFound = 1;
        }
    }

    return price;
}

/**
 * Buys an item from the store. The total price is first calculated by obtaining the base price of the given item
 * and then multiplying it to the specified quantity the player wishes to purchase. If the player has enough money,
 * the item(s) is/are added to the inventory and the total buying price is deducted from their money. If they do not,
 * nothing is bought and they are notified of having insufficient money.
 * Precondition: itemName contains a valid item name, and itemQty is a positive integer.
 * @param itemName contains the name of the item the player wants to purchase.
 * @param itemQty is how many of the item the player wants to purchase.
 * @param itemList is the buyable item list.
 * @param pPlayer is a pointer to the actual variable containing the player's statistics.
 */
void
doBuyItem(char itemName[], int itemQty, struct storeItem itemList[], struct playerStats * pPlayer) {
    float totalBuyPrice = getStoreItemPrice(itemName, itemList, MAX_BUYABLE_ITEMS) * itemQty;
    if (pPlayer->coins >= totalBuyPrice) {
        addItem(itemName, itemQty, pPlayer);
        pPlayer->coins -= totalBuyPrice;
        printf("Bought %d %s for %.2f Ymir coins. Items have been placed in the bag.\n", itemQty, itemName, totalBuyPrice);
    }

    else {
        printf("Not enough money!\n");
    }
}

/**
 * Opens the Buy Items menu. It shows the list of buyable items and the player's inventory/bag and money for
 * convenience. It then gets the user's input and validates it. They can either choose to cancel buying or they
 * could buy an item. If they cancel buying, they go back to the Holgrehenn Store. If they try to buy an item,
 * the item's name and how much of it the user wants to buy are obtained. If they input 0, nothing is bought.
 * If they input a positive integer, they are asked to confirm their action. If they confirm, an attempt to buy
 * the item(s) is made. If they do not confirm, nothing is bought.
 * Precondition: None
 * @param pPlayer is a pointer to the actual variable containing the player's statistics.
 * @param buyItemList contains the list of buyable items, including their names, prices, and selectors.
 * @return None.
 */
void
buyItems(struct playerStats * pPlayer, struct storeItem buyItemList[]) {
    char buyItemInput[MAX_STR_LEN] = "";
    char storeItem[MAX_STR_LEN] = "";
    int itemQty = 0;
    
    system("cls");
    printf("Buy Items\n\n");
    showBuyItemList(buyItemList);
    displayBag(pPlayer);
    displayCoins(pPlayer);
    getBuyItemsInput(buyItemInput, buyItemList);

    if (!strIsSame(buyItemInput, "0")) { // cancel buying
        getStoreItemName(buyItemInput, storeItem, buyItemList, MAX_BUYABLE_ITEMS);
        itemQty = getStoreItemQty(); // itemQty can only be a non-negative integer

        if (itemQty > 0) {
            int userIsSure = getUserConfirmation();
            if (userIsSure == 1) {
                // if user has enough money, the item(s) is/are bought; otherwise, nothing is bought
                doBuyItem(storeItem, itemQty, buyItemList, pPlayer);
            }

            else if (userIsSure == 0) {
                printf("Did not buy anything.\n");
            }
        }

        else if (itemQty == 0) {
            printf("Did not buy anything.\n");
        }
        printf("\n");
        system("pause");
    }
}

/**
 * Gets the user input in the Sell Items menu. The user is asked to choose between selling an item or cancelling
 * selling. Their string input is obtained and validated. The user is asked for their input until they enter a
 * valid input.
 * Precondition: None
 * @param sellItemInput is used to store the user's string input in the Sell Items menu.
 * @param sellItemList contains the list of sellable items, including their names, prices, and selectors.
 * @return None.
 */
void
getSellItemsInput(char sellItemInput[], struct storeItem sellItemList[]) {
    int inputIsValid = 0;

    printf("Select item to sell or type '0' to cancel selling.\n\n");
    do {
        getStrUserInput(sellItemInput, MAX_STR_LEN);
        inputIsValid = validateStoreInput(sellItemInput, "0", sellItemList, MAX_SELLABLE_ITEMS);
    } while (inputIsValid == 0);
}

/**
 * Sells an item from the inventory. The total selling price is first calculated by obtaining the item's base price
 * and multiplying it to the quantity the user wishes to sell. The amount of the item that the player currently has
 * is also obtained. If the amount in the player's inventory is greater than or equal to the amount of items they wish
 * to sell, the items are deducted from the inventory and the total selling price is added to the player's current
 * balance. Otherwise, they are notified of having insufficient items in their inventory.
 * Precondition: itemName contains a valid sellable item name, and itemQty is a positive integer.
 * @param itemName contains the sellable item's name.
 * @param itemQty is how much of the item the player wishes to sell.
 * @param itemList contains the sellable item list.
 * @param pPlayer is a pointer to the actual variable containing the player's statistics.
 * @return None.
 */
void
doSellItem(char itemName[], int itemQty, struct storeItem itemList[], struct playerStats * pPlayer) {
    float totalSellPrice = getStoreItemPrice(itemName, itemList, MAX_SELLABLE_ITEMS) * itemQty;
    int itemQtyInInv = getItemQtyFromInv(itemName, pPlayer);

    if (itemQtyInInv >= itemQty) {
        deductItem(itemName, itemQty, pPlayer);
        pPlayer->coins += totalSellPrice;
        printf("Sold %d %s for %.2f Ymir coins. Items have been removed from the bag.\n", itemQty, itemName, totalSellPrice);
    }

    else {
        printf("Not enough items in inventory!\n");
    }
}

/**
 * Opens the Sell Items menu. It first shows the list of sellable items and the player's inventory/bag and money for
 * convenience. The user's input is obtained and validated. They can either choose to cancel selling or proceed to
 * sell items. If they cancel selling, they go back to the Holgrehenn Store. If they choose to sell items, the item's
 * name and the amount they wish to sell are obtained. If they enter 0, nothing is sold. If they enter a positive
 * integer, they are asked for confirmation. If they confirm, an attempt to sell the item(s) is made. If they do not
 * confirm, nothing is sold.
 * Precondition: None
 * @param pPlayer is a pointer to the actual variable containing the player's statistics.
 * @param sellItemList contains the list of sellable items, including their names, prices, and selectors.
 * @return None.
 */
void
sellItems(struct playerStats * pPlayer, struct storeItem sellItemList[]) {
    char sellItemInput[MAX_STR_LEN] = "";
    char storeItem[MAX_STR_LEN] = "";
    int itemQty = 0;

    system("cls");
    printf("Sell Items\n\n");
    showSellItemList(sellItemList);
    displayBag(pPlayer);
    displayCoins(pPlayer);
    getSellItemsInput(sellItemInput, sellItemList);

    if (!strIsSame(sellItemInput, "0")) { // cancel selling
        getStoreItemName(sellItemInput, storeItem, sellItemList, MAX_SELLABLE_ITEMS);
        itemQty = getStoreItemQty(); // itemQty can only be a non-negative integer

        if (itemQty > 0) {
            int userIsSure = getUserConfirmation();
            if (userIsSure == 1) {
                // if player has enough items in inventory, the item(s) is/are sold; otherwise, nothing is sold
                doSellItem(storeItem, itemQty, sellItemList, pPlayer);
            }

            else if (userIsSure == 0) {
                printf("Did not sell anything.\n");
            }
        }

        else if (itemQty == 0) {
            printf("Did not sell anything.\n");
        }
        printf("\n");
        system("pause");
    }
}

/**
 * Opens the Holgrehenn Store menu. The user is presented with three options: to buy items, to sell items, or to go
 * back to the main menu. If they choose to buy items, the Buy Items menu is opened. If they choose to sell items,
 * the Sell Items menu is opened. The third option is the only way for them to back to the main menu.
 * Precondition: None
 * @param pPlayer is a pointer to the actual variable containing the player's statistics.
 * @param buyItemList contains the list of buyable items, including their names, prices, and selectors.
 * @param sellItemList contains the list of sellable items, including their names, prices, and selectors.
 * @return None.
 */
void
tpToStore(struct playerStats * pPlayer, struct storeItem buyItemList[], struct storeItem sellItemList[]) {
    char storeInput[MAX_STR_LEN] = "";
    int goBackToMainMenu = 0;
    struct prompt storePrompt = {
        "Holgrehenn Store", // prompt title
        { // prompt selector and action
            {"1", "Buy Items"},
            {"2", "Sell Items"},
            {"3", "Back to Main Menu"}
        },
        3 // amount of prompt options
    };

    do {
        system("cls");
        showPrompt(storePrompt);
        getPromptInput(storeInput, storePrompt);

        if (strIsSame(storeInput, "Buy Items")) {
            buyItems(pPlayer, buyItemList);
        }

        else if (strIsSame(storeInput, "Sell Items")) {
            sellItems(pPlayer, sellItemList);
        }

        else if (strIsSame(storeInput, "Back to Main Menu")) {
            goBackToMainMenu = 1;
        }

    } while (goBackToMainMenu == 0);
}

/**
 * Replaces the current title of a prompt with a given string.
 * Precondition: None
 * @param title contains a string of the new title.
 * @param givenPrompt is the prompt which title is to be changed.
 * @return None.
 */
void
changePromptTitle(char title[], struct prompt * givenPrompt) {
    strncpy(givenPrompt->title, title, MAX_STR_LEN);
}

/**
 * Sets the rare fish catch chance based on whether a magical bait is equipped by the player.
 * Precondition: None
 * @param pMagBaitIsEquipped indicates if a magical bait is equipped by the player.
 * @return the appropriate rare fish catch chance.
 */
int
setRareFishCatchChance(int * pMagBaitIsEquipped) {
    int rareFishCatchChance = 0;

    switch (*pMagBaitIsEquipped) {
        case 0: rareFishCatchChance = 10; break; // magical bait is NOT equipped
        case 1: rareFishCatchChance = 30; break; // magical bait is equipped
    }

    return rareFishCatchChance;
}

/**
 * Generates a pseudo-random integer between a given lower bound and upper bound, inclusive.
 * Precondition: None
 * @param lowerBound is the lowest integer that can be generated.
 * @param upperBound is the highest integer that can be generated.
 * @return a pseudo-random integer within the given bounds, inclusive. 
 */
int
genRandomNum(int lowerBound, int upperBound) {
    return rand() % (upperBound + 1 - lowerBound) + lowerBound;
}

/**
 * Sets the obtainable common and rare fish given the fishing location. It first finds the player's current fishing
 * location, then its associated common and rare fish are copied to variables to be used by other functions.
 * Precondition: currentFishingLoc contains a valid fishing location name, and fishingLocsAmt is a positive integer.
 * @param currentFishingLoc contains the player's current fishing location.
 * @param fishingLocations is a list containing the fishing locations' names in the game.
 * @param fishingLocsAmt is the amount of fishing locations in the game.
 * @param currentRareFish is used to store the appropriate rare fish that can be obtained in a fishing location.
 * @param currentCommonFish is used to store the appropriate common fish that can be obtained in a fishing location.
 * @return None.
 */
void
setObtainableFish(char currentFishingLoc[], struct fishingLoc fishingLocations[], int fishingLocsAmt,
                  char currentRareFish[], char currentCommonFish[]) {
    int fishesHasBeenSet = 0;
    for (int i = 0; i < fishingLocsAmt && fishesHasBeenSet == 0; i++) {
        if (strIsSame(currentFishingLoc, fishingLocations[i].name)) {
            strncpy(currentRareFish, fishingLocations[i].rareFish, MAX_STR_LEN);
            strncpy(currentCommonFish, fishingLocations[i].commonFish, MAX_STR_LEN);
            fishesHasBeenSet = 1;
        }
    }
}

/**
 * Uses up the magical bait of the player. Specifically, if the variable magBaitIsEquipped's value is 1,
 * it is set to 0, implying the player no longer has a magical bait equipped, then the player is notified of having
 * used up their magical bait. Nothing is deducted from the inventory/bag as one magical bait is already deducted from
 * the inventory/bag once the player equips one.
 * Precondition: None
 * @param pMagBaitIsEquipped indicates if the user has a magical bait equipped.
 * @return None.
 */
void
useUpMagBait(int * pMagBaitIsEquipped) {
    if (*pMagBaitIsEquipped == 1) {
        *pMagBaitIsEquipped = 0;
        printf("You used up your Magical Bait.\n");
    }
}

/**
 * Catches a fish. The rare fish catch chance is first set based on whether the player has a magical bait equipped.
 * Then a pseudo-random number from 1 to 100 is generated. If that number is less than or equal to the rare fish
 * catch chance, the player catches a rare fish. If it is greater than the catch chance, the player catches a common
 * fish. The fish is then added to the inventory and the player is notified of what fish they caught. If the player
 * had a magical bait equipped prior to catching a fish, it is used up.
 * Precondition: currentRareFish and currentCommonFish contain the appropriate rare and common fish for the fishing
 * location, respectively.
 * @param currentRareFish contains the appropriate rare fish for the fishing location.
 * @param currentCommonFish contains the appropriate common fish for the fishing location.
 * @param pPlayer is a pointer to the actual variable containing the player's statistics.
 * @param pMagBaitIsEquipped indicates whether the player has a magical bait equipped.
 * @return None.
 */
void
catchFish(char currentRareFish[], char currentCommonFish[], struct playerStats * pPlayer, int * pMagBaitIsEquipped) {
    int rareFishCatchChance = setRareFishCatchChance(pMagBaitIsEquipped);
    int rngLowerBound = 1, rngUpperBound = 100;
    int randomNum = genRandomNum(rngLowerBound, rngUpperBound);

    if (randomNum <= rareFishCatchChance) { // player catches a rare fish
        addItem(currentRareFish, 1, pPlayer);
        printf("Wow! You caught a %s! It has been added to the bag.\n", currentRareFish);
    }

    else if (randomNum > rareFishCatchChance) { // player catches a common fish
        addItem(currentCommonFish, 1, pPlayer);
        printf("You caught a %s! It has been added to the bag.\n", currentCommonFish);
    }

    useUpMagBait(pMagBaitIsEquipped);

    system("pause");
}

/**
 * Tries to equip a magical bait. It first gets the amount of magical bait the player currently has in their
 * inventory/bag. It is then checked if they currently have a magical bait equipped. If they do not and they have
 * at least one magical bait in their inventory, it is equipped, the item is deducted from their inventory/bag, and
 * they are notified of equipping a magical bait. If they do not have a magical bait equipped but they do not have
 * at least one in their inventory/bag, they are notified of having no magical bait in their inventory/bag. If they
 * had a magical bait equipped in the first place, then nothing is equipped and deducted from the inventory, and they
 * are notified of already having a magical bait equipped.
 * Precondition: None
 * @param pMagBaitIsEquipped indicates whether the player already has a magical bait equipped.
 * @param pPlayer is a pointer to the actual variable containing the player's statistics.
 * @return None.
 */
void
equipMagBait(int * pMagBaitIsEquipped, struct playerStats * pPlayer) {
    int magBaitInInv = getItemQtyFromInv("Magical Bait", pPlayer);

    if (*pMagBaitIsEquipped == 0) {

        if (magBaitInInv > 0) {
            *pMagBaitIsEquipped = 1;
            deductItem("Magical Bait", 1, pPlayer);
            printf("A Magical Bait has been equipped.\n");
        }

        else if (!(magBaitInInv > 0)) {
            printf("No Magical Bait in inventory!\n");
        }
    }

    else if (*pMagBaitIsEquipped == 1) {
        printf("You already have a Magical Bait equipped!\n");
    }

    system("pause");
}

/**
 * De-equips a magical bait if the player has one equipped. It is checked if the player has one equipped, and if
 * they do, it is unequipped and added back to the inventory/bag. The player is then notified of this. Otherwise,
 * nothing is done.
 * Precondition: None
 * @param pMagBaitIsEquipped indicates whether the player already has a magical bait equipped.
 * @param pPlayer is a pointer to the actual variable containing the player's statistics.
 * @return None.
 */
void
deEquipMagBait(int * pMagBaitIsEquipped, struct playerStats * pPlayer) {
    if (*pMagBaitIsEquipped == 1) {
        *pMagBaitIsEquipped = 0;
        addItem("Magical Bait", 1, pPlayer);
        printf("A Magical Bait was unused so it was unequipped and stored back in the bag.\n");
        system("pause");
    }
}

/**
 * Shows whether the player has a magical bait equipped or not.
 * Precondition: None
 * @param pMagBaitIsEquipped indicates whether the player has a magical bait equipped.
 * @return None.
 */
void
showMagBaitStatus(int * pMagBaitIsEquipped) {
    printf("Magical Bait: %sEquipped\n\n", *pMagBaitIsEquipped == 0 ? "Not " : "");
}

/**
 * Teleports to the fishing location chosen by the user (options #3-#6). The user can choose to catch fish, equip a
 * magical bait, or go back to the main menu. If they choose to catch a fish, they catch a fish based on the fishing
 * location, which chances depend on whether they are equipping a magical bait. If they choose to equip a magical
 * bait, an attempt to equip one is made depending on whether they have already have one equipped and if they have
 * one in their inventory/bag. The third option is the only way to leave the fishing location and go back to the main
 * menu.
 * Precondition: locationName contains a valid fishing location name.
 * @param pPlayer is a pointer to the actual variable containing the player's statistics.
 * @param locationName contains the current fishing location's name.
 * @return None.
 */
void
tpToFishingLocation(struct playerStats * pPlayer, char locationName[]) {
    char fishingLocationInput[MAX_STR_LEN] = "";
    char currentRareFish[MAX_STR_LEN] = "";
    char currentCommonFish[MAX_STR_LEN] = "";
    int magBaitIsEquipped = 0;
    int * pMagBaitIsEquipped = &magBaitIsEquipped;
    struct prompt fishingLocationPrompt = {
        "temp", // temp prompt title
        { // prompt selectors and action
            {"1", "Catch Fish"},
            {"2", "Equip Magical Bait"},
            {"3", "Back to Main Menu"}
        },
        3 // amount of prompt options
    };
    changePromptTitle(locationName, &fishingLocationPrompt);
    struct fishingLoc fishingLocations[] = {
        {"Taal Lake", "Tilapia", "Scaleless Blackfish"},
        {"Galathea Deep", "Sardine", "Mariana Snailfish"},
        {"Dagupan Mangrove Forests", "Bangus", "Mudskipper"},
        {"Mindanao Current", "Tuna", "Hillstream Loach"}
    };
    int fishingLocsAmt = (int)(sizeof(fishingLocations) / sizeof(fishingLocations[0]));
    setObtainableFish(locationName, fishingLocations, fishingLocsAmt, currentRareFish, currentCommonFish);

    int goBackToMainMenu = 0;
    
    do {
        system("cls");
        showMagBaitStatus(pMagBaitIsEquipped);
        showPrompt(fishingLocationPrompt);
        getPromptInput(fishingLocationInput, fishingLocationPrompt);

        if (strIsSame(fishingLocationInput, "Catch Fish")) {
            catchFish(currentRareFish, currentCommonFish, pPlayer, pMagBaitIsEquipped);
        }

        else if (strIsSame(fishingLocationInput, "Equip Magical Bait")) {
            equipMagBait(pMagBaitIsEquipped, pPlayer);
        }

        else if (strIsSame(fishingLocationInput, "Back to Main Menu")) {
            deEquipMagBait(pMagBaitIsEquipped, pPlayer);
            goBackToMainMenu = 1;
        }

    } while (goBackToMainMenu == 0);
}

/**
 * Extracts the location name the user wants to travel to from the main menu input/action. It searches the given input
 * string for the location names, and if there is a match, the matching location name is copied to a variable.
 * Otherwise, the variable is left untouched.
 * Precondition: The location names in locations (the 2D array) are literally in the prompt options in the main menu.
 * @param input is the main menu input/action of the user where the location name is to be extracted from.
 * @param locationName is used to store the name of the location.
 * @param locations is the list of (six) locations in the game (excludes the bag and exiting the program).
 * @return None.
 */
void
getLocationFromInput(char input[], char locationName[], char locations[][MAX_STR_LEN]) {
    int locationHasBeenFound = 0;
    for (int i = 0; i < MAX_LOCATIONS_AMT && locationHasBeenFound == 0; i++) {
        if (strstr(input, locations[i]) != NULL) {
            strncpy(locationName, locations[i], MAX_STR_LEN);
            locationHasBeenFound = 1;
        }
    }
}

/**
 * Executes the action the user wants to in the main menu. In other words, it calls the proper functions for each
 * action/prompt option. The option/action is first converted to a location name, if possible. If locationName is
 * not empty, the player teleports to the designated location. Otherwise, they either get to check their bag or they
 * try to exit the program. The function returns either a 1 (true) or a 0 (false), and it can only return 1 if the
 * player tries to exit the program and confirms their action.
 * Precondition: The location names in locations (the 2D array) are literally in the prompt options in the main menu.
 *               This makes extracting the location names from the mainMenuInput possible.
 * @param mainMenuInput contains the action that the user wants to do, and where the location name is extracted from.
 * @param pPlayer is a pointer to the actual variable containing the player's statistics.
 * @param craftables contains the list of craftable items' names, selectors, and prerequisite items' details.
 * @param buyItemList contains the list of buyable items, including their names, prices, and selectors.
 * @param sellItemList contains the list of sellable items, including their names, prices, and selectors.
 * @return 1 if the user wishes to exit the program and they confirm their action; otherwise, returns 0.
 */
int
doMainMenuAction(char mainMenuInput[], struct playerStats * pPlayer, struct craftableItem craftables[],
                     struct storeItem buyItemList[], struct storeItem sellItemList[]) {
    int exitMainMenu = 0;
    char locationName[MAX_STR_LEN] = "";
    char locations[][MAX_STR_LEN] = {
        "Geffen",
        "Holgrehenn Store",
        "Taal Lake",
        "Galathea Deep",
        "Dagupan Mangrove Forests",
        "Mindanao Current"
    };

    getLocationFromInput(mainMenuInput, locationName, locations);

    if (!strIsSame(locationName, "")) {
        
        if (strIsSame(locationName, "Geffen")) {
            tpToGeffen(pPlayer, craftables);
        }
        
        else if (strIsSame(locationName, "Holgrehenn Store")) {
            tpToStore(pPlayer, buyItemList, sellItemList);
        }
        
        else if (strIsSame(locationName, "Taal Lake") ||
                 strIsSame(locationName, "Galathea Deep") ||
                 strIsSame(locationName, "Dagupan Mangrove Forests") ||
                 strIsSame(locationName, "Mindanao Current")
                ) {
                    tpToFishingLocation(pPlayer, locationName);
                }
    }

    else if (strIsSame(mainMenuInput, "Check Bag")) {
        checkBag(pPlayer);
    }

    else if (strIsSame(mainMenuInput, "Exit Program")) {
        exitMainMenu = exitProg();
    }

    return exitMainMenu;
}

/**
 * Opens the main menu. Presents the main menu options and gets the user input. The action that the user wants to do
 * is then executed until they go back to the main menu, where they are presented the menu options and their input is
 * obtained again. This function will only terminate once they try to exit the program and they confirm their action.
 * Precondition: None
 * @param pPlayer is a pointer to the actual variable containing the player's statistics.
 * @param craftables contains the list of craftable items' names, selectors, and prerequisite items' details.
 * @param buyItemList contains the list of buyable items, including their names, prices, and selectors.
 * @param sellItemList contains the list of sellable items, including their names, prices, and selectors.
 * @return None.
 */
void
mainMenu(struct playerStats * pPlayer, struct craftableItem craftables[], struct storeItem buyItemList[],
              struct storeItem sellItemList[]) {
    
    char mainMenuInput[MAX_STR_LEN] = "";
    int exitMainMenu = 0;

    struct prompt mainMenuPrompt = {
        "Main Menu", // title of prompt
        { // prompt selectors and options
            {"1", "Teleport to Geffen"},
            {"2", "Teleport to Holgrehenn Store"},
            {"3", "Teleport to Taal Lake"},
            {"4", "Teleport to Galathea Deep"},
            {"5", "Teleport to Dagupan Mangrove Forests"},
            {"6", "Teleport to Mindanao Current"},
            {"7", "Check Bag"},
            {"8", "Exit Program"}
        },
        8 // amount of prompt options
    };

    do {
        system("cls");
        showPrompt(mainMenuPrompt);
        getPromptInput(mainMenuInput, mainMenuPrompt);
        exitMainMenu = doMainMenuAction(mainMenuInput, pPlayer, craftables,
                                        buyItemList, sellItemList);
    } while (exitMainMenu == 0);
}

/**
 * Initializes the necessary variables for the game to run, then displays the opening message before showing the main
 * menu to the player.
 * Precondition: None
 * @return None.
 */
void
game(void) {
    srand(time(NULL));

    struct playerStats player = {0};
    struct playerStats * pPlayer = &player;
    initializePlayerStats(pPlayer);
    struct craftableItem craftables[MAX_CRAFTABLE_ITEMS] = {0};
    initializeCraftables(craftables);
    struct storeItem buyItemList[MAX_BUYABLE_ITEMS];
    initializeBuyItemList(buyItemList);
    struct storeItem sellItemList[MAX_SELLABLE_ITEMS] = {0};
    initializeSellItemList(sellItemList);

    system("cls");
    showOpeningMessage();
    system("pause");
    system("cls");

    mainMenu(pPlayer, craftables, buyItemList, sellItemList);
}

/**
 * Executes game() which actually starts the game.
 * Precondition: None
 * @return 0 to signify that the program ran successfully.
 */
int
main(void) {

    game();
    return 0;
}