#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE

#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include "Server.h"
extern float x;

#define DATA_BASE_SIZE 255
// THE ACCOUNTS IN THE SERVER
ST_accountsDB_t accountsDB[DATA_BASE_SIZE] = {
	{1200.0, BLOCKED, "548632104911872224"},
	{9500.0, RUNNING, "773154266711204421"},
	{6000.0, RUNNING, "205517739116643278"},
	{2400.0, RUNNING, "385742961357147251"},
	{1278.0, RUNNING, "473925483763857348"},
	{200.0, RUNNING, "625341951651872956"},
	{10.0, RUNNING, "124301729208282174"},
	{6440.0, RUNNING, "423760075643816171"},
	{7081.0, RUNNING, "542319786202022459"},
	{902.0, BLOCKED, "328579252875387594"} };

ST_transaction_t transaction_data_base[DATA_BASE_SIZE] = { 0 };
// setting a refrence account
ST_accountsDB_t* account = NULL;
//transaction sequence to know how many transactions
int  TransactionSeq = 1;
EN_transState_t recieveTransactionData(ST_transaction_t* transData) {
    if (isValidAccount(&transData->cardHolderData,&account)) 
    {
        transData->transState = FRAUD_CARD;

        return FRAUD_CARD;
    }
    if (isAmountAvailable(&transData->terminalData, &account))
    {
        transData->transState = DECLINED_INSUFFECIENT_FUND;

        return DECLINED_INSUFFECIENT_FUND;
    }
    if (SAVING_FAILED == saveTransaction(transData))
    {
        transData->transState = INTERNAL_SERVER_ERROR;

        return INTERNAL_SERVER_ERROR;
    }
    if (isBlockedAccount(&account)) 
    {
        transData->transState = DECLINED_STOLEN_CARD;

        return DECLINED_STOLEN_CARD;
    }
    account->balance -= transData->terminalData.transAmount;

    transData->transState = APPROVED;
    x= account->balance;
    return APPROVED;
}
EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence) {
    for (int i = 0; i < DATA_BASE_SIZE; i++) 
    {
        if (strcmp(cardData->primaryAccountNumber, accountsDB[i].primaryAccountNumber) == 0) 
        {
            account = &accountsDB[i];

            return SERVER_OK;
        }
    }
    return ACCOUNT_NOT_FOUND;
}
EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence) {
    if (account->state == RUNNING) return SERVER_OK;
    return BLOCKED_ACCOUNT;
}
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence) {
    if (termData->transAmount > account->balance) return LOW_BALANCE;
    return SERVER_OK;
}

EN_serverError_t saveTransaction(ST_transaction_t* transData) {
    if (TransactionSeq >= DATA_BASE_SIZE) return SAVING_FAILED;
    transaction_data_base[TransactionSeq].cardHolderData = transData->cardHolderData;
    transaction_data_base[TransactionSeq].terminalData = transData->terminalData;
    transaction_data_base[TransactionSeq].transState = transData->transState;
    transData->transactionSequenceNumber = TransactionSeq;
    transaction_data_base[TransactionSeq].transactionSequenceNumber = transData->transactionSequenceNumber;
    listSavedTransactions();
    TransactionSeq++;
    return SERVER_OK;
}

void listSavedTransactions(void) {
    printf("\n-------LISTING TRANSACTIONS-----\n");
    printf("Sequence Number of the transaction: %ld\n", transaction_data_base[TransactionSeq].transactionSequenceNumber);
    printf("Date of transaction: %s\n", transaction_data_base[TransactionSeq].terminalData.transactionDate);
    printf("Max Amount of transaction in this terminal: %.2f\n", transaction_data_base[TransactionSeq].terminalData.maxTransAmount);
    printf("Amount of transaction: %.2f\n", transaction_data_base[TransactionSeq].terminalData.transAmount);
    printf("Transaction State: %s\n", getTransactionStatus(transaction_data_base[2].transState));
    printf("PAN of the CARD: %s\n", transaction_data_base[TransactionSeq].cardHolderData.primaryAccountNumber);
    printf("Expiration Date OF THE CARD: %s\n", transaction_data_base[TransactionSeq].cardHolderData.cardExpirationDate);
    printf("Name of the CardHolder: %s\n", transaction_data_base[TransactionSeq].cardHolderData.cardHolderName);
    printf("\-----------DONE LISTING--------\n");
}

