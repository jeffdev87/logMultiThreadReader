1) General Information:

For the code to run successfully, please check the paths specified in the file "utils.h" and change them accordingly. The implementation relies on that full paths to work. See item 3 of this README.

The general process to run the code is as follows:

* First, execute the code to generate the synthetic log data. The logs will be placed in a folder by the name "remote", separated by the folders's name specified by the time you run the data generator code.

For example:

../remote/d1
../remote/d2
...

* After that, you are able to execute the main code where the separator is implemented.

2) Steps to compile and execute:

* data_generator.cpp
  
  Compile: g++ -o datagen data_generator.cpp utils.cpp
  It will generate an executable called datagen
  
  Execute, where -1 (optional) means to clean old data before start, and di is the directory name
  ./datagen -1 d1 d2 d3

* logSep.cpp
  Compile: g++ -o logsep logSep.cpp utils.cpp -lpthread
  It will generate an executable called logsep

  Execute:
  ./logsep
  The results will be stored in the path where you run the logSep code, inside a folder named final_res.

3) Constants to be changed in utils.h BEFORE COMPILE THE CODE:

MAX_ROW_FILE: Controlls the maximum number of lines per log file.
MAX_USERS: Controlls the maximum number of synthetic users. It is used to generate the fake user_id.
MAX_SERVER: Controlls the maximum number of servers, which are simulated by directories. In fact, this constant also controlls the number of threads to be created.
MAX_NUM_FILE_PER_DIR: Controlls the maximum number of files per server.
ROOT_PATH: Root path where the generated data will be placed.
REMOTE_BASE_FOLDER: Folder name where the generated data will be placed inside the ROOT_PATH

