
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <dirent.h>
#include <string.h> 
#include "objamount.h"

#include "BMPWriter.h"
#include "WeaponData.h"

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"  


#define DEBUG
#define DEBUGCHARACTER
//#define STACKDEBUG
#define CUSTOMMOVEMENT

#define RandomArrayMax 1000

#ifdef DEBUG
unsigned char RandomDebugValues[5] = {0,0,0,0,0};
#endif

char GameState = 1;
char GameStateDelta = 1;
char NewArea = 0;
Vector3 calculate_vector(Vector3 A, Vector3 B);
const int windowX = 800*2;
const int windowY = 450*2;



// ----- MOVEMENT
void probecontrols(); // handles inputs for controls during 3D 
void probecontrolsUI(); // handles inputs for controls during 2D
char probecontrolsUImenu(unsigned char UIoptionINTs, unsigned char UIoptionINTsSIDE); // newer version of previous, for menus
char CurrentMenuOption = 0;
char CurrentMenuOptionSide = 0;
long archingX = 1; // Current zone/chunk the character is in
long archingZ = 1; // Current zone/chunk the character is in
float *Px;
float *Py;
float *Pz;

float *Ax;
float *Ay;
float *Az;
Camera *pointcam;
Camera3D *pointcam3D;
Vector3 oldCamPos;
Vector3 *newCamPos;
Vector3 oldLookPos;
Vector3 *newLookPos;

void IncreaseValue(unsigned int KeyPress, unsigned char *ptr, unsigned char ArrayLocation, unsigned maxamount); // i think this is for probecontrolsUImenu

void Constant3DInit(unsigned char UsingChunkCheck);

#ifdef CUSTOMMOVEMENT
Vector3 MoveDelta = {0,0,0};
Vector3 LookDelta = {0,0,0};
#endif

// ----- MALLOCSTUFF
void malloccalls(); // ram allocation along with intialising
void freemalloc(); // frees all the ram
int mapfilesize = 0;
unsigned char *mapdata;
unsigned char *mapdata2;
void postGLcalls(); // same as mallccalls but takes place after the GPU has intialised
int FilesInDir(const char *path); // not used
unsigned char *RandomArray;

// ----- STACK THINGS????

// all of these are defined in filecounter.exe
Model Buildings[Hbuildings]  = { 0 };
Texture2D billboardTs[Htextures];
Model MiscModels[Hmodels];
Texture2D signs[Hsigns];
Texture2D UIimages[Hui];
Font fonts[Hfonts];
Texture2D billboards[Hbillboards];
Model EnviromentModel[Henviroments];
Image MapUIImage;
Texture2D MapUITexture;
Model DungeonBottom[Hdungeon1];
Model DungeonTop[Hdungeon2];
Model DungeonFloor[Hdungeon3];
Model DetailObject[Hdetail];
Model InsideBuilding[Hinside];
Texture2D AnimatedGraphic[Hanimated];
Texture2D FullscreenGraphic[Hfullscreen];

// ----- UI STUFF

void renderUI(); // renders UI for 3D
void renderShopUI(); 
void miningUI();
void mapUI();
void showlimbdamage(); // shows the person in the menu
char UIState = 0;
char UIShopType = 0;
char UIShopeLocalLocation = 0;
void InteractableObject(Model FunObject,Vector3 LocationOfInteractable, char Type, char Data1, char Data2);
Color ColourDamage(char DamageAmount);
#include "TextNames.h"
char amountofitemsplayer = 0;
unsigned int visableitems=0;
void DrawInventory(char InventoryMode);
char* ReturnItemName(
	unsigned char IT0,
	unsigned char IT1,
	unsigned char IT2,
	unsigned char IT3,
	unsigned char IT4,
	unsigned char IT5,
	unsigned char IT6,
	unsigned char IT7,
	unsigned char IT8,
	unsigned char IT9);
char* ReturnItemNameInventory(unsigned char LocationInventory);
int InventorySelection = 0;
void DrawAnimated(Texture2D TheTextureAnime, Vector2 AnimePlacement, int AnimeScale, int AnimeFrame, int AnimeFrameMax); // looping animation
void DrawAnimated2(Texture2D TheTextureAnime, Vector2 AnimePlacement, int AnimeScale, int AnimeFrame, int AnimeFrameMax); // non looping animation
void renderClock(Vector2 ClockLoc);
unsigned char* MaxFramesData;

// ----- CHARACTER
#define amountofdamageablelimbs 50

char LimbDamage[amountofdamageablelimbs];

Ray CharacterRay;
RayCollision AnswerRay;
char *Inventory;
char *InventoryLookup;
int CharacterWeight = 0;
int CharacterWeightMax = 0;
long CharacterMoney = 1000;
void RefreshInventory();
typedef struct {
	char LEVEL;
	char STR;
	char INT;
	char WIL;
	char AGI;
	char END;
	char PER;
} stats;
typedef struct {
	char onehand;
	char twohand;
	char blocking;
	char hand2hand;
	char speech;
	char lockpicking;
	char medical;
	char stealth;
	char ranged;
} skills;
stats PlayerStats = {0};
skills PlayerSkills = {0};
int PlayerHealth;
int PlayerStamina = 100;
int PlayerStaminaMax = 100;
int PlayerMagic;
unsigned char IsSprint = 0; // 0 not sprinting, 1 is sprinting, 2 recharging and cannot use until full
void PutInInventory(char B0,char B1,char B2,char B3,char B4,char B5,char B6,char B7,char B8,char B9);
void DestoryItem(unsigned char ToDestory);
void AttackCheck();

Vector3 ReturnMove = {0,0,0};
Vector3 ReturnLook = {0,0,0};

struct Item{
	char type;
	char extra;
	char value;
	char score;
	char material;
	char amount;
	char enchant;
	char weight;
};
unsigned char EquipmentSlots[12];
void EquipItem(unsigned char InventoryLocation);
void UnequipItem(unsigned char WeaponTypeToUnequip);

// ----- MAP 

int townamount = 0; // this changes depending on the amount of towns in the map file
char *towndata;
int mapsquareroot;
void chunkcheck(int chunksize);
int chuncksize = 100;
void theboundingbox(int bounding);
void moveboxstuff();
long XYcord(long XXYcord, long YXYcord);
void chunkrender(); // this is for rendering the chunks
int renderdistance = 5; // 5 by default, can be changed higher
void chunkanswer(unsigned char chunkanswerchar, int archingCX, int archingCZ, long seedthing);
void chunkanswerbillboard(unsigned char chunkanswerchar, int archingCX, int archingCZ, long seedthing);
void DrawTown(char TownType, int ACX, int ACZ, int PLX, int PLZ);
void DrawSign(char SignType,Vector3 LocationOfSign,Vector3 RotationOfSign,char ExtraData);
void renderUI3d();
void DrawOre(Vector3 OreLoc,char type);
unsigned char *TownInventory;
void GenTownInventory(unsigned int genseed); 
void PutInTownInventory(unsigned int TownInLoc,char B0,char B1,char B2,char B3,char B4,char B5,char B6,char B7,char B8,char B9);
void DungeonMode();
void HallMode();
void DungeonCameraReset();
unsigned char ArchingGameMode = 0x0;
void DungeonChunk(Vector3 VPlacement, unsigned int DungeonNum);
Ray ShootRayFromVector(Vector3 VectorCentre, unsigned char VectorDirection);
unsigned char Pothit = 0;
void floodrandomarray(unsigned char LOWER, unsigned char HIGHER);

typedef struct{
	unsigned char Max;
	Vector3 Placement[10];
} ItemPlacement;

ItemPlacement *DDP;

// ----- MODEL STUFF
void DebugMode();

Shader discardShader;
//Model BlankBoard;
Model meowscarada;
Texture2D Grass;
Texture2D Floor;

Model Ground;
Model Building;
Model SignModel;
Model DoorModel;


void CallModel(Model modelitself, char type, Vector3 Loc, float scaley, char ModelData); // places a simple model
BoundingBox TranslateBoundingBox(BoundingBox box, Vector3 translation);
Color Lcolor(Color Wanted); // changes colour depending on time of day to simulate lighting
// ---- NPC
void NPCcalc();

// ----- TIME
long DayTime = 0;
unsigned long DayNum = 0;
void ProcessTime();
unsigned char DayLighting = 0;

// Day = 20000,

// ----- TEXTURE STUFF

int floortextureSQ = 100;
int floortextureMAX;
Texture2D floortexture;

Camera3D camera = { 0 };

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
	
	malloccalls();
	
	InitWindow(windowX, windowY, "FPS test");
	postGLcalls();
	//Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 2.0f, 2.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type
    DisableCursor();                    // Limit cursor to relative movement inside the window
    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
	
	meowscarada = LoadModel("Models/meowscarada.glb");
	
	
	Px = &camera.position.x;
	Py = &camera.position.y;
	Pz = &camera.position.z;
	
	
	
	Ax = &camera.target.x;
	Ay = &camera.target.y;
	Az = &camera.target.z;
	newCamPos = &camera.position;
	newLookPos = &camera.target;
	
	CharacterRay.position = (Vector3){*Px,*Py,*Pz};
	CharacterRay.direction = (Vector3){*Ax,*Ay,*Az};
	CharacterRay.direction = calculate_vector(camera.position,camera.target);
	
	pointcam = &camera;
	pointcam3D = &camera;
	
	while (!WindowShouldClose()){
		// RUNS EVERY FRAME OF GAME NO MATTER WHAT
		NewArea = 0;
		if (GameStateDelta != GameState) {CurrentMenuOption = 0; CurrentMenuOptionSide = 0;
			if (GameState == 0x0 && GameStateDelta >= 0x20){camera.position = ReturnMove; camera.target = ReturnLook; ArchingGameMode = 0x0;} // ENTERING MAIN WORLD - Restores Location
			if (GameState >= 0x20 && GameStateDelta <= 0x1F){ReturnMove = camera.position; ReturnLook = camera.target; NewArea = 1;} // LEAVING MAIN WORLD - Saves Location i feel like this is bugged
			if (GameState == 0x20 && GameStateDelta == 0x0) {DungeonCameraReset();
				camera.position = (Vector3){ 0.0f, 2.0f, 2.0f }; camera.target = (Vector3){ 0.0f, 2.0f, 0.0f }; camera.up = (Vector3){ 0.0f, 1.0f, 0.0f }; 
				ArchingGameMode = 0x20;
				floodrandomarray(0x50,0xFF);
			}
		}
		GameStateDelta = GameState;
		
		switch (GameState){
		// LOGIC BELOW
		
		case 0x0:
		ProcessTime();
		Constant3DInit(1);
		
		// END OF LOGIC
		BeginDrawing();  // BEGIN 2D
		ClearBackground(Lcolor((Color){ 150, 150, 235, 255 }));
		
		BeginMode3D(camera); // BEGIN 3D
			#ifdef DEBUG
			theboundingbox(chuncksize);
			DrawRay(CharacterRay,PURPLE);
			#endif
			chunkrender();
		EndMode3D(); // END 3D
		renderUI3d();
		#ifdef DEBUG
		DrawRectangle(5, 5, 420, 260, Fade(SKYBLUE, 0.5f));
		DrawText("DEBUG:", 15, 20, 20, BLACK);
		DrawText(TextFormat("Position: (%06.3f, %06.3f, %06.3f)", camera.position.x, camera.position.y, camera.position.z), 15, 40, 20, BLACK);
		DrawFPS(15,60);
		DrawText(TextFormat("Arching: %d,%d",archingX,archingZ),15,80,20,BLACK);
		DrawText(TextFormat("Angle: (%06.3f, %06.3f, %06.3f)", camera.target.x, camera.target.y, camera.target.z), 15, 100, 20, BLACK);
		DrawText(TextFormat("Mapdata: %d XYcord: %d RD: %d",mapdata[XYcord(archingX,archingZ)],XYcord(archingX,archingZ),renderdistance),15,120,20,BLACK);
		DrawText(TextFormat("MAPSIZE: %d",mapfilesize),15,140,20,BLACK);
		SetRandomSeed(XYcord(archingX,archingZ));
		DrawText(TextFormat("AREA VALUE:%d",GetRandomValue(0,100)),15,160,20,BLACK);
		DrawText(TextFormat("FRAME TIME:%06.9f",GetFrameTime()),15,180,20,BLACK);
		DrawText(TextFormat("MOUSE POS:%d:%d",GetMousePosition().x,GetMousePosition().y),15,200,20,BLACK); //DayTime
		DrawText(TextFormat("TIME:%d DAY:%d LIGHTING:%d",DayTime,DayNum,DayLighting),15,220,20,BLACK);
		if (IsSprint==1){DrawText("SPRINTING",120,60,20,BLACK);}
		if (mapdata[XYcord(archingX,archingZ)] == 0xA0) {
			DrawRectangle(1200, 5,350, 400, Fade(RED, 0.5f));
			DrawText("TOWN DEBUG:", 1205, 20, 20, BLACK);
			DrawText(TextFormat("Town Number: %d Town Value: %d",
			mapdata2[XYcord(archingX,archingZ)],towndata[mapdata2[XYcord(archingX,archingZ)]*210+1]), 1205, 40, 20, BLACK);
		}
		#endif
		EndDrawing(); // END 2D
		
		break;
		case 0x1:
		BeginDrawing();  // BEGIN 2D
			probecontrolsUI();
			ClearBackground(BLACK);
			renderUI();
		EndDrawing();
			#ifdef DEBUG
			if (IsKeyPressed(KEY_KP_1)) {GameState = 0x6;} // ENTER DEBUG MODE: PRESS NUM 1 IN INVENTORY
			#endif		
		break;
		case 0x2: // BUILDING UI
		BeginDrawing();
			probecontrolsUI();
			ClearBackground(BLACK);
			renderShopUI();
		EndDrawing();
		break;
		case 0x3: // MINING UI
		BeginDrawing();
			probecontrolsUI();
			ClearBackground(BLACK);
			//renderShopUI();
			miningUI();
		EndDrawing();
		break;
		case 0x4: // MAP
		BeginDrawing();
			probecontrolsUI();
			ClearBackground(BLACK);
			mapUI();
		EndDrawing();
		break;
		case 0x20: //DUNDGON
		DungeonMode();
		break;
		case 0x6: // DEBUG
		DebugMode();
		break;
		case 0x21: // Inside Fight Hall
		ArchingGameMode = 0x21;
		HallMode();
		break;
		
		
		}
	}
	
	CloseWindow();  
	UnloadModel(meowscarada); 
	UnloadTexture(Grass);
	UnloadShader(discardShader);
	freemalloc();
	UnloadTexture(floortexture); 
	UnloadModel(Ground);
	UnloadModel(Building);
	UnloadModel(SignModel);
	UnloadModel(DoorModel);
	
	return 0;
}

