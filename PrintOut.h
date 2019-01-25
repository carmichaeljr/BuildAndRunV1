#ifndef PrintOut
#define PrintOut

#include "TypeDefs.h"
void initilizePrintOut(void);
void resetPrintOut(void);
void println(char *line, char precursorType);
void printList(char *lines[], int numOfLines);
void printProcessInformation(char *line, char stage);
int askYesNoPrompt(char *line);
void printArgumentInformation(ArgumentInfo *argInfo, ArgumentErrorCode *argErrorCodes);
void printFileInformation(CustomFile *fileStruct, CustomFileErrorCode *fileErrorCodes);
void printPassedArguments(char *compilerPath, char *args[], int numOfArgs);
void printCESuccess(int success);
void printElapsedTime(CustomTimer *timer);
void printPause(void);

#endif