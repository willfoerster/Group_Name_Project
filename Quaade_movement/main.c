
//ATTENTION!!!!
//PLEASE IGNORE BELOW DIAGRAM!!!
//THis diagram was back when I was using my own Naming Convention
//Check servo.h for servo channels



/*This Program is designed to allow all the servos to be attached
 * to the skeleton in a predictable way.
 *
 * In order to have proper orientation and operation of the robot,
 * a consistent convention must be established. Below is a top-down
 * view of the robot. The boxes represent each servo and the name
 * assigned to it. Each Group (A-D) represents a limb, while each
 * number represents a joint (1 is shoulder, 2 is knee)
 *
 *    ____                         ____
 *   /    \                       /    \
 *  /  A2  \      TOP-DOWN       /  B2  \
 *  \______/                     \______/
 *         \ ______       ______ /
 *          |      |     |      |
 *          |  A1  |     |  B1  |
 *          |______|     |______|
 *              |           |
 *  FRONT ------------------------- BACK
 *           ___|__       __|___
 *          |      |     |      |
 *          |  D1  |     |  C1  |
 *          |______|     |______|
 *     ____/                     \____
 *    /    \                     /    \
 *   /  D2  \                   /  C2  \
 *   \______/                   \______/
 *
 * It is very important that all the Shoulder-Knee joints get assembled
 * EXACTLY the same. This will guarantee that all the Knee servos behave
 * the same.
 *
 * When the program is initially started, the servos will move to their
 * center postiton (90 degrees).
 *
 * For the first time using this program
 * MAKE SURE THAT NO SERVOS ARE SECURED TO THE SKELETON!! There is a
 * serious risk that the servos will collide with each other if they
 * aren't secured in the correct position.
 *
 * Once all the servos are centered, it is time to attach the servos
 * the skeleton in what is called the Neutral Position. In the Neutral
 * Position, the horn of the servo should be be in-line or parallel to
 * the body of the servo (or as close as possible). The Horn of the
 * servo is used to secure the servo in the correct position
 *
 * Below is the pinning information to connect servos to PCA9685 board
 *
 *   A1-->Group A, servo 1 to servo pin 0 (of PCA9685)
 *   A2-->Group A, servo 2 to servo pin 1 (of PCA9685)
 *   B1-->Group B, servo 1 to servo pin 2 (of PCA9685)
 *   B2-->Group B, servo 2 to servo pin 3 (of PCA9685)
 *   C1-->Group C, servo 1 to servo pin 4 (of PCA9685)
 *   C2-->Group C, servo 2 to servo pin 5 (of PCA9685)
 *   D1-->Group D, servo 1 to servo pin 6 (of PCA9685)
 *   D2-->Group D, servo 2 to servo pin 7 (of PCA9685)
 *
 * Once all the servos are secured in the Neutral Position,
 * pressing the button (S1) on the MSP432 will cause the robot to stand
 * up.
 *
 * IF THE ROBOT DOES NOT STAND UP CORRECTLY, SOMETHING ELSE IS WRONG!
 * Check:
 *       -Servo Orientation at the knee joints
 *       -Wiring to PCA
 *       -Servo Neutral Position
 *
 * MSP Wiring:
 *
 *         msp432p401r
 *      -----------------
 *     |                 |
 *     |                 |
 *     |             P1.7|-->SCL (PCA9685)
 *     |             P1.6|-->SDA (PCA9685)
 *     |                 |
 *     |                 |
 *     |              3V3|-->VCC (PCA9685)
 *     |              GND|-->GND (PCA9685)
 *     |                 |
 *     |_________________|
 *
 *
 *
 *  By: Tyler Davidson
 *  Date: July 17th, 2020
 *
 *
 *
 */

#include "msp.h"
#include "i2c.h"
#include "pca9685.h"
#include "servo.h"
#include "stIMU.h"
#include "stdio.h"



volatile int state = 0;


