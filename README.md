# EE Training
This repository contains the assignments for EE training. 

# First Assignment

## Context
In RoboMaster 2018, the bullet replenishment device is where the Standard Robots collect the bullets. It has two parking slots. For each slot, whenever the car press the button, the gate on its top would open for 2 seconds, the bullets fall, and then the gate itself would close automatically. 
  In the setup() function, the Arduino checks the button state of each slot with a for-loop and delay(2000). The issue with these is that whenever the delay(2000) is called, the for-loop halts and the other slot cannot detect the button state, which means even if we have two slots, only one can be used at a time. Your mission is to make it multi-tasking.

## Requirements
In the assignment you need to implement "Hardware Interrupt" for detecting if the buttons are pressed, and "Timer Interrupt" to substidute delay(). 

Study Reference
  - Hardware Interrupt: https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
  - Timer Interrupt: https://learn.adafruit.com/multi-tasking-the-arduino-part-2/timers
  
  
## Submission
You will use git to retrieve and submit the assignment. Make a branch with your own Slack display name so that we can identify your submission. Failure to put the files in the expected place will be considered an un-qualified submissions.

### Basic guide on using GitHub
* You must have git installed on your PC. Download it from [here](https://git-scm.com/)
* Use `git clone https://github.com/project-herakles/ee_training.git` to clone the repository for yourself. You will see that you will be on `master` branch.
* You will need a new branch for yourself on your computer, and you can do so by using the command: `git checkout -b [branch-name]`
* GitHub requires you to switch to the other branch before you can start pushing code to GitHub. Use: `git checkout [branch-name]`
* After making changes to your code, you need to commit those changes. Use `git commit -am "<your commit message>"` (replace with your own commit message) to commit your code to the branch.
* Now, you need to use: `git push origin [branch_name]` to push code to GitHub.
* GitHub requires you to switch to the branch before you can start pushing code to GitHub. Use: `git checkout [branch-name]` to switch between branches.
* Now, you can `git push origin [branch_name]` to push code to GitHub.

## Assessment
The result would be determined by the actual performance of the device, so we encourage you to use the hardware available at the lab for testing. If you need access to the device, please make a request at the knock_knock channel on Slack.
