# Arduino-Project-PetFeeder

Hi everyone, This also is a final project, i did it in the high school. If you have see my others repositories, you will notice the most of the projects i have uploaded are projects made in the high school. It is like @ITLP-Final-Project-Hardware-Page. 

Ok, let me clarify one thing before at start... This is my first close up with Arduino and ESP32, The reason by i uploaded this uncomplete project is beacause i want to see, in few years or few months, how much I have progressed. So don't be so critical :D. further i am planining follow these projects and finish for will add to my future briefcase. 

I will explain you this code works. In the same way as the others project, the school requested us to make a proyect based in Arduino (Code), but this time, we shoulded add web services, we used the ESP32.

With my friends, we made a program for the Esp32, that control a feed dispenser. It consists mainly of a stepper motor, and the structure was thinked to be impresed by a 3D Printer.

Unfortunately we were never able to carry out, because we didnt had the materials to build it, but we were able to test the electronics.
The program integrate librarys and APIs of time and date, webserver, asynchronous webserver, etc.

The steps follow the program are:

It Takes the hour from a NTP server, and It allocates in a variable, for after comparate it with other variable defined by the user. When both variables are same, It calls a function, that moves the stepper motor and release the food on the container.
With a one of the librarys we can storage a webpage and the variables in the flash memory of the esp32. The webppage is used as User Interface (UI), this allows the user configure the time at which we want the food to release.
It is basically an automatic programming feeding system.

As with my other high school repositories, I leave the guide or explanatory report of all project.
