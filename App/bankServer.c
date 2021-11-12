
#include <stdbool.h>
#include "server1.c"


//function name: isBlocked
//return type: bool
//parameters: long long int llCardNo
//use: to check whether the card is blocked or not

bool isBlocked(long long int llCardNo)
{
    if (first == NULL)
    {
        return false;
    }
    user *cur = first;
    while (cur != NULL)
    {
        if (cur->llCardNo == llCardNo)
        {
            if (cur->iBlocked == 1)
            {
                return true; //password is corect
            }
        }
        cur = cur->link;
    }
    return false;
}

//function name: blockCard
//return type: void
//parameters: long long int llCardNo
//use: to block card from  bank server

void blockCard(long long int llCardNo)
{
    user *cur = first;
    while (cur != NULL)
    {
        if (cur->llCardNo == llCardNo)
        {
            cur->iBlocked = 1;
            return;
        }
        cur = cur->link;
    }
}

//function name:  verifyCard
//return type: bool
//parameters: long long int llCardNo
//use: to verify card

bool verifyCard(long long int llCardNo)
{
    if (first == NULL)
    {
        return false;
    }
    user *cur = first;
    while (cur != NULL)
    {
        if (cur->llCardNo == llCardNo)
        {
            return true; //card has associated with a account
        }
        cur = cur->link;
    }
    return false;
}

//function name:   verifyCardPin
//return type: bool
//parameters: long long int llCardNo, int iPin
//use: to verify card pin

bool verifyCardPin(long long int llCardNo, int iPin)
{
    if (first == NULL)
    {
        return false;
    }
    user *cur = first;
    while (cur != NULL)
    {
        if (cur->llCardNo == llCardNo)
        {
            if (cur->iPin == iPin)
            {
                return true; //password is corect
            }
        }
        cur = cur->link;
    }
    return false;
}

//function name:  checkBalance
//return type: float
//parameters: long long int llCardNo
//use: to check balance from atm

float checkBalance(long long int llCardNo)
{
    if (first == NULL)
    {
        return 0;
    }
    user *cur = first;
    while (cur != NULL)
    {
        if (cur->llCardNo == llCardNo)
        {
            return cur->fBalance; //returns balance
        }
        cur = cur->link;
    }
    return 0;
}

//function name:   verification
//return type: bool
//parameters: long long int llCardNo
//uses: to verify card number

float checkBalanceA(long long int llAccountNo)
{
    if (first == NULL)
    {
        return 0;
    }
    user *cur = first;
    while (cur != NULL)
    {
        if (cur->llAccountNo == llAccountNo)
        {
            return cur->fBalance;
        }
        cur = cur->link;
    }
    return 0;
}

bool verification(long long int llCardNo)
{

    int iPin;
    int iCount = 3;

    if (isBlocked(llCardNo))
    {
        printError("Card is Blocked\nContact Bank");
        return false;
    }
    if (!verifyCard(llCardNo))
    {
        printError("Card Doesn't Exist\nTransaction Failed");
        return false;
    }

reinitiate:
    reEnterPinV:
    printf("Enter the Passwaord: ");
    if (!readPin(&iPin))
    {
        printError("Pin should be 4 digit number");
        goto reEnterPinV;
    }



    if (!verifyCardPin(llCardNo, iPin))
    {
        if (iCount == 0)
        {
            blockCard(llCardNo);
            printError("Card is Blocked\nContact Bank");
            return false;
        }
        iCount--;
        printError("Invalid Pin");
        printf("\n--------------------------------------\n");
        printf("%d Attempts Left\n", iCount);
        printf("--------------------------------------\n\n");
        goto reinitiate;
    }

    return true;
}

//function name: withdraw
//return type: void
//parameters: long long int llCardNo, int iAmt
//uses: to withdraw money from atm
bool checkAccount(long long int llAccountNo)
{
    if (first == NULL)
    {
        return false;
    }
    user *cur = first;
    while (cur != NULL)
    {
        if (cur->llAccountNo == llAccountNo)
        {
            return true;
        }
        cur = cur->link;
    }
    return false;
}

void withdraw(long long int llCardNo, int iAmt)
{
    user *cur = first;
    while (cur != NULL)
    {
        if (cur->llCardNo == llCardNo)
        {
            cur->fBalance -= iAmt; //UPDATES BALANCE
            return;
        }
        cur = cur->link;
    }
}

//function name:  logTnx
//return type: void
//parameters: long long int llAccountNo, long long int llCardNo, int iAmt
//uses: to open transaction file

void logTnx(long long int llAccountNo, long long int llCardNo, int iAmt, char tnxType[SIZE], int tnxSuccess)
{
    FILE *fpTnx = fopen("transaction_log.txt", "a");
    if (fpTnx == NULL)
    {
        printf("Error Cannot open file\n");
        exit(1);
    }
    // fprintf(fpTnx, "{\"Account No.\": \"%lld\", \"Card No.\": \"%lld\", \"Transaction Type\": \"%s\",\"Amount\": \"%d\"}\n", llAccountNo, llCardNo, tnxType, iAmt);
    fprintf(fpTnx, TNX_FORMAT_OUT, llAccountNo, llCardNo, tnxType, iAmt, tnxSuccess);
    fclose(fpTnx);
}

