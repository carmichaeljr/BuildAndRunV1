#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <process.h>
#include <windows.h>
#include "CompileAndExecute.h"
#include "BuildFile.h"
#include "PrintOut.h"
#include "TypeDefs.h"
#include "File.h"
#include "Timer.h"
#define true 1
#define false 0
#define MAX_NUM_OF_EXTENSIONS 7
#define MAX_NUM_OF_FILE_TYPES 20
#define MAX_NUM_OF_ARGS 40

extern CustomTimer ceTimer;
extern CustomFile givenFile;
extern CustomFile buildFile;
int numOfFileTypes=0;
FileType *allFileTypes[MAX_NUM_OF_FILE_TYPES];
CompileAndExecuteErrorCodes cAndEErrorCodes;

void initilizeCompileAndExecuteErrorCodes(void);
	void findNumberOfFileTypes(char *filePath);
	void createBlankFileTypeStructs(FileType *allFileTypes[]);
	void populateFileTypeStructs(char *filePath);
	void setFileTypeExtension(char *buffer, FileType *fileType);
	void setFileTypePath(char *buffer, char *path);
	void setFileTypeBuildPreference(char *buffer, FileType *fileType);
void setGeneralCompileAndExecuteError(void);
int getCompileExecuteStatus(char *executionMethod);
void compile(FileType *currentFile);
void execute(FileType *currentFile);
	FileType* getFileType(void);
	char* getCompilerPath(char *token);
	char* getArgument(char *haystack);
	char* expandArgument(char *arg, char *outputPath);
	int populateArguments(char **compilerPath, char *args[], char *haystack, char *outputPath);
	int addBuildFilePathsToArgs(char *args[], int numOfArgs);
	int padArgsWithNull(char *args[], int argCntr);
	void spawnProgram(char *compilerPath, char *args[]);

//Public Members ===============================================================
void initilizeCompileAndExecute(void){
	initilizeCompileAndExecuteErrorCodes();
	if (access("FileTypes.txt",R_OK|W_OK)!=-1){
		findNumberOfFileTypes("FileTypes.txt");
		createBlankFileTypeStructs(allFileTypes);
		populateFileTypeStructs("FileTypes.txt");
	} else {
		cAndEErrorCodes.noFileTypeFile=true;
	}
	setGeneralCompileAndExecuteError();
}

void runCompileAndExecute(char *executionMethod){
	int ceStatus=getCompileExecuteStatus(executionMethod);
	FileType *currentFileType=getFileType();
	if (cAndEErrorCodes.unrecognizedFileType==false){
		if (currentFileType->canCompile==true && (ceStatus==1 || ceStatus==3)){
			compile(currentFileType);
		} else if (currentFileType->canCompile==false && ceStatus==1){
			println("This type of file cannot be compiled.",'e');
			ceStatus+=askYesNoPrompt("Do you wish to execute?")*2;
		}
		if (ceStatus==2 || ceStatus==3){
			execute(currentFileType);
		}
	}
	setGeneralCompileAndExecuteError();
}

//Private Members ==============================================================
void initilizeCompileAndExecuteErrorCodes(void){
	cAndEErrorCodes.noFileTypeFile=false;
	cAndEErrorCodes.toManyFileTypes=false;
	cAndEErrorCodes.unrecognizedFileType=false;
	cAndEErrorCodes.toManyProcessArgs=false;
	cAndEErrorCodes.generalCompileAndExecuteError=false;
}

void findNumberOfFileTypes(char *filePath){
	int bufferLength=10;
	char buffer[bufferLength];
	FILE *filePointer=fopen(filePath,"r");
	while(fgets(buffer,bufferLength,filePointer)!=NULL){
		if (strstr(buffer,"::") && numOfFileTypes<MAX_NUM_OF_FILE_TYPES){
			numOfFileTypes++;
		} else if (numOfFileTypes>=MAX_NUM_OF_FILE_TYPES){
			cAndEErrorCodes.toManyFileTypes=true;
		}
	}
	fclose(filePointer);
}

void createBlankFileTypeStructs(FileType *allFileTypes[]){
	for(int i=0; i<numOfFileTypes; i++){
		allFileTypes[i]=(FileType*)malloc(sizeof(FileType));
		allFileTypes[i]->canCompile=false;
		allFileTypes[i]->buildFileRequired=false;
		allFileTypes[i]->numOfExtensions=0;
	}
}

