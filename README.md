# Awa
A SCM awareness tool.

A project aimed at increasing developers awareness in collaborative development projects.
Awa does this by sending push-notifications to your desktop. 
Whenever the remote repository updates it will issue an update notification. 

Then start Awa by issuing `awa` in your terminal (assuming you've installed
Awa properly). 

# Install
To install you'll have to issue the following command.

```bash
make && sudo make install
```

It will generate the executable and create a symbolic link to the executable generated in this directory.

# Features

* awa add \<path\> \<branch\> -- \<path\> needs to be an absolute path, \<branch\> defualts to origin/master
* awa remove \<path\> 
* awa log -- prints all logg messages since the start of the program. 
* awa log -n \<nbr\> -- prints the \<nbr\> last logg messages. 
