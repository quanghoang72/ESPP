# Time-Of-Flight project
This project is inspired base on Time-of-Flight (ToF) sensors and during Embedded System Programming Project course. It counts the number of people who enters or exits a "room" on slave node. A 16x2 LCD displays the information on master node. Moreover, the light will automatically be turned on or off depends on the number inside the room.  

To count human comming in the room, the ultrasonic sensor must be triggered prior to DS18B20 sensor and in reverse order to subtract people inside. 
 
## Table of Contents
<!-- TOC -->
- [Hardware](#hardware)
- [Toolchain](#toolchain) 
- [Slave node](#slave-node)    
- [Generate and build Pipeline in Azure](#generate-and-build-pipeline-in-azure)  
- [Perform an automatic Pipeline](#perform-an-automatic-pipeline)  
<!-- TOC -->

## Hardware
-[STM32 Nucleo-L152RE](https://www.st.com/en/evaluation-tools/nucleo-l152re.html)
-[HC-SR04 Distance Sensor](https://datasheetspdf.com/pdf/1380136/ETC/HC-SR04/1)
-[DS18B20 Water-proof Digital Temperature sensor](https://datasheetspdf.com/pdf/1447575/MaximIntegrated/DS18B20/1)
-[LCD 16x2](https://www.dfrobot.com/product-135.html)
-[MAX3485 Transceiver](https://pdf1.alldatasheet.com/datasheet-pdf/view/73221/MAXIM/MAX3485.html)
## Toolchain
-[STM32CubeMX](https://aur.archlinux.org/packages/stm32cubemx/)
-[Platformio](https://docs.platformio.org/en/latest/core/installation.html)
## Slave node
The Slave node presents as an STM32 Nucleo with sensors. To detect a human, the 1-wire DS18B20 measures human temperature    
![create new organization](attachments/new_organization.png)
![Confirm information](attachments/confirm_information.png)  
Pick and name and `+ Create project`  
![Create project](attachments/create_project.png)  
## Integrate with AB (Azure Boards)
To connect between the Azure Board and Github repo  
- `Project setting` > `Github Connections` > `Authorize AzureBoards` > Chose your desired project for this Repo > `Save` > `Approve, Install, & Authorize`  
- Create a task in AB > check for the task ID  
- Develop the code, commit as this syntax,this will link to the workitem ID  
```bash
AB#{ID}
```  
![commit link ID](attachments/commit_AB_ID.png)
![task linked](attachments/task_link.png)  
- To change state to done state, use syntax `Fix AB#{ID}` in commit message  
---
>**NOTE:**
The `fix` syntax only changes workitem when the push commit is in the **main/master** branch, otherwise it will wait to be merge from others branch into main
---  
![fix_commit](attachments/fix_commit.png)
![workitem_auto_to_done](attachments/workitem_done.png)  
- In this project, I create new subscription to notify when the task change state (in this case from `In Progress` > `Done`)  
![create_subscription](attachments/create_subscription.png)  
![subscriptiion_notification](attachments/notification_state_change.png)  
![microsoft_example_git_commit](attachments/git_commit_AB_sample.png)  
## Clone code into repository
You can use either Azure Git repository or Github Repo  
To test this sample project on your the repo. There are 2 options for both version controls:
- #### Azure Git Repository
Import a repository by copy paste url  
![Clone url](attachments/clone_url.png)  
- #### Github Repository
Create your new repo on git without `.gitignore`, `readme.md` and `license`  
Run these code below:   
```bash
$git clone https://github.com/quanghoang72/RTOS-pipeline.git
$git remote rename origin old
$git remote rm old
$git remote add origin https://github.com/your_user_name/your_repo_name
$git branch -M main
$git push -u origin main
```  
Now you have exactly the same code into your own new repository on git  
## Generate and build Pipeline in Azure
The pipeline is instructed and run under YML file. The purpose of this pipeline:  
- Build uC/OS-III code with platformIO CLI
- Static code analysis cppcheck srouce code
- Run unit googletest  
To build one, click `Pipeline` on the left bar of project UI > `Create Pipeline`
- #### Azure Repos Git
Chose your Project repository. It will detect the YML file in the current build directory of that repo code > `Run`
- #### Github
Chose your Project repository > `Run`
## Perform an automatic Pipeline
In the pipeline yml file, there is a way to trigger the pipeline  
```
trigger:
  batch: true
  branches:
    include:
    #- '*' means all
    - master

    # Wildcards patterns allow * to match zero or more characters and ? to match a single character.
    #- releases/* 

    #- branch_name
    #exclude:
    #- bug/*
pr:
 - master
 #- release/*
```
- trigger: cause a pipeline to run whenever you push an update to the specified branches or you push specified tags
- Pull request (PR) triggers cause a pipeline to run whenever a pull request is opened with one of the specified target branches, or when updates are made to such a pull request.  
For more information about trigger can be found at: [trigger pipeline](https://docs.microsoft.com/en-us/azure/devops/pipelines/repos/github?view=azure-devops&tabs=yaml)  
If you want to not trigger the pipeline by push just a small change which **do not affect** the code (ie: readme, image...). Put `[skip ci]` in the commit to skip the build.  
![skip ci](attachments/skip_ci.png)