void populateFileTypeStructs(char *filePath){
	int curFileTypeCntr=-1;
	int bufferLength=256;
	char buffer[bufferLength];
	FILE *filePointer=fopen(filePath,"r");
	while(fgets(buffer,bufferLength,filePointer)!=NULL && curFileTypeCntr<numOfFileTypes){
		if(strstr(buffer,"::")){
			curFileTypeCntr++;
			// printf("FOUND FILE TYPE\n");
			setFileTypeExtension(buffer,allFileTypes[curFileTypeCntr]);
			// for(int i=0; i<allFileTypes[curFileTypeCntr]->numOfExtensions; i++){
				// printf("Extension %d: '%s'\n",i,allFileTypes[curFileTypeCntr]->extensions[i]);
			// }
		} else if((buffer[0]=='C' || buffer[0]=='c') && strstr(buffer,"null")==NULL){
			allFileTypes[curFileTypeCntr]->compilePath=(char *)malloc(sizeof(char)*(strlen(buffer)+1));
			clearStr(allFileTypes[curFileTypeCntr]->compilePath,strlen(buffer));
			setFileTypePath(buffer,allFileTypes[curFileTypeCntr]->compilePath);
			allFileTypes[curFileTypeCntr]->canCompile=true;
			// printf("FileType: %d  CanCompile: %i  Compile Path: %s\n",curFileTypeCntr,
																	// allFileTypes[curFileTypeCntr]->canCompile,
																	// allFileTypes[curFileTypeCntr]->compilePath);
		} else if((buffer[0]=='E' || buffer[0]=='e') && strstr(buffer,"null")==NULL){
			allFileTypes[curFileTypeCntr]->executePath=(char *)malloc(sizeof(char)*(strlen(buffer)+1));
			clearStr(allFileTypes[curFileTypeCntr]->executePath,strlen(buffer));
			setFileTypePath(buffer,allFileTypes[curFileTypeCntr]->executePath);
			// printf("FileType: %d  CanCompile: %i  Execute Path: %s\n",curFileTypeCntr,
																	// allFileTypes[curFileTypeCntr]->canCompile,
																	// allFileTypes[curFileTypeCntr]->executePath);
		} else if((buffer[0]=='O' || buffer[0]=='o') && strstr(buffer,"null")==NULL){
			allFileTypes[curFileTypeCntr]->outputPath=(char *)malloc(sizeof(char)*(strlen(buffer)+1));
			clearStr(allFileTypes[curFileTypeCntr]->outputPath,strlen(buffer));
			setFileTypePath(buffer,allFileTypes[curFileTypeCntr]->outputPath);
			// printf("FileType: %d  CanCompile: %i  Output Path: %s\n",curFileTypeCntr,
																	// allFileTypes[curFileTypeCntr]->canCompile,
																	// allFileTypes[curFileTypeCntr]->outputPath);
		} else if((buffer[0]=='B' || buffer[0]=='b')){
			setFileTypeBuildPreference(buffer,allFileTypes[curFileTypeCntr]);
			// printf("FileType: %d  BuildFileRequired: %s\n",curFileTypeCntr,
															// (allFileTypes[curFileTypeCntr]->buildFileRequired==true)? "True": "False");
		}
	}
}

void setFileTypeExtension(char *buffer, FileType *fileType){
	char *bufferCpy=(char *)malloc(sizeof(char)*(strlen(buffer)+1));
	strcpy(bufferCpy,buffer);
	bufferCpy[getFirstIndexOf(bufferCpy,':')]='\0';
	char *token=strtok(bufferCpy,"|");
	while (token!=NULL && fileType->numOfExtensions<MAX_NUM_OF_EXTENSIONS){
		fileType->extensions[fileType->numOfExtensions]=(char *)malloc(sizeof(char)*(strlen(token)+1));
		strncpy(fileType->extensions[fileType->numOfExtensions],token,5);
		// printf("Ext: '%s' Num: %d\n",fileType->extensions[fileType->numOfExtensions],fileType->numOfExtensions);
		fileType->numOfExtensions++;
		token=strtok(NULL,"|");
	}
	free(bufferCpy);
}

