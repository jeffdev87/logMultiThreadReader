/*
 * Data generator code and functions.
 * 
 * This code aims to generate synthetic log data according to
 * the following sample pattern:
 * 
 * -> 177.126.180.83 -- [13/Dec/2015:16:32:37] "GET /batman.jpg HTTP/1.1" 1658 2814 "-" "userid=8" 
 * 
 * The logs are organized into separated folders, which are specified by the user
 * through input parameters.
 * 
 * For example, if you run: ./datagen d1 d2 d3, it will be generated three folders, d1, d2 and d3 in
 * the root path ROOT_PATH/REMOTE_BASE_FOLDER. These constants should be changed in the utils.h.
 *
 */

#include "utils.h"

/*
 * It generates synthetic log data in the specified file.
 */
void dataGenerator (FILE * f) {

	char date[MAX_STR_DATE],
	     filename[MAX_STR_FILE_NAME];
	
	const char * files[MAX_NUM_FAKE_FILES] = {"/meme.jpg", "/dogs.jpg", "/teste.jpg", 
	                                          "/lolcats.jpg", "/xuxa.jpg", "/batman.jpg", "/lala.jpg", 
	                                          "/pato.jpg", "/casa.jpg", "/lapis.jpg"};
	srand (time(NULL));

	if (f == NULL)
		return;
	
	for (int i = 0; i < MAX_ROW_FILE; i++) {
	
		int userid = rand() % MAX_USERS + 1,
			fileId = rand() % MAX_NUM_FAKE_FILES,
			port1 = rand() % N_PORT + 1,
			port2 = rand() % N_PORT + 1;

		fprintf(f, 
			    "177.126.180.83 -- [%s] \"GET %s HTTP/1.1\" %d %d \"-\" \"userid=%d\"\n", 
			    getStringTime((char *) date), files[fileId], port1, port2, userid);
	}
}

/*
 * Main function to create the directory tree and call
 * the log data generator.
 *
 * Input: folder names to represent the remote servers.
 */
int main (int argc, char* args[]) {

	if (argc <= 1) {
		printf("\nUsage:\n -Enter the name of the directories to be created under the folder %s\n", ROOT_PATH);
		printf(" -Enter the code -1 before the first directory name in order to CLEAN everything before.\n");

		return 0;
	}

	//If the first argument is -1, clean everything before starting.
	//Adjust the firsDirName variable to start reading the folder's names
	//after the -1 code.
	int firstDirName = 1;
	if (args[firstDirName] != NULL && 
		atoi (args[firstDirName]) == -1) {
		removeFolder ((char *) ROOT_PATH, (char *) REMOTE_BASE_FOLDER);
		firstDirName++;
	}
	
	if (!createFolder ((char *) ROOT_PATH, (char *) REMOTE_BASE_FOLDER))
		printf("Keeping the old main folder.\n");
	
	//For each directory name, create it and call the 
	//data generator.
	for (int i = firstDirName; i < argc; i++) {
		if (args[i] != NULL) {
			char path[MAX_STR_PATH];
			path[0] = '\0';

			strcat(path, ROOT_PATH);
			strcat(path, "/");
			strcat(path, REMOTE_BASE_FOLDER);

			if (!createFolder (path, args[i])) {
				printf("Folder already existis (%s).\n", args[i]);
				continue;
			}

			strcat(path, "/");
			strcat (path, args[i]);

			//Create as log files per directory as specicifed by the constant
			//MAX_NUM_FILE_PER_DIR.
			for (int j = 0; j < MAX_NUM_FILE_PER_DIR; j++) {
				
				char file_path[MAX_STR_PATH];
				strcpy(file_path, path);
				
				char filename[MAX_STR_FILE_NAME];
				sprintf(filename, "/log_%d.txt", j);
				
				strcat(file_path, filename);

				//Generate synthetich data into the file.
				FILE * f = fopen(file_path, "w+");
				if (f != NULL) {
					sleep(1);
					printf("Generatig data for file %s...\n", filename);
					dataGenerator(f);
				}
				fclose(f);
			}
		}
	}
	return 0;
}