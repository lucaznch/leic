#ifndef PROCESSING_H
#define PROCESSING_H

/// Processes the files in the given directory with the given number of processes and threads.
/// @param dir_path Directory path.
/// @param number_of_processes Maximum number of processes to spawn. 
/// @param number_of_threads Maximum number of threads to spawn per file.
/// @param delay State access delay in milliseconds.
/// @return 0 if the directory was processed successfully, 1 otherwise.
int process_directory_files(const char *dir_path, int number_of_processes, int number_of_threads, unsigned int delay);

/// Processes the given file with the given number of threads.
/// @param file_entry_name File name of the file to process.
/// @param number_of_threads Maximum number of threads to spawn.
/// @return 0 if the file was processed successfully, 1 otherwise.
int thread_manager_for_file_processing(char* file_entry_name, int number_of_threads);

/// Thread function to process a file.
/// @param args Thread arguments. They must be of type thread_args.
/// @return Pointer to the return value.
void* process_file(void* args);

#endif /// PROCESSING_H