void setFileTypePath(char *buffer, char *path){
	int firstIndex=getFirstIndexOf(buffer,':')+1;
	int lastIndex=getMin(strlen(path),strlen(buffer));
	copyStrBetween(path,buffer,firstIndex,lastIndex);
	path[strcspn(path,"\r\n")]='\0';
}

void setFileTypeBuildPreference(char *buffer, FileType *fileType){
	for(int i=0; i<strlen(buffer); i++){
		buffer[i]=tolower(buffer[i]);
	}
	if (strstr(buffer,"true")!=NULL){
		fileType->buildFileRequired=true;
	}
}

void setGeneralCompileAndExecuteError(void){
	if (cAndEErrorCodes.noFileTypeFile==true || cAndEErrorCodes.toManyFileTypes==true ||
		cAndEErrorCodes.unrecognizedFileType==true || cAndEErrorCodes.toManyProcessArgs==true){
		cAndEErrorCodes.generalCompileAndExecuteError=true;
	}
}

//Return Values: 0: Error, 1: Compile, 2: Execute, 3: Compile and Execute
int getCompileExecuteStatus(char *executionMethod){
	int returnValue=0;
	if (strchr(executionMethod,'c')!=NULL){
		returnValue++;
	}
	if (strchr(executionMethod,'e')!=NULL){
		returnValue+=2;
	}
	return returnValue;
}

FileType* getFileType(void){
	int fileIndexValue=0;
	cAndEErrorCodes.unrecognizedFileType=true;
	for(int i=0; i<numOfFileTypes; i++){
		for(int j=0; j<allFileTypes[i]->numOfExtensions; j++){
			if (strcmp(givenFile.extension,allFileTypes[i]->extensions[j])==0){
				cAndEErrorCodes.unrecognizedFileType=false;
				fileIndexValue=i;
			}
		}
	}
	return allFileTypes[fileIndexValue];
}

void compile(FileType *currentFile){
	int numOfArgs=0;
	char *allArguments[MAX_NUM_OF_ARGS+1];
	char *compilerPath=NULL;
	println("Preparing arguments...",'r');
	numOfArgs=populateArguments(&compilerPath,allArguments,
								currentFile->compilePath,currentFile->outputPath);
	if (currentFile->buildFileRequired==true){
		initilizeBuildFile(generateFilePath("{DR}",givenFile.absoluteFilePath));
		generateBuildFile(currentFile->extensions,currentFile->numOfExtensions);
		numOfArgs=addBuildFilePathsToArgs(allArguments,numOfArgs);
	}
	numOfArgs=padArgsWithNull(allArguments,numOfArgs);
	printPassedArguments(compilerPath,allArguments,numOfArgs);
	printProcessInformation("Compile process started",'s');
	spawnProgram(compilerPath,allArguments);
	printProcessInformation("Compile process ended",'e');
	for(int i=0; i<numOfArgs; i++){
		free(allArguments[i]);
	}
	free(compilerPath);
}

void execute(FileType *currentFile){
	int numOfArgs=0;
	char *allArguments[MAX_NUM_OF_ARGS+1];
	char *compilerPath=NULL;
	println("Preparing arguments...",'r');
	numOfArgs=populateArguments(&compilerPath,allArguments,
								currentFile->executePath,currentFile->outputPath);
	numOfArgs=padArgsWithNull(allArguments,numOfArgs);
	printPassedArguments(compilerPath,allArguments,numOfArgs);
	printProcessInformation("Execution process started",'s');
	spawnProgram(compilerPath,allArguments);
	printProcessInformation("Execution process ended",'e');
	for(int i=0; i<numOfArgs; i++){
		free(allArguments[i]);
	}
	free(compilerPath);
}

