

unsigned int SizeOfBmp;
unsigned char *BMPFile;

void BMPWriter(unsigned char ArrayType[],unsigned char ColourArray[], unsigned int SizeOfArray, unsigned char ExportAsBmp){
	typedef union {
		unsigned int u32;
		unsigned char u8[4];
	} Union32to8;
	

	unsigned int By4Check;
	unsigned int SquareCheck;
	unsigned char AddBuffer = 0;
	unsigned int FileSize =0;
	unsigned int BufferCheck =0;
	unsigned int BMPLocation =54;
	Union32to8 UNIONSTUFF;
	SquareCheck = sqrt(SizeOfArray);
	if (SquareCheck*SquareCheck != SizeOfArray) {printf("SIZE OF ARRAY IS NOT SQUARE %d * %d != %d\n",SquareCheck,SquareCheck,SizeOfArray); return;}
	// CHECK IF BUFFER BYTES ARE REQURIED
	By4Check = SquareCheck*3;
	//By4Check = By4Check/4;
	BufferCheck = By4Check;
	if (BufferCheck % 4 == 0) {printf("NO BUFFERBYTES REQURIED\n"); AddBuffer = 0;} else{
		BufferCheck++;
		if (BufferCheck % 4 == 0) {printf("1 BUFFERBYTES REQURIED\n"); AddBuffer = 1;}
		BufferCheck++;
		if (BufferCheck % 4 == 0) {printf("2 BUFFERBYTES REQURIED\n"); AddBuffer = 2;}
		BufferCheck++;
		if (BufferCheck % 4 == 0) {printf("3 BUFFERBYTES REQURIED\n"); AddBuffer = 3;}
	}
	

	SizeOfBmp = SizeOfArray*3; // +54
	BMPFile = calloc(SizeOfBmp+100,sizeof(unsigned char));
	BMPFile[0] = 0x42;
	BMPFile[1] = 0x4D;
	FileSize = SizeOfBmp+54;
	FileSize = FileSize+AddBuffer*SquareCheck;
	UNIONSTUFF.u32 = FileSize;
	BMPFile[2] = UNIONSTUFF.u8[0];
	BMPFile[3] = UNIONSTUFF.u8[1];
	BMPFile[4] = UNIONSTUFF.u8[2];
	BMPFile[5] = UNIONSTUFF.u8[3];
	// UNUSED SECTOR 
	BMPFile[6] = 0;
	BMPFile[7] = 0;
	BMPFile[8] = 0;
	BMPFile[9] = 0;
	// DATA OFFSET 0x36
	BMPFile[10] = 0x36;
	BMPFile[11] = 0;
	BMPFile[12] = 0;
	BMPFile[13] = 0;
	// SIZE OF HEADER
	BMPFile[14] = 0x28;
	BMPFile[15] = 0;
	BMPFile[16] = 0;
	BMPFile[17] = 0;
	// WIDTH
	UNIONSTUFF.u32 = SquareCheck;
	BMPFile[18] = UNIONSTUFF.u8[0];
	BMPFile[19] = UNIONSTUFF.u8[1];
	BMPFile[20] = UNIONSTUFF.u8[2];
	BMPFile[21] = UNIONSTUFF.u8[3];
	// HEIGHT
	BMPFile[22] = UNIONSTUFF.u8[0];
	BMPFile[23] = UNIONSTUFF.u8[1];
	BMPFile[24] = UNIONSTUFF.u8[2];
	BMPFile[25] = UNIONSTUFF.u8[3];
	// PLANES AND BITS PER PIXEL
	BMPFile[26] = 1;
	BMPFile[27] = 0;
	BMPFile[28] = 0x18;
	BMPFile[29] = 0;
	
	//COMPRESSION, COMPRESSION SIZE, XperMetre, YperMetre, Colours Used, Important Colours,
	for (int i=30; i<54; i++) {
		BMPFile[i] = 0;
		if (i == 34) BMPFile[i] = 0x30;
	}
	SizeOfBmp = SizeOfBmp+54;
	// BEGIN DATA TRANSFER TO BMP
	
	for (unsigned int i=0; i < 100; i++){
		printf("%02X ",ColourArray[i]);
		}
		printf("\n");
	
	for (unsigned int i=1; i<SizeOfArray; i++){
		
		
		BMPFile[BMPLocation] = ColourArray[ArrayType[i]*3];
		BMPLocation++;
		BMPFile[BMPLocation] = ColourArray[ArrayType[i]*3+1];
		BMPLocation++;
		BMPFile[BMPLocation] = ColourArray[ArrayType[i]*3+2];
		BMPLocation++;
		
		
		if (i % SquareCheck == 0) {
			switch(AddBuffer){
				case 0:
				break;
				case 1:
				BMPFile[BMPLocation] = 0x0;
				BMPLocation++;
				break;
				case 2:
				BMPFile[BMPLocation] = 0x0;
				BMPLocation++;
				BMPFile[BMPLocation] = 0x0;
				BMPLocation++;
				break;
				case 3:
				BMPFile[BMPLocation] = 0x0;
				BMPLocation++;
				BMPFile[BMPLocation] = 0x0;
				BMPLocation++;
				BMPFile[BMPLocation] = 0x0;
				BMPLocation++;
				break;
				
			}
		}
		
	}
	
	//BMPFile[0x65] = 0xFF;
	
	
	
	

}