void probecontrols(){
	// IDK DECLARE STUFF HERE MAYBE, USED TO CC65
	#ifdef CUSTOMMOVEMENT
	Vector2 mouseDelta = GetMouseDelta();
	#endif
	
	#ifdef DEBUG
	// CHUNK SKIP
	if (IsKeyPressed(KEY_KP_8)) {archingZ++;}
	if (IsKeyPressed(KEY_KP_2)) {archingZ--;}
	if (IsKeyPressed(KEY_KP_6)) {archingX++;}
	if (IsKeyPressed(KEY_KP_4)) {archingX--;}
	
	// MESS WITH RENDER DISTANCE
	if (IsKeyPressed(KEY_KP_7)) {renderdistance--;}
	if (IsKeyPressed(KEY_KP_9)) {renderdistance++;}
	// ---
	#endif
	
	#ifdef CUSTOMMOVEMENT
	// MOVEMENT
	if (IsSprint==0){
	if (IsKeyDown(KEY_W)) MoveDelta.x += 0.1f;  // FORWARD
	if (IsKeyDown(KEY_S)) MoveDelta.x -= 0.1f;  // BACKSTEP
	if (IsKeyDown(KEY_D)) MoveDelta.y += 0.1f;  // RIGHT
	if (IsKeyDown(KEY_A)) MoveDelta.y -= 0.1f;  // LEFT
	} else {
	if (IsKeyDown(KEY_W)) MoveDelta.x += 0.2f;  // FORWARD
	if (IsKeyDown(KEY_S)) MoveDelta.x -= 0.2f;  // BACKSTEP
	if (IsKeyDown(KEY_D)) MoveDelta.y += 0.2f;  // RIGHT
	if (IsKeyDown(KEY_A)) MoveDelta.y -= 0.2f;  // LEFT
	}
	// MOUSE LOOK
	LookDelta.x = mouseDelta.x * 0.05f;
	LookDelta.y = mouseDelta.y * 0.05f;
	#endif
	
	if (IsKeyPressed(KEY_TAB)) {GameState = 0x1;} // OPEN INVENTORY
	if (IsKeyPressed(KEY_M)) {GameState = 0x4;} // OPEN MAP
	if (IsSprint<2){
	if (IsKeyDown(KEY_LEFT_SHIFT)) {IsSprint = 1;} else {IsSprint = 0;}
	}

}

void probecontrolsUI(){
	if (IsKeyPressed(KEY_TAB)) {GameState = ArchingGameMode; RefreshInventory();}
	if (IsKeyPressed(KEY_D)) {UIState++; if (UIState > 3) {UIState = 0;} InventorySelection=0;}
	if (IsKeyPressed(KEY_A)) {UIState--; if (UIState < 0) {UIState = 3;} InventorySelection=0;}
}

void theboundingbox(int bounding) {
	int H = 1;
	int lineamount = 20;
	float wide = 0.2f;
	
	DrawSphere((Vector3){-bounding,H,-bounding},wide,PURPLE);
	DrawSphere((Vector3){bounding,H,bounding},wide,PURPLE);
	DrawSphere((Vector3){-bounding,H,bounding},wide,PURPLE);
	DrawSphere((Vector3){bounding,H,-bounding},wide,PURPLE);
	
	DrawCylinderWiresEx((Vector3){-bounding,H,-bounding}, (Vector3){-bounding,H,bounding}, wide, wide, lineamount, RED);
	DrawCylinderWiresEx((Vector3){bounding,H,bounding}, (Vector3){bounding,H,-bounding}, wide, wide, lineamount, RED);

	DrawCylinderWiresEx((Vector3){bounding,H,bounding}, (Vector3){-bounding,H,bounding}, wide, wide, lineamount, BLUE);
	DrawCylinderWiresEx((Vector3){bounding,H,-bounding}, (Vector3){-bounding,H,-bounding}, wide, wide, lineamount, BLUE);
	
	
}

void chunkcheck(int chunksize){
	if (*Px > chunksize){*Px = *Px-chunksize*2; archingX--; *Ax = *Ax-chunksize*2;}
	if (*Px < -chunksize){*Px = *Px+chunksize*2; archingX++; *Ax = *Ax+chunksize*2;}
	if (*Pz > chunksize){*Pz = *Pz-chunksize*2; archingZ--; *Az = *Az-chunksize*2;}
	if (*Pz < -chunksize){*Pz = *Pz+chunksize*2; archingZ++; *Az = *Az+chunksize*2;}
	
}


void moveboxstuff(){
	DrawModel(meowscarada, (Vector3){archingX*chuncksize*2,0,archingZ*chuncksize*2}, 1.0f, Lcolor(WHITE));
}



void malloccalls(){
	int filesize;
	FILE *file;
	char *dungeonbuffer;
	unsigned char amountofhousesintown;
	unsigned char TotalHz;
	unsigned char TotalHx;
	unsigned char BuildingType = 0x01;
	unsigned char BuildingRan;
	unsigned char DBFC = 0;
	char loadingbuffer[50];
	
	// -- OPEN MAP
	file = fopen("Map/Map3.bin","rb"); // map 3
	if (file == NULL) {printf("Sorry Couldn't open Map1.bin\n"); return;};
	fseek(file, 0, SEEK_END);
	filesize = ftell(file);
	fseek(file, 0, SEEK_SET);
	printf("Map1.bin:%d bytes\n", filesize);
	mapdata = calloc(filesize+1,sizeof(unsigned char));
	mapdata2 = calloc(filesize+1,sizeof(unsigned char));
	fread(mapdata,filesize,1,file);
	fclose(file);
	mapfilesize = filesize;
	// -- DONE OPENING MAP
	// -- CALCUATING SQAURE ROOT
	mapsquareroot = sqrt(filesize);
	if (mapsquareroot*mapsquareroot==filesize){printf("map size will be %d x %d\n",mapsquareroot, mapsquareroot);}
	else {printf("map size will be, strange, is %d a perfect square?\n",filesize);}
	// -- FINISHED CALCUATING SQAURE ROOT
	// -- GENORATE TOWNS
	SetRandomSeed(31378); // SET SEED FOR TOWN CREATOR
	for (int mappos = 0; mappos < mapfilesize+1; mappos++){
		if (mapdata[mappos] == 0xA0){
			townamount++;
			mapdata2[mappos] = townamount-1;
		}
	}
	
	#define TDS 210 // TownDataSize, 90 IS MAX SIZE
	towndata = calloc(TDS*townamount,sizeof(unsigned char)); 
	
	for (int mappos = 1; mappos < townamount*TDS+1; mappos = mappos+TDS){
		towndata[mappos]=GetRandomValue(1,100); // SET VALUE OF TOWN;
		towndata[mappos+1] = 0x01; // REGION CURRENT UNUSED
		towndata[mappos+2] = GetRandomValue(1,4); // HOUSE AMOUNT X
		towndata[mappos+3] = GetRandomValue(1,4); // HOUSE AMOUNT Z
		TotalHx = towndata[mappos+2]*2+1;
		TotalHz = towndata[mappos+3]*2+1;
		amountofhousesintown = TotalHx*TotalHz;
		towndata[mappos+4] = amountofhousesintown;
		for (int housepos = 0; housepos < amountofhousesintown+1; housepos++){
			BuildingRan = GetRandomValue(0,towndata[mappos]);
			BuildingType = 0x01; // HOUSE
			
			if (BuildingRan > 12){BuildingType = 0x02;} //General Store
			if (BuildingRan > 20){BuildingType = 0x03;} //Inn
			if (BuildingRan > 30){BuildingType = 0x04;} //temple
			if (BuildingRan > 35){BuildingType = 0x05;} //weaponsmith
			if (BuildingRan > 40){BuildingType = 0x06;} //armoury
			if (BuildingRan > 60){BuildingType = 0x07;} //jeweler
			if (BuildingRan > 63){BuildingType = 0x08;} //alchemist
			if (BuildingRan > 75){BuildingType = 0x09;} //clothes
			if (BuildingRan > 80){BuildingType = 0x0A;} //guild
			if (BuildingRan > 95){BuildingType = 0x0B;} //bank
			
			
			
			towndata[10+mappos+housepos] = BuildingType; // BUILDING TYPE
			towndata[10+mappos+housepos+100] = GetRandomValue(0,Hbuildings-1); // PHYISCIAL BUILDING TYPE
		}
		
	}
	// -- FINISH MAKING TOWNS
	// -- SETUP TOWN INVENTORY SYSTE
	long MaxHouseAmount = 81*10; // MAX HOUSES = 81, each house gets 10 inventory slots
	long MaxTownInventory = 81*10;
	TownInventory = calloc(townamount*MaxTownInventory,sizeof(unsigned char));
	
	#ifdef DEBUG
	printf("size of TownInventory:%d bytes\n",townamount*MaxTownInventory);
	
	#endif
	
	// -- ZERO OUT CHARACTER STATS 
	for (char zeroing=0; zeroing<amountofdamageablelimbs; zeroing++){
		LimbDamage[zeroing] = 0;
	}
	LimbDamage[0x10] = 25;
	
	// -- FINISHED ZEROING OUT CHARACTER STATS
	// -- ALLOCATE INVINTORY
	#define INVENTORYSIZE 250
	Inventory = calloc(10*INVENTORYSIZE,sizeof(unsigned char)); //EACH ITEM GETS 10 BYTES
	InventoryLookup = calloc(INVENTORYSIZE,sizeof(unsigned char)); //EACH ITEM GETS 10 BYTES
	// -- FINISH INVENTORY
	
	#ifdef DEBUGCHARACTER


	PlayerStats.LEVEL = 5;
	PlayerStats.STR= 5;
	PlayerStats.INT= 5;
	PlayerStats.WIL= 5;
	PlayerStats.AGI= 5;
	PlayerStats.END= 5;
	PlayerStats.PER= 5;

	PlayerSkills.onehand= 5;
	PlayerSkills.twohand= 5;
	PlayerSkills.blocking= 5;
	PlayerSkills.hand2hand= 5;
	PlayerSkills.speech= 5;
	PlayerSkills.lockpicking= 5;
	PlayerSkills.medical= 5;
	PlayerSkills.stealth= 5;
	PlayerSkills.ranged= 5;
	

	PutInInventory(0x2,0x1,0x0,0x0,0xA,0x2,0x0,0x0,0x0,0xA);
	PutInInventory(0x2,0x2,0x0,0x0,0xA,0x2,0xF,0x0,0x0,0xA);
	PutInInventory(0x2,0x4,0x3,0x0,0xA,0x2,0x0,0x0,0x0,0xA);
	PutInInventory(0x3,0x5,0x3,0x0,0xA,0x2,0x0,0x0,0x0,0x1);
	PutInInventory(0x4,0x6,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1);
	PutInInventory(0x1,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x2);
	PutInInventory(0x2,0xA,0x0,0x0,0xA,0x5,0x0,0x0,0x0,0xA);
	
	#endif
	
	// ---- UI STUFF
	TextNamesRun();
	visableitems = windowY - windowY/4;
	visableitems = visableitems / 50;

	
	#ifdef DEBUG
	for (int greg = 0; greg <mapfilesize+1; greg++){printf("%d : %d : 0x%x : %d \n",greg,mapdata[greg],mapdata[greg],mapdata2[greg]);}
	printf("there are %d town(s) on this map\n",townamount);
	printf("towndata is %d bytes long\n",TDS*townamount);
	
	file = fopen("TownDebug.bin","wb");
	//fwrite(towndata,sizeof(towndata),1,file);
	for (int greg = 0; greg<TDS*townamount; greg++){
		fwrite(&towndata[greg],sizeof(unsigned char),1,file);
	}
	fclose(file);
	#endif
	
	
	// MAP GEN
	unsigned char *MapColourData;
	unsigned int MapColourDataSize = 0;
	file = fopen("Data/D0.bin","rb");
	if (file == NULL) {printf("Sorry Couldn't open D0.bin\n"); return;};
	fseek(file, 0, SEEK_END);
	MapColourDataSize = ftell(file);
	fseek(file, 0, SEEK_SET);
	printf("D0.bin:%d bytes\n", MapColourDataSize);
	MapColourData = calloc(MapColourDataSize+1,sizeof(unsigned char));
	fread(MapColourData,MapColourDataSize,1,file);
	fclose(file);
	
	BMPWriter(mapdata,MapColourData,mapfilesize,0);  // BMPFile
	free(MapColourData);
	MapColourData = NULL;
	MapUIImage = LoadImageFromMemory(".bmp",BMPFile,SizeOfBmp);
	
	#ifdef DEBUG
	file = fopen("MAPVIEW.bmp","wb");
	for (int greg = 0; greg<SizeOfBmp; greg++){
		fwrite(&BMPFile[greg],sizeof(unsigned char),1,file);
	}
	fclose(file);
	#endif
	
	DDP = NULL;
	DDP = calloc(Hdungeon4,sizeof(ItemPlacement));
	dungeonbuffer = NULL;
	dungeonbuffer = calloc(30,sizeof(char));
	if (DDP != NULL){printf("DDP: %d\n",Hdungeon4);} else {printf("DDP: FAILED\n");}
	
	for (int loady=0; loady<Hdungeon4; loady++){
		for (int Resetbuffer=0; Resetbuffer<30; Resetbuffer++){dungeonbuffer[Resetbuffer]=0;} // RESET BUFFER
		
		sprintf(loadingbuffer, "Models/Dungeon4/D%d.bin", loady);
		//Buildings[loady] = LoadModel(loadingbuffer);
		#ifdef DEBUG
		printf("im loading binary data: %s at %d\n",loadingbuffer,loady);
		#endif
		
		file = fopen(loadingbuffer,"rb");
		if (file == NULL) {printf("Sorry Couldn't open D%d.bin\n",loady); return;};
		fseek(file, 0, SEEK_END);
		DDP[loady].Max = ftell(file)/2;
		fseek(file, 0, SEEK_SET);
		
		DBFC = 0;
		fread(dungeonbuffer,DDP[loady].Max*2,1,file);
		#ifdef DEBUG
		printf("CURRENT MAX:%d\n",DDP[loady].Max);
		#endif
		for (int CBF=0; CBF<DDP[loady].Max*2; CBF=CBF+2) { // WRITE CHAR TO STRUCT VECTORS
			DDP[loady].Placement[DBFC].x = dungeonbuffer[CBF];
			DDP[loady].Placement[DBFC].y = 0; // SHOULD ALWAYS BE ZERO
			DDP[loady].Placement[DBFC].z = 0-dungeonbuffer[CBF+1];
			#ifdef DEBUG
			printf("DDP[%d] X:%.2f Z:%.2f\n",DBFC,DDP[loady].Placement[DBFC].x,DDP[loady].Placement[DBFC].z);
			#endif
			DBFC++;
		}
		fclose(file);
	}
	free(dungeonbuffer);
	dungeonbuffer = NULL;
	//RandomArrayMax
	RandomArray = NULL;
	RandomArray = malloc(sizeof(unsigned char)*RandomArrayMax);
	#ifdef DEBUG
	if (RandomArray != NULL) {printf("Random array sucess at: 0x%p\n",RandomArray);}
	else {printf("!!! RANDOM ARRAY FAILED !!!\n");}
	#endif
	RefreshInventory();
	GenTownInventory(1);
	
	
	
	#ifdef DEBUG
	floodrandomarray(0x50,0xFF);
	file = fopen("RandomArray.bin","wb");
	//fwrite(towndata,sizeof(towndata),1,file);
	for (int greg = 0; greg<RandomArrayMax; greg++){
		fwrite(&RandomArray[greg],sizeof(unsigned char),1,file);
	}
	fclose(file);
	#endif
	//MaxFramesData - Loading in D1.bin to figure out how many frames are in each sprite sheet
	MapColourDataSize = 0; // Reusing this veriable since why not.
	file = fopen("Data/D1.bin","rb");
	if (file == NULL) {printf("Sorry Couldn't open D1.bin\n"); return;};
	fseek(file, 0, SEEK_END);
	MapColourDataSize = ftell(file);
	fseek(file, 0, SEEK_SET);
	printf("D1.bin:%d bytes\n", MapColourDataSize);
	MaxFramesData = calloc(MapColourDataSize+1,sizeof(unsigned char));
	fread(MaxFramesData,MapColourDataSize,1,file);
	fclose(file);
	// Load weapons data from WeaponData.h
	LoadWeaponValues();
}

