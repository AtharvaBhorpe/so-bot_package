#include <ros.h>
#include <geometry_msgs/Twist.h>


#define PWM_MIN  0
#define PWMRANGE  255
#define d  353.4
#define x_min  0
#define x_max  1.13
#define enable 22


float u1, u2, u3;
float pwm1, pwm2, pwm3;
float x,y,z;

/*
  left motor driver -> front wheel
  middle motor driver -> right wheel (battery)
  right motor driver -> front wheel

                     1


                             battery

  3                                         2



  m1 ->
*/
int m1_rpwm = 44;
int m1_lpwm = 46;
int m2_rpwm = 9;
int m2_lpwm = 10;
int m3_rpwm = 11;
int m3_lpwm = 12;

ros::NodeHandle nh;
void onTwist(const geometry_msgs::Twist &msg);
float mapFloat(float x1, float out_min, float out_max);
ros::Subscriber<geometry_msgs::Twist> sub("/cmd_vel", &onTwist);

void setup(){
  nh.getHardware()->setBaud(115200);
  nh.initNode();
  nh.subscribe(sub);
}

void onTwist(const geometry_msgs::Twist &msg)
{

    x = msg.linear.x;
    y = msg.linear.y;
    z = msg.angular.z;

//    u1=-0.33*x + 0.58*y + 0.33*z;
//    u2=-0.33*x - 0.58*y + 0.33*z;
//    u3= 0.67*x + 0*y + 0.33*z;

    u1= 0*x + 0.67*y + 0.33*z;
    u2=0.58*x - 0.33*y + 0.33*z;
    u3=-0.58*x - 0.33*y + 0.33*z;

    pwm2 = mapFloat(fabs(u2), PWM_MIN, PWMRANGE);//assuming u2 as left
    pwm1 = mapFloat(fabs(u1), PWM_MIN, PWMRANGE);//assuming u3 as right
    pwm3 = mapFloat(fabs(u3), PWM_MIN, PWMRANGE);//assuming u1 as back

//    Serial.print(pwm1);
//    Serial.print("     ");
//    Serial.print(pwm2);
//    Serial.print("   ");
//    Serial.println(pwm3);
    // Set direction pins and PWM
    if (u1 > 0)
    {
        analogWrite(m1_rpwm, pwm1);
        analogWrite(m1_lpwm, 0);

    }
    else
    {
        analogWrite(m1_lpwm, pwm1);
        analogWrite(m1_rpwm, 0);
    }
    if (u2 > 0)
    {
        analogWrite(m2_rpwm, pwm2);
        analogWrite(m2_lpwm, 0);

    }
    else
    {
        analogWrite(m2_lpwm, pwm2);
        analogWrite(m2_rpwm, 0);
    }
    if (u3 > 0)
    {
        analogWrite(m3_rpwm, pwm3);
        analogWrite(m3_lpwm, 0);

    }
    else
    {
        analogWrite(m3_lpwm, pwm3);
        analogWrite(m3_rpwm, 0);
    }


}
void loop(){
 nh.spinOnce();
}

float mapFloat(float x1, float out_min, float out_max)
{

    return min(out_min + ((x1-x_min)/(x_max-x_min))*(out_max-out_min), out_max);

}
