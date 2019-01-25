#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "File.h"
#include "TypeDefs.h"
#define true 1
#define false 0

CustomFile givenFile;
CustomFileErrorCode givenFileErrorCodes;
CustomFile buildFile;
CustomFileErrorCode buildFileFileErrorCodes;

void initilizeGivenFileStruct(CustomFile *customFile);
	void setFileWritePermission(CustomFile *customFile);
	void setGeneralFileError(CustomFileErrorCode *cFileErrors);

//Public Members ===============================================================
void initilizeFile(CustomFile *cFile, char *filePath, CustomFileErrorCode *cFileErrors){
	initilizeGivenFileStruct(cFile);
	if (access(filePath,R_OK)==0){
		setFileInformation(cFile,filePath);
	} else {
		cFileErrors->fileDoesNotExist=true;
	}
	setGeneralFileError(cFileErrors);
}

char* generateFilePath(char *formatSpec, char *absoluteFilePath){
	// printf("Strlen ABSOLUTEFILEPATH: %d\n",strlen(absoluteFilePath));
	char *formatedFilePath=(char *)malloc(sizeof(char)*(strlen(absoluteFilePath)+1));
	int firstIndex=getFirstIndexOf(formatSpec,'{');
	int lastIndex=getLastIndexOf(formatSpec,'}');
	// printf("firstIndex: %d  lastIndex: %d\n",firstIndex,lastIndex);
	clearStr(formatedFilePath,strlen(absoluteFilePath));
	// printf("formatedFilePath: '%s'\n",formatedFilePath);
	formatedFilePath[0]='\0';
	for(int i=firstIndex; i<lastIndex; i++){
		if (formatSpec[i]=='D' || formatSpec[i]=='d'){
			// printf("Found drive...\n");
			char *tempStr=getFileDrive(absoluteFilePath,strlen(absoluteFilePath));
			// printf("Drive: '%s'\n",tempStr);
			strcat(formatedFilePath,tempStr);
			// printf("new filePath: '%s'\n",formatedFilePath);
			free(tempStr);
			// printf("Freed tempStr\n");
		} else if (formatSpec[i]=='R' || formatSpec[i]=='r'){
			// printf("Found relative path...\n");
			char *tempStr=getFileRelativePath(absoluteFilePath,strlen(absoluteFilePath));
			// printf("Relative path: '%s'\n",tempStr);
			strcat(formatedFilePath,tempStr);
			// printf("new FilePath: '%s'\n",formatedFilePath);
			free(tempStr);
			// printf("Freed tempStr\n");
		} else if (formatSpec[i]=='N' || formatSpec[i]=='n'){
			// printf("Found name...\n");
			char *tempStr=getFileName(absoluteFilePath,strlen(absoluteFilePath));
			// printf("Name: '%s'\n",tempStr);
			strcat(formatedFilePath,tempStr);
			// printf("New filePath: '%s'\n",formatedFilePath);
			free(tempStr);///TODO - HANG MYSELF.
			// printf("Freed tempStr\n");
		} else if (formatSpec[i]=='E' || formatSpec[i]=='e'){
			// printf("Found extension...\n");
			char *tempStr=getFileExtension(absoluteFilePath,strlen(absoluteFilePath));
			// printf("extension: '%s'\n",tempStr);
			strcat(formatedFilePath,tempStr);
			free(tempStr);
			// printf("Freed tempStr\n");
		} else if (formatSpec[i]=='A' || formatSpec[i]=='a'){
			formatedFilePath[0]='\0';
			strcat(formatedFilePath,absoluteFilePath);
		}
	}
	return formatedFilePath;
}

void setFileInformation(CustomFile *cFile, char *filePath){
	cFile->absoluteFilePath=(char *)malloc(sizeof(char)*(strlen(filePath)+1));
	strcpy(cFile->absoluteFilePath,filePath);
	cFile->drive=getFileDrive(cFile->absoluteFilePath,2);
	cFile->relativePath=getFileRelativePath(cFile->absoluteFilePath,150);
	cFile->name=getFileName(cFile->absoluteFilePath,50);
	cFile->extension=getFileExtension(cFile->absoluteFilePath,5);
	setFileWritePermission(cFile);
}

