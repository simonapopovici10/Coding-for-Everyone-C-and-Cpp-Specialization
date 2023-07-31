
#include <stdio.h>
#include <math.h>

int main()
{

    double degree, radian;

    printf("Enter a degree value: ");
    scanf("%lf", &degree);

    //Convert degrees to radians
    radian = degree * (M_PI / 180);

    printf("sin value is %lf \n", sin(radian));
    printf("cos value is %lf \n", cos(radian));

    return 0;
}