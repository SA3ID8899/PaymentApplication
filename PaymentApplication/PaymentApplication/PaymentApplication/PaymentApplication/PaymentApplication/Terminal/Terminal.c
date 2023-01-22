#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE

#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include "Terminal.h"


EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{

    char arr1[11];
    fflush(stdin);
    printf("Please enter the transaction date [DD/MM/YYYY]: ");
    fseek(stdin, 0, SEEK_END);
    fgets(arr1, 11, stdin);
    

    //scanf("%[^\n]%*c", arr1);

    for (int i = 0; i < 11; i++)
    {
        termData->transactionDate[i] = arr1[i];
    }
    uint32_t year = (((termData->transactionDate[6] - '0') * 1000) + ((termData->transactionDate[7] - '0') * 100) + ((termData->transactionDate[8] - '0') * 10) + ((termData->transactionDate[9] - '0')));
    uint32_t month = (((termData->transactionDate[3] - '0') * 10) + (termData->transactionDate[4] - '0'));
    uint32_t day = (((termData->transactionDate[0] - '0') * 10) + (termData->transactionDate[1] - '0'));
    if ((day > 31 || day < 0) || (month > 12 || month < 1) || (year < 1900))
    {
        return  WRONG_DATE;
    }
    else if ((termData->transactionDate[9] == '\0') || (termData->transactionDate[2] != '/') || (termData->transactionDate[5] != '/'))
    {
        return WRONG_DATE;
    }
    else
    {
        printf("The date is: %s\n", arr1);
        return TERMINAL_OK;
    }
}

EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData)
{
    if (!termData || !cardData) { return EXPIRED_CARD; }
    uint8_t* temp_Terminal = termData->transactionDate;
    uint8_t* temp_Card = cardData->cardExpirationDate;
    uint64_t Card_Month, transaction_Month, Card_Year, transaction_Year;
    strtol(termData->transactionDate, &temp_Terminal, 10); //to ignore the days 
    transaction_Month = strtol(++temp_Terminal, &temp_Terminal, 10);// transaction month
    transaction_Year = strtol(++temp_Terminal, &temp_Terminal, 10);// transaction year
    Card_Month = strtol(cardData->cardExpirationDate, &temp_Card, 10);// to get the card expiry month
    Card_Year = strtol(++temp_Card, &temp_Card, 10); // to get the card expiry year
    Card_Year = Card_Year + 2000; //to make the format of the date on the card to be as same as Transaction YYYY
    if (transaction_Year > Card_Year)
    {
        return EXPIRED_CARD;
    }
    else if (transaction_Year == Card_Year && Card_Month <= transaction_Month)
    {
        return EXPIRED_CARD;
    }
    else
    {
        return TERMINAL_OK;
    }
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
    float transaction_amount = -1;
    printf("Enter transaction amount: ");
    scanf("%f", &transaction_amount);
    if (transaction_amount <= 0) 
    { 
        return INVALID_AMOUNT;
    }
    else
    {
        termData->transAmount = transaction_amount;
        return TERMINAL_OK;
    }
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
    if ((termData->transAmount) >= (termData->maxTransAmount))
    {
        return EXCEED_MAX_AMOUNT;
    }
    else
    {
        return TERMINAL_OK;
    }
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData, float maxAmount)
{
    printf("Please enter maximum amount for transaction : ");
    scanf("%f", &maxAmount);

    if (maxAmount <= 0)
    {
        return INVALID_MAX_AMOUNT;
    }
    else
    {
        termData->maxTransAmount = maxAmount;
        return TERMINAL_OK;
    }

}

