Used mimi.cs.mcgill.ca

Some Notes:
IMPORTANT NOTE: all text files must end with empty line (\n) OR with the "quit" command. If a file is piped (ie. use ./mysh < testfile.txt), shell will quit once all commands in file are executed.

- for "quit" command: 
	if are in a script, print "Bye!" and exit script
	if not in a script, print "Bye!' and exit shell
- for "set" command: 
	if too few arguments, prints "Missing argument(s)" and allows user to try again
	if more than 3 arguments, only uses the first 2 arguments (ie. for "set x Montreal Canadians", stores  x = Montreal)
- for "print" command:
	if not given variable to print (ie. "print"), prints "Missing argument" and allows user to try again
	if given multiple arguments (ie. "print x s"), prints only first argument (meaning prints value of x)
- for "run" command:
	if no file name given or file does not exist, prints "Script not found" and allows user to try again
	if have infinite recursion, will keep opening and executing scripts until get stack overflow, at which point "Script not found" is printed and segmentation fault occurs
- for shellmemory.c:
	max 100 variables and corresponding values can be stored in memory
