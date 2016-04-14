#include "utils.h"

//For directory checking
struct stat st = {0};

/*
 * Check whether a folder existis.
 * 
 * Input:
 * -> path: Full path to the folder.
 */
int folderExist(char * path) {
	if (stat (path, &st) == 0)
		return 1;
	return 0;
}

/*
 * Remove the specified folder.
 * 
 * Input:
 * -> root: Full path to the folder.
 * -> dirname: Folder's name.
 */
int removeFolder (char * root, char * dirname) {
	char path[MAX_STR_PATH];
	path[0] = '\0';

	strcat(path, root);
	strcat(path, "/");
	strcat(path, dirname);

	char cmd[MAX_STR_PATH];
	cmd[0] = '\0';

	sprintf(cmd, "rm -rf %s", path);
	
	if (folderExist(path)) {
		printf("Running: %s\n", cmd);
		system(cmd);

		return 1;
	}
	else
		printf("Folder does not exist!\n");

	return -1;
}

/*
 * Remove the specified folder.
 * 
 * Input:
 * -> fullpath: Full path to the folder.
 */
int removeFolder (char * fullpath) {
	char cmd[MAX_STR_PATH];
	cmd[0] = '\0';

	sprintf(cmd, "rm -rf %s", fullpath);	

	if (folderExist(fullpath)) {		
		printf("Running: %s\n", cmd);
		system(cmd);
		return 1;
	}
	else
		printf("Folder does not exist!\n");

	return -1;
}

/*
 * Create the specified folder.
 * 
 * Input:
 * -> fullpath: Full path to the folder.
 */
int createFolder (char * fullpath) {

	if (!folderExist(fullpath)) {
		mkdir(fullpath, 0777);
		printf("Creating directory %s\n", fullpath);

		return 1;
	}

	return 0;
}

/*
 * Create the specified folder.
 * 
 * Input:
 * -> root: Full path to the folder.
 * -> dirname: Folder's name.
 */
int createFolder (char * root, char * dirname) {
	char path[MAX_STR_PATH];
	path[0] = '\0';

	strcat(path, root);
	strcat(path, "/");
	strcat(path, dirname);
	
	if (!folderExist(path)) {
		mkdir(path, 0777);
		printf("Creating directory %s in the path %s.\n", dirname, path);

		return 1;
	}

	return 0;
}

/*
 * Open the specified folder.
 * 
 * Input:
 * -> root: Path to the folder.
 * -> foldername: Folder's name.
 * -> fullpath: Full path to the folder.
 */
DIR * openFolder(char * root, char * foldername, char * fullpath) {
	char path_local[MAX_STR_PATH];
	sprintf(path_local, "%s/%s", root, foldername);

	if (fullpath != NULL) {
		fullpath[0] = '\0';
		strcpy(fullpath, path_local);
	}

	//Open the main directory.
	DIR *mainDir = opendir(path_local);

	return mainDir;
}

/*
 * Open the specified folder.
 * 
 * Input:
 * -> fullpath: Full path to the folder.
 */
DIR * openFolder(char * fullpath) {
	//Open the main directory.
	DIR *mainDir = opendir(fullpath);

	return mainDir;
}

/*
 * Convert an integer into a string with two digits.

 * Input:
 * -> n: Integer to be converted.
 * -> str: Converted int into string.
 */
char * convertToTwoDigitStr (int n, char * str) {
	if (str != NULL) {
		str[0] = '\0';

		if (n < 10)
			sprintf(str, "0%d", n);
		else
			sprintf(str, "%d", n);
	}

	return str;
}

/*
 * Get a string with a time value.
 * 
 * Input:
 * -> strTime: Time to be returned.
 */
char * getStringTime(char * strTime) {
	time_t rawtime;
	struct tm * ptm;
	
	time(&rawtime);

	ptm = localtime(&rawtime);

	const char * months[N_MONTH] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	
	if (strTime != NULL) {
		strTime[0] = '\0';

		char twoDigitStrDay[2], twoDigitStrHour[2], twoDigitStrMin[2], twoDigitStrSec[2];

		sprintf(strTime, "%s/%s/%4d:%s:%s:%s", convertToTwoDigitStr(ptm->tm_mday, twoDigitStrDay), 
			                                   months[ptm->tm_mon], 
			                                   ptm->tm_year + 1900, 
			                                   convertToTwoDigitStr(ptm->tm_hour, twoDigitStrHour), 
			                                   convertToTwoDigitStr(ptm->tm_min, twoDigitStrMin), 
			                                   convertToTwoDigitStr(ptm->tm_sec, twoDigitStrSec));

		//printf("%s\n", strTime);
	}

	return strTime;
}

/*
 * Parse the string and return the user id.
 *
 * Input:
 * -> line: Full string.
 */
int getIdFromLine (char * line) {

	//Find the first occurrence of '='. We know that
	//the user id is after this symbol.
	int idx = strcspn (line, "=");

	char user_id_buff[MAX_NUM_STR];

	int i = 0, buff_id = 0;

	//Read until get the '"' symbol.	
	for (i = idx + 1; (i < strlen(line)) && (line[i] != '\"'); i++) {
		user_id_buff[buff_id++] = line[i];
	}
	user_id_buff[buff_id] = '\0';

	int ret_id = atoi(user_id_buff);
	
	return ret_id;
}

/*
 * Parse the string and return the user id.
 *
 * Input:
 * -> name: Full string.
 * -> id_str: Id got from the string.
 */
char * getUserId(char * name, char * id_str) {
	if ((name == NULL) || (id_str == NULL)) 
		return (char *) "";

	//Find the first occurrence of '_'. We know that
	//the user id is before this symbol.
	int idx = strcspn (name, "_");

	if (idx == strlen(name))
		return (char *) "";

	int i = 0;
	for (i = 0; i < idx && i < strlen(name); i++)
		id_str[i] = name[i];
	id_str[i] = '\0';

	return (char *) id_str;
}