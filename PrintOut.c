#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include "PrintOut.h"
#include "TypeDefs.h"
#define true 1
#define false 0
#define LIGHTGREEN_ON_BLACK 2
#define GOLD_ON_BLACK 6
#define WHITE_ON_BLACK 7
#define TEAL_ON_BLACK 11
#define RED_ON_BLACK 12
#define PINK_ON_BLACK 13
#define YELLOW_ON_BLACK 14

const char *QUESTION_PRINTOUT="?>";
const char *ERROR_PRINTOUT="!>";
const char *REGULAR_PRINTOUT=" >";
HANDLE  hConsole;

void setPrintColor(int colorCode);
void resetPrintColor(void);
void printTrueFalseColored(int trueFalse, char *truePrint, char *falsePrint);

//Public Members================================================================
void initilizePrintOut(void){
	hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
}

void resetPrintOut(void){
	resetPrintColor();
}

void println(char *line, char precursorType){
	char precursor[3];
	switch (precursorType){
		case 'p': strcpy(precursor,QUESTION_PRINTOUT); setPrintColor(TEAL_ON_BLACK); break;
		case 'e': strcpy(precursor,ERROR_PRINTOUT); setPrintColor(RED_ON_BLACK); break;
		case 's': strcpy(precursor,REGULAR_PRINTOUT); setPrintColor(YELLOW_ON_BLACK); break;
		case 'r': strcpy(precursor,REGULAR_PRINTOUT); break;
		default: strcpy(precursor,""); break;
	}
	printf("%s %s\n",precursor,line);
	resetPrintColor();
}

void printList(char *lines[], int numOfLines){
	printf("%s\n",lines[0]);
	for(int i=1; i<numOfLines; i++){
		printf("  . %s\n",lines[i]);
	}
}

void printProcessInformation(char *line, char stage){
	setPrintColor(PINK_ON_BLACK);
	if (stage=='s' || stage=='S'){
		printf("\n>>> %s\n",line);
	} else if(stage=='e' || stage=='E'){
		printf("\n%s <<<\n\n",line);
	}resetPrintColor();
}

int askYesNoPrompt(char *line){
	char str[6];
	setPrintColor(TEAL_ON_BLACK);
	printf("%s %s [Y/N] ",QUESTION_PRINTOUT,line);
	resetPrintColor();
	while(strchr(str,'y')==NULL && strchr(str,'n')==NULL){
		scanf("%5s",str);
	}
	if (strchr(str,'y')!=NULL){
		return true;
	} else {
		return false;
	}
}

void printArgumentInformation(ArgumentInfo *argInfo, ArgumentErrorCode *argErrorCodes){
	printf("Argument Information:\n");
	printf("  . First argument:  '%s'\n",argInfo->funcStr);
	printf("  . Second Argument: '%s'\n",argInfo->givenFile);
	printf("  . First Argument Valid:   ");
	printTrueFalseColored(argErrorCodes->firstArgumentIncorrect,"False","True");
	printf("  . Second Argument Valid:  ");
	printTrueFalseColored(argErrorCodes->secondArgumentIncorrect,"False","True");
	printf("  . General Argument Error: ");
	printTrueFalseColored(argErrorCodes->generalArgumentError,"True","False");
	printf("\n");
}

void printFileInformation(CustomFile *fileStruct, CustomFileErrorCode *fileErrorCodes){
	printf("\n'%s%s' File Information:\n",fileStruct->name,fileStruct->extension);
	printf("  . Absolute Path: '%s'\n",fileStruct->absoluteFilePath);
	printf("  . Drive: '%s'\n",fileStruct->drive);
	printf("  . Relative Path: '%s'\n",fileStruct->relativePath);
	printf("  . Name: '%s'\n",fileStruct->name);
	printf("  . Extension: %5s\n",fileStruct->extension);
	printf("  . File Exists:      ");
	printTrueFalseColored(fileErrorCodes->fileDoesNotExist,"False","True");
	printf("  . Write Permission: ");
	printTrueFalseColored(!fileStruct->canWrite,"False","True");
	printf("\n");
}

void printPassedArguments(char *compilerPath, char *args[], int numOfArgs){
	printf("Expanded args passed to program located at: '%s'\n",compilerPath);
	for(int i=0; i<numOfArgs; i++){
		printf("  . Arg[%d]: '%s'\n",i,args[i]);
	}
}

void printCESuccess(int success){
	setPrintColor(YELLOW_ON_BLACK);
	printf("Compile/Execute was: ");
	if (success==0){
		printf("SUCCESSFULL\n");
	} else if (success>0){
		printf("UNSUCCESSFULL\n");
	} else {
		printf("NOT RUN\n");
		printf("  . Please make sure the file you specified exists.\n");
		printf("  . Error code: %d\n",success);
	}
	resetPrintColor();
}

void printElapsedTime(CustomTimer *timer){
	setPrintColor(GOLD_ON_BLACK);
	printf("Duration info:\n");
	printf("  . Total Elapsed Time: %d h :%d m :%d s ,%d milliseconds\n",
		   timer->hours,timer->minutes,timer->seconds,timer->millisecs);
	printf("  . Total Number of Clock Ticks: %d\n",timer->difference);
	resetPrintColor();
}

void printPause(void){
	setPrintColor(LIGHTGREEN_ON_BLACK);
	printf("Press any key to continue...\n");
	resetPrintColor();
	char temp=getch();
}

//Private Members ==============================================================
void setPrintColor(int colorCode){
	switch(colorCode){
		case RED_ON_BLACK: SetConsoleTextAttribute(hConsole, RED_ON_BLACK); break;
		case TEAL_ON_BLACK: SetConsoleTextAttribute(hConsole, TEAL_ON_BLACK); break;
		case PINK_ON_BLACK: SetConsoleTextAttribute(hConsole, PINK_ON_BLACK); break;
		case GOLD_ON_BLACK: SetConsoleTextAttribute(hConsole, GOLD_ON_BLACK); break;
		case YELLOW_ON_BLACK: SetConsoleTextAttribute(hConsole, YELLOW_ON_BLACK); break;
		case LIGHTGREEN_ON_BLACK: SetConsoleTextAttribute(hConsole, LIGHTGREEN_ON_BLACK); break;
		default: SetConsoleTextAttribute(hConsole, WHITE_ON_BLACK); break;
	}
}

void resetPrintColor(void){
	setPrintColor(WHITE_ON_BLACK);
}

void printTrueFalseColored(int trueFalse, char *truePrint, char *falsePrint){
	if (trueFalse==true){
		setPrintColor(RED_ON_BLACK);
	}
	printf("%5s\n",(trueFalse==true)? truePrint: falsePrint);
	resetPrintColor();
}