void freemalloc(){
	free(mapdata);
	free(towndata);
	free(mapdata2);
	free(Inventory);
	free(InventoryLookup);
	free(BMPFile);
	free(TownInventory);
	free(DDP);
	free(RandomArray);
	free(MaxFramesData);
	//free();
	
	UnloadTexture(MapUITexture);
	UnloadImage(MapUIImage);
	for (int unloady=0; unloady<Hbuildings; unloady++){
	UnloadModel(Buildings[unloady]);}
	for (int unloady=0; unloady<Hmodels; unloady++){
	UnloadModel(MiscModels[unloady]);}
	for (int unloady=0; unloady<Htextures; unloady++){
	UnloadTexture(billboardTs[unloady]);}
	for (int unloady=0; unloady<Hsigns; unloady++){
	UnloadTexture(signs[unloady]);}
	for (int unloady=0; unloady<Hui; unloady++){
	UnloadTexture(UIimages[unloady]);}
	for (int unloady=0; unloady<Hfonts; unloady++){
	UnloadFont(fonts[unloady]);}
	for (int unloady=0; unloady<Hbillboards; unloady++){
	UnloadTexture(billboards[unloady]);}
	for (int unloady=0; unloady<Henviroments; unloady++){
	UnloadModel(EnviromentModel[unloady]);}
	for (int unloady=0; unloady<Hdungeon1; unloady++){
	UnloadModel(DungeonBottom[unloady]);}
	for (int unloady=0; unloady<Hdungeon2; unloady++){
	UnloadModel(DungeonTop[unloady]);}
	for (int unloady=0; unloady<Hdungeon3; unloady++){
	UnloadModel(DungeonFloor[unloady]);}
	for (int unloady=0; unloady<Hdetail; unloady++){
	UnloadModel(DetailObject[unloady]);}
	for (int unloady=0; unloady<Hinside; unloady++){
	UnloadModel(InsideBuilding[unloady]);}
	for (int unloady=0; unloady<Hanimated; unloady++){
	UnloadTexture(AnimatedGraphic[unloady]);}
	for (int unloady=0; unloady<Hfullscreen; unloady++){
	UnloadTexture(FullscreenGraphic[unloady]);}

	//FullscreenGraphic
}

void postGLcalls(){
	char loadingbuffer[50];
	Grass = LoadTexture("Graphics/grasstest.png");
	discardShader = LoadShader(0, "Graphics/billboard.fs"); // Billboard shader	
	floortexture = LoadTexture ("Graphics/Floor/Main.png"); // Floor texture
	floortextureMAX = floortexture.width/floortextureSQ;
	//Ground = GenMeshCustom();
	Floor =  LoadTexture ("Graphics/Floor/1.png"); // Floor texture
	SignModel = LoadModel("Models/sign.glb");
	DoorModel = LoadModel("Models/door.glb");
	Mesh DummyMesh = GenMeshPlane(chuncksize*2,chuncksize*2,1,1);
	SetTextureFilter(Floor, TEXTURE_FILTER_POINT);
	
	//UploadMesh(&DummyMesh, false);
	Ground = LoadModelFromMesh(DummyMesh);
	UploadMesh(&DummyMesh, false);
	//Ground = LoadModelFromMesh(GenMeshPlane(chuncksize*2,chuncksize*2,5,5));
	Ground.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = Floor;
	
	Building = LoadModel("Models/Buildings/B1.glb");
	
	
	MapUITexture = LoadTextureFromImage(MapUIImage);
	
	for (int loady=0; loady<Hbuildings; loady++){
		//sprintf(loadingbuffer, "Models/Buildings/B%d.glb", loady);
		sprintf(loadingbuffer, "Models/Buildings/B%d.glb", loady);
		Buildings[loady] = LoadModel(loadingbuffer);
		
		#ifdef DEBUG
		printf("im loading model: %s at %d\n",loadingbuffer,loady);
		#endif
	}
	for (int loady=0; loady<Hsigns; loady++){
		//sprintf(loadingbuffer, "Models/Buildings/B%d.glb", loady);
		sprintf(loadingbuffer, "Graphics/Sign/%d.png", loady);
		signs[loady] = LoadTexture(loadingbuffer);
		
		#ifdef DEBUG
		printf("im loading sign texture: %s at %d\n",loadingbuffer,loady);
		#endif
	}
	for (int loady=0; loady<Hui; loady++){
		//sprintf(loadingbuffer, "Models/Buildings/B%d.glb", loady);
		sprintf(loadingbuffer, "Graphics/UI/UI%d.png", loady);
		UIimages[loady] = LoadTexture(loadingbuffer);
		
		#ifdef DEBUG
		printf("im loading UIimages texture: %s at %d\n",loadingbuffer,loady);
		#endif
	}
	for (int loady=0; loady<Hfonts; loady++){
		//sprintf(loadingbuffer, "Models/Buildings/B%d.glb", loady);
		sprintf(loadingbuffer, "Graphics/Font/F%d.ttf", loady);
		//fonts[loady] = LoadFont(loadingbuffer);
		fonts[loady] = LoadFontEx(loadingbuffer, 100, NULL, 0);
		#ifdef DEBUG
		printf("im loading UIimages texture: %s at %d\n",loadingbuffer,loady);
		#endif
	}
	//Hbillboards
	for (int loady=0; loady<Hbillboards; loady++){
		sprintf(loadingbuffer, "Graphics/Billboard/BILL%d.png", loady);
		billboards[loady] = LoadTexture(loadingbuffer);
		
		#ifdef DEBUG
		printf("im loading Billboard texture: %s at %d\n",loadingbuffer,loady);
		#endif
	}
	//EnviromentModel[Henviroments]
	for (int loady=0; loady<Henviroments; loady++){
		sprintf(loadingbuffer, "Models/Enviroment/EV%d.glb", loady);
		EnviromentModel[loady] = LoadModel(loadingbuffer);
		
		#ifdef DEBUG
		printf("im loading Enviroment Models: %s at %d\n",loadingbuffer,loady);
		#endif
	}
	//DungeonBottom[Hdungeon1]
	for (int loady=0; loady<Hdungeon1; loady++){
		sprintf(loadingbuffer, "Models/Dungeon1/DB%d.glb", loady);
		DungeonBottom[loady] = LoadModel(loadingbuffer);
		
		#ifdef DEBUG
		printf("im loading Dungeon Bottom Models: %s at %d\n",loadingbuffer,loady);
		#endif
	}
	//DungeonTop[Hdungeon2]
	for (int loady=0; loady<Hdungeon2; loady++){
		sprintf(loadingbuffer, "Models/Dungeon2/DR%d.glb", loady);
		DungeonTop[loady] = LoadModel(loadingbuffer);
		
		#ifdef DEBUG
		printf("im loading Dungeon Roof Models: %s at %d\n",loadingbuffer,loady);
		#endif
	}
	//DungeonFloor[Hdungeon3]
	for (int loady=0; loady<Hdungeon3; loady++){
		sprintf(loadingbuffer, "Models/Dungeon3/DF%d.glb", loady);
		DungeonFloor[loady] = LoadModel(loadingbuffer);
		
		#ifdef DEBUG
		printf("im loading Dungeon Floor Models: %s at %d\n",loadingbuffer,loady);
		#endif
	}
	for (int loady=0; loady<Hdetail; loady++){
		sprintf(loadingbuffer, "Models/Detail/O%d.glb", loady);
		DetailObject[loady] = LoadModel(loadingbuffer);
		
		#ifdef DEBUG
		printf("im loading Dungeon Detail Models: %s at %d\n",loadingbuffer,loady);
		#endif
	}
	for (int loady=0; loady<Hinside; loady++){
		sprintf(loadingbuffer, "Models/Inside/IN%d.glb", loady);
		InsideBuilding[loady] = LoadModel(loadingbuffer);
		
		#ifdef DEBUG
		printf("im loading Inside Building Models: %s at %d\n",loadingbuffer,loady);
		#endif
	}
	for (int loady=0; loady<Hanimated; loady++){
		sprintf(loadingbuffer, "Graphics/Animated/A%d.png", loady);
		AnimatedGraphic[loady] = LoadTexture(loadingbuffer);
		
		#ifdef DEBUG
		printf("im loading Animated GIF texture: %s at %d\n",loadingbuffer,loady);
		#endif
	}
	for (int loady=0; loady<Hfullscreen; loady++){
		sprintf(loadingbuffer, "Graphics/FullScreen/FS%d.png", loady);
		FullscreenGraphic[loady] = LoadTexture(loadingbuffer);
		
		#ifdef DEBUG
		printf("im loading Animated Fullscreen texture: %s at %d\n",loadingbuffer,loady);
		#endif
	}
	

/*
Model InsideBuilding[Hinside];
Texture2D AnimatedGraphic[Hanimated];
*/

}