void resetFileInformation(CustomFile *cFile){
	initilizeGivenFileStruct(cFile);
}

char* getFileDrive(char *haystack, int maxLength){
	int firstIndex=0;
	int lastIndex=getFirstIndexOf(haystack,'\\');
	char *str=(char *)malloc(sizeof(char)*(lastIndex-firstIndex+1));
	clearStr(str,lastIndex-firstIndex);
	if (lastIndex-firstIndex<=maxLength){
		copyStrBetween(str,haystack,firstIndex,lastIndex);
	}
	return str;
}

char* getFileRelativePath(char *haystack, int maxLength){
	int firstIndex=getFirstIndexOf(haystack,'\\');
	int lastIndex=getLastIndexOf(haystack,'\\')+1;
	char *str=(char *)malloc(sizeof(char)*(lastIndex-firstIndex+1));
	clearStr(str,lastIndex-firstIndex);
	if (lastIndex-firstIndex<=maxLength){
		copyStrBetween(str,haystack,firstIndex,lastIndex);	
	}
	return str;
}

char* getFileName(char *haystack, int maxLength){
	int firstIndex=getLastIndexOf(haystack,'\\')+1;
	int lastIndex=getLastIndexOf(haystack,'.');
	char *str=(char *)malloc(sizeof(char)*(lastIndex-firstIndex+1));
	clearStr(str,lastIndex-firstIndex);
	if (lastIndex-firstIndex<=maxLength){
		copyStrBetween(str,haystack,firstIndex,lastIndex);
	}
	return str;
}

char* getFileExtension(char *haystack, int maxLength){
	int firstIndex=getLastIndexOf(haystack,'.');
	int lastIndex=strlen(haystack);
	char *str=(char *)malloc(sizeof(char)*(lastIndex-firstIndex+1));
	clearStr(str,lastIndex-firstIndex);
	if (lastIndex-firstIndex<=maxLength){
		copyStrBetween(str,haystack,firstIndex,lastIndex);
	}
	return str;
}

//TODO - SPLIT UP. Somehow...
void deleteLines(CustomFile *cFile, int *lines, int numOfLinesToDel){
	int addLine=true;
	int lineCntr=0;
	int bufferLength=256;
	char buffer[bufferLength];
	char *relativePath=generateFilePath("{DR}",cFile->absoluteFilePath);
	char *newFileDir=(char *)malloc(sizeof(char)*(strlen(relativePath)+12));
	strcpy(newFileDir,relativePath);
	strcat(newFileDir,"NewFile.txt");
	FILE *oldFilePntr=fopen(cFile->absoluteFilePath,"r");
	FILE *newFilePntr=fopen(newFileDir,"w");
	if (oldFilePntr!=NULL && newFilePntr!=NULL){
		while(fgets(buffer,bufferLength,oldFilePntr)!=NULL){
			for(int i=0; i<numOfLinesToDel; i++){
				if (lineCntr==lines[i]){
					addLine=false;
				}
			}
			if (addLine==true){
				fprintf(newFilePntr,"%s",buffer);
			}
			addLine=true;
			lineCntr++;
		}
		fclose(oldFilePntr);
		fclose(newFilePntr);
		remove(cFile->absoluteFilePath);
		rename(newFileDir,cFile->absoluteFilePath);
	}
	free(relativePath);
	free(newFileDir);
}

//Private Members ==============================================================
void initilizeGivenFileStruct(CustomFile *customFile){
	customFile->canWrite=false;
}

void setFileWritePermission(CustomFile *customFile){
	if (access(customFile->absoluteFilePath,W_OK)==0){
		customFile->canWrite=true;
	}
}

void setGeneralFileError(CustomFileErrorCode *cFileErrors){
	if (cFileErrors->fileDoesNotExist==true){
		cFileErrors->generalFileError=true;
	}
}