#include "server1.c"

//function name: createsAccountNo
//return type:long long int
//parameters: none
//use: to create account in bank
long long int createsAccountNo()
{
    long long int llFirstAccount = 1132568899;
    if (iAccCount == 0)
    {
        return llFirstAccount;
    }
    return (llFirstAccount + iAccCount * 10);
}

//function name: createCardNo
//return type:long long int
//parameters: none
//use: to create card number
long long int createCardNo()
{
    long long int llFirstCard = 0405551214;
    if (iAccCount == 0)
    {
        return llFirstCard;
    }
    return (llFirstCard + iAccCount * 10);
}
//function name: creatAccount
//return type: void
//parameters: none
//use: to create Account in bank

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
//function name:  checkCard
//return type:bool
//parameter: long long int llCardNo
//use: to check card
bool checkCard(long long int llCardNo)
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
            return true;
        }
        cur = cur->link;
    }
    return false;
}

//function name:   checkBalance
//return type: float
//parameter: long long int llCardNo
//use: to checkBalance
float checkBalance(long long int llAccountNo)
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

//function name:   creatAccount
//return type: void
//parameter: none
//use: to creat account
void creatAccount()
{
    char sName[SIZE];
    char sPhoneNo[SIZE];
    long long int llAccountNo;
    float fBalance;
    long long int llCardNo;
    int iPin;
    int iBlocked;

reEnterName:
    printf("Enter the Name of account holder : ");
    scanf("%s", sName);
    if (!checkValidString(sName))
    {
        printError("Invalid Name");
        goto reEnterName;
    }
reEnterPhoneNo:
    printf("Enter the phone number of account holder : ");
    scanf("%s", sPhoneNo);
    if (!checkValidPhoneNo(sPhoneNo))
    {
        printError("Invalid Phone No");
        goto reEnterPhoneNo;
    }
    llAccountNo = createsAccountNo();
    printf("Enter the amount to be deposited : ");
    scanf("%f", &fBalance);
    llCardNo = createCardNo();
reEnterPin:
    printf("Enter the new 4 digit atm iPin : ");
    if (!readPin(&iPin))
    {
        printError("Pin should be 4 digit number");
        goto reEnterPin;
    }

    iBlocked = 0; //false, card is not blocked

    addUser(sName, sPhoneNo, llAccountNo, fBalance, llCardNo, iPin, iBlocked);
}

//function name: displayBalance
//return type: void
//parameters: none
//use: to display balance

void displayBalance()
{
    long long int llAccountNo;
reenterdb:
    printf("Enter the Account No. : ");
    // scanf("%lld", &llAccountNo1);
    if (!readAccountNo(&llAccountNo))
    {
        printError("Enter the Valid Account No");
        goto reenterdb;
    }
    if (!checkAccount(llAccountNo))
    {
        printError("Account Doesn't Exist");
        return;
    }
    if (first == NULL)
    {
        return;
    }
    user *cur = first;
    while (cur != NULL)
    {
        if (cur->llAccountNo == llAccountNo)
        {
            printf("current balance = %f\n", cur->fBalance); //prints balance
            return;
        }
        cur = cur->link;
    }
    return;
}

//function name: deposit
//return type: void
//parameters: long long llAccountNo, int iAmt
//use: to deposit money in bank

void deposit(long long llAccountNo, int iAmt)
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

//function name: depositBank
//return type: void
//parameters: none
//use: to deposit in bank

void depositBank()
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
    deposit(llAccountNo, iAmt);
    printf("---------Transaction Details----------\n");
    printf("Account No       : %lld\n", llAccountNo);
    printf("Amount Deposited : %d\n", iAmt);
    printf("Balance          : %f\n", checkBalance(llAccountNo));
    printf("--------------------------------------\n");
}

void withdraw(long long llAccountNo, int iAmt)
{
    user *cur = first;
    while (cur != NULL)
    {
        if (cur->llAccountNo == llAccountNo)
        {
            cur->fBalance -= iAmt; //UPDATES BALANCE
            return;
        }
        cur = cur->link;
    }
}

//function name: withdrawBank
//return type: void
//parameters: none
//use: to withdraw money from  bank

void withdrawBank()
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