long XYcord(long XXYcord, long YXYcord){
	long AN;
	if (XXYcord < 0){return mapfilesize;}
	if (YXYcord < 0){return mapfilesize;}
	if (XXYcord > mapsquareroot){return mapfilesize;}
	if (YXYcord > mapsquareroot){return mapfilesize;}
	AN = YXYcord * mapsquareroot;
	AN = AN + XXYcord;
	if (AN <= mapsquareroot*mapsquareroot){return AN;}else{return mapfilesize;}
}

void chunkrender(){

	long locationonmap = 0;
	int Xchunk =-renderdistance;
	int Zchunk =-renderdistance;
	while (Xchunk <= renderdistance){
		Zchunk =-renderdistance;
		while(Zchunk <= renderdistance){
			//locationonmap = XYcord(Xchunk-archingX,Zchunk-archingZ);
			locationonmap = XYcord(archingX-Xchunk,archingZ-Zchunk);
			chunkanswer(mapdata[locationonmap],Xchunk,Zchunk, locationonmap);
			Zchunk++;
		}
		Xchunk++;
	}
	BeginShaderMode(discardShader);
	Xchunk =-renderdistance; Zchunk =-renderdistance;
	while (Xchunk <= renderdistance){
		Zchunk =-renderdistance;
		while(Zchunk <= renderdistance){
			//locationonmap = XYcord(Xchunk-archingX,Zchunk-archingZ);
			locationonmap = XYcord(archingX-Xchunk,archingZ-Zchunk);
			chunkanswerbillboard(mapdata[locationonmap],Xchunk,Zchunk, locationonmap);
			Zchunk++;
		}
		Xchunk++;
	}
	
	EndShaderMode();
	
	
}

void chunkanswer(unsigned char chunkanswerchar, int archingCX, int archingCZ, long seedthing){
	int placeX =archingCX*chuncksize*2;
	int placeZ =archingCZ*chuncksize*2;
	//if (chunkanswerchar == 999){return;}
	if (chunkanswerchar == 0){return;}
	if (chunkanswerchar == 1){DrawModel(meowscarada, (Vector3){placeX,0,placeZ}, 1.0f, Lcolor(WHITE));return;}
	if (chunkanswerchar == 2){DrawModel(meowscarada, (Vector3){placeX,1,placeZ}, 1.0f, Lcolor(BLUE));return;}
	if (chunkanswerchar == 3){DrawModel(meowscarada, (Vector3){placeX,1,placeZ}, 1.0f, Lcolor(RED));return;}
	// GRASS FIELD
	if (chunkanswerchar == 0x10){
		DrawModel(Ground,(Vector3){placeX,0,placeZ},1,Lcolor(WHITE));
		//DrawModel(Buildings[1],(Vector3){placeX,0,placeZ},1,WHITE);
		
		return;
		}
	// GRASS FIELD WITH ORE
	if (chunkanswerchar >= 0x11 && chunkanswerchar <= 0x17){ //x >= min && x <= max   chunkanswerchar == 0x11
		DrawModel(Ground,(Vector3){placeX,0,placeZ},1,Lcolor(WHITE));
		DrawOre((Vector3){placeX,0,placeZ},chunkanswerchar-0x11); // LAST PART IS FOR TYPE OF ORE, 0-6		
		return;
		}	
	// VILLAGE
	if (chunkanswerchar == 0xA0){
		DrawModel(Ground,(Vector3){placeX,0,placeZ},1,Lcolor(WHITE)); // GROUND
		DrawTown(0xA0,archingCX,archingCZ,placeX,placeZ);	
		return;
	}
	
	if (chunkanswerchar == 0xA1){
		DrawModel(Ground,(Vector3){placeX,0,placeZ},1,Lcolor(WHITE)); // GROUND
		CallModel(EnviromentModel[1],0x0D,(Vector3){placeX,0,placeZ},1,0);
	
	}
	
	
	
	return;
}
void chunkanswerbillboard(unsigned char chunkanswerchar, int archingCX, int archingCZ, long seedthing){
	int placeX =archingCX*chuncksize*2;
	int placeZ =archingCZ*chuncksize*2;
	
	if (chunkanswerchar == 0x10){
		SetRandomSeed(seedthing);
		for (int gengrass = 0; gengrass < 80; gengrass++){
			DrawBillboard(*pointcam,billboards[0],(Vector3){placeX+GetRandomValue(-chuncksize,chuncksize),1,placeZ+GetRandomValue(-chuncksize,chuncksize)},2,Lcolor(WHITE));
			DrawBillboard(*pointcam,billboards[1],(Vector3){placeX+GetRandomValue(-chuncksize,chuncksize),3,placeZ+GetRandomValue(-chuncksize,chuncksize)},6,Lcolor(WHITE));
		}
		
	}
	
	
	return;
}

int FilesInDir(const char *path) {
    DIR *dir;
    struct dirent *entry;
    int file_count = 0;

    // Open the directory
    dir = opendir(path);
    if (dir == NULL) {
        perror("Error opening directory");
        return -1; // Indicate an error
    }

    // Read entries from the directory
    while ((entry = readdir(dir)) != NULL) {
        // Skip "." and ".." entries (current and parent directory)
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        file_count++;
        
    }

    // Close the directory
    closedir(dir);

    return file_count;
}


void CallModel(Model modelitself, char type, Vector3 Loc, float scaley, char ModelData){
	BoundingBox box; // = GetMeshBoundingBox(tetoModel.meshes[0]);
	// TYPES
	// 0x01 = BUILDING YOU CAN KINDA ENTER
	
	if (type < 0x0C){
		DrawModel(modelitself,Loc,scaley,Lcolor(WHITE));
		//box = GetMeshBoundingBox(modelitself.meshes[0]);
		box = GetModelBoundingBox(modelitself);
		//PX = box.max.x;
		//NX = box.min.x;
		//PZ = box.max.z;
		//NZ = box.min.z;
		//DrawSign(1,Loc+(Vector3){PX,2,0},(Vector3){0});
		//DrawSign(type,(Vector3){Loc.x+NX,5,Loc.z},(Vector3){0});
		DrawSign(type,(Vector3){Loc.x+box.min.x,5,Loc.z},(Vector3){0}, ModelData);
		if (CheckCollisionBoxSphere(box,(Vector3){*Px-Loc.x,*Py,*Pz-Loc.z},0.1f)){*newCamPos = oldCamPos; *newLookPos = oldLookPos;}	
		return;
	}
	// ORE
	
	if (type == 0x0C) {
		DrawModel(modelitself,Loc,scaley,Lcolor(WHITE));
		box = GetModelBoundingBox(modelitself);
		if (CheckCollisionBoxSphere(box,(Vector3){*Px-Loc.x,*Py,*Pz-Loc.z},0.1f)){*newCamPos = oldCamPos; *newLookPos = oldLookPos;}
		InteractableObject(modelitself,Loc,0x2, ModelData, 0x0);
	}
	
	if (type == 0x0D){ // DUNGEON ENTRENCE 
		DrawModel(modelitself,Loc,scaley,Lcolor(WHITE));
		box = GetModelBoundingBox(modelitself);
		DrawSign(12,(Vector3){Loc.x+box.min.x,5,Loc.z},(Vector3){0},0xFF);
		if (CheckCollisionBoxSphere(box,(Vector3){*Px-Loc.x,*Py,*Pz-Loc.z},0.1f)){*newCamPos = oldCamPos; *newLookPos = oldLookPos;}	
		return;
		
	}
	
	
}
void DrawTown(char TownType, int ACX, int ACZ, int PLX, int PLZ){
	static int BuildingDistance = 20;
	int HouseNumber = 0;
	int townnumberRAM = mapdata2[XYcord(PLX,PLZ)]*210;
	int MaxRowX = towndata[townnumberRAM+2];
	int MaxRowZ = towndata[townnumberRAM+3];
	
	for (int RowX = -MaxRowX; RowX<MaxRowX+1; RowX++){
	for (int RowZ = -MaxRowZ; RowZ<MaxRowZ+1; RowZ++){
		CallModel(
		Buildings[towndata[townnumberRAM+10+HouseNumber+100]], // BUILDING
		towndata[townnumberRAM+11+HouseNumber], // TYPE
		//BuildingTypeArray[HouseNumber],
		(Vector3){PLX+RowX*BuildingDistance,0,PLZ+RowZ*BuildingDistance}, // LOCATION
		1, //SCALE
		HouseNumber); //MODEL DATA AKA HOUSE NUMBER
		HouseNumber++;
			
			
			
	}
		
}
	
	

}

void DrawSign(char SignType,Vector3 LocationOfSign,Vector3 RotationOfSign, char ExtraData){ // 1=Building 2=Dungeon 
	/*
		if (SignType > Hsigns) {SignType = 1;}
		if (SignType == 0) {SignType = 1;}
		if (SignType < 0) {SignType = 1;}
		*/
		SignModel.materials[2].maps[MATERIAL_MAP_DIFFUSE].texture = signs[SignType];
		DrawModel(SignModel,LocationOfSign,1,Lcolor(WHITE));
		DoorModel.transform = MatrixRotateY(1.5708f);
		DrawModel(DoorModel,(Vector3){LocationOfSign.x,0,LocationOfSign.z},1,Lcolor(WHITE));
		
		
		if (SignType==12){
		InteractableObject(DoorModel,LocationOfSign,0x05,SignType, ExtraData);
		}else{
		InteractableObject(DoorModel,LocationOfSign,0x01,SignType, ExtraData);
		}
		
}

/*

BUILDING TYPES:
0x00 NON ENTERING TYPE
--100% CHANCE OF SPAWNING--
0x01 HOUSING 
0x02 GENERAL STORE
0x03 INN
--HIGH CHANCE-- 
0x04 TEMPLE
--LOW CHANCE--
0x05 WEAPON SMITH
0x06 ARMOURER
0x07 JEWELER
0x08 ALCHEMIST/CHEMIST
0x09 CLOTHES STORE
0x0A GUILD??
--ONLY HIGH VALUE-- >75
0x0B BANK
*/


void DrawOre(Vector3 OreLoc,char type){
	CallModel(EnviromentModel[0],0xC,OreLoc,1,type);
	//CallModel(Model modelitself, char type, Vector3 Loc, float scaley, char ModelData){
}

