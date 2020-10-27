# Zombie_Arena

This is a personal project. The purpose is to train myself to game developpement.
This is based on the Book "Beginning C++ Game Programming *Second Edition*" of John Horton. 

# Brief

Zombie Arena is a mini game where the player has to suvive at multiple zombies waves.
To achieve this, He cans shoot zombies and pick boosts up like ammos or health pack.

# How to install 


First, you have to install the SFML library (it's free) depends on you configurations at: https://www.sfml-dev.org/download.php

Then setting up you IDE, (personaly I use Visual Studio) this way it will manage the dependencies for you. 
In VS, here the way to go:

1. Open the solution by Visual studio
2. Now the project is open, from the main menu select Zombie_Arena properties (or a wrench icon near the top right),
At this stage you should have a property window open
3. Select all configuration from the configuration dropdown menu
4. Select C/C++ and General from left-hand menu
5. Now locate the Additional Include edit box and type the full-path of your SFML include folder.
As example for me it's `D:\Application\SFML-2.5.1\include`be sure that is the **full path**.
6. Then click on Apply to save what you have done so far
7. Now from the left-hand menu select Linker and General
8. Find the Additional Library Directories edit box and type as before the full-path of the `SFML\lib`directory.
For me it is `D:\Application\SFML-2.5.1\lib`. Vary depends on how and where you have install SFML.
9. Then click on Apply to save
10. As you perform before, switch the Configuration dropdown menu to Debug
11. Select Linker and then Input
12. Find Addition Dependencies edit box and click on it to add the following 
`sfml-graphics-d.lib;sfml-window-d.lib;sfml-system-d.lib;sfml-network-d.lib;sfml-audio-d.lib;`
Be careful to not overwrite or delete any text that is already here.
13. Click on OK
14. Click Apply and OK

Now, you have completed the configuration of the project. you can look at the code or run it by hitting F5.

# Let's enjoy the game

The game is now in an early state, some others features may be added in the future.


