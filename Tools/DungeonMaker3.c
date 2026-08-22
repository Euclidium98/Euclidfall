#include <stdio.h>
#include <stdlib.h>

#define ANSIReset "\x1b[0m"

void gotoxy(int x, int y);

unsigned char *data = NULL;
unsigned char *colourdata = NULL;
unsigned int squared = 5;

char ANSIColour[17][15] = {
        "\x1b[31m", 
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

void TypeCharacter();
void malloccalls();
void freeeverything();
void DrawScreen();

void main(){
	printf("\x1b[0m");
	printf("\033[2J");

	//printf("Enter Square Root:");
	
	DrawScreen();


	freeeverything();
}

void DrawScreen(){
	for (int i=0; i<squared; i++){
		printf("\n");
		for (int o=0; o<squared; o++){
			printf("%02X",data[i]);
		}
	}	
}

void TypeCharacter(){

}

void gotoxy(int x, int y) {
    // \033 is the ESC character
    printf("\033[%d;%dH", y, x);
}

void malloccalls(){
	data = calloc(squared*squared,sizeof(unsigned char));
}
void freeeverything(){
	free(data);
	free(colourdata);
}