void showlimbdamage(){
	int LocOnScreen = 1100;
	LimbDamage[0] = 0;
	LimbDamage[1] = 50;
	LimbDamage[2] = 16;
	LimbDamage[3] = 100;
	LimbDamage[4] = 16;
	LimbDamage[5] = 16;
	/*
	DrawTexture(UIimages[1],LocOnScreen,1,WHITE); //BASE MODEL
	if (LimbDamage[0] >= 100) {DrawTexture(UIimages[2],LocOnScreen,1,WHITE);} //ARM RIGHT
	if (LimbDamage[1] >= 100) {DrawTexture(UIimages[3],LocOnScreen,1,WHITE);} //ARM LEFT
	if (LimbDamage[2] >= 100) {DrawTexture(UIimages[4],LocOnScreen,1,WHITE);} //LEG RIGHT
	if (LimbDamage[3] >= 100) {DrawTexture(UIimages[5],LocOnScreen,1,WHITE);} //LEG LEFT
	if (LimbDamage[4] >= 100) {DrawTexture(UIimages[6],LocOnScreen,1,WHITE);} //TORSO
	if (LimbDamage[5] >= 100) {DrawTexture(UIimages[7],LocOnScreen,1,WHITE);} //HEAD
	*/
	DrawTexture(UIimages[2],LocOnScreen,1,ColourDamage(LimbDamage[0]));
	DrawTexture(UIimages[3],LocOnScreen,1,ColourDamage(LimbDamage[1]));
	DrawTexture(UIimages[4],LocOnScreen,1,ColourDamage(LimbDamage[2]));
	DrawTexture(UIimages[5],LocOnScreen,1,ColourDamage(LimbDamage[3]));
	DrawTexture(UIimages[6],LocOnScreen,1,ColourDamage(LimbDamage[4]));
	DrawTexture(UIimages[7],LocOnScreen,1,ColourDamage(LimbDamage[5]));
	

}

void renderUI(){
	
	switch (UIState){
	case 0:
	DrawTextEx(fonts[0],"Health",(Vector2){5,5},100,1,WHITE);
	if (LimbDamage[0x10] >= 30){DrawTextEx(fonts[0],TextFormat("Body Temp:High"),(Vector2){5,60},100,1,WHITE);}
	else{
		if (LimbDamage[0x10] < 20) {DrawTextEx(fonts[0],TextFormat("Body Temp:Low"),(Vector2){5,60},100,1,WHITE);}
		else{DrawTextEx(fonts[0],TextFormat("Body Temp:Nominal"),(Vector2){5,60},100,1,WHITE);}
	}
	showlimbdamage();
	break;
	case 1:
	
	DrawTextEx(fonts[0],"Inventory",(Vector2){5,5},100,1,WHITE);
	DrawTextEx(fonts[0],TextFormat("Weight: %d/%d",CharacterWeight,CharacterWeightMax),(Vector2){windowX-600,5},100,1,WHITE);
	DrawTextEx(fonts[0],TextFormat("Money: %d",CharacterMoney),(Vector2){windowX-600,100},100,1,WHITE);
	DrawRectangle(0,windowY/4,windowX,1000,(Color){0x22,0x00,0x00,0xFF}); //(Color){0xFF,0xFF,0xFF,0xFF}
	
	DrawInventory(1);
	//DrawTextEx(fonts[0],TextFormat("Item: %s",Matstrings[5]),(Vector2){100,windowY/4},100,1,WHITE);
	
	break;
	case 2:
	DrawTextEx(fonts[0],"Quests",(Vector2){5,5},100,1,WHITE);
	break;
	case 3:
	DrawTextEx(fonts[0],"Stats",(Vector2){5,5},100,1,WHITE);
	DrawTextEx(fonts[0],TextFormat("LEVEL:%d",PlayerStats.LEVEL),(Vector2){400,5},100,1,WHITE);
	DrawTextEx(fonts[0],TextFormat("STR:%d",PlayerStats.STR),(Vector2){5,100},100,1,WHITE);
	DrawTextEx(fonts[0],TextFormat("INT:%d",PlayerStats.INT),(Vector2){5,150},100,1,WHITE);
	DrawTextEx(fonts[0],TextFormat("WIL:%d",PlayerStats.WIL),(Vector2){5,200},100,1,WHITE);
	DrawTextEx(fonts[0],TextFormat("AGI:%d",PlayerStats.AGI),(Vector2){5,250},100,1,WHITE);
	DrawTextEx(fonts[0],TextFormat("END:%d",PlayerStats.END),(Vector2){5,300},100,1,WHITE);
	DrawTextEx(fonts[0],TextFormat("PER:%d",PlayerStats.PER),(Vector2){5,350},100,1,WHITE);
	
	DrawTextEx(fonts[0],TextFormat("One Handed:%d",PlayerSkills.onehand),(Vector2){300,100},100,1,WHITE);
	DrawTextEx(fonts[0],TextFormat("Two Handed:%d",PlayerSkills.twohand),(Vector2){300,150},100,1,WHITE);
	DrawTextEx(fonts[0],TextFormat("Blocking:%d",PlayerSkills.blocking),(Vector2){300,200},100,1,WHITE);
	DrawTextEx(fonts[0],TextFormat("Hand to Hand:%d",PlayerSkills.hand2hand),(Vector2){300,250},100,1,WHITE);
	DrawTextEx(fonts[0],TextFormat("Speech:%d",PlayerSkills.speech),(Vector2){300,300},100,1,WHITE);
	DrawTextEx(fonts[0],TextFormat("Lockpicking:%d",PlayerSkills.lockpicking),(Vector2){300,350},100,1,WHITE);
	DrawTextEx(fonts[0],TextFormat("Medical:%d",PlayerSkills.medical),(Vector2){300,400},100,1,WHITE);
	DrawTextEx(fonts[0],TextFormat("Stealth:%d",PlayerSkills.stealth),(Vector2){300,450},100,1,WHITE);
	DrawTextEx(fonts[0],TextFormat("Ranged:%d",PlayerSkills.ranged),(Vector2){300,500},100,1,WHITE);
	/*
	PlayerStats.LEVEL
	PlayerStats.STR
	PlayerStats.INT
	PlayerStats.WIL
	PlayerStats.AGI
	PlayerStats.END
	PlayerStats.PER

	PlayerSkills.onehand
	PlayerSkills.twohand
	PlayerSkills.blocking
	PlayerSkills.hand2hand
	PlayerSkills.speech
	PlayerSkills.lockpicking
	PlayerSkills.medical
	PlayerSkills.stealth
	PlayerSkills.ranged
	*/
	break;
	}
	#ifdef DEBUG
	DrawText(TextFormat("UISTATE: %d ITEMS IN INV: %d VISABLE ITEMS:%d",UIState,amountofitemsplayer,visableitems),15,80,20,RED);
	#endif
}

void renderShopUI(){
	char probeXamount = 0;
	
	DrawTextEx(fonts[0],TextFormat("%s",Buildingstrings[UIShopType]),(Vector2){5,5},100,1,WHITE);
	switch (UIShopType){
		case 0x2: case 0x5: case 0x6: case 0x7: case 0x8: case 0x9: // SHOP TYPES
		if (CurrentMenuOptionSide == 0) {probeXamount = 0;}
		if (CurrentMenuOptionSide == 1) {probeXamount = amountofitemsplayer;}
		probecontrolsUImenu(1,2);
		if (CurrentMenuOptionSide == 0) {
			DrawTextEx(fonts[0],"BUY",(Vector2){windowX/2,5},100,1,WHITE);
		}
		if (CurrentMenuOptionSide == 1) {
			DrawTextEx(fonts[0],"SELL",(Vector2){windowX/2,5},100,1,WHITE);
			DrawInventory(2);
		}
		break;
		case 0x1: // THIS IS A HOME
		
		break;
		case 0x3: // THIS IS A INN
		
		probeXamount = probecontrolsUImenu(1,2);
		
		if (CurrentMenuOptionSide == 0) {
			DrawTextEx(fonts[0],"SLEEP",(Vector2){windowX/2,5},100,1,WHITE);
		}
		if (CurrentMenuOptionSide == 1) {
			DrawTextEx(fonts[0],"FIGHT",(Vector2){windowX/2,5},100,1,WHITE);
			DrawTextEx(fonts[0],"Info here about fight",(Vector2){5,100},100,1,WHITE);
		}
			#ifdef DEBUG
			DrawText(TextFormat("INN SCREEN - CurrentOPtion Up:%d, Current OPtion side:%d",CurrentMenuOption,CurrentMenuOptionSide),15,500,20,RED);
			#endif
		if (CurrentMenuOptionSide >= 1 && probeXamount>=0) {GameState=0x21;}
		// to fight, has to go to gamemode 0x7
		break;
		case 0x4: // THIS IS A TEMPLE
		
		break;
		case 0xA: // THIS IS A GUILD
		
		break;
		case 0xB: // THIS IS A BANK
		
		break;
		
	}
	
	#ifdef DEBUG
	DrawText(TextFormat("YOU'RE IN A BUILDING, BUILDING TYPE: %d, WHICH IS HOUSE NUMBER %d",UIShopType,UIShopeLocalLocation),15,80,20,RED);
	#endif
}

Vector3 calculate_vector(Vector3 B, Vector3 A) {
    Vector3 vectorAN;
    vectorAN.x = B.x - A.x;
    vectorAN.y = B.y - A.y;
    vectorAN.z = B.z - A.z;
    return vectorAN;
}

void InteractableObject(Model FunObject,Vector3 LocationOfInteractable,char Type, char Data1, char Data2){
	BoundingBox Ibox;
	#ifdef DEBUG
	Ibox = TranslateBoundingBox(GetModelBoundingBox(FunObject),LocationOfInteractable);
	DrawBoundingBox(Ibox,BLUE);
	#endif
	//DrawModel(FunObject,(Vector3){0,0,0},1,WHITE);
	switch(Type){
	case 0x1: //AWESOME DOOR! 
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		Ibox = TranslateBoundingBox(GetModelBoundingBox(FunObject),LocationOfInteractable);
		
		CharacterRay = GetScreenToWorldRay((Vector2){800,450},camera); //  800 IS HALF OF WindowX, 450 IS HALF OF WindowY
		AnswerRay = GetRayCollisionBox(CharacterRay,Ibox);
		if (AnswerRay.hit && AnswerRay.distance < 5.75f){ //&& AnswerRay.distance < 0.75f
			//printf("WE CHECKING DOORS!\n");
			GameState = 0x2;
			UIShopType = Data1;
			UIShopeLocalLocation = Data2;
			AnswerRay.hit = 0;
			}
		
	}
	break;
	case 0x2: //AWESOME ORE! 
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		Ibox = TranslateBoundingBox(GetModelBoundingBox(FunObject),LocationOfInteractable);
		
		CharacterRay = GetScreenToWorldRay((Vector2){800,450},camera); //  800 IS HALF OF WindowX, 450 IS HALF OF WindowY
		AnswerRay = GetRayCollisionBox(CharacterRay,Ibox);
		if (AnswerRay.hit && AnswerRay.distance < 5.75f){ //&& AnswerRay.distance < 0.75f
			//printf("WE CHECKING ORES!\n");
			GameState = 0x3;
			UIShopType = Data1;
			UIShopeLocalLocation = Data2;
			AnswerRay.hit = 0;
			}
		
	}
	break;
	case 0x3: //AWESOME NPC!
	
	break;
	case 0x4: //AWESOME LOOT!
	
	break;
	case 0x5: //AWESOME DUNGEON DOOR!
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		Ibox = TranslateBoundingBox(GetModelBoundingBox(FunObject),LocationOfInteractable);
		
		CharacterRay = GetScreenToWorldRay((Vector2){800,450},camera); //  800 IS HALF OF WindowX, 450 IS HALF OF WindowY
		AnswerRay = GetRayCollisionBox(CharacterRay,Ibox);
		if (AnswerRay.hit && AnswerRay.distance < 5.75f){
			//printf("WE CHECKING DOORS!\n");
			GameState = 0x20;
			AnswerRay.hit = 0;
			}
		
	}
	break;
	
	}
}

BoundingBox TranslateBoundingBox(BoundingBox box, Vector3 translation) {
    BoundingBox newBox = box;
    newBox.min.x += translation.x;
    //newBox.min.y += translation.y;
    newBox.min.z += translation.z;
    newBox.max.x += translation.x;
    //newBox.max.y += translation.y;
    newBox.max.z += translation.z;
    return newBox;
}

Color ColourDamage(char DamageAmount){
	#define MaxColourDamage 100
	char Normal = 0;
	// YELLOW > 0
	char Orange = MaxColourDamage/2;
	char Red = MaxColourDamage;
	if (DamageAmount == 0) {return WHITE;}else {
		if (DamageAmount < Orange) {return YELLOW;} else {
			if (DamageAmount == Red) {return RED;} else{
				return ORANGE;
			}
		}
		
	}
}

void PutInInventory(char B0,char B1,char B2,char B3,char B4,char B5,char B6,char B7,char B8,char B9){
	signed int AvSpace=-1;
	for (int CS=0; CS < INVENTORYSIZE+1; CS++){
		if (Inventory[CS*10]==0x00){AvSpace=CS*10;CS=INVENTORYSIZE+2;}
	}
	if (AvSpace >= 0){
		Inventory[AvSpace]=B0;
		Inventory[AvSpace+1]=B1;
		Inventory[AvSpace+2]= 0; //B2;
		Inventory[AvSpace+3]=B3;
		Inventory[AvSpace+4]=B4;
		Inventory[AvSpace+5]=B5;
		Inventory[AvSpace+6]=B6;
		Inventory[AvSpace+7]=B7;
		Inventory[AvSpace+8]=B8;
		Inventory[AvSpace+9]=B9;	
	}
	RefreshInventory();
}

