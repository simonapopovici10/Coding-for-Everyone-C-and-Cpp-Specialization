#include <stdio.h> 
#include <stdlib.h> // for the exit() and malloc()

/*Here  I have created the MAX macro for finding the maximum value into the array*/
#define MAX(x, y) (x > y ? x : y)

/*Main PrograM starts from here*/
int main()
{
	/*Initialize the file pointer with the NULL*/
	FILE *toRead = NULL , *toWrite=NULL;
	/*Opening the inputdata.txt file for storing the elemnts in data[]*/
	toRead = fopen ("inputdata.txt" , "r");
	/*Opening the answer-hw.txt file for storing the final output in it.*/
	toWrite = fopen ("answer-hw3.txt","w");
	
	/*
	*	Element  ----> variable is for finding all the available int elements in the file
	*	loop  -------> variable is used for the displaying the specific element of array data[]
	*	prev_number -> variable is used for stroing the previous element to find max
	*/
	int element = 0 ,loop =0 , prev_number =0, max =0;
	
	/*
	*	Sum ------> variable is for finding sum  of all the elements in data[]
	*	average --> variable is used for the finding the average of the array and store it into another file
	*/
	double sum =0,average=0;
	
	/*Data is the Dynamic memory for data[]. The pointer should be initialize with the NULL*/
	int *data=NULL;
	
	/*inputdata.txt file is available or not available checking. If the file is not available than not to do any function*/
	if (toRead == NULL)
	{
		printf("Getting an error while reading a file\r\n");
		exit(0); //exit from the main file as the file is not present
	}
	else
	{
		printf("File open sucessfully \r\n");
	}
	/*As per defination of the program first int element in the file will tell us the total available elements in the file*/
	fscanf(toRead,"%d",&element);
	
	/*As the elements may vary as per the user input we have created dynamic array  for the array.*/
	data = (int *)malloc(element * sizeof(int));
	
	/*copy the elements to the dynamic memory of the data in the form of int we used fscanf()*/
	for(;fscanf(toRead,"%d",&*(++data)) != EOF;)
	{
		max = MAX(prev_number,*(data)); // fining the max value by comparing the variable with the last value
		printf("Element %d: %d \r\n",loop,*(data)); // as per the given input need to print all the elements on std o/p.
		sum = sum + (double)(*(data)); // fining the sum for the average calculation 
		loop++; // loop is used to just print the element number. (optional)
		prev_number = *(data);// storing the data into previous data for checking max
	}
	/*finding the average of the all the elements in the data[] from the file*/
	average =  sum / element;
	/*Printing the MAX  value and average value on std o/p as per given assignment*/
	printf("Average - %f and MAX - %d",average, max);
	/*Storing the MAX  value and average value to the o/p file*/
	fprintf(toWrite,"Average - %f and MAX - %d",average, max);
	
	/*closing both the files.*/
	fclose(toRead);
	fclose(toWrite);
}
