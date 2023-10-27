/*
 * Source code for the 1516Y Cal High Robotics Team
 * TSOA2, 2023
 *
 * Description:
 * 	WARNING: this code is only in it's testing stages.
 *
 * 	Currently this code accepts input from a RC controller
 * 	and tests motor movement. While the robot is being designed,
 * 	I am playing around with various algorithms.
 *
 *	You can accelerate two motors with the R and D buttons.
 *	This is mainly for testing the acceleration function,
 *	and will probably be later implemented into a turning
 *	function or something, or into future autonomous code.
 *
 * 	There are also some other buttons that allow you to rotate
 * 	the crane. That part isn't too exciting.
 *
 * Usage:
 * 	This code cannot be compiled using any standard C compiler.
 * 	This is a dialect called ROBOTC, and it will only work
 * 	on Vex hardware. However, if you would like to run it,
 * 	follow these steps:
 *
 *	- Make sure all motors are connected to respective ports.
 * 	- Connect the Vex Cortex to a Windows computer via a USB-USB (male) cord.
 * 	- Launch the ROBOTC Vex application.
 * 	- Load the code into the text editor.
 * 	- Compile the code.
 * 	- Make sure the Vex Cortex is powered off.
 * 	- Download the binary to the Cortex, and then unplug the cord.
 * 	- Use the cord to connect the Cortex with the RC controller.
 * 	- Make sure the Vex Cortex is attached to a battery.
 * 	- Turn on the Vex Cortex, and then turn on the RC controller.
 */

#pragma config(Motor,  port2,           MOTOR1,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           MOTOR2,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           CRANE,         tmotorVex393_MC29, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
 * Button assignments for remote control
 */
 
#define CRANE_FWD_CHANNEL Btn6U
#define CRANE_RVS_CHANNEL Btn5U

#define MOTOR_CHANNEL1 Btn8R
#define MOTOR_CHANNEL2 Btn8D

/*
 *  Motor specifics
 */
 
#define CRANE_TURN_TIME  (1) /* Default motor turn time (drive_time()) */
#define CRANE_TURN_SPEED (45) /* The speed at which the crane rotates */

#define DRIVE_ACCEL (1) /* Acceleration */
#define DRIVE_LATENCY (10) /* The more increase, the more latency between you and the motor */

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

void duo_drive_time(unsigned int m1, unsigned int m2, unsigned int time, signed char speed1, signed char speed2)
{
	unsigned int start_time = time1[T1];
	unsigned int current_time = 0;
	for (;;){
		motor[m1] = speed1;
		motor[m2] = speed2;
		current_time = time1[T1];
		if ((current_time - start_time) >= time) {
			motor[m1] = 0;
			motor[m2] = 0;
			break;
		}
	}
}

void drive_accel(unsigned int chan1, unsigned int chan2, unsigned int m1, unsigned int m2)
{
	signed char velocity1 = 0;
	signed char velocity2 = 0;
accel_start:
	while (vexRT[chan1] || vexRT[chan2]) {
		duo_drive_time(m1, m2, DRIVE_LATENCY, velocity1, velocity2);
		if (vexRT[chan1]) {
			if (velocity1 >= vexRT[chan1]) {
				velocity1 = vexRT[chan1];
			} else {
				velocity1 += DRIVE_ACCEL;
			}
		}
		if (vexRT[chan2]) {
			if (velocity2 >= vexRT[chan2]) {
				velocity2 = vexRT[chan2];
			} else {
				velocity2 += DRIVE_ACCEL;
			}
		}
	}

	while (velocity1 > 0 && velocity2 > 0) {
		duo_drive_time(m1, m2, DRIVE_LATENCY, velocity1, velocity2);
		if (velocity1 > 0) {
			velocity1 -= DRIVE_ACCEL;
		}
		if (velocity2 > 0) {
			velocity2 -= DRIVE_ACCEL;
		}
		if (vexRT[chan1] || vexRT[chan2]) {
			goto accel_start;
		}
	}
}

void check_rc()
{
	if (vexRT[CRANE_FWD_CHANNEL]) {
		drive_time(CRANE, CRANE_TURN_TIME, CRANE_TURN_SPEED);
		//motor[CRANE_ROTATE] = CRANE_TURN_SPEED;
	} else if (vexRT[CRANE_RVS_CHANNEL]) {
		drive_time(CRANE, CRANE_TURN_TIME, -CRANE_TURN_SPEED);
		//motor[CRANE_ROTATE] = -CRANE_TURN_SPEED;
	}

	if (vexRT[MOTOR_CHANNEL1] || vexRT[MOTOR_CHANNEL2]) {
		drive_accel(MOTOR_CHANNEL1, MOTOR_CHANNEL2, MOTOR1, MOTOR2);
	}

	//motor[CRANE_ROTATE] = 0;
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