reenterAmtW:
    printf("Enter the amount to withdraw : ");
    if (!readAmt(&iAmt))
    {
        printError("Enter a valid Amount");
        goto reenterAmtW;
    }
    if (iAmt > 100000)
    {
        printError("Only 1,00,000 allowed to withdrawn Per Transaction");
        return;
    }
    if (iAmt > checkBalance(llAccountNo))
    {
        printError("Insufficient Balance");
        return;
    }
    withdraw(llAccountNo, iAmt);
    deposit(llAccountNo, iAmt);
    printf("---------Transaction Details----------\n");
    printf("Account No       : %lld\n", llAccountNo);
    printf("Amount withdrawn : %d\n", iAmt);
    printf("Balance          : %f\n", checkBalance(llAccountNo));
    printf("--------------------------------------\n");
}

//function name: accountTransfer
//return type: void
//parameters: none
//use: to transfer money from one account to another account

void accountTransfer()
{
    long long int llAccountNo1;
    long long int llAccountNo2;
    int iAmt;
reenter1:
    printf("Enter the From Account No. : ");
    if (!readAccountNo(&llAccountNo1))
    {
        printError("Enter the Valid Account No");
        goto reenter1;
    }

    if (!checkAccount(llAccountNo1))
    {
        printError("Account Doesn't Exist");
        return;
    }

reenter:
    printf("Enter the To Account No. : ");
    if (!readAccountNo(&llAccountNo2))
    {
        printError("Enter the Valid Account No");
        goto reenter;
    }
    if (!checkAccount(llAccountNo2))
    {
        printError("Account Doesn't Exist");
        return;
    }

    if (llAccountNo1 == llAccountNo2)
    {
        printError("Both Accounts are same");
        return;
    }

reenterAT:
    printf("Enter the amount to be transfered : ");
    if (!readAmt(&iAmt))
    {
        printError("Enter a valid Amount");
        goto reenterAT;
    }

    if (iAmt > checkBalance(llAccountNo1))
    {
        printError("Insufficient Balance");
        return;
    }

    withdraw(llAccountNo1, iAmt);
    deposit(llAccountNo2, iAmt);
    printf("---------Transaction Details----------\n");
    printf("From Account No       : %lld\n", llAccountNo1);
    printf("Transfered to         : %lld\n", llAccountNo2);
    printf("Amount Transfered     : %d\n", iAmt);
    printf("Balance               : %f\n", checkBalance(llAccountNo1));
    printf("--------------------------------------\n");
}
//function name: unBlockcard
//return type: void
//parameters: none
//use: to unblock the card

void unBlockCard()
{
    long long int llCardNo;

reenterPin:
    printf("Enter the card No. : ");
    if (!readCardNo(&llCardNo))
    {
        printError("Enter the Valid Card No");
        goto reenterPin;
    }

    if (!checkCard(llCardNo))
    {
        printError("card Doesn't Exist");
        return;
    }

    user *cur = first;
    while (cur != NULL)
    {
        if (cur->llCardNo == llCardNo)
        {
            if (cur->iBlocked == 0)
            {
                printSuccess("Card is not blocked");
                return;
            }
            cur->iBlocked = 0;
            printSuccess("Card UnBlocked Successfully");
            return;
        }
        cur = cur->link;
    }
    printError("Card UnBlocking Unsuccessful");
}
//function name: changePhoneNo
//return type: void
//parameters: none
//use: to change phone number

void changePhoneNo()
{
    long long int llAccountNo;
    char sNewPhoneNo[SIZE];
reentercp:
    printf("Enter the Account No. : ");
    if (!readAccountNo(&llAccountNo))
    {
        printError("Account Doesn't Exist");
        goto reentercp;
    }
    if (!checkAccount(llAccountNo))
    {
        printError("Account Doesn't Exist");
        return;
    }

reEnterPhoneNocp:
    printf("Enter the new Phone No.: ");
    scanf("%s", sNewPhoneNo);
    if (!checkValidPhoneNo(sNewPhoneNo))
    {
        printError("Invalid Phone No");
        goto reEnterPhoneNocp;
    }
    user *cur = first;
    while (cur != NULL)
    {
        if (cur->llAccountNo == llAccountNo)
        {

            strcpy(cur->sPhoneNo, sNewPhoneNo);
            printSuccess("Phone Number changed Successfully");
            return;
        }
        cur = cur->link;
    }
    printError("Phone Number changing was unsuccessful");
}
//function name:  closeAccount
//return type: void
//parameters: none
//use: to close account

