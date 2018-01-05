#pragma once
/*****************************************************************************
 *	APPLICATION:	$safeprojectname$
 *	MODULE:			functions
 *	PURPOSE:
 *	AUTHOR(S):		user
 *
 *	1/3/2018 8:48:01 PM	Created.
 *****************************************************************************/
#include "functions.h"
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>

#define lengthOfLine MAX_PATH
#define lengthOfThisFile 11

void pause();

void echo(char *text, FILE *stream);

void cd(char *path);

void dir(char *path, FILE *stream);

void env(char *empty, FILE *stream);

void help(char *function, FILE *stream);

void interpretateLine(char *line);

void prepareToRedirection(char *redirection, char *outputFileName, char *command, void (*request)(char *, FILE *));

void processDataFromUser();

void processDataFromFile(char *inFileName);