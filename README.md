# Research Track Second Assignment

### Exercise Objectives ###

The robot should move autonomously through a race track, withouth hitting the margins of the track. 
The user must be able to change the robot's speed through the use of a service. A minimum of two ros
nodes must be used. The nodes should comunicate by using the tools provided by the ros system.

### Chosen Approach to Complete the Objectives ### 

The main idea behind this solution is that the robot should keep driving straight until it gets 
too close to the wall in front of it. When the distance from the front wall falls under a given 
threshold, the robot will start regulating its linear (or tangential) speed in proportion to the 
distance from the closest bit of wall in front of it. While the linear speed gets proportionally 
reduced, an angular component to the total speed is added, so that the robot can move away from 
the closest wall facing its sides. When the robot gets too close to a wall, its linear speed is 
set to zero and the robot is only allowed to turn on its vertical axis.

The node that links user inputs with he robot's behaviour is called race_eng.cpp while the node 
controlling the robot's movements is called pilot.cpp. The pilot node is both a subscriber to it's
simulated sensors' node and a server for the service occasionally called by the race_eng node to
slow down or speed up the robot. The influence of this "controller" node on the general speed of
the robot is small but measurable and its impact is sent back to the race_eng node as a response
from the aformentioned service. The race_eng node can also quit the whole simulation, killing all 
processes with it, when the simulation is launched with the launch file found in this repository.
 

### Pseudo-code for the Solution that has Been Implemented ###
#### pilot.cpp ####
    
    subscribe to the /base_scan topic
    
    initialise as a pubblisher for motor commands on the /cmd_vel topic
    
    initialise as a server for the service /speedvar
    
    wait for new messages on the /base_scan topic
    
        when a message arrives:
              
              copy the ranges array into a temporary array
              
              save the shortest range in the "front side" interval
              save the shortest range in the "left side" interval
              save the shortest range in the "right side" interval
              
              save the shortest distance
              
                if (front range over 9) : drive straight 
              
                if not: is left wall closer than the right wall?
                
                        if yes: is the closest wall far enough?
                              
                              if yes: turn right and keep going straight
                              
                              if not: turn right but don't move forward
                       
                        if not: is the closest wall far enough?
                              
                              if yes: turn left and keep going straight
                              
                              if not: turn left but don't move forward
                
              publish the decision taken on the cmd_vel topic
              
        
        when the service /speedvar is called:      
        
                take the command field from the request:
                    
                    if it's w: increase base speed
                    
                    if it's s: decrease base speed
                    
                    if it's r: set base speed to zero
        
                send current speed and applied variation as a response to the caller


#### race_eng.cpp ####

    initialise as a ros node
    initialise as a client to the service /reset_positions
    initialise as a client to the service /speedvar
    
    while(the command !=EOF)
        read command from user:
            
            if it's w, s, or r: call /speedvar and send them as requests 
            if it's p: call /reset_positions
            if it's q: return 0, exit the program

### Contents of the Package ###

- launch folder: for the go.launch script needed to run everything

- src folder: for the two .cpp scripts of the nodes developed for this assignment

- srv folder: for the Speedvar.srv file used to define the structure of the service's messages

- world folder: contains the script for the simulated environment and the image of the track

- CmakeLists.txt and package.xml files are for ros functionalities

	
### Running the Simulator ###

After downloading this pre-built package, running this launch command from the terminal will start the ros system
and all the nodes necessary to run this simulation af described in this readme file. 

```
$ roslaunch second_assignment go.launch
```


### Future Improvements ###

Although the robot can finish the track while maintaining a decent speed, it's movements aren't as fluid as they
could be afer some extensive tuning of the constant parameters present in its pilot.cpp script. The way the ranges
array was passed from its message structure to the script could also be improved with a correct use of pointers.
While it is possible to change the speed of the robot from the race_eng node, the robot's autonomous 
speed_regulation system largely overpowers the effects of a speed change issued by the user. A simpler 
autonomous_driving model, with constant speeds for example, would more easily show the effects of user intervention
on the robot's movements.


