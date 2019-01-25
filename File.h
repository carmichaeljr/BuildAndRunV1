#ifndef File
#define File

#include "TypeDefs.h"
extern CustomFile givenFile;
extern CustomFileErrorCode givenFileErrorCodes;
extern CustomFile buildFile;
extern CustomFileErrorCode buildFileFileErrorCodes;
void initilizeFile(CustomFile *cFile, char *filePath, CustomFileErrorCode *cFileErrors);
void setFileInformation(CustomFile *cFile, char *absoluteFilePath);
void resetFileInformation(CustomFile *cFile);
char* generateFilePath(char *formatSpec, char *absoluteFilePath);
char* getFileDrive(char *haystack, int maxLength);
char* getFileRelativePath(char *haystack, int maxLength);
char* getFileName(char *haystack, int maxLength);
char* getFileExtension(char *haystack, int maxLength);
void deleteLines(CustomFile *cFile, int *lines, int numOfLinesToDel);

#endif