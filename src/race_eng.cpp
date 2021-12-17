#include "ros/ros.h"
#include "std_srvs/Empty.h"
#include "second_assignment/Speedvar.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <string.h> 

int main(int argc, char **argv){

	//main initializer
	ros::init(argc,argv,"race_eng");
	ros::NodeHandle nh;

// Services
//------------------------------------//
	ros::ServiceClient res_client;
	res_client = nh.serviceClient<std_srvs::Empty>("/reset_positions");
	std_srvs::Empty var;

	ros::ServiceClient speedvar_client;
	speedvar_client = nh.serviceClient<second_assignment::Speedvar>("/speedvar");
	second_assignment::Speedvar decision;
	

	//ros::ServiceClient client4 =nh.serviceClient<turtlebot_controller::Velocity>("/velocity");
	//turtlebot_controller::Velocity vel_srv;


	char command =' ';

		printf("Possible Options:\n");
	printf("--------------------------------------------------------\n");
	printf("Speed up: w\n");
	printf("\nSlow down: s\n");
	printf("\nReset position: p\n");
	printf("\nReset speed variation: r\n");
	printf("\nQuit the race and close everything: q\n");
	printf("Enter commands as characters from keyboard:\n");
	printf("--------------------------------------------------------\n");
	fflush(stdout);

	while((command=getchar()) != EOF){
		
		switch(command){

			case 'w':
			
			printf("\nreceived w, speeding up\n");
			
			decision.request.command='w';
			speedvar_client.waitForExistence();
			speedvar_client.call(decision);
			printf("speed variation is: %f\n",decision.response.variation);
			printf("current speed is: %f\n",decision.response.current_speed);
			printf("\nInput another command\n");
			
			break;
			
			case's':
			
			printf("\nreceived s, slowing down\n");
			
			decision.request.command='s';
			speedvar_client.waitForExistence();
			speedvar_client.call(decision);
			printf("speed variation is: %f\n",decision.response.variation);
			printf("current speed is: %f\n",decision.response.current_speed);
			printf("\nInput another command\n");
			
			break;

			case 'r':

			printf("\nreceived r, canceling speed variation\n");
			
			decision.request.command='r';
			speedvar_client.waitForExistence();
			speedvar_client.call(decision);
			printf("speed variation is: %f\n",decision.response.variation);
			printf("current speed is: %f\n",decision.response.current_speed);


			printf("\nInput another command\n");

			break;
			
			case 'p':
			
			printf("\nreceived p, resetting positions\n");
			
			res_client.waitForExistence();
			res_client.call(var);
			
			printf("\nInput another command\n");
			
			break;

			case 'q':

			printf("You decided to quit the race\n");
			return 0;

			break;
			
			default:
			//do nothing
			;

			break;
		}



	ros::spinOnce();
    
    }//while loop
	
	return 0;
}