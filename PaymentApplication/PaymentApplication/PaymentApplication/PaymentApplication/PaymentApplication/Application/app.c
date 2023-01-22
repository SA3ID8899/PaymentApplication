#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE

#include "app.h"
#include <stdio.h>
#include "../Card/Card.h"
#include "../Terminal/Terminal.h"
#include "../Server/Server.h"
float x;

void startapp()
{
	char statement_1[26] = "----------HELLO----------";
	char statement_2[26] = "------LOADING DATA-------";
	char statement_3[26] = "------LOADING DONE-------";
	for (int i = 0; i < 26; i++)
	{
		printf("%c", statement_1[i]);
		Sleep(10);
	}
	printf("\n");
	Sleep(2000);
	for (int i = 0; i < 26; i++)
	{
		printf("%c", statement_2[i]);
		Sleep(10);
	}
	printf("\n");
	Sleep(2000);
	for (int i = 0; i < 26; i++)
	{
		printf("%c", statement_3[i]);
		Sleep(10);
	}
	printf("\n");
	Sleep(2000);
	/*------------------------FOR THE CARD------------------------*/
	ST_cardData_t  card_data;
	EN_cardError_t condition_card;
	condition_card = getCardHolderName(&card_data);
	while (condition_card == WRONG_NAME)
	{
		printf("The name that you entered isn't in the correct format\nPlease Try Again.\n");
		condition_card =getCardHolderName(&card_data);
	}
	condition_card = getCardExpiryDate(&card_data);
	while (condition_card == WRONG_EXP_DATE)
	{
		printf("The expiry date that you entered isn't in the correct format\nPlease Try Again.\n");
		condition_card = getCardExpiryDate(&card_data);
	}
	condition_card = getCardPAN(&card_data);
	while (condition_card == WRONG_PAN)
	{
		printf("The PAN that you entered isn't in the correct format\nPlease Try Again.\n");
		condition_card =getCardPAN(&card_data);
	}

	/*-----------------------FOR THE TERMINAL-----------------------*/

	ST_terminalData_t terminal_data;
	EN_terminalError_t condition_terminal;
	condition_terminal = setMaxAmount(&terminal_data,2000.00);
	while (condition_terminal == INVALID_AMOUNT)
	{
		printf("The amount that you entered isn't in the correct format\nPlease Try Again.\n");
		condition_terminal = setMaxAmount(&terminal_data,2000.00);

	}
	condition_terminal = getTransactionDate(&terminal_data);
	while (condition_terminal == WRONG_DATE)
	{
		printf("The date that you entered isn't in the correct format\nPlease Try Again.\n");
		condition_terminal = getTransactionDate(&terminal_data);
	}
	condition_terminal = isCardExpired(&card_data,&terminal_data);
	if (condition_terminal == EXPIRED_CARD)
	{
		printf("CARD IS EXPIRED PLEASE CONTACT THE BANK\n");
		condition_terminal = isCardExpired(&card_data, &terminal_data);
		return;
	}
	condition_terminal = getTransactionAmount(&terminal_data);
	while (condition_terminal == INVALID_AMOUNT)
	{
		printf("The amount that you entered isn't correct\nPlease Try Again.\n;");
		condition_terminal = getTransactionAmount(&terminal_data);
	}
	condition_terminal = isBelowMaxAmount(&terminal_data);
	if (condition_terminal== EXCEED_MAX_AMOUNT)
	{
		printf("You have the reached the maximum amount.\n");
		return;
	}

	/*-----------------------FOR THE SERVER-----------------------*/
	/*ST_transaction_t transactiondata;
	transactiondata.cardHolderData = card_data;
	transactiondata.terminalData = terminal_data;*/
	ST_transaction_t Transactiondata = { .cardHolderData = card_data,
									.terminalData = terminal_data,
	};
	EN_transState_t ERROR_STATE = recieveTransactionData(&Transactiondata);
	if (ERROR_STATE == FRAUD_CARD)
	{
		printf("\n----------DECLINED FRAUD CARD----------\n");
	}
	else if(ERROR_STATE == DECLINED_INSUFFECIENT_FUND)
	{
		printf("\n----------DECLINED INSUFFECIENT FUND----------\n");
	}
	else if (ERROR_STATE == DECLINED_STOLEN_CARD)
	{
		printf("\n----------DECLINED STOLEN CARD......CONTACTING AUTHORITIES--------\n");
	}
	else if (ERROR_STATE == APPROVED)
	{
		printf("\n----------APPROVED----------\n");
		printf("\n-----TRANSACTION IS DONE-----\n");
		printf("Remaining amount = %f\n\n",x);

	}
	else
	{
		printf("HERE IS THE PROBLEM\n");
	}
}
