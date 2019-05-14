#include <stdio.h>
#define READ_SIZE 3

FILE* toConvert;
FILE* convertedText;

int toWrite;
char currentRead[READ_SIZE];
int charactersRead;
int keepRunning = 1;
int conversion;
int toBase64;
char base64Table[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuywxyz0123456789+/";

void printBits(char* readToPrint);
void printIntBitsToConvert(int toPrint);

int main(int argc, char** argv){
	if(argc < 2){
		printf("ERROR! No input file detected!\n");
		return 1;
	}

	if(argc < 3){
		toWrite = 0;

	} else  {
		toWrite = 1;
		convertedText = fopen(argv[2],"w+");
	}

	toConvert = fopen(argv[1],"r");



	while(keepRunning){
		charactersRead = fread(currentRead, sizeof(char), READ_SIZE, toConvert);
		for(int x = charactersRead; x < READ_SIZE; x++) currentRead[x] = 0;
		//printBits(currentRead);

		conversion = 0;
		for(int x = 0; x < READ_SIZE; x++){
			conversion <<= 8;
			conversion |= currentRead[x];
		}
		//printIntBitsToConvert(conversion);

		for(int x = 0; x < 4; x++){
			toBase64 = conversion >> (6 * ( 3 - x));
			toBase64 &= 0x3f;
			//printIntBitsToConvert(toBase64);

			if( (6 * x) > charactersRead * 8 ) {
				if(toWrite) fprintf(convertedText, "=");
				else printf("=");
			}
			else {
				if(toWrite) fprintf(convertedText,"%c",base64Table[toBase64]);
				else printf("%c",base64Table[toBase64]);
			}

		}

		if(charactersRead < READ_SIZE){
			keepRunning = 0;
		}
	}

	fclose(toConvert);
	if(toWrite) fclose(convertedText);

	printf("\n");

}

void printBits(char* readToPrint){
	char currentChar;
	char temp;
	for(int chr = 0; chr < READ_SIZE; chr++){
		currentChar = readToPrint[chr];
		printf("%c %X ",readToPrint[chr],currentChar);
		for(int x = 0; x < 8; x++){
			temp = currentChar & 0x80;
			temp >>= 7;
			temp &= 1;
			if(temp == 1){
				printf("1");
			} else printf("0");
			currentChar <<= 1;
		}
		printf("\t");
	}
	printf("\n");
}

void printIntBitsToConvert(int toPrint){
	int currentInt = toPrint;
	int temp;
	currentInt <<= 8;
	for(int x = 0; x < 8 * (sizeof(int)-1); x++){
		temp = currentInt & 0x80000000;
		temp >>= 31;
		temp &= 1;
		if(temp == 1) printf("1");
		else printf("0");
		currentInt <<= 1;
	}

	printf("\n");
}