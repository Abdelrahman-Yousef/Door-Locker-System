# Door-Locker-System
## I implemented the Door Locker Security System to unlock a door using a password with the specifications listed below:
### 1- I used two ATmega32 Microcontrollers with frequency 1Mhz.
### 2- The project is designed and implemented based on the layered architecture model.
### ℳc1 → HMI_ECU (Human Machine Interface) with 4x16 LCD and 4x4 keypad.
### ℳc2 → Control_ECU with EEPROM, Buzzer, and Dc-Motor.
### 3- HMI_ECU is just responsible for interaction with the user just take inputs through keypad and display messages on the LCD.
### 4- CONTROL_ECU is responsible for all the processing and decisions in the system like password checking, open the door and activate the system alarm.
### 5- System Sequence:
### Step1 – Create a System Password
         - The LCD displays “Please Enter Password”.
         - The user enters a password consists of 5 numbers, I displays * in the screen for each number.
         - The user presses enter button.
         - I asks the user to renter the same password for confirmation by display this message “Please re-enter the same Pass”.
         - The user enters a password consists of 5 numbers, I displays * in the screen for each number. 
         - the user presses enter button.
         - HMI_ECU sends the two passwords to the Control_ECU through the UART.
         - If the two passwords are matched then the system has a password now and I saves it inside the EEPORM and go to Step 2.
         - If the two passwords are unmatched then repeat step 1 again.
### Step2 - Main Options
         -  The LCD will always display the main system option (open door or change pass).
### Step3 - Open Door
         - The LCD displays “Please Enter Password”.
         - the user enters the password then presses enter button. 
         - HMI_ECU sends the Password to the Control_ECU and it compares it with the one saved in the EEPROM.
         - if two passwords are matched:
              • rotates motor for 15-seconds CW and display a message on the screen “Door is Unlocking”.
              • hold the motor for 3-seconds.
              • rotates motor for 15-seconds A-CW and display a message on the screen “Door is Locking”.
 ### Step 4 - Change Password
          - The LCD displays “Please Enter Password”.
          - The user enters the password then presses enter button. 
          - HMI_ECU sends the Password to the Control_ECU and it compares it with the one saved in the EEPROM.
          - if two passwords are matched:
               • Repeat Step 1. 
 ### Step 5
          - if the two passwords are unmatched at step 3 (Open Door) or step 4 (Change Password).
          - I asks the user one more time for the password.
          - The LCD displays “Please Enter Password”.
          - The user enters the password then presses enter button. 
          - HMI_ECU sends the password to the Control_ECU and it compares it with the one saved in the EEPROM.
          - if two passwords are matched then open the door or change the password in steps 3 and 4.
          - if the two passwords are not matched again then I asks the user one last time for the password.
          - if two passwords are matched then open the door or change the password in steps 3 and 4.
          - if the two passwords are not matched for the third consecutive time, then:
               • Activate Buzzer for 1-minute.
               • Display error message on LCD for 1 minute.
               • System is locked no inputs from Keypad will be accepted during this time period.
               • Go to Step 2 the main options again.
 
