# Nikita Hakala 24TIETOA
Coursework for C++ -programming basics class, aiming for 5 points

Basic functions:
- 
- The program either generates a random number (40-300) dividend by 2 or reads a number from the top of .txt file (based on user input)
  - This number indicates how many hotel rooms there are
  - Half of these rooms are singed to be 1 person rooms and the other half 2 person rooms
  - Single rooms cost 100 euros / night
  - Two person rooms cost 150 euros / night
 

- New booking
  - Asks for booking details
  - Possibility to discard booking before saving it
  - Creates random number between 10000-99999 for booking number (checks if unique)

- Search for a booking
  - By name
  - By booking number
    - Check-out
      - After searching for the right booking there is a possibility to check-out
      - The room becomes available again
 
- Show hotel info
  - Shows occupancy of all rooms and total of all available rooms by type
 
The program always checks for invalid inputs

File syntax
-
First row indicates total amount of hotel rooms   
Room number  
Booker name (empty if room is available)  
Room type (1 or 2)  
Total price (0 if room is available)  
Booking number (0 if room is available)  
Occupancy (0 for available and 1 if not)  