void RefreshInventory(){
	CharacterWeight = 0;
	for (int gh = 0; gh>=INVENTORYSIZE; gh++){
		InventoryLookup[gh] = 0;
	}
	
	amountofitemsplayer = 0;	
	int pointinlookup = 0;

	for (int gh = 0; gh<INVENTORYSIZE+1; gh++){
		if(Inventory[gh*10]> 0x00){
			
			InventoryLookup[pointinlookup]=gh;
			pointinlookup++;
			amountofitemsplayer++;
			CharacterWeight = CharacterWeight + Inventory[gh*10+9];
			
			
		}
		
		
	}
	printf("UPDATED INVENTORY\n");
	CharacterWeight = CharacterWeight + CharacterMoney/100;
	CharacterWeightMax = PlayerStats.STR*50;

	//for (int UIII; UIII<INVENTORYSIZE+1; UIII++){if (InventoryLookup[UIII] != 0x00){amountofitemsplayer++;}else{UIII = INVENTORYSIZE+1;}}
	
	
}

void renderUI3d(){
	if (IsSprint==2){
		DrawTextEx(fonts[0],TextFormat("Stamina:%d",PlayerStamina),(Vector2){5,windowY-150},90,1,RED); //SHOW RED STAMINA BAR
	} else {
		DrawTextEx(fonts[0],TextFormat("Stamina:%d",PlayerStamina),(Vector2){5,windowY-150},90,1,GREEN); //SHOW GREEN STAMINA BAR
	}
	//DrawAnimated(Texture2D TheTextureAnime, Vector2 AnimePlacement, int AnimeScale, int AnimeFrame, int AnimeFrameMax)
	AttackCheck();
	
	renderClock((Vector2){500,100});
	//DrawAnimated(FullscreenGraphic[0],(Vector2){1,1},2,1,31);
	//DrawTexture(FullscreenGraphic[0],1,1,WHITE);
	/*
	if (DayTime >= 60000){DayTime = DayTime-60000; DayNum++;}
	if (DayTime >= 0) {DayLighting = 1;}
	if (DayTime > 5000) {DayLighting = 0;}
	if (DayTime > 35000) {DayLighting = 1;}
	if (DayTime > 40000) {DayLighting = 2;}
	*/
}

void renderClock(Vector2 ClockLoc){
	if (GameState == 0x0 && DayTime<35000) {
		DrawAnimated(AnimatedGraphic[0],ClockLoc,2,DayTime/816,51);
	#ifdef DEBUG
	DrawText(TextFormat("CLOCK:%d",DayTime/816),500,windowY-100,20,RED); // 816 magic number, how many time units for each clock frame
	#endif
	} else {DrawAnimated(AnimatedGraphic[0],ClockLoc,2,0,51);
	#ifdef DEBUG
	DrawText(TextFormat("CLOCK:DEAD"),500,windowY-100,20,RED);
	#endif
	}

}

void DrawInventory(char InventoryMode){
	int InventorySpacing = 0;
	switch (InventoryMode){
	case 1: case 2:
	DrawTextEx(fonts[0],TextFormat(">",CharacterWeight,CharacterWeightMax),(Vector2){5,windowY/4},100,1,WHITE);
	//visableitems
	for (int items = InventorySelection; items<amountofitemsplayer; items++){
		/*
		DrawTextEx(fonts[0],TextFormat("   %X,%X,%X,%X,%X,%X,%X,%X,%X,%X",
		Inventory[InventoryLookup[items]*10],
		Inventory[InventoryLookup[items]*10+1],
		Inventory[InventoryLookup[items]*10+2],
		Inventory[InventoryLookup[items]*10+3],
		Inventory[InventoryLookup[items]*10+4],
		Inventory[InventoryLookup[items]*10+5],
		Inventory[InventoryLookup[items]*10+6],
		Inventory[InventoryLookup[items]*10+7],
		Inventory[InventoryLookup[items]*10+8],
		Inventory[InventoryLookup[items]*10+9]
		),(Vector2){5,windowY/4+50*InventorySpacing},100,1,WHITE);
		*/
		if (Inventory[InventoryLookup[items]*10+2] == 0){
		DrawTextEx(fonts[0],TextFormat("   %s",ReturnItemNameInventory(InventoryLookup[items])),(Vector2){5,windowY/4+50*InventorySpacing},90,1,WHITE);}
		else{
		DrawTextEx(fonts[0],TextFormat("   %s",ReturnItemNameInventory(InventoryLookup[items])),(Vector2){5,windowY/4+50*InventorySpacing},90,1,GREEN);
		}
		InventorySpacing++;
	}
	if (IsKeyPressed(KEY_W)) {InventorySelection--;}
	if (IsKeyPressed(KEY_S)) {InventorySelection++;}
	if (InventorySelection < 0) {InventorySelection = amountofitemsplayer-1;}
	if (InventorySelection > amountofitemsplayer-1) {InventorySelection = 0;InventorySelection = 0;}
	break;
	}
	if (InventoryMode == 1){ //EQUIPING MODE
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		if (Inventory[InventorySelection*10+2] == 0){EquipItem(InventorySelection);} else {UnequipItem(Inventory[InventorySelection*10+2]);}
		}
	}
	if (InventoryMode == 2){ //SELLING MODE
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			DestoryItem(InventorySelection);
			
			RefreshInventory();
		}
	}
	#ifdef DEBUG
	if (IsKeyPressed(KEY_P)) {UnequipItem(0x4);} //EquipItem
	DrawText(TextFormat("%d,%d,%d,",Inventory[10],Inventory[11],Inventory[12]),5,windowY-100,20,RED);
	DrawText(TextFormat("%d,%d,%d,",Inventory[0],Inventory[1],Inventory[2]),5,windowY-120,20,RED);
	DrawText(TextFormat("%d,%d,%d,",InventoryLookup[0],InventoryLookup[1],InventoryLookup[2]),5,windowY-50,20,RED);
	DrawText(TextFormat("InventorySelection: %d,",InventorySelection),100,windowY-50,20,BLUE);
	DrawText(
	TextFormat("   %X,%X,%X,%X,%X,%X,%X,%X,%X,%X",
		Inventory[InventorySelection*10],
		Inventory[InventorySelection*10+1],
		Inventory[InventorySelection*10+2],
		Inventory[InventorySelection*10+3],
		Inventory[InventorySelection*10+4],
		Inventory[InventorySelection*10+5],
		Inventory[InventorySelection*10+6],
		Inventory[InventorySelection*10+7],
		Inventory[InventorySelection*10+8],
		Inventory[InventorySelection*10+9]
		),500,windowY-50,20,BLUE);
	DrawText(TextFormat("AKA: %s",ReturnItemNameInventory(InventorySelection)),100,windowY-80,20,BLUE);
	#endif
}

char* ReturnItemName(
	unsigned char IT0,
	unsigned char IT1,
	unsigned char IT2,
	unsigned char IT3,
	unsigned char IT4,
	unsigned char IT5,
	unsigned char IT6,
	unsigned char IT7,
	unsigned char IT8,
	unsigned char IT9){
		
	static char buffer[70];
	switch (IT0){
		case 0x01: // USELESS ITEM
		snprintf(buffer,sizeof(buffer),"%s WEIGHT:%d",Uselessstrings[IT1],IT9);
		break;
		case 0x02: // WEAPONS
		if (IT1 == 0x3 || IT1 == 0x4 ){
		snprintf(buffer,sizeof(buffer),"%s %s DEFENCE:%d WEIGHT:%d",Matstrings[IT5],Weaponstrings[IT1],IT4,IT9);
		} else{
		snprintf(buffer,sizeof(buffer),"%s %s DAMAGE:%d WEIGHT:%d",Matstrings[IT5],Weaponstrings[IT1],IT4,IT9);
		}
		break;
		case 0x03: //CLOTHES
		snprintf(buffer,sizeof(buffer),"%s %s DEFENCE:%d WEIGHT:%d",Matstrings[IT5],Clothestrings[IT1],IT4,IT9);
		break;
		case 0x04: //POTIONS
		if (IT1 < 17){
		snprintf(buffer,sizeof(buffer),"potion of %s",Potionstrings[IT1]);
		} else {
		snprintf(buffer,sizeof(buffer),"%s",Potionstrings[IT1]);	
		}
		break;
		case 0x05: //AMMO
		
		break;
		default:
		snprintf(buffer,sizeof(buffer),
		"ERROR: %X-%X-%X-%X-%X-%X-%X-%X-%X-%X",
		IT0,IT1,IT2,IT3,IT4,IT5,IT6,IT7,IT8,IT9
		);
		break;
		
		
	}
	//snprintf(buffer,sizeof(buffer),"The Awesome %s and %s",Weaponstrings[2],Weaponstrings[1]);

	return buffer;
}

char* ReturnItemNameInventory(unsigned char LocationInventory){
	return ReturnItemName(
	Inventory[LocationInventory*10+0],
	Inventory[LocationInventory*10+1],
	Inventory[LocationInventory*10+2],
	Inventory[LocationInventory*10+3],
	Inventory[LocationInventory*10+4],
	Inventory[LocationInventory*10+5],
	Inventory[LocationInventory*10+6],
	Inventory[LocationInventory*10+7],
	Inventory[LocationInventory*10+8],
	Inventory[LocationInventory*10+9]
	);
}

void EquipItem(unsigned char InventoryLocation){
	char typeofitem4 = 0;
	char typeofitem3 = 0;
	char typeofitem2 = Inventory[InventoryLocation*10+1];
	char typeofitem1 = Inventory[InventoryLocation*10];
	if (typeofitem1 == 0x2){ // WEAPON SLOTS
		switch (typeofitem2){
			case 0x0: typeofitem3 =0x1; break; // ONE HAND SWORD
			case 0x1: typeofitem3 =0x1; break; // ONE HAND AXE
			case 0x2: typeofitem3 =0x1; break; // ONE HAND MACE
			case 0x3: typeofitem3 =0x3; typeofitem4 = 0x1; break; // OFF HAND SHIELD (SMALL)
			case 0x4: typeofitem3 =0x3; typeofitem4 = 0x1; break; // OFF HAND SHIELD (BIG)
			case 0xA: typeofitem3 =0x1; typeofitem4 = 0x3; break; // TWO HAND SWORD
			case 0xB: typeofitem3 =0x1; typeofitem4 = 0x3; break; // TWO HAND AXE
			case 0xC: typeofitem3 =0x1; typeofitem4 = 0x3; break; // TWO HAND HAMMER
			case 0xD: typeofitem3 =0x1; typeofitem4 = 0x3; break; // TWO HAND STAFF
			case 0xE: typeofitem3 =0x1; typeofitem4 = 0x3; break; // TWO HAND BOW
			case 0xF: typeofitem3 =0x1; typeofitem4 = 0x3; break; // TWO HAND CROSSBOW
		}
	}
	
	if (typeofitem1 == 0x3){ // ARMOUR SLOTS
		switch (typeofitem2){
			case 0x1: typeofitem3 =0x5; break; // HELMET, HEAD
			case 0x2: typeofitem3 =0x6; break; // CHESTPLATE, TORSO
			case 0x3: typeofitem3 =0x6; break; // GLOVES, HANDS
			case 0x4: typeofitem3 =0x7; break; // PANTS, LEGS
			case 0x5: typeofitem3 =0x8; break; // BOOTS, FEET
			case 0x6: typeofitem3 =0x9; break; //CLOAK, CLOAK
		}
	}
	
	if (typeofitem1 == 0x4){ // POTION SLOT
		typeofitem3 =0x4;
	
	}
	/*
	if (typeofitem1 == 0x5) // AMMO SLOT
	*/
	
	if (typeofitem4 == 0){ // SIMPLE SWAP
		UnequipItem(typeofitem3);
		Inventory[InventoryLocation*10+2] = typeofitem3;
	}
	else { // COMPLEX SWAP
		UnequipItem(typeofitem3);
		Inventory[InventoryLocation*10+2] = typeofitem3;
		UnequipItem(typeofitem4);
	}
	
}

void UnequipItem(unsigned char WeaponTypeToUnequip){
	//WeaponTypeToUnequip++;
	#ifdef DEBUG
	printf("UNEQUIPING WRAPON TYPE:%X\n",WeaponTypeToUnequip);
	#endif
	EquipmentSlots[WeaponTypeToUnequip] = 0;
	
	for (int equipmentcycle = 0; equipmentcycle < INVENTORYSIZE; equipmentcycle++){
		if (Inventory[equipmentcycle*10+2] == WeaponTypeToUnequip) {Inventory[equipmentcycle*10+2] = 0;}
	}
	
	//EquipmentSlots[12]
}

