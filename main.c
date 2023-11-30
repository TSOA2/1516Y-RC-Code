#pragma config(Motor,  port2,			MOTOR1,		   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,			MOTOR2,		   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,			MOTOR3,		   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,			MOTOR4,		   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,			CRANE_ROT,	   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,			CRANE_RAISE,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,			CRANE_GRAB,   tmotorVex393_MC29, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard				 !!*//

/*
 * Source code for the 1516Y Cal High Robotics Team
 * TSOA2, 2023
 *
 * Description:
 *	WARNING: this code is only in it's testing stages.
 *
 *	Currently this code accepts input from a RC controller
 *	and tests motor movement. While the robot is being designed,
 *	I am playing around with various algorithms.
 *
 *	You can accelerate two motors with the R and D buttons.
 *	This is mainly for testing the acceleration function,
 *	and will probably be later implemented into a turning
 *	function or something, or into future autonomous code.
 *
 *	There are also some other buttons that allow you to rotate
 *	the crane. That part isn't too exciting.
 *
 * Usage:
 *	This code cannot be compiled using any standard C compiler.
 *	This is a dialect called ROBOTC, and it will only work
 *	on Vex hardware. However, if you would like to run it,
 *	follow these steps:
 *
 *	- Make sure all motors are connected to respective ports.
 *	- Connect the Vex Cortex to a Windows computer via a USB-USB (male) cord.
 *	- Launch the ROBOTC Vex application.
 *	- Load the code into the text editor.
 *	- Compile the code.
 *	- Make sure the Vex Cortex is powered off.
 *	- Download the binary to the Cortex, and then unplug the cord.
 *	- Use the cord to connect the Cortex with the RC controller.
 *	- Make sure the Vex Cortex is attached to a battery.
 *	- Turn on the Vex Cortex, and then turn on the RC controller.
 */

/*
 * Button assignments for remote control
 */

#define CRANE_FWD_CHANNEL Btn6U
#define CRANE_RVS_CHANNEL Btn5U

#define CRANE_RAISE_CHANNEL Btn6D
#define CRANE_LOWER_CHANNEL Btn5D

#define CRANE_CLOSE_CHANNEL Btn7D
#define CRANE_OPEN_CHANNEL Btn8D

#define MOTOR_CHANNEL1 Ch3
#define MOTOR_CHANNEL2 Ch4

/*
 *	Motor specifics
 */

#define CRANE_TURN_TIME  (1) /* Default motor turn time (drive_time()) */
#define CRANE_TURN_SPEED (20) /* The speed at which the crane rotates */

#define CRANE_RAISE_TIME (1)
#define CRANE_RAISE_SPEED (20)

#define CRANE_GRAB_TIME (1)
#define CRANE_GRAB_SPEED (10)

#define DRIVE_LATENCY (10) /* The more increase, the more latency between the controller and the motor */

void drive_time(unsigned int m1, unsigned int time, signed char speed1)
{
	unsigned int start_time = time1[T1];
	unsigned int current_time = 0;
	for (;;) {
		motor[m1] = speed1;
		current_time = time1[T1];
		if ((current_time - start_time) >= time) {
			motor[m1] = 0;
			break;
		}
	}
}

void joystick_control(unsigned int chan1, unsigned int chan2, unsigned int m1, unsigned int m2, unsigned int m3, unsigned int m4)
{
	signed char speed = vexRT[chan1];
	signed char turn = vexRT[chan2];

	// motor 1 and 2 are right
	// motor 3 and 4 are left

	if (turn > 0) {
		motor[m1] = speed - turn;
		motor[m2] = speed - turn;
		motor[m3] = speed;
		motor[m4] = speed;
	} else {
		motor[m1] = speed;
		motor[m2] = speed;
		motor[m3] = speed + turn;
		motor[m4] = speed + turn;
	}
}

void crane_rot_control(unsigned int fwd_channel, unsigned int rvs_channel, unsigned int crane_rot_motor)
{
	if (vexRT[fwd_channel]) {
		drive_time(crane_rot_motor, CRANE_TURN_TIME, CRANE_TURN_SPEED);
	} else if (vexRT[rvs_channel]) {
		drive_time(crane_rot_motor, CRANE_TURN_TIME, -CRANE_TURN_SPEED);
	}
}

void crane_raise_control(unsigned int raise_channel, unsigned int lower_channel, unsigned int crane_raise_motor)
{
	if (vexRT[raise_channel]) {
		drive_time(crane_raise_motor, CRANE_RAISE_TIME, CRANE_RAISE_SPEED);
	} else if (vexRT[lower_channel]) {
		drive_time(crane_raise_motor, CRANE_RAISE_TIME, -CRANE_RAISE_SPEED);
	}
}

void crane_grab_control(unsigned int open_channel, unsigned int close_channel, unsigned int crane_grab_motor)
{
	if (vexRT[close_channel]) {
		drive_time(crane_grab_motor, CRANE_GRAB_TIME, -CRANE_GRAB_SPEED);
	} else if (vexRT[open_channel]) {
		drive_time(crane_grab_motor, CRANE_GRAB_TIME, CRANE_GRAB_SPEED);
	}
}

/* Poll RC commands */
void check_rc()
{
	joystick_control(MOTOR_CHANNEL1, MOTOR_CHANNEL2, MOTOR1, MOTOR2, MOTOR3, MOTOR4);
	crane_rot_control(CRANE_FWD_CHANNEL, CRANE_RVS_CHANNEL, CRANE_ROT);
	crane_raise_control(CRANE_RAISE_CHANNEL, CRANE_LOWER_CHANNEL, CRANE_RAISE);
	crane_grab_control(CRANE_OPEN_CHANNEL, CRANE_CLOSE_CHANNEL, CRANE_GRAB);
}

task main()
{
	/*
	 * Main control loop
	 */
	for (;;) {
		check_rc();
	}
}
