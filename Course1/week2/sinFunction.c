//Write a program that prints the sine function for an input x between (0, 1)

#include <stdio.h>
#include <math.h>

#define PI 3.14159265

int main(void)
{

    double number, val, sine;

    printf("Enter a number:"); //The given number represent the number of degrees
    scanf("%lf", &number);

    val = PI / 180;           //Converts degrees into radians
    sine = sin(number * val); // Calculates the sine of the given number which was converted from degrees into radians

    printf("The sine if %lf is %lf", number, sine); //Printing the sine of the number given

    return 0;
}