Color Lcolor(Color Wanted){
	/*
	if (DayLighting == 0){
	Wanted.r = Wanted.r/2;
	Wanted.g = Wanted.g/2;
	Wanted.b = Wanted.b/2;
	}
	*/
	if (DayLighting == 1){
	Wanted.r = Wanted.r/2;
	Wanted.g = Wanted.g/2;
	Wanted.b = Wanted.b/2;
	}
	if (DayLighting == 2){
	Wanted.r = Wanted.r/4;
	Wanted.g = Wanted.g/4;
	Wanted.b = Wanted.b/4;
	}
	if (DayLighting == 10){
	Wanted.r = Wanted.r/3;
	Wanted.g = Wanted.g/4;
	Wanted.b = Wanted.b/4;
	}
	//Wanted.a
	return Wanted;
}

void ProcessTime(){ //TIME TO PAST THROUGH 1 CHUNK: 2400 Time units therefore about 25 chunks a day
	DayTime++;
	if (DayTime >= 60000){DayTime = DayTime-60000; DayNum++;}
	if (DayTime >= 0) {DayLighting = 1;}
	if (DayTime > 5000) {DayLighting = 0;}
	if (DayTime > 35000) {DayLighting = 1;}
	if (DayTime > 40000) {DayLighting = 2;}
	// PER TICK
	
	
	if (DayTime%2==0){
		//MULTIPLE'S OF 2
		#ifdef DEBUG
		DrawLine(0,windowY-15,windowX,windowY-15,YELLOW);
		
		#endif
		if (DayTime%10==0){
			//MULTIPLE'S OF 10
			#ifdef DEBUG
			DrawLine(0,windowY-10,windowX,windowY-10,RED);
			#endif
			if (IsSprint==1){PlayerStamina--;} // STAMINA CHECK FOR RUNNING
			if (IsSprint!=2){if (PlayerStamina <= 0) {IsSprint=2;}} // CHECK TO SEE IF EXHUSTED
			
			
			if (DayTime%100==0){
				//MULTIPLE'S OF 100
				#ifdef DEBUG
				DrawLine(0,windowY-5,windowX,windowY-5,PURPLE);
				#endif
				if (PlayerStamina < PlayerStaminaMax){if (IsSprint!=1){PlayerStamina++;}}//REGEN STAMINA
			}
		}
	}
} 

void miningUI(){
	char mininganswer = probecontrolsUImenu(2,0);
	
	DrawTextEx(fonts[0],"ORE VEIN",(Vector2){5,5},100,1,WHITE);
	DrawTextEx(fonts[0],TextFormat("%s",Uselessstrings[UIShopType]),(Vector2){windowX-400,5},100,1,WHITE);
	
	if (CurrentMenuOption == 0) DrawTextEx(fonts[0],"FOCUS MINE",(Vector2){5,300},100,1,WHITE);
	else DrawTextEx(fonts[0],"FOCUS MINE",(Vector2){5,300},100,1,GREEN);
	if (CurrentMenuOption == 1) DrawTextEx(fonts[0],"CAUTIOUS MINE",(Vector2){5,500},100,1,WHITE);
	else DrawTextEx(fonts[0],"CAUTIOUS MINE",(Vector2){5,500},100,1,GREEN);
	
	
	if (mininganswer >= 0) {
		switch (mininganswer){
		case 0:
			DayTime = DayTime+2400;
			ProcessTime();
			// ADD RANDOM ENCOUNTER THING HERE, MEDIUM RISK
			PutInInventory(0x1,UIShopType,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1);
		break;
		case 1:
			DayTime = DayTime+4800;
			ProcessTime();
			// ADD RANDOM ENCOUNTER THING HERE, LOW RISK
			PutInInventory(0x1,UIShopType,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1);
		break;
	}}
	#ifdef DEBUG
	DrawText(TextFormat("MINING SCREEN - UIShopType AKA ore:%d, Location:%d, CurrentOPtion:%d",UIShopType,UIShopeLocalLocation,CurrentMenuOption),15,80,20,RED);
	#endif
	
}
//CurrentMenuOption
char probecontrolsUImenu(unsigned char UIoptionINTs, unsigned char UIoptionINTsSIDE){
	if (IsKeyPressed(KEY_W)) {CurrentMenuOption--;}
	if (IsKeyPressed(KEY_S)) {CurrentMenuOption++;}
	if (CurrentMenuOption < 0) {CurrentMenuOption = UIoptionINTs-1;}
	if (CurrentMenuOption > UIoptionINTs-1) {CurrentMenuOption = 0;}
	
	if (UIoptionINTsSIDE > 0){
		if (IsKeyPressed(KEY_A)) {CurrentMenuOptionSide--;}
		if (IsKeyPressed(KEY_D)) {CurrentMenuOptionSide++;}
		if (CurrentMenuOptionSide < 0) {CurrentMenuOptionSide = UIoptionINTsSIDE-1;}
		if (CurrentMenuOptionSide > UIoptionINTsSIDE-1) {CurrentMenuOptionSide = 0;}
	}
	
	
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {return CurrentMenuOption;}
	else {
		return -1;
	}
}


void mapUI(){
	DrawTextureEx(MapUITexture,(Vector2){50,50},0,100,WHITE);
	//DrawTextureRec(MapUITexture,(Rectangle){windowX/2,windowY/2,10,10},(Vector2){windowX/2,windowY/2},WHITE);
	DrawTextEx(fonts[0],"MAP",(Vector2){5,5},100,1,WHITE);
}


void DestoryItem(unsigned char ToDestory){
	if (Inventory[InventorySelection*10+2] > 0) {UnequipItem(Inventory[InventorySelection*10+2]);}
	Inventory[InventorySelection*10] = 0;
	Inventory[InventorySelection*10+1] = 0;
	Inventory[InventorySelection*10+2] = 0;
	Inventory[InventorySelection*10+3] = 0;
	Inventory[InventorySelection*10+4] = 0;
	Inventory[InventorySelection*10+5] = 0;
	Inventory[InventorySelection*10+6] = 0;
	Inventory[InventorySelection*10+7] = 0;
	Inventory[InventorySelection*10+8] = 0;
	Inventory[InventorySelection*10+9] = 0;
	#ifdef DEBUG
	printf("DESTORYING ITEM: 0x%02X\n",ToDestory);
	#endif
}


void GenTownInventory(unsigned int genseed){
	unsigned int CurrentSwitch = 0;
	
	for (unsigned int GtiMain=0; GtiMain<townamount; GtiMain++){ // READ EACH TOWN
		for(unsigned int GtiSub=0; GtiSub<=81; GtiSub++){
			CurrentSwitch = GtiMain*0xD2;
			CurrentSwitch = CurrentSwitch+GtiSub;
			switch(towndata[CurrentSwitch]){
			case 1: case 3: // HOUSE, INN

			break;
			case 2: // GENERAL STORE
			PutInTownInventory(CurrentSwitch,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1);
			break;
			case 4: // TEMPLE
			
			break;
			case 5: //WEAPON STORE
			
			break;
			case 6: //ARMOUR STORE
			
			break;
			case 7: //JEWLER
			
			break;
			case 8: //ALCHEMIST
			
			break;
			case 9: //CLOTHESE STORE
			
			break;
			case 10: //GUILD
			
			break;
			case 11: //BANK
			
			break;
			case 0:break; // NO BUILDING	
			}
		}
	}
	
	#ifdef DEBUG
	printf("--- GenTownInventory ---\n");
	printf("GenTownInventory: Refeshed Town Inventory\n");
	printf("GenTownInventory: Total Towns:%d\n",townamount);
	printf("GenTownInventory: Houses Per Town:%d\n",81);
	printf("GenTownInventory: Total Houses:%d\n",townamount*81);
	FILE *InventoryFile;
	long MaxTownInventory = 81*10;
	InventoryFile = fopen("TownInventory.bin","wb");
	for (int greg = 0; greg<townamount*MaxTownInventory; greg++){
		fwrite(&TownInventory[greg],sizeof(unsigned char),1,InventoryFile);
	}
	fclose(InventoryFile);
	#endif
}

void PutInTownInventory(unsigned int TownInLoc,char B0,char B1,char B2,char B3,char B4,char B5,char B6,char B7,char B8,char B9){
	unsigned int AvSpace=TownInLoc*10;
	TownInventory[AvSpace]=B0;
	TownInventory[AvSpace+1]=B1;
	TownInventory[AvSpace+2]= 0; //B2;
	TownInventory[AvSpace+3]=B3;
	TownInventory[AvSpace+4]=B4;
	TownInventory[AvSpace+5]=B5;
	TownInventory[AvSpace+6]=B6;
	TownInventory[AvSpace+7]=B7;
	TownInventory[AvSpace+8]=B8;
	TownInventory[AvSpace+9]=B9;	
}

void DebugMode(){ // RandomDebugValues: veriable, array number, first trigger, max arrary number
	#ifdef DEBUG
	if (RandomDebugValues[2] == 0) {
			printf("WARNING: ENTERED DEBUG MODE\n");
			//Camera camera = { 0 };
			camera.position = (Vector3){ 25.0f, 25.0f, 25.0f }; 
			camera.target = (Vector3){ 0.0f, 12.0f, 0.0f };    
			camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          
			camera.fovy = 45.0f;                                
			camera.projection = CAMERA_PERSPECTIVE;  
			//UpdateCamera(&camera, CAMERA_ORBITAL);			
			RandomDebugValues[2] = 1;
			if (RandomDebugValues[0]==4){
				camera.position = (Vector3){ 6.0f, 3.0f, 6.0f }; 
				camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };    
			}
	}	
	#endif
	if (RandomDebugValues[0]!=2){UpdateCamera(&camera, CAMERA_ORBITAL);} 
		else {camera.position = (Vector3){ 0.0f, 0.0f, 10.0f };
		camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };  
		camera.projection = CAMERA_ORTHOGRAPHIC;  
	}
	BeginDrawing();
		//ClearBackground((Color){0,82,172,255});
		ClearBackground((Color){0,82,162,255});
		#ifdef DEBUG
		DrawText(TextFormat("DEBUG SCREEN: LOT:%d ARRAY NUM:%d",RandomDebugValues[0],RandomDebugValues[1]),15,40,70,RED);
		BeginMode3D(camera);
			//DrawModel(Buildings[2],(Vector3){0,0,0}, 1.0f, WHITE);
			if (RandomDebugValues[0]==0){DrawModel(Buildings[RandomDebugValues[1] ],(Vector3){0,0,0}, 1.0f, WHITE);}
			if (RandomDebugValues[0]==1){DrawModel(EnviromentModel[RandomDebugValues[1] ],(Vector3){0,0,0}, 1.0f, WHITE);}
			if (RandomDebugValues[0]==2){
				SignModel.materials[2].maps[MATERIAL_MAP_DIFFUSE].texture = signs[RandomDebugValues[1]];
				DrawModel(SignModel,(Vector3){10,10,0},10,Lcolor(WHITE));
			}
			if (RandomDebugValues[0]==3){
				DrawModel(DungeonBottom[RandomDebugValues[1]],(Vector3){0,0,0}, 1.0f, WHITE);
				DrawModel(DungeonTop[RandomDebugValues[1]],(Vector3){0,0,0}, 1.0f, WHITE);
				DrawModel(DungeonFloor[0],(Vector3){0,0,0}, 1.0f, WHITE);
				for (int Itemstuff=0; Itemstuff<DDP[RandomDebugValues[1]].Max;Itemstuff++){
					if (Itemstuff==0){DrawSphere((DDP+RandomDebugValues[1])->Placement[Itemstuff],1,BLUE);}
					else{DrawSphere((DDP+RandomDebugValues[1])->Placement[Itemstuff],1,RED);}
				}
			}
			if (RandomDebugValues[0]==4){DrawModel(DetailObject[RandomDebugValues[1]],(Vector3){0,0,0}, 1.0f, WHITE);}
			DrawGrid(20, 10.0f);
		EndMode3D();
			if (RandomDebugValues[0]==5){
				DrawAnimated2(FullscreenGraphic[RandomDebugValues[1]],(Vector2){windowX/2,windowY/2},2,RandomDebugValues[5],MaxFramesData[RandomDebugValues[1]]);
				if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){RandomDebugValues[5] = 0;} else {RandomDebugValues[5]++;}
				if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){RandomDebugValues[5] = 0;}
				} else {RandomDebugValues[5] = 0;}
		#endif
		
	if (Hdungeon1 != Hdungeon2){DrawText(TextFormat("DIFFERENCE IN ROOF/WALL %d:%d",Hdungeon1,Hdungeon2),30,100,70,RED);}
	EndDrawing();
	#ifdef DEBUG
	//FIND MAX:
	if (RandomDebugValues[0]==0){RandomDebugValues[3] = Hbuildings-1;}
	if (RandomDebugValues[0]==1){RandomDebugValues[3] = Henviroments-1;}
	if (RandomDebugValues[0]==2){RandomDebugValues[3] = Hsigns-1;}
	if (RandomDebugValues[0]==3){RandomDebugValues[3] = Hdungeon1-1;}
	if (RandomDebugValues[0]==4){RandomDebugValues[3] = Hdetail-1;}
	if (RandomDebugValues[0]==5){RandomDebugValues[3] = Hfullscreen-1;}
	
	
	if (IsKeyPressed(KEY_W)) {RandomDebugValues[1]++; if (RandomDebugValues[1] > RandomDebugValues[3]) {RandomDebugValues[1] = 0;}}