void closeAccount()
{
    long long int llAccountNo;
reenterca:
    printf("Enter the Account No. : ");
    if (!readAccountNo(&llAccountNo))
    {
        printError("Enter the Valid Account No");
    }
    if (!checkAccount(llAccountNo))
    {
        printError("Account Doesn't Exist");
        return;
    }

    if ((first->llAccountNo) == llAccountNo)
    {
        user *cur = first;
        user *temp = first;
        first = first->link;

        if (first == NULL)
        {
            last = NULL;
            printSuccess("Account Closed Successfully");
        }

        free(temp);
        return;
    }
    if (last->llAccountNo == llAccountNo)
    {

        user *cur = first;
        user *prev = NULL;

        while (cur->link != NULL)
        {
            prev = cur;
            cur = cur->link;
        }
        printSuccess("Account Closed Successfully");
        free(cur);
        prev->link = NULL;
        return;
    }
    else
    {
        user *cur = first;
        user *prev = NULL;
        while (cur->llAccountNo != llAccountNo)
        {
            prev = cur;
            cur = cur->link;
        }
        printSuccess("Account Closed Successfully");
        prev->link = cur->link;
        free(cur);
        return;
    }
    printError("Account Closing was unsuccessful");
}

//function name:  revertUnsuccessful
//return type: void
//parameters: none
//use: to unsuccessful transaction


void revertUnsuccessful()
{
    FILE *Tnx = fopen("transaction_log.txt", "r");
    if (Tnx == NULL)
    {
        printf("Error Cannot open file\n");
        exit(1);
    }
    fseek(Tnx, 0, SEEK_END);
    if (0 == ftell(Tnx))
    {
        printf("NO RECORDS!! File is empty\n");
        printf("Create new Account\n");
    }
    else
    {
        rewind(Tnx);
        int r = 0;
        FILE *temp = fopen("temp_transaction_log.txt", "w");
        long long int llAccountNo;
        long long int llCardNo;
        char sTnxType[SIZE];
        int iAmt;
        int tnxSuccess;
        while ((fscanf(Tnx, TNX_FORMAT_IN, &llAccountNo, &llCardNo, sTnxType, &iAmt, &tnxSuccess)) != EOF)
        {
            if (tnxSuccess == 1)
            {
                fprintf(temp, TNX_FORMAT_OUT, llAccountNo, llCardNo, sTnxType, iAmt, tnxSuccess);
            }
            if (tnxSuccess == 0)
            {
                r = 1;
                deposit(llAccountNo, iAmt);
            }
        }
        fclose(Tnx);
        fclose(temp);
        printf("Hello");

        remove("transaction_log.txt");
        rename("temp_transaction_log.txt", "transaction_log.txt");
        if (r == 1)
        {
            printSuccess("Unsucessful Transactions Reverted");
        }
        if (r == 0)
        {
            printError("No Unsucessful Transactions to revert");
        }
    }
}

int main()
{
    fileOpen();
    int choice;
    printf("---------Welcome to Bank------------\n");
    while (1)
    {
        printf("--------------------------------------\n");
        printf("1.More Options\n2.Exit\n");
        printf("--------------------------------------\n");
        scanf("%d", &choice);
        if (choice == 1)
        {
            while (1)
            {
                printf("--------------------------------------\n");
                printf("1.Create Account\n2.Deposit\n3.Withdraw\n4.Display Balance\n5.Account Transfer\n6.Unblock Card\n7.Change Phone No.\n8.Close Account\n9.Revert Unsuccessful Transaction\n10.Display\n11.Exit\n");
                printf("--------------------------------------\n");
                scanf("%d", &choice);
                switch (choice)
                {
                case 1:
                    creatAccount();
                    break;
                case 2:
                    depositBank();
                    break;
                case 3:
                    withdrawBank();
                    break;
                case 4:
                    displayBalance();
                    break;
                case 5:
                    accountTransfer();
                    break;
                case 6:
                    unBlockCard();
                    break;
                case 7:
                    changePhoneNo();
                    break;
                case 8:
                    closeAccount();
                    break;
                case 9:
                    revertUnsuccessful();
                    break;
                case 10:
                    display();
                    break;
                case 11:
                    fileWrite();
                    exit(0);
                    break;
                }
                printf("--------------------------------------\n");
            }
        }
        if (choice == 2)
        {
            exit(0);
        }
    }

    return 0;
}
