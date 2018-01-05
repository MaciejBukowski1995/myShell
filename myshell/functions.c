/*****************************************************************************
 *	APPLICATION:	$safeprojectname$
 *	MODULE:			functions
 *	AUTHOR(S):		user
 *****************************************************************************/
#include "functions.h"

void pause()
{
	printf("%s  ", "Pause, press ENTER to continue");
	char key;
	do
	{
		key = getch();
	} while (key != 13);
	printf("\n");
}

void echo(char *text, FILE *stream)
{
	fprintf(stream, "%s\n", text);
}

void cd(char *path)
{
	char *currentPath = 0;
	currentPath = _getcwd(currentPath, lengthOfLine);
	if (!path) printf("%s %s\n", "Current directory is : ", currentPath);
	else if (chdir(path)) printf("%s\n", "This directory does not exist!");
	else SetEnvironmentVariableA("PWD", path);
}

void dir(char *path, FILE *stream)
{

	HANDLE fileHandle;
	WIN32_FIND_DATAA findData;
	strcat(path, "\\*");
	fileHandle = FindFirstFileA(path, &findData);
	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		do
		{
			fprintf(stream, "%s\n", findData.cFileName);
		} while (FindNextFileA(fileHandle, &findData));
	}
	FindClose(fileHandle);
}

void env(char *empty, FILE *stream)
{
	LPTSTR lpszVariable;
	LPVOID lpvEnv;

	lpvEnv = GetEnvironmentStrings();

	for (lpszVariable = (LPTSTR)lpvEnv; *lpszVariable; lpszVariable++)
	{
		while (*lpszVariable) fprintf(stream, "%c", *lpszVariable++);
		fprintf(stream, "%c", '\n');
	}

	FreeEnvironmentStrings(lpvEnv);
}

void help(char *function, FILE *stream)
{
	FILE *manFile;
	char line[lengthOfLine];
	char pathToManFile[lengthOfLine];
	GetModuleFileNameA(0, pathToManFile, lengthOfLine);
	pathToManFile[strlen(pathToManFile) - lengthOfThisFile] = 0;
	strcat(pathToManFile, "man\\");
	if (function == 0)
	{
		strcat(pathToManFile, "man_global.txt");
		manFile = fopen(pathToManFile, "r");
	}
	else
	{
		strcat(pathToManFile, function);
		strcat(pathToManFile, ".txt");
		manFile = fopen(pathToManFile, "r");
	}
	if (manFile)
	{
		fprintf(stream, "\n");
		if (function) fprintf(stream, "%s\n", function);
		while (fgets(line, lengthOfLine, manFile))
		{
			if (line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = 0;
			fprintf(stream, "\t%s\n", line);
			line[0] = 0;
		}
		fprintf(stream, "\n");
		fclose(manFile);
	}
	else printf("%s\n", "Man file does not exist in proper diretory!");
}

void interpretateLine(char *line)
{
	char temporaryLine[lengthOfLine];
	strcpy(temporaryLine, line);
	char *command = strtok(temporaryLine, " ");

	if (command == 0)
	{

	}
	else if (!strcmp(command, "quit"))
	{
		exit(EXIT_SUCCESS);
	}
	else if (!strcmp(command, "clr"))
	{
		system("cls");
	}
	else if (!strcmp(command, "pause"))
	{
		pause();
	}
	else if (!strcmp(command, "help"))
	{
		char *redirection = 0;
		char *outputFileName = 0;
		char *tmp = 0;
		tmp = strtok(NULL, " ");
		if (tmp == 0) command = 0;
		else if (strcmp(tmp, ">") && strcmp(tmp, ">>"))
		{
			command = tmp;
			redirection = strtok(NULL, " ");
			outputFileName = strtok(NULL, " ");
		}
		else
		{
			command = 0;
			redirection = tmp;
			outputFileName = strtok(NULL, " ");
		}

		prepareToRedirection(redirection, outputFileName, command, help);
	}
	else if (!strcmp(command, "environ"))
	{
		char *redirection = 0;
		char *outputFileName = 0;
		redirection = strtok(NULL, " ");
		outputFileName = strtok(NULL, " ");

		prepareToRedirection(redirection, outputFileName, 0, env);
	}
	else if (!strcmp(command, "echo"))
	{
		char *redirection = 0;
		char *outputFileName = 0;
		command = strtok(NULL, "]");
		command = command + 1;
		redirection = strtok(NULL, " ");
		outputFileName = strtok(NULL, " ");
		if (command != 0 && strlen(command) > 1)
		{
			prepareToRedirection(redirection, outputFileName, command, echo);
		}
		else printf("%s\n", "Wrong parameter!");
	}
	else if (!strcmp(command, "dir"))
	{
		char *redirection = 0;
		char *outputFileName = 0;
		command = strtok(NULL, "]");
		command = command + 1;
		redirection = strtok(NULL, " ");
		outputFileName = strtok(NULL, " ");
		if (command != 0 && strlen(command) > 1)
		{
			prepareToRedirection(redirection, outputFileName, command, dir);
		}
		else printf("%s\n", "Wrong parameter!");
	}
	else if (!strcmp(command, "cd"))
	{
		command = strtok(NULL, "]");
		if (command)command = command + 1;
		cd(command);
	}
	else
	{
		PROCESS_INFORMATION	 pi;
		STARTUPINFO			 si;
		memset(&pi, 0, sizeof(PROCESS_INFORMATION));
		memset(&si, 0, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		if (!CreateProcessA(NULL, line, NULL, NULL, 0, 0, NULL, NULL, &si, &pi)) printf("%s\n", "Wrong command!");
	}
}

void prepareToRedirection(char *redirection, char *outputFileName, char *command, void(*request)(char *, FILE *))
{
	if (redirection != 0 && outputFileName != 0)
	{
		if (!strcmp(redirection, ">"))
		{
			FILE * outFile = fopen(outputFileName, "w");
			if (!outFile) printf("%s\n", "Failed save to file!");
			else request(command, outFile);
			fclose(outFile);
		}
		else if (!strcmp(redirection, ">>"))
		{
			FILE * outFile = fopen(outputFileName, "a");
			if (!outFile) printf("%s\n", "Failed save to file!");
			else request(command, outFile);
			fclose(outFile);
		}
		else printf("%s\n", "Wrong redirection operator!");
	}
	else request(command, stdout);
}

void processDataFromUser()
{
	char line[lengthOfLine];
	char *path = 0;
	for (;;)
	{
		path = _getcwd(path, lengthOfLine);
		line[0] = 0;
		printf("%s  ", path);
		scanf("%[^\n]", line);
		while (getchar() != '\n');
		interpretateLine(line);
	}
}

void processDataFromFile(char *inFileName)
{
	FILE * inFile = fopen(inFileName, "r");
	char line[lengthOfLine];
	char *path = 0;
	if (inFile)
	{
		while (fgets(line, lengthOfLine, inFile))
		{
			path = _getcwd(path, lengthOfLine);
			if (line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = 0;
			printf("%s  ", path);
			printf("%s\n", line);
			interpretateLine(line);
			line[0] = 0;
		}
		fclose(inFile);
	}
	else printf("%s", "File couldn't be opened properly!");
}