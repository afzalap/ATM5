#include "bankServer.c"

float fT;       //Amount in ATM
int iM = 10000; //limit per transaction
// int iK = 3;     // limit for no.of transactions per day

FILE *fpATM;

//function name:startAtm
//return type: void
//parameter: none
//use: to open atm file.
void startAtm()
{
    fpATM = fopen("atm_details.txt", "r");
    fscanf(fpATM, "%f", &fT);
    fclose(fpATM);
}

//function name:closeAtm
//return type:void
//parameter: none
//use: to close atm file
void closeAtm()
{
    fpATM = fopen("atm_details.txt", "w");
    fprintf(fpATM, "%f", fT);
    fclose(fpATM);
}

//function name: withDrawAtm
//return type:void
//parameter: none
//use: to withdraw amount from atm
void withDrawAtm()
{
    long long int llCardNo;
    int iPin;
    int iAmt;
    int choice;
    int tnxSuccess = 0;
reenterCard:
    printf("Enter the card No. : ");
    if (!readCardNo(&llCardNo))
    {
        printError("Enter the Valid Card No");
        goto reenterCard;
    }
    // scanf("%lld", &llCardNo);

    if (!verification(llCardNo))
    {
        return;
    }

reenterAmtW:
    printf("Enter the amount to be withdrawn: ");
    if (!readAmt(&iAmt))
    {
        printError("Enter a valid Amount");
        goto reenterAmtW;
    }

    if (fT < iAmt || iAmt > checkBalance(llCardNo))
    {
        printError("Insufficient Balance\nTransaction Failed");
        return;
    }
    if (iAmt > iM)
    {
        printError("only 10,000 Allowed Per transaction\nTransaction Failed");
        return;
    }

    withdraw(llCardNo, iAmt);
    fT -= iAmt;

    while (true)
    {
        printf("press 1 to collect card\n");
        scanf("%d", &choice);
        if (choice == 1)
        {
            printSuccess("Card Collected");
            break;
        }
        printf("--------------------------------------\n");
    }

    while (true)
    {
        printf("press 1 to collect cash\n");
        scanf("%d", &choice);
        if (choice == 1)
        {
            printSuccess("Cash Collected");
            break;
        }
        printf("--------------------------------------\n");
    }
    while (true)
    {
        printf("Press 1 if Transcation was Successful\n");
        printf("Press 2 if Transcation was Not Successful\n");
        scanf("%d", &choice);
        if (choice == 1)
        {
            tnxSuccess = 1;
            printSuccess("Transaction was Successful");
            break;
        }
        else{
            break;
        }
        printf("--------------------------------------\n");
    }
    printRW(llCardNo, iAmt, tnxSuccess);
}

//function name: checkBalanceAtm
//return type:void
//parameters: none
//use: check balance from atm
void checkBalanceAtm()
{
    long long int llCardNo;
    int iPin;
reenterCard2:
    printf("Enter the card No. : ");
    if (!readCardNo(&llCardNo))
    {
        printError("Enter the Valid Card No");
        goto reenterCard2;
    }
    if (!verification(llCardNo))
    {
        return;
    }
    printf("Balance : %f\n", checkBalance(llCardNo));
    printf("--------------------------------------\n");
}

//function name:  changePin
//return type:void
//parameter: none
//use: to change pin
void changePin()
{
    long long int llCardNo;
    int iNewPin;
    int iConfPin;

reenterCard3:
    printf("Enter the card No. : ");
    if (!readCardNo(&llCardNo))
    {
        printError("Enter the Valid Card No");
        goto reenterCard3;
    }

    if (!verification(llCardNo))
    {
        return;
    }

reEnterPin4:
    printf("Enter the new 4 digit atm iPin : ");
    if (!readPin(&iNewPin))
    {
        printError("Pin should be 4 digit number");
        goto reEnterPin4;
    }
    printf("Confirm pin : ");
    scanf("%d", &iConfPin);

reEnterPin5:
    printf("Confirm pin : ");
    if (!readPin(&iConfPin))
    {
        printError("Pin should be 4 digit number");
        goto reEnterPin5;
    }
    if (iNewPin == iConfPin)
    {
        changePinServer(llCardNo, iNewPin);
        printSuccess("Pin changed successfully");
        return;
    }
    else
    {
        printError("New Pin Doesn't match with confirmation pin\nSorry Could not change Pin");
        return;
    }

    return;
}

//function name:  depositAtm
//return type:void
//parameter: none
//use: to deposit money
void depositAtm()
{
    int iChoice;
    while (true)
    {
        printf("Deposit Method\n");
        printf("1.Card\n2.Cardless\n");
        scanf("%d", &iChoice);
        switch (iChoice)
        {
        case 1:
            cardDeposit();
            return;
        case 2:
            cardLessDeposit();
            return;

        default:
            break;
        }
    }
}

int main()
{
    startAtm();
    fileOpen();
    printf("-----------------------Welcome to the ATM----------------------\n");
    if (fT == 0)
    {
        printf("-----------------------NO SERVICE OUT OF CASH------------------\n");
        exit(0);
    }
    int choice;
    while (1)
    {
        printf("--------------------------------------\n");
        printf("1.Withdraw\n2.Check Balance\n3.Change Pin\n4.Deposit\n5.Exit\n");
        printf("--------------------------------------\n");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            withDrawAtm();
            break;
        case 2:
            checkBalanceAtm();
            break;
        case 3:
            changePin();
            break;
        case 4:
            depositAtm();
            break;
        case 5:
            closeAtm();
            fileWrite();
            exit(0);
        case 6:
            display();
            break;
        }
        printf("--------------------------------------\n");
    }
    return 0;
}