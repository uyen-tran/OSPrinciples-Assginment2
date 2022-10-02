# OSPrinciples-Assginment2

**REMOTE SHELL USING SOCKET PROGRAMMING**
 
100 points 
 
Due: Friday, October 15, 2021, 11:59pm 
 
This assignment consists of writing a C/C++ program for a remote shell system. It involves a Server and a Client process. The server process resides on another machine on  the  network  and  the  user  provides  commands  to  the  server  via  a  client  process  on  a local  machine.  The  client  sends  the  commands  to  the  server,  the  server  executes  them, and sends the output back to the client.  

The  goal  of  this assignment  is  to  practice  some  system  calls  and  process management  by  the  operating  system.  In  addition,  it  reinforces  the  operating  system support for client server programming using sockets. You will write a basic remote shell in which a user specifies an arbitrary shell command to be executed, and it is sent over socket connection and executed on a remote server. 

(Note: use connection-oriented communication (socket programming using TCP))  

You will write a server and a client program:  

**Server:** The server will run on the remote machine.  

• It will bind to a TCP socket at a port known to the client and waits for a Connection Request from Client. 

• When it receives a connection, it forks a child process to handle this connection. The Server must handle multiple clients at a time. 

• The parent process loops back to wait for more connections. 

• The  child  process  executes  the  given  shell  command  (received  from  the  client), returning  all  stdout  and  stderr  to  the  client.  (Hence,  the  server  will  not  display the output of the executed command) 

• The server can assume that the shell command does not use stdin. 
  
**Client:** The client will run on the local machine.  
• From the command line, the user will specify the host (where the server resides) 
and the command to be executed. 

• The client will then connect to the server via a TCP socket. 

• The client sends the command to the server. 

• The client will display any output received from the server to the stdout. 

• After displaying the output, the client waits for next command from the user. 

• The client will not close/exit until the user enters “quit” command. 

**Creating a Child Process**
At Server side, for each incoming client, a child process is forked which executes the command received from the client.  

The separate child process is created using the fork() system call, and the user’s command is executed using one of the system calls in the exec() family.  

Note:(The example below is one way of doing this, you are welcome to do it your own way!)  

This will require parsing the command into separate tokens and storing the tokens in an array of character strings, say args. For example, if the command entered at the prompt is:  
ps -ael  
Te values stored in the args array are:  
args[0] = "ps"  
args[1] = "-ael"  
args[2] = NULL  
This args array will be passed to the execvp() function, which has the following prototype:  
execvp(char *command, char *params[]);  

Here, command  represents the command to be performed and params stores the parameters  to  this  command.  For  this  assignment,  the  execvp()  function  should  be invoked as execvp(args[0], args). 
