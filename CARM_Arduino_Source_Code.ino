/*****************************************************************************/
/*****************************************************************************/
/*                     Robot Car-Arm System                             */
/*****************************************************************************/
/*                                                           */
/*                           Final Project                              */
/*           Smart Construction Transportation System(Smart CTS)             */
/*                Due on 23 December 2019 (Mon)                       */
/*                                                           */
/*****************************************************************************/
/*             Programming Copright & Environment :                          */
/* Team Name : CARM                                                     */
/* Author : 최성봉, 이성주, 이동규, 가인온투                                   */
/* Date : 2019.12.22                                           */
/* PL : C / C++                                                   */
/*---------------------------------------------------------------------------*/
/* Objective : Script for a smart construction transportation system       */
/*             run on arduino                                     */
/*---------------------------------------------------------------------------*/


/*****************************************************************************/
/*                      Included Header Files                      */
/*****************************************************************************/
#include <Servo.h>
#include <ArduinoJson.h>

/*****************************************************************************/
/*                         DEFINE's                            */
/*****************************************************************************/

/* Automobile */
#define ENA 6
#define EN1 7
#define EN2 3

#define EN3 4
#define EN4 2
#define ENB 5

/* Bluetooth */
#define HC06 Serial3

/*Line Tracer */
#define LT_L A3
#define LT_C A4
#define LT_R A5

/*****************************************************************************/
/*                      Global Variables                         */
/*****************************************************************************/

/* Robot_Arm Servo Motor */
Servo servo1, servo2, servo3, servo4;

char automobile_Direction = 'S';

/* Automobile Speed Variable */
int speed = 110;
int Automobile_Right_speed = 122;
int Automobile_Left_speed = 125;

/*****************************************************************************/
/*                       parsing function                         */
/*****************************************************************************/
/*****************************************************************************/
/* Input : N/A                                                  */
/* Output: Response                                               */
/* Logic : When Bluetooth available, read bluetooth, add to another variable */
/*          and return the variable                                       */
/*****************************************************************************/
String parsing()
{
    String response = "";
    if (HC06.available())
    {
        long int time = millis(); //현재 시간 설정
        int timeout = 3000;       //ms
        while ((time + timeout) > millis())
        {
            while (HC06.available())
            {
                char c = HC06.read();
                response += c;
            }
        }
    }
    return response;
}

/*****************************************************************************/
/*                        line function                         */
/*****************************************************************************/
/*****************************************************************************/
/* Input :   N/A                                                    */
/* Output:   N/A                                                  */
/* Logic :   Configures all 3 line tracing sensors as Input                 */
/*****************************************************************************/
void line()
{
    pinMode(LT_L, INPUT);
    pinMode(LT_C, INPUT);
    pinMode(LT_R, INPUT);
}
/*****************************************************************************/
/*                     lt_isLeft function                         */
/*****************************************************************************/
/*****************************************************************************/
/* Input :    N/A                                                    */
/* Output:  true/false                                           */
/* Logic :   If left tracer detects light return true, otherwise false       */
/*****************************************************************************/
bool lt_isLeft()
{
    int ret = digitalRead(LT_L);
    return ret == 1 ? true : false;
}
/*****************************************************************************/
/*                      lt_isRight fucntion                      */
/*****************************************************************************/
/*****************************************************************************/
/* Input :    N/A                                                    */
/* Output:  true/false                                            */
/* Logic :   If right tracer detects light return true, otherwise false       */
/*****************************************************************************/
bool lt_isRight()
{
    int ret = digitalRead(LT_R);
    return ret == 1 ? true : false;
}
/*****************************************************************************/
/*                      lt_isCenter function                      */
/*****************************************************************************/
/*****************************************************************************/
/* Input :    N/A                                                    */
/* Output:  true/false                                            */
/* Logic :   If center tracer detects light return true, otherwise false       */
/*****************************************************************************/
bool lt_isCenter()
{
    int ret = digitalRead(LT_C);
    return ret == 1 ? true : false;
}
/*****************************************************************************/
/*                      Automobile_Forward function                */
/*****************************************************************************/
/*****************************************************************************/
/* Input :  N/A                                                  */
/* Output:    N/A                                                  */
/* Logic :   Makes the robot-car go forward                               */
/*****************************************************************************/

void Automobile_Forward()
{
    digitalWrite(EN1, HIGH);
    digitalWrite(EN2, LOW);
    analogWrite(ENA, speed);

    digitalWrite(EN3, LOW);
    digitalWrite(EN4, HIGH);
    analogWrite(ENB, speed);
}
/*****************************************************************************/
/*                      Automobile_Left function                       */
/*****************************************************************************/
/*****************************************************************************/
/* Input :  N/A                                                  */
/* Output:    N/A                                                  */
/* Logic :   Makes the robot-car turn left                               */
/*****************************************************************************/

