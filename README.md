## FACE RECOGNIZER PROGRAM ##

The program is a client-server program which implements face recognition algorithm base on dot product action.
The code is written in c++ language.

The server will contain a database of 64 persons. Each person will have a vector of a fixed size of 256 integers.

The client will have 2 options to communicate with the server:
- requesting AddPerson - adding a person to the server existing database. The server will return an answer confirming the addition have been commited.
- requesting FindPerson - finding the best persons matches out of the server existing database. The server will return a list of people that are the best matches.

In both cases, the program will generate a rendom vector of ints which represent the person to add/find.


** instructions to run the program: **

1) run Server.exe 

2) run the Client.exe and add to it a command line arguments by the choises bellow:

- Adding person:

first argument = 1
second argumnt and above - person's name to add.

example: ./Client.exe 1 ori ben guy jouvani


- Find person:

first argument = 2
second argument = the number of matches the user want the server to return.

example: ./Client.exe 2 5 (5 representing the number of matches that the server will return and print to the user)
 
