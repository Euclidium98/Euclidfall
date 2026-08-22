#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "C:\Users\Euclidium\Documents\C\raylibtest\BMPWriter.h"

unsigned int testy = 300;

typedef union {
	unsigned int u32;
	unsigned char u8[4];
} Union32to8;

Union32to8 UNIONSTUFF;

void malloccalls();
unsigned int filesize;
unsigned int mapfilesize;
unsigned char *mapdata;
unsigned char *colourdata;


void main(){
	printf("RUNNING\n");
	malloccalls();
	UNIONSTUFF.u32 = testy;
	printf("U32: %08X\n",UNIONSTUFF.u32);
	printf("U8:  %02X%02X%02X%02X\n",UNIONSTUFF.u8[3],UNIONSTUFF.u8[2],UNIONSTUFF.u8[1],UNIONSTUFF.u8[0]);
	
	BMPWriter(mapdata,colourdata,mapfilesize,1);
	
	printf("SIZE OF BMP:%d\n",SizeOfBmp);
	for (int i = 0; i <SizeOfBmp; i++){
		//if (i == 0x36) printf("     ");
		printf("%02X",BMPFile[i]);
		
	}
	
	
	
	free(BMPFile);
	free(colourdata);
	free(mapdata);
}

void malloccalls(){
	FILE *file;
	file = fopen("Map5.bin","rb");
	if (file == NULL) {printf("Sorry Couldn't open Map1.bin\n"); return;};
	fseek(file, 0, SEEK_END);
	mapfilesize = ftell(file);
	fseek(file, 0, SEEK_SET);
	printf("Map1.bin:%d bytes\n", mapfilesize);
	mapdata = calloc(mapfilesize+1,sizeof(unsigned char));
	fread(mapdata,mapfilesize,1,file);
	fclose(file);
	
	
	file = fopen("D0.bin","rb");
	if (file == NULL) {printf("Sorry Couldn't open D0.bin\n"); return;};
	fseek(file, 0, SEEK_END);
	filesize = ftell(file);
	fseek(file, 0, SEEK_SET);
	printf("D0.bin:%d bytes\n", filesize);
	colourdata = calloc(filesize+1,sizeof(unsigned char));
	fread(colourdata,filesize,1,file);
	fclose(file);
	
	
}