void getTransactionDateTest(void)
{
    printf("Tester Name: Mohamed Said Riad\n");
    printf("Function Name: getTransactionDate\n");

    ST_terminalData_t terminalData;

    printf("TEST CASE 1:\n");
    printf("Input Data: NULL\n");
    printf("Expected Result: %d\n", WRONG_DATE);
    EN_cardError_t STATUS = getTransactionDate(&terminalData);
    printf("Actual Result: %d\n", STATUS);
    printf("\n");

    printf("TEST CASE 2:\n");
    printf("Input Data: 25/2/2022\n");
    printf("Expected Result: %d\n", WRONG_DATE);
    EN_cardError_t STATUS_1 = getTransactionDate(&terminalData);
    printf("Actual Result: %d\n", STATUS_1);
    printf("\n");

    printf("TEST CASE 3:\n");
    printf("Input Data: 25/02/2022\n");
    printf("Expected Result: %d\n", TERMINAL_OK);
    EN_cardError_t STATUS_2 = getTransactionDate(&terminalData);
    printf("Actual Result: %d\n", STATUS_2);
    printf("\n");

    printf("TEST CASE 4:\n");
    printf("Input Data: 25/14/2022\n");
    printf("Expected Result: %d\n", WRONG_DATE);
    EN_cardError_t STATUS_3 = getTransactionDate(&terminalData);
    printf("Actual Result: %d\n", STATUS_3);
    printf("\n");

}
void isCardExpiredTest(void)
{
    printf("Tester Name: Mohamed Said Riad\n");
    printf("Function Name: isCardExpired\n");

    ST_terminalData_t terminalData;
    ST_cardData_t   cardData;
    strcpy(cardData.cardExpirationDate, "05/25");//seeting the Expiration date to 05/2025


    printf("TEST CASE 1:\n");
    strcpy(terminalData.transactionDate, "17/05/2026");
    printf("Input Data: %s\n", terminalData.transactionDate);
    printf("Expected Result: %d\n", EXPIRED_CARD);
    EN_cardError_t STATUS = isCardExpired(&terminalData,&cardData);
    printf("Actual Result: %d\n", STATUS);
    printf("\n");

    printf("TEST CASE 2:\n");
    strcpy(terminalData.transactionDate, "17/07/2025");
    printf("Input Data: %s\n", terminalData.transactionDate);
    printf("Expected Result: %d\n", EXPIRED_CARD);
    EN_cardError_t STATUS_1 = isCardExpired(&terminalData, &cardData);
    printf("Actual Result: %d\n", STATUS_1);
    printf("\n");

    printf("TEST CASE 3:\n");
    strcpy(terminalData.transactionDate, "17/07/2022");
    printf("Input Data: %s\n", terminalData.transactionDate);
    printf("Expected Result: %d\n", TERMINAL_OK);
    EN_cardError_t STATUS_2 = isCardExpired(&terminalData, &cardData);
    printf("Actual Result: %d\n", STATUS_2);
    printf("\n");
}
void getTransactionAmountTest(void)
{
    printf("Tester Name: Mohamed Said Riad\n");
    printf("Function Name: getTransactionAmountTest\n");
    ST_terminalData_t terminalData;

    printf("TEST CASE 1:\n");
    printf("Input Data: 0\n");
    printf("Expected Result: %d\n", INVALID_AMOUNT);
    EN_cardError_t STATUS = getTransactionAmount(&terminalData);
    printf("Actual Result: %d\n", STATUS);
    printf("\n");

    printf("TEST CASE 2:\n");
    printf("Input Data: 512.45\n");
    printf("Expected Result: %d\n", TERMINAL_OK);
    EN_cardError_t STATUS_1 = getTransactionAmount(&terminalData);
    printf("Actual Result: %d\n", STATUS_1);
    printf("\n");

    printf("TEST CASE 3:\n");
    printf("Input Data: -512.45\n");
    printf("Expected Result: %d\n", INVALID_AMOUNT);
    EN_cardError_t STATUS_2 = getTransactionAmount(&terminalData);
    printf("Actual Result: %d\n", STATUS_2);
    printf("\n");

    printf("TEST CASE 4:\n");
    printf("Input Data: ABC\n");
    printf("Expected Result: %d\n", INVALID_AMOUNT);
    EN_cardError_t STATUS_3 = getTransactionAmount(&terminalData);
    printf("Actual Result: %d\n", STATUS_3);
    printf("\n");

}
void isBelowMaxAmountTest(void)
{
    printf("Tester Name: Mohamed Said Riad\n");
    printf("Function Name: isBelowMaxAmount\n");
    ST_terminalData_t terminalData;
    terminalData.maxTransAmount = 600; // setting the maximum amount to be 600

    printf("TEST CASE 1:\n");
    terminalData.transAmount = 1000;
    printf("Input Data: 1000.00\n");
    printf("Expected Result: %d\n", EXCEED_MAX_AMOUNT);
    EN_cardError_t STATUS = isBelowMaxAmount(&terminalData);
    printf("Actual Result: %d\n", STATUS);
    printf("\n");

    printf("TEST CASE 1:\n");
    terminalData.transAmount = 200;
    printf("Input Data: 200.00\n");
    printf("Expected Result: %d\n", TERMINAL_OK);
    EN_cardError_t STATUS_1 = isBelowMaxAmount(&terminalData);
    printf("Actual Result: %d\n", STATUS_1);
    printf("\n");

}
void setMaxAmountTest(void)
{
    printf("Tester Name: Mohamed Said Riad\n");
    printf("Function Name: setMaxAmount\n");
    ST_terminalData_t terminalData;

    printf("TEST CASE 1:\n");
    printf("Input Data: 0\n");
    printf("Expected Result: %d\n", INVALID_MAX_AMOUNT);
    EN_cardError_t STATUS = setMaxAmount(&terminalData,0);
    printf("Actual Result: %d\n", STATUS);
    printf("\n");

    printf("TEST CASE 2:\n");
    printf("Input Data: 888.19\n");
    printf("Expected Result: %d\n", TERMINAL_OK);
    EN_cardError_t STATUS_1 = setMaxAmount(&terminalData,888.19);
    printf("Actual Result: %d\n", STATUS_1);
    printf("\n");

  
    printf("TEST CASE 3:\n");
    printf("Input Data: -888.19\n");
    printf("Expected Result: %d\n", INVALID_MAX_AMOUNT);
    EN_cardError_t STATUS_2 = setMaxAmount(&terminalData,-888.19);
    printf("Actual Result: %d\n", STATUS_2);
    printf("\n");
}
void isValidCardPANTest(void)//optional
{

}
