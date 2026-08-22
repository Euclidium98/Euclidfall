#include <stdio.h>
#include <stdlib.h>


char *data;
char inputdata[2];
int filenumber = 0;
int totalamount = 0;


void main(){
	printf("LOADING\n");
	data = calloc(50,sizeof(char));
	inputdata[0] =0;
	inputdata[1] =0;
	printf("Daggerfall 2 Dungeon Maker\n\n");
	printf("FILE NUMBER:\n");
	scanf("%d", &filenumber);
	printf("FILE NUMBER = %d\n\n",filenumber);

	scanf("%d %d", inputdata[0], inputdata[1]);
	
	
	
	for (int i=0; i<25; i++){
		printf("\n");
		printf("EMPTY NUM: %d:", i);
		scanf("%d %d", inputdata[i], inputdata[i+25]);
		
		if (inputdata[i] > 50) {i = 100;} else {
		totalamount++; }
	}
	printf("/nFINAL DATA:\n");
	for (int i=0; i<=totalamount; i++){
		printf("X:%d Y:%d\n");
		
	}
	
	free(data);
	printf("\nCOMPLETE\n");
	getchar();
	return;
}