void recieveTransactionDataTest(void)
{
    printf("Tester Name: Mohamed Said Riad\n");
    printf("Function Name: receiveTransactionData\n");
    ST_transaction_t transactionData;

    printf("TEST CASE 1:\n");
    transactionData.terminalData.transAmount = 5600;
    strcpy(transactionData.cardHolderData.primaryAccountNumber, "385742961357147251");
    printf("Input Data: 385742961357147251 \n");
    printf("Expected Result: %d\n", DECLINED_INSUFFECIENT_FUND);
    printf("Transaction Amount : 5600\n");
    EN_serverError_t STATUS = recieveTransactionData(&transactionData);
    printf("Actual Result: %d\n", STATUS);
    printf("\n");

    printf("TEST CASE 2:\n");
    strcpy(transactionData.cardHolderData.primaryAccountNumber, "448631103921872224");
    printf("Input Data: 448631103921872224 \n");
    printf("Expected Result: %d\n", FRAUD_CARD);
    EN_serverError_t STATUS_1 = recieveTransactionData(&transactionData);
    printf("Actual Result: %d\n", STATUS_1);
    printf("\n");

    printf("TEST CASE 3:\n");
    transactionData.terminalData.transAmount = 1600;
    strcpy(transactionData.cardHolderData.primaryAccountNumber, "328579252875387594");
    printf("Input Data: 328579252875387594 \n");
    printf("Expected Result: %d\n", DECLINED_STOLEN_CARD);
    printf("Transaction Amount : 1600\n");
    EN_serverError_t STATUS2 = recieveTransactionData(&transactionData);
    printf("Actual Result: %d\n", STATUS2);
    printf("\n");

    printf("TEST CASE 4:\n");
    transactionData.terminalData.transAmount = 1600;
    strcpy(transactionData.cardHolderData.primaryAccountNumber, "385742961357147251");
    printf("Input Data: 385742961357147251 \n");
    printf("Expected Result: %d\n", APPROVED);
    printf("Transaction Amount : 1600\n");
    EN_serverError_t STATUS_3 = recieveTransactionData(&transactionData);
    printf("Actual Result: %d\n", STATUS_3);
    printf("\n");

}
void isValidAccountTest(void)
{
    printf("Tester Name: Mohamed Said Riad\n");
    printf("Function Name: isValidAccount\n");
    ST_transaction_t transactionData;
    ST_accountsDB_t reference;

    printf("TEST CASE1:\n");
    strcpy(transactionData.cardHolderData.primaryAccountNumber, "385742961357147251");
    printf("Input Data: 385742961357147251 \n");
    printf("Expected Result: %d\n", SERVER_OK);
    EN_serverError_t STATUS = isValidAccount(&transactionData.cardHolderData,&reference);
    printf("Actual Result: %d\n", STATUS);
    printf("\n");

    printf("TEST CASE1:\n");
    strcpy(transactionData.cardHolderData.primaryAccountNumber, "383742861257147251");
    printf("Input Data: 383742861257147251 \n");
    printf("Expected Result: %d\n", ACCOUNT_NOT_FOUND);
    EN_serverError_t STATUS_1 = isValidAccount(&transactionData.cardHolderData, &reference);
    printf("Actual Result: %d\n", STATUS_1);
    printf("\n");

}
void isBlockedAccountTest(void)
{
    printf("Tester Name: Mohamed Said Riad\n");
    printf("Function Name: isBlockedAccount\n");
    ST_transaction_t transactionData;
    ST_accountsDB_t reference;


    printf("TEST CASE1:\n");
    strcpy(transactionData.cardHolderData.primaryAccountNumber, "773154266711204421");
    printf("Input Data: 773154266711204421 \n");
    printf("Expected Result: %d\n", SERVER_OK);
    EN_serverError_t STATUS = isValidAccount(&transactionData.cardHolderData,&reference);
    printf("Actual Result: %d\n", STATUS);
    printf("\n");

    printf("TEST CASE2:\n");
    strcpy(transactionData.cardHolderData.primaryAccountNumber, "548632104911872224");
    printf("Input Data: 548632104911872224 \n");
    printf("Expected Result: %d\n", BLOCKED_ACCOUNT);
    EN_serverError_t STATUS_1 = isValidAccount(&transactionData.cardHolderData, &reference);
    printf("Actual Result: %d\n", STATUS_1);
    printf("\n");
}
void isAmountAvailableTest(void)
{
    printf("Tester Name: Mohamed Said Riad\n");
    printf("Function Name: isAmountAvailable\n");
    ST_transaction_t transactionData;
    ST_cardData_t cardData;
    ST_accountsDB_t reference;

    printf("TEST CASE1:\n");
    strcpy(transactionData.cardHolderData.primaryAccountNumber, "773154266711204421");
    isValidAccount(&transactionData.cardHolderData,&reference);
    printf("Input Data: 773154266711204421 \n");
    printf("Expected Result: %d\n", SERVER_OK);
    transactionData.terminalData.transAmount = 1000;
    printf("Available Money :9500 Requested :1000\n");
    EN_serverError_t STATUS = isAmountAvailable(&transactionData.terminalData,&reference);
    printf("Actual Result: %d\n", STATUS);
    printf("\n");

    printf("TEST CASE2:\n");
    strcpy(transactionData.cardHolderData.primaryAccountNumber, "773154266711204421");
    isValidAccount(&transactionData.cardHolderData,&reference);
    printf("Input Data: 773154266711204421 \n");
    printf("Expected Result: %d\n", LOW_BALANCE);
    transactionData.terminalData.transAmount = 10000;
    printf("Available Money :9500 Requested :10000\n");
    EN_serverError_t STATUS_1 = isAmountAvailable(&transactionData.terminalData, &reference);
    printf("Actual Result: %d\n", STATUS_1);
    printf("\n");
}
void saveTransactionTest(void)
{
    printf("Tester Name: Mohamed Said Riad\n");
    printf("Function Name: saveTransaction\n");
    ST_transaction_t transactionData;

    printf("TEST CASE1: \n");
    strcpy(transactionData.cardHolderData.cardHolderName, "Mohamed Said Riad Mohamed");
    strcpy(transactionData.cardHolderData.primaryAccountNumber, "773154266711204421");
    strcpy(transactionData.cardHolderData.cardExpirationDate, "01/25");
    transactionData.terminalData.maxTransAmount = 8000;
    transactionData.terminalData.transAmount = 1000;
    strcpy(transactionData.terminalData.transactionDate, "01/01/2000");
    printf("Input Data: This transaction is number 0 so it's OK\n");
    EN_serverError_t STATUS = saveTransaction(&transactionData);
    printf("Expected Result: %d\n", SERVER_OK);
    printf("Actual Result: %d\n", STATUS);

    printf("TEST CASE2: \n");
    strcpy(transactionData.cardHolderData.cardHolderName, "Mohamed Said Riad Mohamed");
    strcpy(transactionData.cardHolderData.primaryAccountNumber, "773154266711204421");
    strcpy(transactionData.cardHolderData.cardExpirationDate, "01/25");
    transactionData.terminalData.maxTransAmount = 8000;
    transactionData.terminalData.transAmount = 1000;
    strcpy(transactionData.terminalData.transactionDate, "01/01/2000");
    printf("Input Data: This transaction is number 300 so it's not OK\n");
    TransactionSeq = 300;
    EN_serverError_t STATUS_1 = saveTransaction(&transactionData);
    printf("Expected Result: %d\n", SAVING_FAILED);
    printf("Actual Result: %d\n", STATUS_1);
}
void listSavedTransactionsTest(void)
{
    printf("Tester Name: Mohamed Said Riad\n");
    printf("Function Name: listSavedTransaction\n");
    ST_transaction_t transactionData;

    printf("TEST CASE1: \n");
    strcpy(transactionData.cardHolderData.cardHolderName, "Mohamed Said Riad Mohamed");
    strcpy(transactionData.cardHolderData.primaryAccountNumber, "773154266711204421");
    strcpy(transactionData.cardHolderData.cardExpirationDate, "01/25");
    transactionData.terminalData.maxTransAmount = 8000;
    transactionData.terminalData.transAmount = 1000;
    strcpy(transactionData.terminalData.transactionDate, "01/01/2000");
    printf("Input Data: This transaction is number 0 so it's OK\n");
    EN_serverError_t STATUS = saveTransaction(&transactionData);
    printf("Expected Result: %d\n", SERVER_OK);
    printf("Actual Result: %d\n", STATUS);
}

char* getTransactionStatus(EN_transState_t transState) {
    switch (transState) {
    case APPROVED: return "Transaction Approved";
    case DECLINED_INSUFFECIENT_FUND: return "Declined due to(Insufficient fund)";
    case DECLINED_STOLEN_CARD: return "Declined due to (Blocked Account)";
    case FRAUD_CARD: return "Declined due to (Fraud Card)";
    case INTERNAL_SERVER_ERROR: return "Declined due to (Internal Server Error)";
    default: return "EEROR UNKNOWN";
    }
}