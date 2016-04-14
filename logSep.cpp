/*
 * Log data separator using PThreads.
 *
 * This code aims to separate log data by userid.
 * 
 * For this code to run successfully, it is expected the
 * following sample directory tree, where di represents 
 * a remote "server".
 * 
 * ./d1
 * ./d1/log_1.txt
 * ./d1/log_2.txt
 * ...  
 * ./d2
 * ./d2/log_1.txt
 * ...
 *
 * The results will be placed in the directory where you run this code, into the folder
 * ./final_res. The logs will be separated by the userid.
 * 
 * For example.
 *
 * ./final_res/1.txt
 * ./final_res/2.txt
 * ..
 *  
 * Jefferson William Teixeira
 * 13/12/15
 */
#include<pthread.h>

#include "utils.h"

#define RES_FOLDER "res"
#define FINAL_RES_FOLDER "final_res"

/*
 * Thread params type.
 * -> thread_id: The thread identifier
 * -> res_path: The path where the auxiliar result (log files separated by id) will be placed.
 * -> path: The input path, where the log data are placed.  
 */
typedef struct thread_data{
   int  thread_id;
   char res_path[MAX_STR_PATH];
   char path[MAX_STR_PATH];
} threadData;

/* 
 * Function to parser the log file and split the entries by userId.
 * Input:
 * -> f: File to be parsed.
 * -> path: Output path to store the results.
 * -> th_id: Thread id that called this function.
 */
int parseLogFile (FILE * f, char * path, int th_id) {

	if (f == NULL)
		return -1;

	char line[MAX_LOG_STR];
	rewind (f);
	
	while ((fgets(line, MAX_LOG_STR, f) != NULL) && !feof(f)) {
		int id_line = getIdFromLine (line);
		//printf("Id: %d\n", id_line);

		char user_file_path[MAX_STR_PATH];
		sprintf(user_file_path, "%s/%d_%d.txt", (char *) path, id_line, th_id);

		//Open/Create a file specific for the returned id
		FILE * fout = fopen(user_file_path, "a");

		//Store the log entry into the file
		if (fout != NULL) {
			fprintf(fout, "%s", line);
		}
		fclose(fout);
	}

	return 1;
}

/* 
 * Function to read each file inside the specified path and call the parser.
 * This function is called by the threads.
 *
 * Input:
 * -> params: Thread params. See threadData data type for details.
 *
 */
void * organizeLogsByUserId (void * params) {
	
	threadData * p = (threadData *) params;
	
	char res_path[MAX_STR_PATH];
	sprintf(res_path, "%s", (char *) p->res_path);

	//Point to the specified path
	DIR * server_dir = openFolder((char *) p->path);
	if (server_dir == NULL)
		return NULL;

	int i = 0;
	struct dirent *dp = NULL;
	while ((dp = readdir(server_dir)) != NULL) {
		//Skipping useless references.
		if (strcmp(dp->d_name, ".") != 0 && 
			strcmp(dp->d_name, "..") != 0) {

			//Path to the 'res' folder
			char filepath[MAX_STR_PATH];
			sprintf(filepath, "%s/%s", (char *) p->path, dp->d_name);
			
			FILE * f = NULL;
			if ((f = fopen(filepath, "r+")) != NULL) {
				printf("Opened file %s\n", filepath);
				
				//Parse and store the result in the specified path			
				parseLogFile(f, (char *) res_path, p->thread_id);
				fclose(f);
				i++;
			}
		}
	}
	closedir(server_dir);

	printf("%d files processed.\n", i);

	return NULL;
}

/* 
 * Merge the result obtained by each thread into single files per user id.
 *
 * Input:
 * -> input_path: Path where the aux results were placed.
 * -> output_path: Path where the final results will be placed.
 *
 */
int mergeLogFilesById (char * input_path, char * output_path) {
	
	//Point to the main folder
	DIR *mainDir = openFolder(input_path);
	if (mainDir == NULL)
		return -1;
	
	//Create the directory where the final results will be placed
	createFolder (output_path);

	//Run over the aux files.
	struct dirent *dp = NULL;
	while ((dp = readdir(mainDir)) != NULL) {

		//Skipping useless references.
		if (strcmp(dp->d_name, ".") != 0 && 
			strcmp(dp->d_name, "..") != 0) {

			char curr_file_path[MAX_STR_PATH];
			sprintf(curr_file_path, "%s/%s", input_path, dp->d_name);
			FILE * f = fopen(curr_file_path, "r+");
			if (f == NULL)
				continue;

			//Aux files have the format "userid_threadId". Get just the user
			//id to create the final file.
			char out_file_path[MAX_STR_PATH],
				 user_id_buff[MAX_NUM_STR];

			sprintf(out_file_path, "%s/%s.txt", output_path, getUserId(dp->d_name, user_id_buff));					
			FILE * fout = fopen(out_file_path, "a");
			if (fout == NULL)
				continue;

			char line[MAX_LOG_STR];
			while ((fgets(line, MAX_LOG_STR, f) != NULL) && !feof(f)) {					
				fprintf(fout, "%s", line);
			}

			fclose(fout);
			fclose(f);
		}
	}
	closedir(mainDir);

	return 1;
}

/*
 * Main function to create the threads that will perform the file processing.
 */
int main() {
	
	char server_path[MAX_STR_PATH];

	//Open the main directory containing all the folders
	//that simulate the servers. 
	DIR *mainDir = openFolder((char *) ROOT_PATH, (char *) REMOTE_BASE_FOLDER, server_path);

	if(mainDir == NULL)
		return -1;

	//Define the aux output folder
	char res_path[MAX_STR_PATH];
	sprintf(res_path, "./%s", (char *) RES_FOLDER);

	//Define the output folder
	char output_path[MAX_STR_PATH];
	sprintf(output_path, "./%s", (char *) FINAL_RES_FOLDER);
	
	//Clean old results
	removeFolder ((char *) res_path);
	removeFolder ((char *) output_path);

	//Create the aux directory where the interm. results will be placed
	createFolder ((char *) res_path);

	//Array of threads and their arguments (data)
	pthread_t org_log_threads[MAX_SERVER];
	threadData thread_data_array[MAX_SERVER];

	struct dirent *dp = NULL;
	int thr_id = 0;

	//Run over the directories inside the main one
	//and create a thread to process each folder.
	while((dp = readdir(mainDir)) != NULL &&
		   thr_id < MAX_SERVER) {

		//Skipping useless references.
		if (strcmp(dp->d_name, ".") != 0 && 
			strcmp(dp->d_name, "..") != 0) {

			//Storing thread data
			thread_data_array[thr_id].thread_id = thr_id;
			sprintf(thread_data_array[thr_id].path, "%s/%s", server_path, dp->d_name);
			sprintf(thread_data_array[thr_id].res_path, "%s", res_path);

			printf("Creating thread to process the folder: %s\n", thread_data_array[thr_id].path);
			
			if(pthread_create(&org_log_threads[thr_id], NULL, organizeLogsByUserId, (void *) &thread_data_array[thr_id])) {
				fprintf(stderr, "Error creating thread\n");
				return 1;
			}

			thr_id++;
		}
	}
	closedir(mainDir);

	//Joining all the threads to retrieve the results.
	for (int i = 0; i < thr_id; i++) {
		if (pthread_join(org_log_threads[i], NULL)) {
			fprintf(stderr, "Error joining thread\n");
			return 2;
		}	
	}

	printf("Merging result files...\n");

	//Merge the results.
	mergeLogFilesById(res_path, output_path);

	//Remove aux folder
	removeFolder(res_path);

	pthread_exit(NULL);

	return 0;
}