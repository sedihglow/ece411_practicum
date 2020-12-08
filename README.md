# ece411_practicum
ece411 practicum project Team 16

# how to upload to ESP
1. Start upload from arduino IDE.
  - It still sit on connecting for this time.
2. Pull the reset pin low or unplug the device
3. Pull GPIO2 low with a button.
4. Let the reset pin go high or plug back in the device while gpio2 is low
  - The code should start uploading
5. let go of the GPIO2 button to ground.

# ESP program flow
1. Check for user packet
2. if user sent a packet, handle the request for moisture threshold or pump time
3. read the moisture sensor
4. If the moisture sensor reads below threshold and water is in the tank, water plant
5. repeat 1-4

# user program
To compile use the makefile $make
To run you can use the makefile $make run
The user program will prompt the user with a menu
  - change moisture threshold
  - change pump time
  - exit program
  
The moisture threshold can be values of 1 to 1000

The pump time can be between 1 and 10 seconds