void main(void)
 {
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	//set up Button Interrupt
    P1->DIR &= ~BIT1;
    P1->OUT |= BIT1;
    P1->REN |= BIT1;
    P1->IES |= BIT1;
    P1->IFG = 0;
    P1->IE  |= BIT1;

    //enable interrupts
    NVIC_EnableIRQ(PORT1_IRQn);
	__enable_irq();

    I2C_OPEN_STRUCT_TypeDef i2c_open_struct;

    i2c_open_struct.uca10 = 0;                              //no self 10-bit Address
    i2c_open_struct.ucsla10 = 0;                            //no 10-bit Addressing
    i2c_open_struct.ucmm = 0;                               //No Multi-Master
    i2c_open_struct.ucmst = EUSCI_B_CTLW0_MST;              //Master Mode
    i2c_open_struct.ucsselx = EUSCI_B_CTLW0_SSEL__SMCLK;    //SMCLK to be selected (3MHz)
    i2c_open_struct.ucbrx = 30;                             //Prescaler for Selected Clock.
                                                            //(100kHz)

    //This sets up the I2C driver to operate with the
    //correct settings.
    //EUSCI_B0 uses P1.7 as SCL and P1.6 as SDA
    i2c_open(EUSCI_B0, &i2c_open_struct);

    pca9685_init(); //sets up PCA to output at the correct frequency
    //config_LIS3MDL();

    int delay;
    int angle;
    int target = 0;
    int stepcount = 0;

    //Forward movement
    while(1)
    {

        if (state == 0)
            {

            if (target == 0) //check for target: no target
                {
                //starting positions
                         servo_write(URL,90-60);
                         servo_write(LRL,90+10);
                         servo_write(URA,90+60);
                         servo_write(LRA,90+70);
                         servo_write(ULL,90+60);
                         servo_write(LLL,90+30);
                         servo_write(ULA,90-60);
                         servo_write(LLA,90-30);

                     for (angle = 1; angle < 40; angle = angle + 1) //Move Body Forwards: by moving all legs backwards
                     {
                         for (delay = 0; delay < 500; delay ++); // slow leg movement
                         servo_write(LRL,100 - angle);
                         servo_write(LRA,160 - angle);
                         servo_write(LLL,120 + angle);
                         servo_write(LLA,60 + angle);
                     }

                     servo_write(ULL,140); //lift left leg
                     for(angle = 1; angle < 80; angle = angle + 1) // move left leg forwards
                      {
                         for (delay = 0; delay < 500; delay ++); // slow leg movement
                         servo_write(LLL,160 - angle);
                      }
                     servo_write(ULL,150);//lower left leg
                     servo_write(ULA,40);//lift arm leg
                     for(angle = 1; angle < 80; angle = angle + 1)// move left arm forwards
                      {
                         for (delay = 0; delay < 500; delay ++); // slow leg movement
                         servo_write(LLA,100 - angle);
                      }
                     servo_write(ULA,30);//lower left arm
                     for (angle = 1; angle < 40; angle = angle + 1)//Move Body Forwards: by moving all legs backwards
                     {
                         for (delay = 0; delay < 500; delay ++); // slow leg movement
                         servo_write(LRL,60 - angle);
                         servo_write(LRA,120 - angle);
                         servo_write(LLL,80 + angle);
                         servo_write(LLA,20 + angle);
                     }
                     servo_write(URL,40);//lift right leg
                     for(angle = 1; angle < 80; angle = angle + 1)// move right leg forwards
                      {
                         for (delay = 0; delay < 500; delay ++); // slow leg movement
                         servo_write(LRL,20 + angle);
                      }
                     servo_write(URL,30);//lower right leg
                     servo_write(URA,140);//lift right arm
                     for(angle = 1; angle < 80; angle = angle + 1)// move right arm forwards
                      {
                         for (delay = 0; delay < 500; delay ++); // slow leg movement
                         servo_write(LRA,80 + angle);
                      }
                     servo_write(URA,150);//lower right arm

                     stepcount ++; // count steps so we can sweep for targets after 6 steps

                 if (stepcount == 5)
                 {
                    //camera movement with central turret
                    servo_write(VCT,95); //set vertical servo tilted slightly up
                    for (angle = 1; angle < 90; angle = angle + 1)
                    {
                        for (delay = 0; delay < 1000; delay ++); // slow turret movement
                        servo_write(HCT,90 + angle); // start looking forward then sweeps
                    }
                    for (angle = 1; angle < 180; angle = angle + 1)
                    {
                        for (delay = 0; delay < 1000; delay ++); // slow turret movement
                        servo_write(HCT,180 - angle); // sweeps 180-0 degrees
                    }
                    for (angle = 1; angle < 90; angle = angle + 1)
                    {
                        for (delay = 0; delay < 1000; delay ++); // slow turret movement
                        servo_write(HCT,0 + angle); // sweeps from 0-90 degrees
                    }
                    stepcount = 0;
                 }

                }
            else if (target == 1) // target found
                {
                    for (delay = 0; delay < 10000; delay ++); // stops turret to fire laser
                }


            }
        else if (state == 1)
        {
            //stop forward movement for targeting
            servo_write(URL,90); //90 Degrees represents the Neutral Position
            servo_write(LRL,90);
            servo_write(URA,90);
            servo_write(LRA,90);
            servo_write(ULL,90);
            servo_write(LLL,90);
            servo_write(ULA,90);
            servo_write(LLA,90);

        }
    }

}


