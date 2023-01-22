#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include  <string.h>
#include "Card.h"


EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	char name[30] = { '\0' };
	printf("Please enter card holder name: ");
	fflush(stdin);
	scanf("%[^\n]%*c", name);
	if (!name)
	{				//if the name = NULL
		return WRONG_NAME;			//return wrong name error
	}
	if (name[19] == '\0' || name[25] != '\0')
	{		//if the name is less than 20 or larger than 25
		return WRONG_NAME;										//return wrong name error
	}
	for (int i = 0; i < 25; i++)
	{
		cardData->cardHolderName[i] = name[i];			//Store the entered name in cardHolderName array
	}
	printf("Card Holder Name is: %s\n", name);
	//printf("Card Holder name is: %s\n", cardData->cardHolderName);		//Print the name
	return CARD_OK;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData) {

	char ExpDate[8] = { '\0' };
	uint32_t Year, Month;
	printf("Enter expiry date [MM/YY]: ");
	fseek(stdin, 0, SEEK_END);
	scanf("%s", ExpDate);
	if (!ExpDate)
	{
		return WRONG_EXP_DATE;
	}
	if (ExpDate[6] != '\0' || ExpDate[3] == '\0')// M/YY or MM/YYYY
	{
		return WRONG_EXP_DATE;
	}
	Year = (ExpDate[3] - '0') * 10 + (ExpDate[4] - '0');
	Month = (ExpDate[0] - '0') * 10 + (ExpDate[1] - '0');

	if ((Year > 99 || Year < 0) || Month < 1 || Month>12 || ExpDate[2] != '/')
	{
		return WRONG_EXP_DATE;
	}
	for (int i = 0; i < 6; i++)
	{
		cardData->cardExpirationDate[i] = ExpDate[i];
	}
	printf("Card expiry date is:\t %s\n", cardData->cardExpirationDate);
	return CARD_OK;
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData) {
	char CardPAN[30] = { '\0' };
	printf("Enter Card PAN (17 -> 20) characters: ");
	fseek(stdin, 0, SEEK_END);
	scanf("%s", CardPAN);

	if (!CardPAN) { return WRONG_PAN; }
	if (CardPAN[20] != '\0' || CardPAN[16] == '\0') { return WRONG_PAN; }
	for (int j = 0; j < 19; j++)
	{
		cardData->primaryAccountNumber[j] = CardPAN[j];
	}
	printf("This Card's PAN is: \t %s\n", CardPAN);
	return CARD_OK;
}



void getCardHolderNameTest(void)
{
	printf("Tester Name: Mohamed Said Riad\n");
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
	printf("Tester Name: Mohamed Said Riad\n");
	printf("Function Name: getCardExpiryDateTest\n");
	ST_cardData_t cardData;
	printf("TEST CASE 1:\n");
	printf("Input Data: NULL\n");
	printf("Expected Result: %d\n", WRONG_EXP_DATE);
	EN_cardError_t STATUS = getCardExpiryDate(&cardData);
	printf("Actual Result: %d\n", STATUS);
	printf("\n");

	printf("TEST CASE 2:\n");
	printf("Input Data: 25/25\n"); //WRONG FORMAT
	printf("Expected Result: %d\n", WRONG_EXP_DATE);
	EN_cardError_t STATUS_1 = getCardExpiryDate(&cardData);
	printf("Actual Result: %d\n", STATUS_1);
	printf("\n");

	printf("TEST CASE 3:\n");
	printf("Input Data: 11/25\n"); 
	printf("Expected Result: %d\n", CARD_OK);
	EN_cardError_t STATUS_2 = getCardExpiryDate(&cardData);
	printf("Actual Result: %d\n", STATUS_2);
	printf("\n");

	printf("TEST CASE 4:\n");
	printf("Input Data: 11/2025\n");// WRONG FORMAT
	printf("Expected Result: %d\n", WRONG_EXP_DATE);
	EN_cardError_t STATUS_3 = getCardExpiryDate(&cardData);
	printf("Actual Result: %d\n", STATUS_3);
	printf("\n");
}

void getCardPANTest(void)
{
	printf("Tester Name: Mohamed Said Riad\n");
	printf("Function Name: getCardPANTest\n");
	ST_cardData_t cardData;
	printf("TEST CASE 1:\n");
	printf("Input Data: NULL\n");
	printf("Expected Result: %d\n", WRONG_PAN);
	EN_cardError_t STATUS = getCardPAN(&cardData);
	printf("Actual Result: %d\n", STATUS);
	printf("\n");

	printf("TEST CASE 2:\n");
	printf("Input Data: 12563\n");//less than the required number
	printf("Expected Result: %d\n", WRONG_PAN);
	EN_cardError_t STATUS_1 = getCardPAN(&cardData);
	printf("Actual Result: %d\n", STATUS_1);
	printf("\n");

	printf("TEST CASE 3:\n");
	printf("Input Data: 548632104911872224009\n");
	printf("Expected Result: %d\n", WRONG_PAN);
	EN_cardError_t STATUS_2 = getCardPAN(&cardData);
	printf("Actual Result: %d\n", STATUS_2);
	printf("\n");

	printf("TEST CASE 4:\n");
	printf("Input Data: 548632104911872224\n");//from the list
	printf("Expected Result: %d\n", CARD_OK);
	EN_cardError_t STATUS_3 = getCardPAN(&cardData);
	printf("Actual Result: %d\n", STATUS_3);
	printf("\n");

	
}