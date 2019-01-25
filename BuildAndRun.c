//To Compile: gcc -o BuildAndRun.exe BuildAndRun.c Arguments.h Arguments.c TypeDefs.h TypeDefs.c CompileAndExecute.h CompileAndExecute.c BuildFile.h BuildFile.c File.h File.c Timer.h Timer.c PrintOut.h PrintOut.c ErrorCheckAndFix.h ErrorCheckAndFix.c
//To run:     BuildAndRun.exe ce "C:\Users\JackCarmichael\Documents\Programming\HTML\HelloWorld.html"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "TypeDefs.h"
#include "File.h"
#include "Arguments.h"
#include "BuildFile.h"
#include "CompileAndExecute.h"
#include "ErrorCheckAndFix.h"
#include "PrintOut.h"
#define true 1
#define false 0

extern ArgumentInfo argInfo;
extern ArgumentErrorCode argErrorCodes;
extern CompileAndExecuteErrorCodes cAndEErrorCodes;
extern CustomFile givenFile;
extern CustomFileErrorCode givenFileErrorCodes;

void runFunctionality(void);
void compileAndExecute(void);
void openSettings(void);

int main(int argc, char *argv[]){
	initilizePrintOut();
	initilizeArguments(argc,argv);
	printArgumentInformation(&argInfo,&argErrorCodes);
	if (argErrorCodes.generalArgumentError==false){
		runFunctionality();
	}
	if (argErrorCodes.generalArgumentError==true || 
		givenFileErrorCodes.generalFileError==true ||
		cAndEErrorCodes.generalCompileAndExecuteError==true){
		runErrorCheckAndFix();
	}
	printPause();
	resetPrintOut();
}

void runFunctionality(void){
	if(strcmp(argInfo.funcStr,"c ")==0 || strcmp(argInfo.funcStr,"e ")==0 ||
	   strcmp(argInfo.funcStr,"ce")==0){
		compileAndExecute();
	} else if(strcmp(argInfo.funcStr,"s ")==0){
		openSettings();
	} else if(strcmp(argInfo.funcStr,"r ")==0){
		//recoverFiles();
		println("The recovery feature has not been implemented yet",'r');
	}
}

void compileAndExecute(void){
	initilizeFile(&givenFile,argInfo.givenFile,&givenFileErrorCodes);
	initilizeCompileAndExecute();
	printFileInformation(&givenFile,&givenFileErrorCodes);
	if (cAndEErrorCodes.generalCompileAndExecuteError==false &&
		givenFileErrorCodes.generalFileError==false){
		runCompileAndExecute(argInfo.funcStr);
	}
}

void openSettings(void){
	ShellExecute(NULL,NULL,"FileTypes.txt",NULL,NULL,SW_SHOWNORMAL);
}