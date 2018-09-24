# ee_training
  This place is for EE training assignments. To submit the assignments, please create a branch named after your display name on Slack, and then push to the remote repo. For the files not on the expected location, they are not acknowledged.

# first assignment
1. context
  In RoboMaster 2018, the bullet replenishment device is where the Standard Robots collect the bullets. It has two parking slots. For each slot, whenever the car press the button, the gate on its top would open for 2 seconds, the bullets fall, and then the gate itself would close automatically. 
  In the setup() function, the Arduino checks the button state of each slot with a for-loop and delay(2000). The issue with these is that whenever the delay(2000) is called, the for-loop halts and the other slot cannot detect the button state, which means even if we have two slots, only one can be used at a time. Your mission is to make it multi-tasking.

2. requirements
  In the assignment you need to implement "Hardware Interrupt" for detecting if the buttons are pressed, and "Timer Interrupt" to substidute delay(). 
  
  study reference
  - Hardware Interrupt: https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
  - Timer Interrupt: https://learn.adafruit.com/multi-tasking-the-arduino-part-2/timers
  
  
3. submission
  You will use Git to retrieve and submit the assignment.  Use `$git clone` to get a copy on your local repo, `$git push` to update the file locally, an `$git commit` to upload the file onto your branch named after your Slack display name.
  
  
4. accessment
  The result would be determined by th actually performance of the device, so if you need access to the device, please make a request at the knock_knock channel on Slack.
