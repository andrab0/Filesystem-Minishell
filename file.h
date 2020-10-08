#include <stdlib.h>
#include <stdbool.h>
#ifndef __FILE_H__
#define __FILE_H__

typedef struct Directory {
// The name of the directory
char *name;

// TODO: The list of files of the current directory
struct ListOfFiles *nextFile;

// TODO: The list of directories of the current directory
struct ListOfDir *nextDir;

// The parent directory of the current directory (NULL for the root directory)
struct Directory *parentDir;
} Directory;

// DO NOT MODIFY THIS STRUCTURE
typedef struct File {
// The name of the file
char *name;

// The size of the file
int size; // length of data

// The content of the file
char *data;

// The directory in which the file is located
Directory *dir;
} File;

typedef struct ListOfDir {
struct Directory *newDir;
struct ListOfDir *next;
} ListOfDir;

typedef struct ListOfFiles {
struct File *newFile;
struct ListOfFiles *next;
} ListOfFiles;

void CreateRoot(Directory **root, Directory **current);
void CreateTouch(Directory **current
, ListOfFiles **headFile, char *arr2, char *arr3);
void CreateMkdir(Directory **current
, ListOfDir **headDir, char *arr2);
void PrintFiles(ListOfFiles *cap);
void PrintDirs(ListOfDir *cap);
void CreateCd(Directory **current
, ListOfFiles **headFile, ListOfDir **headDir, char *arr2);
void CreatePwd(Directory *current);
void CreateTree(Directory *current
, ListOfFiles *headFile, ListOfDir *headDir, int value);
void CreateRmdir(ListOfDir **headDir, Directory *current, char *arr2);



#endif /* __FILE_H__ */