/* Port1 ISR */
void PORT1_IRQHandler(void){
    volatile uint32_t j;

    //Change State to Standing Position
    if(P1->IFG & BIT1)
        state = 1;

    // Delay for switch debounce
    for(j = 0; j < 100000; j++)

    P1->IFG &= ~BIT1;
}

/*servo_write(URL,90); //90 Degrees represents the Neutral Position
        servo_write(LRL,90);
        servo_write(URA,90);
        servo_write(LRA,90);
        servo_write(ULL,90);
        servo_write(LLL,90);
        servo_write(ULA,90);
        servo_write(LLA,90);*/



/*
                for(angle = 1; angle < 180; angle = angle + 1)
                 {
                    servo_write(URL,angle); //Because the Orientations are opposite of the other servos,
                    servo_write(LRL,angle); //the direction the servo must move to stand up is also opposote
                    servo_write(URA,angle);
                    servo_write(LRA,angle);
                    servo_write(ULL,angle);
                    servo_write(LLL,angle);
                    servo_write(ULA,angle); //See Above Comment
                    servo_write(LLA,angle);
                 }*/
            /*
                servo_write(URL,40);
                for(angle = 1; angle < 40; angle = angle + 1)
                 {
                    for (delay = 0; delay < 2000; delay ++);
                    servo_write(LRL,60 + angle);
                 }
                servo_write(URL,30);
                servo_write(URA,140);
                for(angle = 1; angle < 40; angle = angle + 1)
                 {
                    for (delay = 0; delay < 2000; delay ++);
                    servo_write(LRA,120 + angle);
                 }
                servo_write(URA,150);*/
      /*servo_write(URL,90); //90 Degrees represents the Neutral Position
        servo_write(LRL,90);
        servo_write(URA,90);
        servo_write(LRA,90);
        servo_write(ULL,90);
        servo_write(LLL,90);
        servo_write(ULA,90);
        servo_write(LLA,90);

        servo_write(URL,90); //90 Degrees represents the Neutral Position
        servo_write(LRL,90);
        servo_write(URA,90);
        servo_write(LRA,90);
        servo_write(ULL,90);
        servo_write(LLL,90);
        servo_write(ULA,90);
        servo_write(LLA,90);

        servo_write(URL,90-45); //Because the Orientations are opposite of the other servos,
        servo_write(LRL,90-45); //the direction the servo must move to stand up is also opposote
        servo_write(URA,90+45);
        servo_write(LRA,90+45);
        servo_write(ULL,90+45);
        servo_write(LLL,90+45);
        servo_write(ULA,90-45); //See Above Comment
        servo_write(LLA,90-45);

        */
/*
    int16_t mx, my, mz;
    uint8_t data;
    int i;

    }*/