//	IncreaseValue(KEY_D,&RandomDebugValues,1,RandomDebugValues[3]);
	if (IsKeyPressed(KEY_S)) {RandomDebugValues[1]--; if (RandomDebugValues[1] > RandomDebugValues[3]) {RandomDebugValues[1] = RandomDebugValues[3];}}
	
	// CYCLE THROUGH DIFFERENT VERIABLES
	if (IsKeyPressed(KEY_D)) {RandomDebugValues[2] = 0; RandomDebugValues[0]++;RandomDebugValues[1] = 0;}
	if (IsKeyPressed(KEY_A)) {RandomDebugValues[2] = 0; RandomDebugValues[0]--;RandomDebugValues[1] = 0;}
	if (IsKeyPressed(KEY_Q)) {RandomDebugValues[2] = 0;} // RESET CAMERA TO ORBIT
	#endif
}
/*
void IncreaseValue(unsigned int KeyPress, unsigned char *ptr, unsigned char ArrayLocation, unsigned maxamount){
	if (IsKeyPressed(KeyPress)){*(ptr+ArrayLocation)++; if (*(ptr+ArrayLocation)>maxamount){*(ptr+ArrayLocation)=0;} printf("READ FOUND, VALUE:%d\n",*(ptr+ArrayLocation));}
}
*/

void Constant3DInit(unsigned char UsingChunkCheck){
		#ifdef CUSTOMMOVEMENT
		LookDelta = (Vector3){0,0,0};
		MoveDelta = (Vector3){0,0,0};
		#endif
		probecontrols();
		if (1==UsingChunkCheck){ //CHECK CHUNKABLE MODE
			chunkcheck(chuncksize);
		}
		oldCamPos = camera.position;
		oldLookPos = camera.target;
		#ifdef CUSTOMMOVEMENT
		UpdateCameraPro(&camera,
		MoveDelta, // MOVEMENT [FORWARD,RIGHT,UP]
		LookDelta, // ROTATION [RIGHT,DOWN,SPIN]
		0);
		#else
		UpdateCamera(&camera, CAMERA_FIRST_PERSON); //CAMERA_FIRST_PERSON
		#endif
		CharacterRay.direction = calculate_vector((Vector3){*Px,*Py,*Pz},(Vector3){*Ax,*Ay,*Az});
		CharacterRay.position = (Vector3){*Px,*Py,*Pz};
		Pothit = 0;
	
}

void DungeonCameraReset(){
	
	
}

void DungeonMode(){
	ProcessTime();
	Constant3DInit(0); // Stating 9 due to arching postioning not being used
	RandomDebugValues[3] = 0;
	
	BeginDrawing();
		ClearBackground((Color){50,50,50,255});
	
	BeginMode3D(camera);
		DungeonChunk((Vector3){0,0,0},0);
		
		#ifdef DEBUG
		for(int intlaz = 0; intlaz<8; intlaz++){
			if (intlaz != 7){
			DrawRay(ShootRayFromVector((Vector3){camera.position.x,camera.position.y-1,camera.position.z},intlaz),RED);
			} else {DrawRay(ShootRayFromVector((Vector3){camera.position.x,camera.position.y-1,camera.position.z},intlaz),BLUE);}
		}
		#endif
	EndMode3D();
	if (Pothit == 1) {DrawRectangle(5, 5, 420, 260,RED);}
	EndDrawing();
	
}

void DungeonChunk(Vector3 VPlacement, unsigned int DungeonNum){
	RayCollision RayAnswer = { 0 };
	DrawModel(DungeonBottom[DungeonNum], VPlacement, 1.0f, Lcolor(WHITE)); //WALLS
	DrawModel(DungeonTop[DungeonNum], VPlacement, 1.0f, Lcolor(WHITE)); //ROOF
	DrawModel(DungeonFloor[0], VPlacement, 1.0f, Lcolor(WHITE)); //FLOOR
	
	for (int DOD=0; DOD<DDP[DungeonNum].Max; DOD++){
		if (RandomArray[DOD]>=0x10 && RandomArray[DOD]<=0x10+Hdetail){
			DrawModel(DetailObject[RandomArray[DOD]-0x10],DDP->Placement[RandomArray[DOD]-0x10],1,Lcolor(WHITE));
			//DrawSphere(DDP->Placement[DOD],1,RED);
		}
	}
	
	for (int LZC=0; LZC<8; LZC++){
		RayAnswer = GetRayCollisionMesh(
			ShootRayFromVector((Vector3){camera.position.x,camera.position.y,camera.position.z},LZC),
			DungeonBottom[DungeonNum].meshes[0],
			DungeonBottom[DungeonNum].transform
		);
		if (RayAnswer.distance < 0.15f && RayAnswer.hit) {Pothit = 1;}
	}
}

Ray ShootRayFromVector(Vector3 VectorCentre, unsigned char VectorDirection){ //0-7
	#define VSAM 5
	Ray ReturnRay;
	switch(VectorDirection){
		case 0: //NORTH
		ReturnRay.direction = (Vector3){VSAM,0,0};
		break;
		case 1: //NORTH EAST
		ReturnRay.direction = (Vector3){VSAM,0,VSAM};
		break;
		case 2: //EAST
		ReturnRay.direction = (Vector3){0,0,VSAM};
		break;
		case 3: //SOUTH EAST
		ReturnRay.direction = (Vector3){-VSAM,0,VSAM};
		break;
		case 4: // SOUTH
		ReturnRay.direction = (Vector3){-VSAM,0,0};
		break;
		case 5: // SOUTH WEST
		ReturnRay.direction = (Vector3){-VSAM,0,-VSAM};
		break;
		case 6: // WEST
		ReturnRay.direction = (Vector3){0,0,-VSAM};
		break;
		case 7: // NORTH WEST
		ReturnRay.direction = (Vector3){VSAM,0,-VSAM};
		break;
	}
	ReturnRay.position = VectorCentre;
	return ReturnRay;
}	

void floodrandomarray(unsigned char LOWER, unsigned char HIGHER) { // LOWER THEN IS OBJ, HIGHER THEN IS ENINME
	SetRandomSeed(XYcord(archingX,archingZ));
	for (int FRA=0; FRA<RandomArrayMax; FRA++){
		RandomArray[FRA] = GetRandomValue(0x00,0xFF);
		if (RandomArray[FRA] <= LOWER){
			// LOWER STUFF HERE
			RandomArray[FRA] = GetRandomValue(0x10,0x10+Hdetail);
		} else {
			if (RandomArray[FRA] >= HIGHER){
			// HIGHER STUFF HERE	
			RandomArray[FRA] = 0xFF;
			} else {
				RandomArray[FRA] = 0;
			}
		}
	}
}

/*
NPC STRUT:
.type uchar
.subtype uchar
.level uchar
.position vector3
.deltaposition vector3
.looking vector3
.Health int
.fatigue int
.Hmovement int
.Hattacking int
.mana int
.inventory *int?

*/

void NPCcalc(){
	
	
}

void HallMode(){
	if (NewArea == 1){ // FIRST ENTRANCE
		#ifdef DEBUG
		printf("Your in the inn for the first time!\n");
		#endif
		camera.position = (Vector3){ 0.0f, 2.0f, 2.0f }; // Camera position
		camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };    
	}
	
	ProcessTime();
	DayLighting = 10;
	Constant3DInit(0); // Stating 9 due to arching postioning not being used
	
	BeginDrawing();
		ClearBackground((Color){50,50,50,255});
	
	BeginMode3D(camera);
		//DungeonChunk((Vector3){0,0,0},0);
		DrawModel(DungeonFloor[0], (Vector3){0,0,0}, 1.0f, Lcolor(WHITE)); //FLOOR
		DrawModel(InsideBuilding[0], (Vector3){0,0,0}, 1.0f, Lcolor(WHITE));
		
	EndMode3D();
	// UI HERE>>
	EndDrawing();
	
	#ifdef DEBUG
	if (IsKeyPressed(KEY_KP_9)) {GameState = 0x0;}
	#endif
}

void DrawAnimated(Texture2D TheTextureAnime, Vector2 AnimePlacement, int AnimeScale, int AnimeFrame, int AnimeFrameMax){
	//if (AnimeFrame>AnimeFrameMax){}
	int AnimeRealFrame = AnimeFrame;
	while (AnimeRealFrame > AnimeFrameMax) {AnimeRealFrame = AnimeRealFrame - AnimeFrameMax;}
	
	
	int AnimeSizeX = TheTextureAnime.width / AnimeFrameMax;
	int AnimeSizeY = TheTextureAnime.height;
	Rectangle RecSource = (Rectangle){AnimeSizeX*AnimeRealFrame,0,AnimeSizeX,AnimeSizeY}; //51 on clock
	Rectangle RecDest = (Rectangle){AnimePlacement.x,AnimePlacement.y,RecSource.width * AnimeScale,RecSource.height * AnimeScale};
	
	DrawTexturePro(TheTextureAnime,RecSource,RecDest,(Vector2){RecDest.width/2,RecDest.height/2},0,WHITE );
	//DrawTextureRec(TheTextureAnime,RecSource,AnimePlacement,WHITE);
}
void DrawAnimated2(Texture2D TheTextureAnime, Vector2 AnimePlacement, int AnimeScale, int AnimeFrame, int AnimeFrameMax){
	if (AnimeFrame < AnimeFrameMax){
	
	
	int AnimeSizeX = TheTextureAnime.width / AnimeFrameMax;
	int AnimeSizeY = TheTextureAnime.height;
	Rectangle RecSource = (Rectangle){AnimeSizeX*AnimeFrame,0,AnimeSizeX,AnimeSizeY}; //51 on clock
	Rectangle RecDest = (Rectangle){AnimePlacement.x,AnimePlacement.y,RecSource.width * AnimeScale,RecSource.height * AnimeScale};
	
	DrawTexturePro(TheTextureAnime,RecSource,RecDest,(Vector2){RecDest.width/2,RecDest.height/2},0,WHITE );
	}
	//DrawTextureRec(TheTextureAnime,RecSource,AnimePlacement,WHITE);
}

int AttackState = 100;
int IdealStance = 1;
unsigned char WeaponDrawn = 1;
int WeaponCoolDown = 30;
unsigned char WeaponSelect = 1;
int WeaponFrame = 0;

void AttackCheck(){
	
	//IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
	if (WeaponDrawn){
		if (IsKeyPressed(KEY_Z)){WeaponDrawn = 0;} // Sheeth weapon
		//---- choose your stance
		if (IsKeyPressed(KEY_ONE)) {IdealStance = 1;}
		if (IsKeyPressed(KEY_TWO)) {IdealStance = 2;}
		if (IsKeyPressed(KEY_THREE)) {IdealStance = 3;}
		//----
		if (AttackState == 0) {WeaponFrame = 0;}  // Reset Animation Frames for Weapon
		else {WeaponFrame++; AttackState--;}
		
		if (AttackState <= 0 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){ // ATTACK
			AttackState = MaxFramesData[WeaponSelect] + WeaponCoolDown;
			
			//DoAttack();
		}
		
		// Decide animation
		if (WeaponFrame <= MaxFramesData[WeaponSelect]) {
			DrawAnimated2(FullscreenGraphic[WeaponSelect],(Vector2){windowX/2,windowY/2},2,WeaponFrame,MaxFramesData[WeaponSelect]);
		}
		//-----
		#ifdef DEBUG
		DrawText(TextFormat("STANCE:%d ATTACK:%d FRAME:%d",IdealStance,AttackState,WeaponFrame),windowX-400,180,20,BLACK);
		//MaxFramesData
		#endif
	} else {if (IsKeyPressed(KEY_Z)){WeaponDrawn = 1;AttackState = 100; WeaponFrame = MaxFramesData[WeaponSelect]+1;}} // Draw Weapon
}