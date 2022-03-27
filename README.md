# Arduino-Project-PetFeeder
Hi everyone, This also is a final project i did in the high school. If you have see my others repositories, you notice what basically the most of the projects i upload are projects what i did in the high school. I tell you the same what i did tell in the @ITLP-Final-Project-Hardware-Page. this is my first close up with Arduino and ESP32, The reason waht i uploaded this uncomplete and amateur project is beacause want to see, in few years, how much I have improved. So don't be so critical :D.
I will explain to you what is this project. In the same way with the others project, the school requested us to make a proyect based in Arduino (Code), but this time aditionaly using the ESP32, this for the comunicattion and allocate a webpage.
With my friends, we made a program for the Esp32 what controls a feed dispenser for any mascot. it consist mainly of a stepper motor, and the structure is thinked to be impresed with a 3D Printer. 
Unfortunately we were never able to carry out, because we didnt had the materials to build it, but we were able to test the electronics.
The program integrate librarys and APIs of time and date, webserver, asynchronous webserver, etc. Basically the function of the project is:
Takes a hour from a NTP server, and its allocate in a variable, for after comparate it with a variable defined by the user. When both variables are same, its call a function what moves the stepper motor and release the food on the food container of our mascot. 
With a one of the librarys we able storage a webpage and the variables in the flash memory of the esp32. This webpage allows us used it like a User Interface (UI).
From that we can configure the hour what we want the food release.
It is basically an automatic feeding system.
As with my other high school repositories, I leave the guide or explanatory report of all project.
