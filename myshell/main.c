/*****************************************************************************
 *	APPLICATION:	myshell
 *	MODULE:			main
 *	PURPOSE:
 *	AUTHOR(S):		user
 *
 *	12/17/2017 8:30:39 AM	Created.
 *****************************************************************************/



/*****************************************************************************
 *	THE MAIN ENTRY POINT TO THE PROGRAM.
 *
 *	input:	argc	The number of arguments of the programme.
 *			argv	The vector of arguments of the programme.
 *	return:	The code of the reason the process was terminated for.
 *			The value defaults to zero.
 *****************************************************************************/


int main(int argc, char* argv[])
{
	if (argc == 1) processDataFromUser();
	else processDataFromFile(argv[1]);
	system("pause");
	return 0;
}
