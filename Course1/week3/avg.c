#include <stdio.h>

int main(void)
{

    int weights;
    float weight[1000], sum = 0.0, avg;

    printf("Enter the number of weights from 0 to 1000: ");
    scanf("%d", &weights);

    for (int i = 0; i < weights; i++)
    {
        printf("%d. Enter number: ", i + 1);
        scanf("%f", &weight[i]);
        sum += weight[i];
    }

    avg = sum / weights;
    printf("Average = %.2f", avg);

    return 0;
}