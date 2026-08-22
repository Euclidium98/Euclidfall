#include <stdio.h>
#include <stdlib.h>
#include <string.h> 


#define NULLT 0x00

unsigned char *buffer;
unsigned char *bufferINT;
unsigned char read = 0;
int b=1;
int rowamount = 0 ;
int crash = 0;

unsigned char returnchar(int place,unsigned char amount);
unsigned char charcon(unsigned char NUM);
unsigned char finddigits(unsigned char L);
void Nullterm();
int filesize;

int main(int argc, char *arg[]){
	// OPEN FILE AND PUT INTO RAM

	if (argc == 1) {printf("Run command again with file name.csv\n"); return -1;}

	FILE *file;
	file = fopen(arg[1],"rb");
	if (file == NULL) {printf("Could not open CSV\n"); return -1;};
	fseek(file, 0, SEEK_END);
	filesize = ftell(file);
	fseek(file, 0, SEEK_SET);
	printf("CSV:%d bytes\n", filesize);
	buffer = calloc(filesize+1,sizeof(unsigned char));
	bufferINT = calloc(filesize+1,sizeof(unsigned char));
	fread(buffer,filesize,1,file);
	fclose(file);
	
	for(int i=0; i<filesize+1; i++){
		if (buffer[i] == 0x2C){buffer[i] = NULLT;};
		if (buffer[i] == 0xD && buffer[i+1] == 0xA ){buffer[i] = NULLT; buffer[i+1] = NULLT;rowamount++;};
	}
//	printf("\n");
	
	bufferINT[0] = returnchar(0,finddigits(0));
	for (int i=0; i<filesize+1; i++){
		
		if (buffer[i] == NULLT){read = 1;}
		if (buffer[i] != NULLT && read == 1) {
			read = 0;
			bufferINT[b] = returnchar(i,finddigits(i));
			//printf("ANS: %d:%X\n",i,returnchar(i,finddigits(i)));
			
			b++;
			}
		
	}

	if (crash==1){printf("ERROR: CELL OVER 255, BEYOND 1 BYTE\n"); return -1;}
	
	printf("BIN:%d bytes\n",b);
	printf("HEX:\n");
	for(int i=0; i<b+1; i++){
		printf("%X ",bufferINT[i]);
	}
	printf("\nDEC:\n");
		for(int i=0; i<b+1; i++){
		printf("%d ",bufferINT[i]);
	}
	//printf("\n ROWS:%d\n",rowamount);
	
	
	
	file = fopen("OUT.bin","wb");
	
	for (int greg = 0; greg<b; greg++){
		fwrite(&bufferINT[greg],sizeof(unsigned char),1,file);
	}
	
	fclose(file);
	
	free(buffer);
	free(bufferINT);
	return 1;
}

unsigned char returnchar(int place, unsigned char amount){
	unsigned char AN = 0;
	unsigned char D1 = 0;
	unsigned char D2 = 0;
	unsigned char D3 = 0;
	if (amount == 1){D3 = charcon(buffer[place]);}
	if (amount == 2){D2 = charcon(buffer[place])*10; D3 = charcon(buffer[place+1]);}
	if (amount == 3){D1 = charcon(buffer[place])*100; D2 = charcon(buffer[place+1])*10; D3 = charcon(buffer[place+2]);}
	AN=D3+D2+D1;
	if (AN > 255){crash = 1;}
	//if (AN == 0) {printf("NULL ANSWER");}
	return AN;
}


unsigned char charcon(unsigned char NUM){
	if (NUM==48){return 0;}
	if (NUM==49){return 1;}
	if (NUM==50){return 2;}
	if (NUM==51){return 3;}
	if (NUM==52){return 4;}
	if (NUM==53){return 5;}
	if (NUM==54){return 6;}
	if (NUM==55){return 7;}
	if (NUM==56){return 8;}
	if (NUM==57){return 9;}
}

unsigned char finddigits(unsigned char L){
	if (buffer[L+1] == NULLT) {return 1;}
	if (buffer[L+2] == NULLT) {return 2;}
	if (buffer[L+3] == NULLT) {return 3;}
	if (buffer[L+4] == NULLT) {return 4;}
	if (buffer[L+5] == NULLT) {return 5;}
	
}

void Nullterm() {
	for(int i=0; i<filesize+1; i++){
		if (buffer[i] == 0x2C){buffer[i] = NULLT;};
		if (buffer[i] == 0xD && buffer[i+1] == 0xA ){buffer[i] = NULLT; buffer[i+1] = NULLT;rowamount++;};
	}	
}

