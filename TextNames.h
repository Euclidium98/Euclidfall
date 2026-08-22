

#define MAX_WORDS_MATERIAL 17
#define MAX_WORDS_WEAPON 17
#define MAX_WORDS_CLOTHES 7
#define MAX_WORDS_POTIONS 19
#define MAX_WORDS_USELESS 8
#define MAX_WORDS_BUILDINGNAME 11
#define MAX_LEN 32 

char materialstring[MAX_WORDS_MATERIAL][MAX_LEN] = {
        "copper", 
		"tin", 
		"bismuth", 
		"zinc", 
		"nickel",
        "leather", 
		"lead", 
		"cast iron", 
		"wrought iron",
		"bronze",
		"bismuth Bronze",
		"brass",
		"steel grade I",
		"steel grade II",
		"steel grade III",
		"uranium",
		""
    };
char weaponstring[MAX_WORDS_WEAPON][MAX_LEN] = {
        "sword",
		"axe",
		"mace",
		"small shield",
		"tall shield",
		"",
		"",
		"",
		"",
		"",
		"big sword",
		"big axe",
		"big hammer",
		"staff",
		"bow",
		"crossbow",
		""
    };
	
char clothestring[MAX_WORDS_CLOTHES][MAX_LEN] = {
        "helmet",
		"chestplate",
		"gloves",
		"pants",
		"boots",
		"cloak",
		""
    };
	
char potionstring[MAX_WORDS_POTIONS][MAX_LEN] = {
        "nothing",
		"minor healing",
		"average healing",
		"lots of healing",
		"epic",
		"yellow",
		"3am",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"cookie",
		"soup",
		""
    };
	
char uselessstring[MAX_WORDS_USELESS][MAX_LEN] = {
        "copper ore",
		"tin ore",
		"bismuth ore",
		"zinc ore",
		"iron ore",
		"lead ore",
		"uranium ore",
		""
    };	

char buildingstring[MAX_WORDS_BUILDINGNAME][MAX_LEN] = {
        "NULL",
		"House",
		"General Store",
		"Inn",
		"Temple",
		"Weapon Store",
		"Armoury",
		"Jewler",
		"Alchemist",
		"Clothes Store",
		"Guild",
		"Bank",
		""
    };	
	
char *Matstrings[MAX_WORDS_MATERIAL];
char *Weaponstrings[MAX_WORDS_WEAPON];
char *Clothestrings[MAX_WORDS_CLOTHES];
char *Potionstrings[MAX_WORDS_POTIONS];
char *Uselessstrings[MAX_WORDS_USELESS];
char *Buildingstrings[MAX_WORDS_BUILDINGNAME];

void TextNamesRun(){
for (int i = 0; i < MAX_WORDS_MATERIAL; i++) {
        Matstrings[i] = materialstring[i];  // Point to start of each
}
for (int i = 0; i < MAX_WORDS_WEAPON; i++) {
        Weaponstrings[i] = weaponstring[i];  // Point to start of each
}
for (int i = 0; i < MAX_WORDS_CLOTHES; i++) {
        Clothestrings[i] = clothestring[i];  // Point to start of each
}
for (int i = 0; i < MAX_WORDS_POTIONS; i++) {
        Potionstrings[i] = potionstring[i];  // Point to start of each
}
for (int i = 0; i < MAX_WORDS_USELESS; i++) {
        Uselessstrings[i] = uselessstring[i];  // Point to start of each
}
for (int i = 0; i < MAX_WORDS_BUILDINGNAME; i++) {
        Buildingstrings[i] = buildingstring[i];  // Point to start of each
}


}