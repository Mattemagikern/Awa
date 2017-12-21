# Awa
A SCM awareness tool.

A project aimed at increasing developers awareness in collaborative development projects.
Awa does this by sending push-notifications to your desktop. 
Whenever the remote repository updates it will issue an update notification. 

To make Awa watch your folders, create a file named .awa in your home
directory.
Enter absolute paths to the directory you'd like to watch like this:

`path <actual_path>`

Then start Awa by issuing `awa` in your terminal (assuming you've installed
Awa properly). 

# Install
To install you'll have to issue the following command.

`bash
make && sudo make install
`

It will generate the executable and create a symbolic link to the executable generated in this directory.


# ToDo
* Make it work across posix
* Allow for more options for the wacher
* Allow for more SCM tools than git. 
