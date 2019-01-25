#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "TypeDefs.h"
#define true 1
#define false 0

//Public Members================================================================
//Int methods
int getMin(int first, int second){
	if (first<second){
		return first;
	} else if (second<first){
		return second;
	} else {
		return first;
	}
}

void addIntToList(ArrayListInt *arrayList, int num){
	arrayList->list=(int *)realloc(arrayList->list,(sizeof(int)*(arrayList->numOfItems+1)));
	arrayList->list[arrayList->numOfItems]=num;
	arrayList->numOfItems++;
}

//String methods
void clearStr(char *str, int maxLength){
	for(int i=0; i<maxLength; i++){
		str[i]=' ';
	}
	str[maxLength]='\0';
}

char* replaceTokenInStr(char *haystack, char *replacement ,char tokenStart, char tokenEnd){
	int startIndex=getFirstIndexOf(haystack,'{');
	int endIndex=getLastIndexOf(haystack,'}')+1;
	int returnStrSize=(startIndex+1)+(strlen(haystack)-endIndex)+strlen(replacement);
	char *returnStr=(char *)malloc((returnStrSize+1)*sizeof(char));
	clearStr(returnStr,returnStrSize-1);
	for(int i=0; i<startIndex; i++){
		returnStr[i]=haystack[i];
	}
	for(int i=0; i<strlen(replacement); i++){
		returnStr[i+startIndex]=replacement[i];
	}
	for(int i=0; i<strlen(haystack)-endIndex; i++){
		returnStr[i+strlen(replacement)+startIndex]=haystack[i+endIndex];
	}
	returnStr[returnStrSize]='\0';
	return returnStr;
}

void copyStrBetween(char *reciever, char *giver, int startIndex, int endIndex){
	if (endIndex-startIndex<=strlen(reciever)){
		for(int i=startIndex; i<endIndex; i++){
			reciever[i-startIndex]=giver[i];
		}
		reciever[endIndex-startIndex]='\0';
	}
}

int getFirstIndexOf(char *str, char firstDelimiter){
	char *token=strchr(str,firstDelimiter);
	return (token-str);
}

int getLastIndexOf(char *str, char lastDelimiter){
	char *token=strrchr(str,lastDelimiter);
	return (token-str);
}