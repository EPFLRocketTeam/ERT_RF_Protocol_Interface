[<img src="https://user-images.githubusercontent.com/28660469/227711535-03fd1d77-1fad-4e00-9703-57e4a85180ed.png" width=100 align="right" >](https://epflrocketteam.ch/)

# ERT RF Protocol Interface
<img src="https://user-images.githubusercontent.com/28660469/227712654-80d11d6f-1579-4451-bb96-b4893685d71e.png" width=80 align="left">
This repository should be used as a **git submodule** for the RF interface such as packet definition.
The goal is to minimize the protocol interface difficulties encountered between the AV-GS-GSE-PL subsystems.
By having this git submodule repository shared between subsystems, no more interface documents are required as a git pull would be sufficient to be updated.

___________________________________________________________
### How to set up the git submodule in your code ?
Follow this [link](https://git-scm.com/book/en/v2/Git-Tools-Submodules) or directly the instructions below.

Go in your main project folder, which is alreay a git repository, and run:
```
git submode add https://github.com/EPFLRocketTeam/ERT_RF_Protocol_Interface.git
```
By default, it will create a folder name "ERT_RF_Protocol_Interface". You can add a different path at the end of the command if you want it to go elsewhere.

### How to get the last modifications ?
You will need to run git pull but from the submodule directory
```
git pull
```

### How to push code to it ?
Once you made some modifications to the code of this submodule, you need to go in the submodule folder (ERT_RF_Protocol_Interface), then you can run `git status` to see if some changes are seen. If yes you need to add this files, commit with a message and finally push.
```
git add PacketDef.h
git commit -m "uplink packet update"
git push
```
Note that as it is a private repository, you will need to connect to your account. You can read the next [section](#how-to-create-a-github-token-) for more infos.

- [ ] TODO: Maybe it is possible to directly push on this repo from the main repo ?

### How to create a GitHub token ?
You can find some infos [here](https://docs.github.com/fr/authentication/keeping-your-account-and-data-secure/creating-a-personal-access-token) or

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
Then, when you `git push` for example, you will need to write them, but only the first time.
```
$ git clone https://github.com/USERNAME/REPO.git
Username: YOUR_USERNAME
Password: YOUR_TOKEN
```
_______________________________________________________
Some useful links:
* Git command cheat sheet https://education.github.com/git-cheat-sheet-education.pdf
* Markdown summary https://www.markdownguide.org/basic-syntax/
