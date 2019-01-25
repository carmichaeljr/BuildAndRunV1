#ifndef TypeDef
#define TypeDef

#include <time.h>
typedef struct ArrayListInt {
	int *list;
	int numOfItems;
} ArrayListInt;

typedef struct ArgumentInfo {
	char funcStr[4];
	char funcStrPossibilities[6][4];
	char *givenFile;
} ArgumentInfo;

typedef struct FileTypes {
	int canCompile;
	char *compilePath;
	char *executePath;
	char *outputPath;
	char *extensions[7];
	int numOfExtensions;
	int buildFileRequired;
} FileType;

typedef struct CustomFile {
	char *absoluteFilePath;
	char *drive;
	char *extension;
	char *relativePath;
	char *name;
	int canWrite;
} CustomFile;

typedef struct CustomTimer {
	clock_t difference;
	int hours;
	int minutes;
	int seconds;
	int millisecs;
	clock_t start;
	clock_t end;
} CustomTimer;


typedef struct ArgumentErrorCodes {
	int noArgumentsGiven;
	int firstArgumentIncorrect;
	int secondArgumentIncorrect;
	int generalArgumentError;
} ArgumentErrorCode;

typedef struct CustomFileErrorCodes {
	int fileDoesNotExist;
	int generalFileError;
} CustomFileErrorCode;

typedef struct BuildFileErrorCodes {
	int errorPopulatingBuildFile;
	int generalBuildFileError;
} BuildFileErrorCodes;

typedef struct CompileAndExecuteErrorCodes {
	int noFileTypeFile;
	int toManyFileTypes;
	int unrecognizedFileType;
	int toManyProcessArgs;
	int generalCompileAndExecuteError;
} CompileAndExecuteErrorCodes;

int getMin(int first, int second);
void addIntToList(ArrayListInt *arrayList, int num);
void clearStr(char *str, int maxLength);
void copyStrBetween(char *reciever, char *giver, int startIndex, int endIndex);
int getFirstIndexOf(char *str, char firstDelimiter);
int getLastIndexOf(char *str, char lastDelimiter);
char* replaceTokenInStr(char *haystack, char *replacement ,char tokenStart, char tokenEnd);

#endif