//function name:  printR
//return type: void
//parameters: long long int llCardNo, int iAmt
//uses: to print transaction Recipt

void logTnxA(long long int llAccountNo, int iAmt, char tnxType[SIZE])
{
    FILE *fpTnx = fopen("transaction_log.txt", "a");
    if (fpTnx == NULL)
    {
        printf("Error Cannot open file\n");
        exit(1);
    }
    fprintf(fpTnx, TNX_FORMAT_OUT, llAccountNo, tnxType, iAmt, 1);
    fclose(fpTnx);
}

void printRW(long long int llCardNo, int iAmt, int tnxSuccess)
{
    char tnxType[SIZE] = "Withdraw";
    user *cur = first;
    while (cur != NULL)
    {
        if (cur->llCardNo == llCardNo)
        {
            printf("---------Transaction Recipt-----------\n");
            printf("Account No       : %lld\n", cur->llAccountNo);
            printf("CardNo           : %lld\n", cur->llCardNo);
            printf("Amount Deposited : %d\n", iAmt);
            printf("Balance          : %f\n", checkBalance(llCardNo));
            printf("--------------------------------------\n");

            logTnx(cur->llAccountNo, cur->llCardNo, iAmt, tnxType, tnxSuccess);
            return;
        }
        cur = cur->link;
    }
}

void printRD(long long int llCardNo, int iAmt)
{
    user *cur = first;
    char txnType[SIZE] = "Card Deposit";
    while (cur != NULL)
    {
        if (cur->llCardNo == llCardNo)
        {
            printf("---------Transaction Recipt-----------\n");
            printf("Account No       : %lld\n", cur->llAccountNo);
            printf("CardNo           : %lld\n", cur->llCardNo);
            printf("Amount Deposited : %d\n", iAmt);
            printf("Balance          : %f\n", checkBalance(llCardNo));
            printf("--------------------------------------\n");

            logTnx(cur->llAccountNo, cur->llCardNo, iAmt, txnType, 1);
            return;
        }
        cur = cur->link;
    }
}

void printRA(long long int llAccountNo, int iAmt)
{
    user *cur = first;
    char tnxType[SIZE] = "CardlessDeposit";
    while (cur != NULL)
    {
        if (cur->llAccountNo == llAccountNo)
        {
            printf("---------Transaction Recipt-----------\n");
            printf("Account No       : %lld\n", cur->llAccountNo);
            printf("Amount Deposited : %d\n", iAmt);
            printf("Balance          : %f\n", checkBalanceA(llAccountNo));
            printf("--------------------------------------\n");

            logTnx(cur->llAccountNo, cur->llCardNo, iAmt, tnxType, 1);
            return;
        }
        cur = cur->link;
    }
}

void changePinServer(long long int llCardNo, int iNewPin)
{
    user *cur = first;
    while (cur != NULL)
    {
        if (cur->llCardNo == llCardNo)
        {
            cur->iPin = iNewPin; //UPDATES BALANCE
            return;
        }
        cur = cur->link;
    }
}

void ServerCardDeposit(long long int llCardNo, int iAmt)
{
    user *cur = first;
    while (cur != NULL)
    {
        if (cur->llCardNo == llCardNo)
        {
            cur->fBalance += iAmt; //UPDATES BALANCE
            return;
        }
        cur = cur->link;
    }
}

void ServerCardLessDeposit(long long int llAccountNo, int iAmt)
{
    user *cur = first;
    while (cur != NULL)
    {
        if (cur->llAccountNo == llAccountNo)
        {
            cur->fBalance += iAmt; //UPDATES BALANCE
            return;
        }
        cur = cur->link;
    }
}

void cardDeposit()
{
    long long int llCardNo;
    int iPin;
    int iAmt;

    reenterCard6:
    printf("Enter the card No. : ");
    if (!readCardNo(&llCardNo))
    {
        printError("Enter the Valid Card No");
        goto reenterCard6;
    }

    if (!verification(llCardNo))
    {
        return;
    }

    reenterAmtD:
    printf("Enter the amount to deposit : ");
    if (!readAmt(&iAmt))
    {
        printError("Enter a valid Amount");
        goto reenterAmtD;
    }

    if (iAmt > 100000)
    {
        printError("Only 1,00,000 allowed to deposit Per Transaction");
        return;
    }

    ServerCardDeposit(llCardNo, iAmt);

    printRD(llCardNo, iAmt);
    return;
}

void cardLessDeposit()
{
    long long int llAccountNo;
    int iAmt;

    reenterd:
    printf("Enter the Account No. : ");
    if (!readAccountNo(&llAccountNo))
    {
        printError("Enter the Valid Account No");
        goto reenterd;
    }
    if (!checkAccount(llAccountNo))
    {
        printError("Account Doesn't Exist");
        return;
    }

    reenterAmtCD:
    printf("Enter the amount to deposit : ");
    if (!readAmt(&iAmt))
    {
        printError("Enter a valid Amount");
        goto reenterAmtCD;
    }

    if (iAmt > 100000)
    {
        printError("Only 1,00,000 allowed to deposit Per Transaction");
        return;
    }

    ServerCardLessDeposit(llAccountNo, iAmt);

    printRA(llAccountNo, iAmt);

    return;
}