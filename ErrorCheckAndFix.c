// #include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ErrorCheckAndFix.h"
#include "TypeDefs.h"
#include "Arguments.h"
#include "CompileAndExecute.h"
#include "PrintOut.h"
#include "File.h"
#define true 1
#define false 0

extern ArgumentErrorCode argErrorCodes;
extern CompileAndExecuteErrorCodes cAndEErrorCodes;
extern CustomFileErrorCode givenFileErrorCodes;
extern BuildFileErrorCodes buildFileErrorCodes;

void runArgErrorHelp(void);
void printFirstArgumentHelp(void);
void printSecondArgumentHelp(void);
void runGivenFileErrorHelp(void);
void runCAndEErrorHelp(void);
void printRecognizedFileTypes(char *filePath);
void runBuildFileHelp(void);

//Public Members ===============================================================
void runErrorCheckAndFix(void){
	println("Error encountered,running error check...",'r');
	if (argErrorCodes.generalArgumentError==true){
		runArgErrorHelp();
	}
	if (givenFileErrorCodes.generalFileError==true){
		runGivenFileErrorHelp();
	}
	if (cAndEErrorCodes.generalCompileAndExecuteError==true){
		runCAndEErrorHelp();
	}
	if (buildFileErrorCodes.generalBuildFileError==true){
		runBuildFileHelp();
	}
}

//Private Members ==============================================================
void runArgErrorHelp(void){
	if (argErrorCodes.noArgumentsGiven==true){
		println("No arguments were given.",'e');
		char *lines[]={"How to run program:",
					   "BuildAndRun.exe [Run Type] [Absolute File Path]",
					   "BuildAndRun.exe [Run Type]"};
		printList(lines,3);
		printFirstArgumentHelp();
		printSecondArgumentHelp();
	}
	if (argErrorCodes.firstArgumentIncorrect==true){
		println("The first argument that was given is incorrect.",'e');
		printFirstArgumentHelp();
	}
	if (argErrorCodes.secondArgumentIncorrect==true){
		println("The second argument that was given is incorrect.",'e');
		printSecondArgumentHelp();
	}
}

void printFirstArgumentHelp(void){
	char *lines[]={"Possible first arguments:",
				   "'c'		Compiles the program that is specified by the second argument",
				   "'e'		Executes the program that is specified by the second argument",
				   "'ce'	Compiles and executes the program specified by the second argument",
				   "'r'		Runs the restore functionality",
				   "'s'		Opens the settings document for editing."};
	printList(lines,6);
}

void printSecondArgumentHelp(void){
	char *lines[]={"Possible second arguments:",
				   "\"File Path\"	An absolute file path that points to the file to compile/execute",
				   "This must point to a real file, otherwise an error will be encountered",
				   "The second argument is only needed when the first argument is 'c','e', or 'ce'",
				   "If a second argument is given, with a different first argument than listed above, it will be ignored"};
	printList(lines,5);
}

void runGivenFileErrorHelp(void){
	if (givenFileErrorCodes.fileDoesNotExist==true){
		println("The given file to compile/execute does not exist.",'e');
	}
}

void runCAndEErrorHelp(void){
	if (cAndEErrorCodes.noFileTypeFile==true){
		println("Error: 404 ;)",'e');
		char *lines[]={"The text file that contains the compile/execute paths is missing.",
					   "A new one will be created, but all data is lost.",
					   "To add compile/execute paths again, run this program with the 's' option."};
		printList(lines,3);
		FILE *filePointer=fopen("FileTypes.txt","a");
	} else if (cAndEErrorCodes.unrecognizedFileType==true){
		println("Unrecognized file type.",'e');
		char *lines[]={"To add this file type, run this program with the 's' option"};
		printList(lines,1);
		printRecognizedFileTypes("FileTypes.txt");
	} else if (cAndEErrorCodes.toManyProcessArgs==true){
		println("Max number of args reached!",'e');
		println("The maximum number of arguments is 10",'r');
	}
}

//Misplaced responsibility??
void printRecognizedFileTypes(char *filePath){
	int bufferLength=10;
	char buffer[bufferLength];
	FILE *filePointer=fopen(filePath,"r");
	printf("\nRecognized file types are as follows:\n");
	while(fgets(buffer,bufferLength,filePointer)!=NULL){
		if (strstr(buffer,"::")){
			int lastIndex=getLastIndexOf(buffer,':')-1;
			char tempStr[lastIndex+1];
			tempStr[lastIndex]='\0';
			clearStr(tempStr,lastIndex);
			copyStrBetween(tempStr,buffer,0,lastIndex);
			printf("  . *%s\n",tempStr);
		}
	}
	fclose(filePointer);
}

void runBuildFileHelp(void){
	if (buildFileErrorCodes.errorPopulatingBuildFile==true){
		println("There was an error populating the nuild file.",'e');
		printf("  . Try running the program again.\n");
	}
}