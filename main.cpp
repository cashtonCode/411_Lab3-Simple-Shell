/*
author: Cashton Christensen
date: 1-28-2019
purpose: You will learn how to write a LINUX shell program. This will give you the opportunity to learn how new child processes are created and how parent processes can follow up on a child process.
*/
#include <unistd.h>	//c functions required
#include <iostream>	//IO functions cout cin
#include <string>	//all string functions needed
#include <fstream>	//c++ file functions
#include <fcntl.h>	//c file open functions
#include <csignal>	//signal processing
#include <cstdlib>	//EXIT_SUCCESS constant
#include <algorithm>	//remove_if function
#include <dirent.h>	//to test if directory exists
#include <sys/types.h>	//defines directory structure and other types
#include <sys/wait.h>	//for wait() function
#include <bits/stdc++.h>	//

#include <stdio.h>	//for write() function

int main(){
	std::string command = "", command_sub = "", first_word = "", next_param = "", repeat_output = "", repeat_filename = "";
	char buffer[1024];
	int length_command, hiMom[2], get_from_child;
	std::ofstream h_file, repeat_file;
	std::ifstream h_file2;
	std::string console_string;
	pid_t p;
	h_file.open("history_file.txt");

	while(command != "quit")
	{
		std::cout<< "cashton_christensen@Simple_Shell:~$ ";//print a prompt //CHANGE
		std::getline(std::cin,command);//get user command
		h_file << command << std::endl;
		console_string = "";
		if(command.find(" ") == -1)//if there are no paramters
		{
			length_command = command.size();//find the command
			next_param = "";//resetting next_param
		}
		else//there are parameters
		{
			length_command = command.find(" ");//find the command
			command_sub = command.substr(length_command+1, command.size());
			if(command_sub.find(" ") == -1)//find the parameter
  			next_param = command_sub.substr(0, command_sub.size());
			else
  			next_param = command_sub.substr(0, command_sub.find(" "));
		}

		first_word = command.substr(0, length_command);//setting the command whether there is a parameter or not
		//PARSING THE COMMAND AND ANY PARAMETERS
		if(first_word == "myprocess")
		{
			//no parameters
			//return the current process id
			std::cout<< "Current process ID: " << getpid() << std::endl;
		}
		else if(first_word == "allprocesses")
		{
			//no paramters
			//return all current processes
			std::cout<< "All current processes:\n" << system("ps") << std::endl;
		}
		else if(first_word == "chgd")
		{
			// 1 paramter
			//check for directory name in next_param.
			//change the current working directory to next_param
			//if next_param == ""; no change
			console_string = "cd ";
			if(next_param == ""){}
			else
				console_string.append(next_param);

			const char *a = console_string.c_str();
			system(a);
			//change directory to param
		}
		else if(first_word == "clr")
		{
			//no parameters
			//clear the screen
			system("clear");
		}
		else if(first_word == "dir")
		{
			// 1 parameter
			//list the contents of the directory (name in next_param)
			console_string = "ls -al ";//making a command line string
			console_string.append(next_param);
			const char *b = console_string.c_str();//system only takes char strings
			system(b);
		}
		else if(first_word == "environ")
		{
			//no parameters
			//list all environment settings
			system("env");
		}
		else if(first_word == "quit")
		{
			//no parameters
			//quit the shell w/ 0 return value. Close the history file and display it to the screen. use system.exit()
			std::cout<< "History File:\n\n";
			h_file.close();
			h_file2.open("history_file.txt");
			std::cout << h_file2.rdbuf();
			h_file2.close();
			//history file stuff
			return(0);
		}
		else if(first_word == "help")
		{
			//no parameters
			//display the user manual. Write in reference to the above 7 supported commands. Plus something else.
			std::cout << "  myprocess        - Return the current process ID." << std::endl
				  << "  allprocesses     - Return all current processes." << std::endl
				  << "  chgd <directory> - Change the working directory to <directory>." << std::endl
				  << "  clr              - Clear the screen" << std::endl
				  << "  dir <directory>  - List the contents of directory <directory. Blank <directory> will list the contents of the current directory." << std::endl
				  << "  environ          - List all the environment settings." << std::endl
				  << "  quit             - Quit the shell. Displays history file to the screen." << std::endl
				  << "  help             - Display the user manual." << std::endl
				  << "  repeat <string>  - outputs the string to the console. If user types a redirection operator '>' after the string, then redirect to the file specified." << std::endl
				  << "  hiMom            - Prints a message." << std::endl
				  << "  all other inputs - Sent to the parent shell for execution." << std::endl;
		}
		else if(first_word == "repeat")
		{
			//1 parameter
			//output the string to the console
			//if the user includes the redirection operator > then the string should be output to a file named after the >
			if(command_sub.find("<") == -1)//if the user doesn't provide a file to print output to
				std::cout << command_sub << std::endl;
			else	//separate the output and the file; open the file; send output to file
			{
				repeat_output = command_sub.substr(0, command_sub.find("<"));
				command_sub = command_sub.substr(command_sub.find("<"), command_sub.size());
				repeat_filename = command_sub.substr(command_sub.find(" ")+1, command_sub.size());
				repeat_file.open(repeat_filename);
				repeat_file << repeat_output;
				repeat_file.close();
			}
		}
		else if(first_word == "hiMom")
		{
			pipe(hiMom);//open pipe
			char hi[30] = {"Hello. How are you today?"};

			switch(fork())
			{
				case -1:
					std::cout << "error forking\n";
					break;

				case 0://child
					close(hiMom[0]);//close reading end of pipe
					//send string to parent
					write(hiMom[1], hi, strlen(hi)+1);//write string to pipe
					close(hiMom[1]);//close writing end of pipe
					exit(1);
					break;

				default://parent
					close(hiMom[1]);//close writing end of pipe
					wait(NULL);//wait for child to send string
					get_from_child = read(hiMom[0], buffer, sizeof(buffer));//read string from pipe
					std::cout << buffer << std::endl;
					close(hiMom[0]);//close reading end of pipe
					break;
			}
		}
		else
		{
			std::string console_string = command;//making a command line string
			const char *c = console_string.c_str();//system only takes char strings
			system(c);
			//relay the command line to the parent shell for execution using the system function
		}
	}//while loop end
	return 0;
}