void Automobile_Left()
{
    digitalWrite(EN1, LOW);
    digitalWrite(EN2, HIGH);
    analogWrite(ENA, Automobile_Left_speed);
    digitalWrite(EN3, LOW);
    digitalWrite(EN4, HIGH);
    analogWrite(ENB, Automobile_Left_speed);
}
/*****************************************************************************/
/*                      Automobile_Right function                       */
/*****************************************************************************/
/*****************************************************************************/
/* Input :  N/A                                                  */
/* Output:    N/A                                                  */
/* Logic :   Makes the robot-car turn right                               */
/*****************************************************************************/
void Automobile_Right()
{
    digitalWrite(EN1, HIGH);
    digitalWrite(EN2, LOW);
    analogWrite(ENA, Automobile_Right_speed);

    digitalWrite(EN3, HIGH);
    digitalWrite(EN4, LOW);
    analogWrite(ENB, Automobile_Right_speed);
}
/*****************************************************************************/
/*                      Automobile_Backward function                  */
/*****************************************************************************/
/*****************************************************************************/
/* Input :  N/A                                                  */
/* Output:    N/A                                                  */
/* Logic :   Makes the robot-car go reverse                               */
/*****************************************************************************/
void Automobile_Backward()
{
    digitalWrite(EN1, LOW);
    digitalWrite(EN2, HIGH);
    analogWrite(ENA, speed);

    digitalWrite(EN3, HIGH);
    digitalWrite(EN4, LOW);
    analogWrite(ENB, speed);
}
/*****************************************************************************/
/*                      Automobile_Stop function                       */
/*****************************************************************************/
/*****************************************************************************/
/* Input :  N/A                                                  */
/* Output:    N/A                                                  */
/* Logic :   Makes the robot-car motionless                               */
/*****************************************************************************/
void Automobile_Stop()
{
    digitalWrite(EN1, LOW);
    digitalWrite(EN2, LOW);
    analogWrite(ENA, 0);
    digitalWrite(EN3, LOW);
    digitalWrite(EN4, LOW);
    analogWrite(ENB, 0);
}
/*****************************************************************************/
/*                      lt_mode_update function                       */
/*****************************************************************************/
/*****************************************************************************/
/* Input :  N/A                                                  */
/* Output:    N/A                                                  */
/* Logic :   update car status according to line tracing sensor update          */
/*****************************************************************************/
void lt_mode_update()
{
    while (true)
    {
        if (automobile_Direction == 'U' || automobile_Direction == 'D')
            break;
        int ll = lt_isLeft();
        int cc = lt_isCenter();
        int rr = lt_isRight();

        if (cc && ll && rr)
        {
            if (automobile_Direction == 'L' || automobile_Direction == 'R')
            {
                Automobile_Stop();
                break;
            }
            else
                Automobile_Forward();
        }
        else if ((cc && ll && !rr) || (!cc && ll && !rr)) // 왼쪽만 인식되면 좌회전
            Automobile_Left();
        else if ((cc && !ll && rr) || (!cc && !ll && rr)) //  오른쪽 인식인식되면 우회전
            Automobile_Right();
        else if (!cc && !ll && !rr) // 정지
        {
            if (automobile_Direction == 'F')
            {
                Automobile_Forward();
                delay(300);
                Automobile_Stop();
                break;
            }
            else if (automobile_Direction == 'L')
                Automobile_Left();
            else if (automobile_Direction == 'R')
                Automobile_Right();
            else
                ;
        }
        else
            ;
    }
}
/*****************************************************************************/
/*                      Arm_On function                             */
/*****************************************************************************/
/*****************************************************************************/
/* Input :  N/A                                                  */
/* Output:    N/A                                                  */
/* Logic :   Attach servo 1,2,3,4 variable accordingly to 8,9,10 and 11 pin     */
/*****************************************************************************/
void Arm_On()
{
    servo1.attach(8);
    servo2.attach(9);
    servo3.attach(10);
    servo4.attach(11);
}
/*****************************************************************************/
/*                      Arm_Off function                             */
/*****************************************************************************/
/*****************************************************************************/
/* Input :  N/A                                                  */
/* Output:    N/A                                                  */
/* Logic :   Detach servo 1,2,3,4 variable accordingly from its pin          */
/*****************************************************************************/
void Arm_Off()
{
    servo1.detach();
    servo2.detach();
    servo3.detach();
    servo4.detach();
}
/*****************************************************************************/
/*                      Servo_Set function                             */
/*****************************************************************************/
/*****************************************************************************/
/* Input :  servo, pos                                            */
/* Output:    N/A                                                  */
/* Logic :   Servo motor is engaged untill it reaches our desired position    */
/*****************************************************************************/
void Servo_Set(Servo servo, int pos)
{
    int cur = servo.read();
    if (cur == pos)
        ;
    else if (cur < pos)
        while (cur <= pos)
        {
            servo.write(++cur);
            delay(7);
        }
    else
        while (cur >= pos)
        {
            servo.write(--cur);
            delay(7);
        }
}
/*****************************************************************************/
/*                      Servo_Set function   (Overloaded)               */
/*****************************************************************************/
/*****************************************************************************/
/* Input :  servo_num, pos                                          */
/* Output:    N/A                                                  */
/* Logic :   Passes information to the other Servo_set function according to  */
/*          the inputted servo motor number                            */
/*****************************************************************************/
void Servo_Set(int servo_num, int pos)
{
    if (servo_num == 1)
        Servo_Set(servo1, pos);
    else if (servo_num == 2)
        Servo_Set(servo2, pos);
    else if (servo_num == 3)
        Servo_Set(servo3, pos);
    else if (servo_num == 4)
        Servo_Set(servo4, pos);
    else
        ;
}
/*****************************************************************************/
/*                      pick_up function                         */
/*****************************************************************************/
/*****************************************************************************/
/* Input :  N/A                                                  */
/* Output:    N/A                                                  */
/* Logic :   With a define movement, servo motors are engaged to pick up      */
/*          object                                              */
/*****************************************************************************/
void pick_up()
{
    Arm_On();
    delay(20);
    Servo_Set(servo2, 90);
    delay(10);
    Servo_Set(servo4, 0);
    delay(10);
    Servo_Set(servo1, 90);
    delay(10);
    Servo_Set(servo3, 170);
    delay(10);
    Servo_Set(servo1, 0);
    delay(10);
    Servo_Set(servo3, 90);
    delay(10);
    Servo_Set(servo4, 90);
    delay(10);
    Servo_Set(servo2, 60);
    delay(10);
    Arm_Off();
    delay(10);
}
/*****************************************************************************/
/*                      put_down function                         */
/*****************************************************************************/
/*****************************************************************************/
/* Input :  N/A                                                  */
/* Output:    N/A                                                  */
/* Logic :   With a define movement, servo motors are engaged to put down     */
/*          the object                                           */
/*****************************************************************************/
void put_down()
{
    Arm_On();
    delay(10);
    Servo_Set(2, 90);
    delay(10);
    Servo_Set(4, 180);
    delay(10);
    Servo_Set(3, 170);
    delay(10);
    Servo_Set(1, 90);
    delay(10);
    Servo_Set(3, 90);
    delay(10);
    Servo_Set(1, 0);
    delay(10);
    Servo_Set(4, 90);
    delay(10);
    Servo_Set(2, 60);
    delay(10);
    Arm_Off();
}
/*****************************************************************************/
/*                      Arm_Init function                         */
/*****************************************************************************/
/*****************************************************************************/
/* Input :  N/A                                                  */
/* Output:    N/A                                                  */
/* Logic :   With fixed values, arm servo motors are set to a initial position*/
/*****************************************************************************/
void Arm_Init()
{
    Arm_On();
    delay(10);
    Servo_Set(1, 0);
    delay(10);
    Servo_Set(4, 90);
    delay(10);
    Servo_Set(3, 90);
    delay(10);
    Servo_Set(2, 60);
    delay(10);
    Arm_Off();
}
/*****************************************************************************/
/*                       setup function                         */
/*****************************************************************************/
/*****************************************************************************/
/* Input :  N/A                                                  */
/* Output:    N/A                                                  */
/* Logic :   Initialize pin modes, serial and bluetoth begins listening for     */
/*         incoming connections                                  */
/*****************************************************************************/
void setup()
{
    Serial.begin(9600);
    HC06.begin(9600);
    pinMode(ENA, OUTPUT);
    pinMode(EN1, OUTPUT);
    pinMode(EN2, OUTPUT);
    pinMode(EN3, OUTPUT);
    pinMode(EN4, OUTPUT);
    pinMode(ENB, OUTPUT);
    Arm_Init();
    line();
}
/*****************************************************************************/
/*                       Control function                         */
/*****************************************************************************/
/*****************************************************************************/
/* Input :  N/A                                                  */
/* Output:    N/A                                                  */
/* Logic :   Main control section which controls the arms and car part        */
/*         according to automobile_Direction value                      */
/*****************************************************************************/
void Control()
{
    if (automobile_Direction == 'U')
    {
        pick_up();
        delay(500);
    }
    else if (automobile_Direction == 'D')
    {
        put_down();
        delay(500);
    }
    else if (automobile_Direction == 'L')
    {
        delay(500);
        Automobile_Left(); //Automobile_Left
    }
    else if (automobile_Direction == 'R') //Automobile_Right
    {
        delay(500);
        Automobile_Right();
    }
    else if (automobile_Direction == 'F') //Automobile_Forward
    {
        Arm_Init();
        Automobile_Forward();
    }
    else if (automobile_Direction == 'B')
        Automobile_Backward(); //Automobile_Backward
    else if (automobile_Direction == 'S')
        Automobile_Stop(); //stop
    else
        ;
}
/*****************************************************************************/
/*                       loop function                            */
/*****************************************************************************/
/*****************************************************************************/
/* Input :  N/A                                                  */
/* Output:    N/A                                                  */
/* Logic :   Arduino board is actively controlled using a collaboration         */
/*         of control and lt_mode_update function                      */
/*****************************************************************************/
void loop()
{
    String response = parsing();
    for (int i = 0; i < response.length(); i++)
    {
        automobile_Direction = response[i];
        Control();
        if (automobile_Direction != 'F')
            delay(500);
        if (automobile_Direction != 'S')
            lt_mode_update();
    }
}