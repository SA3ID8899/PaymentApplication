#include <stdlib.h>
#include<stdio.h>
#include"../PaymentApplication/Card/Card.h"

void getCardHolderNameTest(void);
void getCardExpiryDateTest(void);
void getCardPANTest(void);
#define Test_Cases 5

int main()
{
	getCardHolderNameTest();
	getCardExpiryDateTest();
	getCardPANTest();

	return 0;
}

void getCardHolderNameTest(void)
{
	printf("\nTester Name: Mohamed Said Riad\n");
	printf("Function Name: getCardHolderName\n");
	ST_cardData_t cardData;
	printf("TEST CASE 1:\n");
	printf("Input Data: NULL\n");
	printf("Expected Result: %d\n", WRONG_NAME);
	EN_cardError_t STATUS = getCardHolderName(&cardData);
	printf("Actual Result: %d\n", STATUS);
	printf("\n");

	printf("TEST CASE 2:\n");
	printf("Input Data: Mohamed Said Riad Mohamed\n");
	printf("Expected Result: %d\n", CARD_OK);
	EN_cardError_t STATUS_1 = getCardHolderName(&cardData);
	printf("Actual Result: %d\n", STATUS_1);
	printf("\n");

	printf("TEST CASE 3:\n");
	printf("Input Data: Mohamed Said\n"); // less than the required number
	printf("Expected Result: %d\n", WRONG_NAME);
	EN_cardError_t STATUS_2 = getCardHolderName(&cardData);
	printf("Actual Result: %d\n", STATUS_2);
	printf("\n");

	printf("TEST CASE 4:\n"); 
	printf("Input Data: Mohamed Said Riad\n");// still less than the required number
	printf("Expected Result: %d\n", WRONG_NAME);
	EN_cardError_t STATUS_3 = getCardHolderName(&cardData);
	printf("Actual Result: %d\n", STATUS_3);
	printf("\n");
}

void getCardExpiryDateTest(void)
{

}

void getCardPANTest(void)
{

}