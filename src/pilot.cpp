#include "ros/ros.h"

//topics' message types
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/LaserScan.h"

//server message types
#include "second_assignment/Speedvar.h"

#include <string.h> 


//The robot takes 720 readings for 180 degrees, so 4 readings for each degree
//We consider three separate sectors with these margins:

const int left_max=40;   //10 degrees for each sector
const int front_min=340;
const int front_max=380;
const int right_min=680;

//some general speeds
const int cruising_speed=10;
const int turn_left=1;
const int turn_right=-1;

const float min_th=0.3;
const float speed_coeff=0.7;
//declaring this globally because it's also used
//in the callback function
ros::Publisher pub;

int i;
float speedvariations=0.0;
float speed=0;
float last_speed;

//This function is called by the service
bool speedvar (second_assignment::Speedvar::Request &req, second_assignment::Speedvar::Response &res ){

	//interpret the command
	switch(req.command){

		case'w':
			speed=speed+0.1;
		break;

		case's':
			speed=speed-0.1;
		break;

		case'r':
			speed=0;
		break;	
	}//end of switch
    
    printf("speedvar is %f\n",speed);
	res.variation=speed;
	res.current_speed=speed+last_speed;
	return true;

}//end of speedvar

//This function returns th minimum valur in any float array
float check_dist(float array[], int arr_min, int arr_max){	
	int i;
	float max_dist;
	float tmp_dist=-32;

	for(i=arr_min;i<arr_max;i++){
	//	printf("%d ",i);
		tmp_dist = -array[i]>tmp_dist ? -array[i] : tmp_dist;
	
	}
	printf("---\n");
	return -tmp_dist;
}

float find_min(float num1,float num2,float num3){
	float min =-32;
	min = -num1 > min ? -num1 : min;
	min = -num2 > min ? -num2 : min;
	min = -num3 > min ? -num3 : min;

	return -min;
}

void pilotCallback(const sensor_msgs::LaserScan::ConstPtr& msg){

	//int dim_ranges = msg->ranges.size();
	//ROS_INFO("DIMENSION: %d, left front right [%f,%f,%f]\n",dim_ranges, msg->ranges[0],msg->ranges[360],msg->ranges[720]);

	
	float front_dist, left_dist, right_dist, min_dist;
	float tmp_dist=0.0;
	float tmp[721];

	for(i=0;i<720;i++){
		tmp[i]=msg->ranges[i];
//		printf(" (%d,%f)",i,tmp[i]);
	}

	printf("---\n");
	front_dist=check_dist(tmp,front_min, front_max);
	printf("front dist is %f\n", front_dist);	
	left_dist=check_dist(tmp,0, left_max);
	printf("left dist is %f\n", left_dist);
	right_dist=check_dist(tmp,right_min, 720);
	printf("right dist is %f\n", right_dist);

	min_dist=find_min(front_dist,left_dist, right_dist);
	printf("min dist is:%f\n", min_dist);

	geometry_msgs::Twist directions;
///*

	speedvariations=speed;
	printf("[speedvar is %f]\n",speedvariations);



	if(front_dist>9){
		directions.linear.x= speedvariations + cruising_speed;
		directions.angular.z=0;
	
	}else{
		if(left_dist>right_dist){
			
			if(min_dist>min_th){

				directions.linear.x = speedvariations + front_dist*speed_coeff;
			
			}else{

				directions.linear.x = 0;
			}
			
			directions.angular.z = turn_right;
		
		}else{
			
			if(min_dist>min_th){

				directions.linear.x = speedvariations + front_dist*speed_coeff;
			
			}else{

				directions.linear.x = 0;
			}
			directions.angular.z = turn_left;
		
		}

	}

	if(directions.linear.x<0){
		directions.angular.x=0;
		directions.linear.x=0;
		printf("\nThe car stopped, reset position or speed up again\n");
	}

	last_speed=directions.linear.x;
	pub.publish(directions);



//*/
	
}//end of callback


int main(int argc, char **argv){
	//main initializer
	ros::init(argc,argv,"pilot");
	ros::NodeHandle nh;

// Intializing topics
//------------------------------------//
	
	//subscribing to sensors
	ros::Subscriber sub;
	sub=nh.subscribe("/base_scan",1,pilotCallback);
	

	//publishing motor commands
	pub=nh.advertise<geometry_msgs::Twist>("/cmd_vel",1);

// Initializing services
//------------------------------------//

	// Initializing the speed_var service as a server:
	ros::ServiceServer service;
	service = nh.advertiseService("/speedvar",speedvar);
	printf("advertised\n");
	fflush(stdout);
	

	ros::spin();


	return 0;
}