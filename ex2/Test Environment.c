/*
Authors - Eli Slavutsky (308882992) & Nisan Tagar (302344031)
Project Name - Ex2
Description - This program impelments an application testing tool. Test are run in parallel using threads
*/

// Includes --------------------------------------------------------------------
#include "TestEnvironment.h"


// Function Definitions --------------------------------------------------------

/*
Function createAppTestList
------------------------
Description � 
Parameters	� 
Returns		� 0 for success, -1 for failure
*/
int createAppTestList(char *tst_file_path, test_app **test_list_ptr) {

	test_app *tmp_t = NULL;
	char line[MAX_LINE_LEN];

	// Set test input file for reading
	FILE *fp_test = fopen(tst_file_path, "r");
	if (fp_test == NULL)							 
	{
		printf("Failed to open input test file for reading\n");	 
		return (-1);
	}

	while (fgets(line, MAX_LINE_LEN, fp_test))	// read line from file
	{
		tmp_t = MakeTestFromLine(line);			// create a test entry from the line
		if (tmp_t == NULL)
			return -1;
		AddTestToList(test_list_ptr, tmp_t);	//add test to the test list
	}
	fclose(fp_test);
	return 0;

}

/*
Function createTestResults
------------------------
Description �
Parameters	�
Returns		� 0 for success, -1 for failure
*/
int createTestResults(char *report_file_path[]) {

	// Set test results output file for writing
	FILE *fp_results = fopen(report_file_path, "w");
	if (fp_results == NULL)							// Handle errors
	{
		printf("Failed to open test results file for writing\n");
		exit(1);
	}
}

/*
Function MakePilotFromLine
------------------------
Description � the function gets a string containing pilot data and creates a pilots strcut.
Parameters	� *line is a string which includes pilot data.
Returns		� Pointer to new test data entry
*/
test_app *MakeTestFromLine(char *line)
{
	char c, *tmp_t, *location;
	test_app *new_test;
	new_test = (test_app*)malloc(sizeof(test_app));
	if (NULL == new_test)
	{
		return new_test;
	}

	line = trimwhitespace(line); // Trim leading/trailing whitespace
	tmp_t = line;

	// Copy test application path
	location = strchr(tmp_t, ' ');
	*location = '\0';
	strcpy(new_test->app_path, tmp_t);		
	tmp_t = location + 1;

	// Find and copy application command line arguments
	location = strrchr(tmp_t, ' ');
	if (location) { // Command line arguments present in line
		*location = '\0';
		strcpy(new_test->app_args, tmp_t);
		tmp_t = location + 1;
	}
	
	// Copy application test expected results file path
	strcpy(new_test->app_exp_results_path, tmp_t);

	new_test->next_test = NULL;
	strcpy(new_test->app_test_results, "");

	return new_test;
}

/*
* A simplified API for creating threads.
* Input Arguments:
*   p_start_routine: A pointer to the function to be executed by the thread.
*     This pointer represents the starting address of the thread.
*     The function has to have this specific signature:
*       DWORD WINAPI FunctionName(LPVOID lpParam);
*     With FunctionName being replaced with the function's name.
* Output Arguments:
*   p_thread_id: A pointer to a variable that receives the thread identifier.
*     If this parameter is NULL, the thread identifier is not returned.
* Return:
*   If the function succeeds, the return value is a handle to the new thread.
*   If the function fails, the return value is NULL.
*   To get extended error information, call GetLastError.
* Notes:
*   This function is just a wrapper for CreateThread.
*/
static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
	LPDWORD p_thread_id)
{
	HANDLE thread_handle;

	if (NULL == p_start_routine)
	{
		printf("Error when creating a thread");
		printf("Received null pointer");
		exit(ERROR_CODE);
	}

	if (NULL == p_thread_id)
	{
		printf("Error when creating a thread");
		printf("Received null pointer");
		exit(ERROR_CODE);
	}

	thread_handle = CreateThread(
		NULL,            /*  default security attributes */
		0,               /*  use default stack size */
		p_start_routine, /*  thread function */
		NULL,            /*  argument to thread function */
		0,               /*  use default creation flags */
		p_thread_id);    /*  returns the thread identifier */

	if (NULL == thread_handle)
	{
		printf("Couldn't create thread\n");
		exit(ERROR_CODE);
	}

	return thread_handle;
}

/*
Function createTestResults
------------------------
Description � This function returns a pointer to a substring of the original string. If the given string was allocated dynamically, the caller must not overwrite
that pointer with the returned value, since the original pointer must be deallocated using the same allocator with which it was allocated.  
The return value must NOT be deallocated using free() etc.
Parameters	� Pointer to the original string
Returns		� Pointer to a substring of the original string
*/
char *trimwhitespace(char *str)
{
	char *end;

	// Trim leading space
	while (isspace((unsigned char)*str)) str++;

	if (*str == 0)  // All spaces?
		return str;

	// Trim trailing space
	end = str + strlen(str) - 1;
	while (end > str && isspace((unsigned char)*end)) end--;

	// Write new null terminator character
	end[1] = '\0';

	return str;
}