#ifndef UTILS_H
#define UTILS_H

#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<ctime>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>

//Constants
#define MAX_STR_DATE 30
#define MAX_STR_FILE_NAME 40
#define MAX_STR_PATH 200
#define N_MONTH 12
#define MAX_NUM_FAKE_FILES 10
#define MAX_LOG_STR 200
#define MAX_NUM_STR 10

//Data generator parameters
#define MAX_NUM_FILE_PER_DIR 10 //Controlls the maximum number of files per server.
#define N_PORT 3000 //Controlls the number of fake ports.
#define MAX_ROW_FILE 100000 //Controls the maximum number of lines per log file.
#define MAX_USERS 100 //Controls the maximum number of synthetic users. It is used to generate the fake user_id.
#define MAX_SERVER 4 //Controlls the maximum number of servers, which are simulated by directories.

//Paths
#define ROOT_PATH "/home/william/Documentos/desafioChaordic" //Root path where the generated data will be placed.
#define REMOTE_BASE_FOLDER "remote" //Folder name where the generated data will be placed inside the ROOT_PATH

int folderExist(char * path);
int removeFolder (char * root, char * dirname);
int removeFolder (char * fullpath);
int createFolder (char * fullpath);
int createFolder (char * root, char * dirname);
DIR * openFolder(char * root, char * foldername, char * fullpath);
DIR * openFolder(char * fullpath);
char * convertToTwoDigitStr (int n, char * str);
char * getStringTime(char * strTime);
int getIdFromLine (char * line);
char * getUserId(char * name, char * id_str);
#endif