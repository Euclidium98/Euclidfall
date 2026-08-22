
typedef struct {
    unsigned char animation[4]; //what FS file is used
    unsigned char maxframes[4]; //(unused) how many frames for file, as per /data/D1.bin
	unsigned char moves; // 3-4
	unsigned char damage[4]; // base damage for hit regardless of skill or material
	unsigned char bluntdamage[4]; // damage through armour
	unsigned char handed; //1-Right hand, 2-Left hand, 3-Both
	unsigned int timedelay[4]; // how may frames each move will take as a base (this does not take into account the time taken for the animation), should be > maxframes
	unsigned int hitdelay[4]; // after user input, how many frames until the hit will be hit.
} WeaponDataStruct;

/*
Move 1: Defence
Move 2: Balanced
Move 3: Strong
Move 4: Special, non-standard
*/
void LoadWeaponValues();
WeaponDataStruct WeaponData[21];

//0. Onehanded sword
//1. Onehanded Axe
//2. Mace
//3. Small Shield
//4. Big Shield
//5.
//6.
//7.
//8. Spear
//9. Polearm or beak
//10. Big sword
//11. Big Axe
//12. Big Hammer
//13. Staff
//14. Bow
//15. CrossBow
//16. Gun
//17. Sling
//18.
//19. Drill
//20. Unarmed
//21.
//22.
//23.
//24.
//25.
//26.
//27.
//28.
//29.
// {{0,0,0,0},{0,0,0,0},0,{0,0,0,0},{0,0,0,0},0,{0,0,0,0},{0,0,0,0}}

/*
Move 1: Defence
Move 2: Balanced
Move 3: Strong
Move 4: Special, non-standard
*/
// Anime - Frames - Moves - Damage - BluntDamage - Handed - timedelay - hitdelay
void LoadWeaponValues(){
	 WeaponData[0] = (WeaponDataStruct){{1,0,2,0},{0,0,0,0},3,{0,0,0,0},{0,0,0,0},1,{0,0,0,0},{0,0,0,0}}; // Onehanded sword		
	 WeaponData[1] = (WeaponDataStruct){{4,3,5,0},{0,0,0,0},3,{0,0,0,0},{0,0,0,0},1,{0,0,0,0},{7,8,12,0}}; // Onehanded Axe
	 WeaponData[2] = (WeaponDataStruct){{7,6,8,0},{0,0,0,0},3,{0,0,0,0},{0,0,0,0},1,{0,0,0,0},{15,8,17,0}}; // Mace
	 WeaponData[3] = (WeaponDataStruct){{0,0,0,0},{0,0,0,0},3,{0,0,0,0},{0,0,0,0},2,{0,0,0,0},{0,0,0,0}}; // Small Shield
	 WeaponData[4] = (WeaponDataStruct){{0,0,0,0},{0,0,0,0},3,{0,0,0,0},{0,0,0,0},2,{0,0,0,0},{0,0,0,0}}; // Big Shield
	 WeaponData[5] = (WeaponDataStruct){{0,0,0,0},{0,0,0,0},3,{0,0,0,0},{0,0,0,0},0,{0,0,0,0},{0,0,0,0}}; //
	 WeaponData[6] = (WeaponDataStruct){{0,0,0,0},{0,0,0,0},3,{0,0,0,0},{0,0,0,0},0,{0,0,0,0},{0,0,0,0}}; //
	 WeaponData[7] = (WeaponDataStruct){{0,0,0,0},{0,0,0,0},3,{0,0,0,0},{0,0,0,0},0,{0,0,0,0},{0,0,0,0}}; //
	 WeaponData[8] = (WeaponDataStruct){{0,0,0,0},{0,0,0,0},3,{0,0,0,0},{0,0,0,0},3,{0,0,0,0},{0,0,0,0}}; // Spear
	 WeaponData[9] = (WeaponDataStruct){{0,0,0,0},{0,0,0,0},3,{0,0,0,0},{0,0,0,0},3,{0,0,0,0},{0,0,0,0}}; // Polearm or beak
	WeaponData[10] = (WeaponDataStruct){{0,0,0,0},{0,0,0,0},3,{0,0,0,0},{0,0,0,0},3,{0,0,0,0},{0,0,0,0}}; // Big sword
	WeaponData[11] = (WeaponDataStruct){{0,0,0,0},{0,0,0,0},3,{0,0,0,0},{0,0,0,0},3,{0,0,0,0},{0,0,0,0}}; // Big Axe
	WeaponData[12] = (WeaponDataStruct){{0,0,0,0},{0,0,0,0},3,{0,0,0,0},{0,0,0,0},3,{0,0,0,0},{0,0,0,0}}; // Big Hammer
	WeaponData[13] = (WeaponDataStruct){{0,0,0,0},{0,0,0,0},3,{0,0,0,0},{0,0,0,0},3,{0,0,0,0},{0,0,0,0}}; // Staff
	WeaponData[14] = (WeaponDataStruct){{0,0,0,0},{0,0,0,0},3,{0,0,0,0},{0,0,0,0},3,{0,0,0,0},{0,0,0,0}}; // Bow
	WeaponData[15] = (WeaponDataStruct){{0,0,0,0},{0,0,0,0},3,{0,0,0,0},{0,0,0,0},3,{0,0,0,0},{0,0,0,0}}; // CrossBow
	WeaponData[16] = (WeaponDataStruct){{0,0,0,0},{0,0,0,0},3,{0,0,0,0},{0,0,0,0},3,{0,0,0,0},{0,0,0,0}}; // Gun
	WeaponData[17] = (WeaponDataStruct){{0,0,0,0},{0,0,0,0},3,{0,0,0,0},{0,0,0,0},3,{0,0,0,0},{0,0,0,0}}; // Sling
	WeaponData[18] = (WeaponDataStruct){{0,0,0,0},{0,0,0,0},3,{0,0,0,0},{0,0,0,0},0,{0,0,0,0},{0,0,0,0}}; //
	WeaponData[19] = (WeaponDataStruct){{0,0,0,0},{0,0,0,0},3,{0,0,0,0},{0,0,0,0},3,{0,0,0,0},{0,0,0,0}}; // Drill
	WeaponData[20] = (WeaponDataStruct){{0,0,0,0},{0,0,0,0},3,{0,0,0,0},{0,0,0,0},3,{0,0,0,0},{0,0,0,0}}; // Unarmed	
}

int CalcWeaponDmg(char type, char weapontype, char material, char fightingstat){
	switch (type){
	case 1:	
		return 
	break;
	case 2:	
		return 
	break;
	case 3:	
		return 
	break;
	}
}

