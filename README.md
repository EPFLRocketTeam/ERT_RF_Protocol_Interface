# ERT RF Protocol Interface
This repository should be used as a git submodule for the RF interface such as packet definition.


___________________________________________________________
### How to set up the git submodule in your code ?
Follow this link https://git-scm.com/book/en/v2/Git-Tools-Submodules or directly the instructions below.

Go in your main project folder, which is alreay a git repository, and run:
```
git submode add https://github.com/EPFLRocketTeam/ERT_RF_Protocol_Interface.git
```
By default, it will create a folder name "ERT_RF_Protocol_Interface". You can add a different path at the end of the command if you want it to go elsewhere.


### How to push code to it ?
As it is a private repository, you need to connect to your account. 


### How to create a GitHub token ?
You can find some infos (here)[https://docs.github.com/fr/authentication/keeping-your-account-and-data-secure/creating-a-personal-access-token] or

1. Click on your profile picture top right, and got to **Settings**
2. Then go to **Developer settings** at the complete bottom
3. Then to **Personal Access Tokens > Tokens (classic)** , i.e. here https://github.com/settings/tokens
4. **Generate New Token**

<img src="https://user-images.githubusercontent.com/28660469/227711049-973ce670-d139-44f8-9eeb-8125b7475126.png" width=1000 >

Note that as you will see this token only once, you need to save it.
If you want to avoid to insert your username and password (token) each time, run this before writing them. 
:warning: This might not be really safe as they will be stored on your computer in plain text.
```
git config credential.helper store
```
Then, when you `git push` for example, you will need to write them only the first time.
```
$ git clone https://github.com/USERNAME/REPO.git
Username: YOUR_USERNAME
Password: YOUR_TOKEN
```

Some useful links:
* Git commmand cheat sheet https://education.github.com/git-cheat-sheet-education.pdf
* Markdown summary https://www.markdownguide.org/basic-syntax/
