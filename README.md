# ece411_practicum
ece411 practicum project Team 16

# how to upload to ESP
1. Start upload from arduino IDE.
  - It will sit on connecting for this time.
2. Pull the reset pin low or unplug the device
3. Pull GPIO2 low with a button.
4. Let the reset pin go high or plug back in the device while gpio2 is low
  - The code should start uploading
5. let go of the GPIO2 button to ground.

# ESP setup program
1. Setup sensor pins
2. Setup I2C for moisture sensor
3. Try to connect to wifi using credentials in EEPROM
4. If credentials are bad, start a wifi hotspot on ESP
5. Connect to ESP using its wifi hotspot by connecting to the IP Address from a web browser
6. Once credentials are changed it reconnects using the new credentials.

# ESP main loop program flow
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

