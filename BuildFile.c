#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include "BuildFile.h"
#include "TypeDefs.h"
#include "File.h"
#include "PrintOut.h"
#define true 1
#define false 0

extern CustomFile buildFile;
extern CustomFileErrorCode buildFileFileErrorCodes;
ArrayListInt linesToDelete;
BuildFileErrorCodes buildFileErrorCodes;

void createBuildFile(char *absPath);
void clearBuildFile(void);
	void setCompSectionDeleteLines(void);
	void setUserSectionDeleteLines(void);
void populateBuildFile(char *absDirPath, char *extensions[], int numOfExtensions);
	int isSubDirectory(char *path);
	void appendFilePath(char *absPath);
void setGeneralBuildFileError(void);

//Public Members ===============================================================
void initilizeBuildFile(char *relativePath){
	char *absolutePath=(char *)malloc(sizeof(char)*(strlen(relativePath)+14));
	strcpy(absolutePath,relativePath);
	strcat(absolutePath,"BuildFile.txt");
	if (access(absolutePath,R_OK)!=0){
		createBuildFile(absolutePath);
	}
	initilizeFile(&buildFile,absolutePath,&buildFileFileErrorCodes);
}

void generateBuildFile(char *extensions[], int numOfExtensions){
	println("Generating build file...",'r');
	char *tempDir=generateFilePath("{DR}",buildFile.absoluteFilePath);
	clearBuildFile();
	populateBuildFile(tempDir,extensions,numOfExtensions);
	free(tempDir);
}

//Private Members ==============================================================
void createBuildFile(char *absPath){
	FILE *filePntr=fopen(absPath,"w");
	fprintf(filePntr,"::UserDefined\n");
	fprintf(filePntr,"\n");
	fprintf(filePntr,"::CompDefined\n");
	fclose(filePntr);
}

void clearBuildFile(void){
	setCompSectionDeleteLines();
	setUserSectionDeleteLines();
	deleteLines(&buildFile,linesToDelete.list,linesToDelete.numOfItems);
	setGeneralBuildFileError();
}

void setCompSectionDeleteLines(void){
	int curLine=0;
	int foundCompSection=false;
	int bufferLength=256;
	char buffer[bufferLength];
	FILE *filePntr=fopen(buildFile.absoluteFilePath,"r");
	while(fgets(buffer,bufferLength,filePntr)!=NULL){
		if (foundCompSection==true){
			addIntToList(&linesToDelete,curLine);
		} else if (strstr(buffer,"::CompDefined")!=NULL){
			foundCompSection=true;
		}
		curLine++;
	}
	fclose(filePntr);
}

void setUserSectionDeleteLines(void){
	int curLine=0;
	int stop=false;
	int bufferLength=256;
	char buffer[bufferLength];
	FILE *filePntr=fopen(buildFile.absoluteFilePath,"r");
	while (fgets(buffer,bufferLength,filePntr)!=NULL && stop==false){
		buffer[getLastIndexOf(buffer,'\n')]='\0';
		// printf("userDefinedLine: '%s'\n",buffer);
		if (strstr(buffer,"::")==NULL){
			if (access(buffer,R_OK)!=0 && stop==false){
				printf("  . The line '%s' added by the user is invalid and will be deleted.\n",buffer);
				// printf("Found line to delete: %d\n",curLine);
				addIntToList(&linesToDelete,curLine);
			}
		} else if (strstr(buffer,"::CompDefined")!=NULL){
			stop=true;
		}
		curLine++;
	}
	fclose(filePntr);
}

void populateBuildFile(char *absDirPath, char *extensions[], int numOfExtensions){
	DIR *dir;
	struct dirent *entry;
	if ((dir=opendir(absDirPath))!=NULL){
		while ((entry=readdir(dir))!=NULL){
			// printf("'%s'\n",entry->d_name);
			char *tempAbsPath=(char *)malloc(sizeof(char)*(strlen(absDirPath)+strlen(entry->d_name)+1));
			sprintf(tempAbsPath,"%s%s",absDirPath,entry->d_name);
			for(int i=0; i<numOfExtensions; i++){
				if (strstr(entry->d_name,extensions[i])!=NULL){
					appendFilePath(tempAbsPath);
				}
			}
			if (isSubDirectory(tempAbsPath)){
				// printf("Directory found. '%s' Should follow...\n",tempAbsPath);
				tempAbsPath=(char *)realloc(tempAbsPath,sizeof(char)*(strlen(tempAbsPath)+2));
				strcat(tempAbsPath,"\\");
				populateBuildFile(tempAbsPath,extensions,numOfExtensions);
			}
			free(tempAbsPath);
		}
		closedir(dir);
	} else {
		buildFileErrorCodes.errorPopulatingBuildFile=true;
	}
}

int isSubDirectory(char *path){
	int returnValue=false;
	struct stat statBuffer;
	if (getLastIndexOf(path,'.')!=strlen(path)-1){
		if (stat(path,&statBuffer)!=0){
			returnValue=false;
		}
		returnValue=S_ISDIR(statBuffer.st_mode);
	}
	return returnValue;
}

void appendFilePath(char *absPath){
	FILE *filePntr=fopen(buildFile.absoluteFilePath,"a");
	if (filePntr!=NULL){
		fprintf(filePntr,"\"%s\"\n",absPath);
		fclose(filePntr);
	}
}

void setGeneralBuildFileError(void){
	if (buildFileErrorCodes.errorPopulatingBuildFile==true){
		buildFileErrorCodes.generalBuildFileError=true;
	}
}