int populateArguments(char **compilerPath, char *args[], char *haystack, char *outputPath){
	int argCntr=0;
	char *tempHaystack=(char *)malloc(sizeof(char)*(strlen(haystack)+1));
	strcpy(tempHaystack,haystack);
	char *token=strtok(tempHaystack,">");
	while (token!=NULL && argCntr<MAX_NUM_OF_ARGS){
		// printf("token: '%s'\n",token);
		if (strstr(token,"Compiler=")!=NULL){
			*compilerPath=getCompilerPath(token);
			args[argCntr]=generateFilePath("{NE}",*compilerPath);
			// printf("Compiler path: '%s'\n",*compilerPath);
			// printf("CompilerNameAndExt: '%s'\n",args[argCntr]);
			argCntr++;
		} else if (strstr(token,"Arg=")!=NULL){
			char *newToken=getArgument(token);
			// printf("newToken: '%s'\n",newToken);
			char *expandedToken=expandArgument(newToken,outputPath);
			// printf("expandedToken: '%s'\n",expandedToken);
			if (expandedToken==NULL){
				args[argCntr]=newToken;
				free(expandedToken);
			} else {
				args[argCntr]=expandedToken;
				free(newToken);
			}
			// printf("arg[%d]: '%s'\n",argCntr,args[argCntr]);
			argCntr++;
		}
		token=strtok(NULL,">");
	}
	return argCntr;
}

char* getCompilerPath(char *haystack){
	int firstIndex=getFirstIndexOf(haystack,'=')+1;
	int lastIndex=strlen(haystack);
	char *tempCompilerPath;//TODO - Why cant these two lines be combined? Creates free error when they are.
	tempCompilerPath=(char *)malloc(sizeof(char)*(lastIndex-firstIndex+1));
	clearStr(tempCompilerPath,lastIndex-firstIndex);
	copyStrBetween(tempCompilerPath,haystack,firstIndex,lastIndex);
	// printf("Compiler path: '%s'\n",tempCompilerPath);
	return tempCompilerPath;
}

char* getArgument(char *haystack){
	int firstIndex=getFirstIndexOf(haystack,'=')+1;
	int lastIndex=strlen(haystack);
	char *tempArg;
	tempArg=(char *)malloc(sizeof(char)*(lastIndex-firstIndex+1));
	clearStr(tempArg,lastIndex-firstIndex);
	copyStrBetween(tempArg,haystack,firstIndex,lastIndex);
	// printf("arg: '%s'\n",tempArg);
	return tempArg;
}

char *expandArgument(char *arg, char *outputPath){
	char *returnStr;
	if (strstr(arg,"{FP")!=NULL){
		char *filePath=generateFilePath(arg,givenFile.absoluteFilePath);
		returnStr=replaceTokenInStr(arg,filePath,'{','}');
	} else if (strstr(arg,"{OP}")!=NULL){
		returnStr=replaceTokenInStr(arg,outputPath,'{','}');
	} else {
		returnStr=NULL;
	}
	// printf("ReturnStr: '%s'\n",returnStr);
	return returnStr;
}

int addBuildFilePathsToArgs(char *args[], int argCntr){
	int bufferLength=256;
	char buffer[bufferLength];
	FILE *filePntr=fopen(buildFile.absoluteFilePath,"r");
	while (fgets(buffer,bufferLength,filePntr)!=NULL && argCntr<MAX_NUM_OF_ARGS){
		if (strstr(buffer,"::")==NULL && strstr(buffer,givenFile.absoluteFilePath)==NULL){
			buffer[getLastIndexOf(buffer,'\n')]='\0';
			args[argCntr]=(char *)malloc(sizeof(char)*(strlen(buffer)+1));
			strcpy(args[argCntr],buffer);
			argCntr++;
		}
	}
	fclose(filePntr);
	return argCntr;
}

int padArgsWithNull(char *args[], int argCntr){
	args[argCntr]=malloc(sizeof(NULL));
	args[argCntr]=NULL;
	return (argCntr+1);
}

void spawnProgram(char *compilerPath, char *args[]){
	int spawnRv=-1;
	setStartTime(&ceTimer);
	if (compilerPath!=NULL){
		spawnRv=spawnv(P_WAIT,compilerPath,(const char *const *)args);
	} else if (strcmp(givenFile.extension,".html")==0){
		spawnRv=(int)ShellExecute(NULL,"open",args[0],NULL,NULL,SW_SHOWNORMAL);
		if (spawnRv>=32) spawnRv==0;
		else spawnRv=-1;
	} else {
		spawnRv=system(args[0]);
	}
	setEndTime(&ceTimer);
	calculateTimerDifferences(&ceTimer);
	printElapsedTime(&ceTimer);
	printCESuccess(spawnRv);
}
