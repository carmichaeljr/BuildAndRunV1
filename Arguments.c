#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "Arguments.h"
#include "TypeDefs.h"
#define true 1
#define false 0

ArgumentInfo argInfo;
ArgumentErrorCode argErrorCodes;


void initilizeArgInfo(void);
void initilizeArgErrorCodes(void);
void checkFirstArgument(char *firstArg);
int isExecuteFileFuncMode(void);
void checkSecondArgument(char *path);
void setGeneralArgumentError(void);

//Public Members ===============================================================
void initilizeArguments(int argc, char *argv[]){
	initilizeArgInfo();
	initilizeArgErrorCodes();
	if (argc<=1){
		argErrorCodes.noArgumentsGiven=true;
	} else {
		if (argc>=2){
			checkFirstArgument(argv[1]);
		}
		if (argc>=3 && isExecuteFileFuncMode()==true){
			checkSecondArgument(argv[2]);
		}
	}
	setGeneralArgumentError();
}

//Private Members ==============================================================
void initilizeArgInfo(void){
	strcpy(argInfo.funcStr,"  ");
	strcpy(argInfo.funcStrPossibilities[0],"c ");
	strcpy(argInfo.funcStrPossibilities[1],"ce");
	strcpy(argInfo.funcStrPossibilities[2],"e ");
	strcpy(argInfo.funcStrPossibilities[3],"r ");
	strcpy(argInfo.funcStrPossibilities[4],"s ");
	// clearStr(argInfo.givenFile,150);
}

void initilizeArgErrorCodes(void){
	argErrorCodes.noArgumentsGiven=false;
	argErrorCodes.firstArgumentIncorrect=false;
	argErrorCodes.secondArgumentIncorrect=false;
	argErrorCodes.generalArgumentError=false;
}

void checkFirstArgument(char *firstArg){
	argErrorCodes.firstArgumentIncorrect=true;
	if (strlen(firstArg)<=2){
		strncpy(argInfo.funcStr,firstArg,2);
		if (strlen(argInfo.funcStr)<=1){
			strcat(argInfo.funcStr," ");
		}
		strcat(argInfo.funcStr,"\0");
		// printf("Copied first arg: '%s'\n",argInfo.funcStr);
		for(int i=0; i<5; i++){
			// printf("Proposed arg: '%s' Current arg: '%s'\n",argInfo.funcStrPossibilities[i],argInfo.funcStr);
			if (strcmp(argInfo.funcStr,argInfo.funcStrPossibilities[i])==0){
				argErrorCodes.firstArgumentIncorrect=false;
				// printf(" . First arg recognized as: '%s'\n",argInfo.funcStrPossibilities[i]);
			}
		}
	}
}

int isExecuteFileFuncMode(void){
	if(strcmp(argInfo.funcStr,"c ") || strcmp(argInfo.funcStr,"ce") ||
		strcmp(argInfo.funcStr,"e ")){
		return true;
	}
	return false;
}

void checkSecondArgument(char *path){
	argErrorCodes.secondArgumentIncorrect=true;
	if (access(path,R_OK)==0){
		argInfo.givenFile=(char *)malloc(sizeof(char)*256);
		strncpy(argInfo.givenFile,path,255);
		strcat(argInfo.givenFile,"\0");
		argErrorCodes.secondArgumentIncorrect=false;
	}
}

void setGeneralArgumentError(void){
	if (argErrorCodes.noArgumentsGiven==true || argErrorCodes.firstArgumentIncorrect==true || 
		argErrorCodes.secondArgumentIncorrect==true){
		argErrorCodes.generalArgumentError=true;
	}
}