#include<stdio.h>

int main(void)
{
	int a,b;
	printf("Welcome to our simple calculator ! \n");
	printf("Enter the first number: ");
	scanf("%i", &a);
	printf("Enter the second number: ");
	scanf("%i", &b);
	printf("LET'S DIVIDE THESE TWO GUYS! , HERE IS THE RESULT: %.2f", (float)a/b);
}
	
