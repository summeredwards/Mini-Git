MiniGit Version Control System
-------------------------------

This program acts as a toy version-control system. The MiniGit program has the following functionality:
1. Adding files to the current commit
2. Removing files from the current commit
3. Committing changes 
4. Checking out a specific version based on a commit number 

The program comes with a user interface so the user can interact with the program. The user is given a 
list of choices within a menu. The program will continue to run indefinitely until the user chooses "Quit".

Running the program will prompt the user with an option to initialize an empty repository in the current
directory. If the user chooses to initialize, an empty repository "minigit" will be created. 

Files in the current directory can be added and commited into the rpository while the program runs. If changes
are made to the files, commiting them will save the changes. If the user wishes to go back to previous 
versions of their files, they can do that via "Checkout".