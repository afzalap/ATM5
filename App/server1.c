//For creating nodes, reading from file and writing to the file

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define SIZE 20

FILE *fp;

const char *USER_FORMAT_IN = "{\"Name\": \"%[^\"]\" ,\"Phone No.\": \"%[^\"]\" ,\"Account No.\": \"%lld\" ,\"Balance\": \"%f\" ,\"Card No.\": \"%lld\" ,\"ATM Pin\": \"%d\",\"isBlocked\": \"%d\"}\n";
const char *USER_FORMAT_OUT = "{\"Name\": \"%s\" ,\"Phone No.\": \"%s\" ,\"Account No.\": \"%lld\" ,\"Balance\": \"%f\" ,\"Card No.\": \"%lld\" ,\"ATM Pin\": \"%d\",\"isBlocked\": \"%d\"}\n";

const char *TNX_FORMAT_IN = "{\"Account No.\": \"%lld\" , \"Card No.\": \"%lld\" , \"Transaction Type\": \"%[^\"]\" , \"Amount\": \"%d\" , \"Transaction Status\": \"%d\"}\n";
const char *TNX_FORMAT_OUT = "{\"Account No.\": \"%lld\" , \"Card No.\": \"%lld\" , \"Transaction Type\": \"%s\" , \"Amount\": \"%d\" , \"Transaction Status\": \"%d\"}\n";

typedef struct user
{
    char sName[SIZE];
    char sPhoneNo[SIZE];
    long long int llAccountNo;
    float fBalance;
    long long int llCardNo;
    int iPin;
    int iBlocked;
    struct user *link;
} user;

user *first, *last = NULL;
int iAccCount = 0;

user *createnode()
{
    user *ptr;
    ptr = (user *)malloc(sizeof(user));

    if (ptr == NULL)
        printf("Can't allocate required memory\n");
    else
        return ptr;
}

//function name:   addUser
//return type: void
//parameters: char sName[], char sPhoneNo[], long long int llAccountNo, float fBalance, long long int llCardNo, int iPin, int iBlocked
//uses: to add user to createnode

void addUser(char sName[], char sPhoneNo[], long long int llAccountNo, float fBalance, long long int llCardNo, int iPin, int iBlocked)
{
    user *temp, *cur;
    temp = createnode();
    temp->link = NULL;
    strcpy(temp->sName, sName);
    strcpy(temp->sPhoneNo, sPhoneNo);
    temp->llAccountNo = llAccountNo;
    temp->fBalance = fBalance;
    temp->llCardNo = llCardNo;
    temp->iPin = iPin;
    temp->iBlocked = iBlocked;

    iAccCount++;

    if (last == NULL)
    {
        first = last = temp;
        return;
    }
    last->link = temp;
    last = temp;
}
//function name: display
//return type: void
//parameters: none
//uses: to display cur->sName, cur->sPhoneNo, cur->llAccountNo, cur->fBalance, cur->llCardNo, cur->iPin, cur->iBlocked

// for our reference
void display()
{
    user *cur = first;
    if (last == NULL)
    {
        printf("No Accounts\n");
        return;
    }
    printf("\n\n============================================================================================================\n");
        printf("============================================================================================================\n");
    printf("  %-12s\t%-15s\t%-12s\t%-10s\t%-12s\t%-6s\t\t%-3s\n", "Name", "Phone No.", "Account No.", "Balance", "Card No.", "Atm Pin", "isBlocked");
        printf("============================================================================================================\n\n");

    while (cur != NULL)
    {
        printf("  %-12s\t%-15s\t%-12lld\t%-10.3f\t%-12lld\t%-6d\t\t%-3d\n\n", cur->sName, cur->sPhoneNo, cur->llAccountNo, cur->fBalance, cur->llCardNo, cur->iPin, cur->iBlocked);
        cur = cur->link;
    }
                printf("============================================================================================================\n");
                            printf("============================================================================================================\n\n");
}
//function name:  fileWrite
//return type: void
//parameters: none
//uses: to write the file
void fileWrite()
{
    user *cur = first;
    fclose(fp);
    fp = fopen("Account_details.txt", "w");
    while (cur != NULL)
    {
        fprintf(fp, USER_FORMAT_OUT, cur->sName, cur->sPhoneNo, cur->llAccountNo, cur->fBalance, cur->llCardNo, cur->iPin, cur->iBlocked);
        cur = cur->link;
    }
    fclose(fp);
}
//function name:  fileOpen
//return type: void
//parameters: none
//uses: to open file

void fileOpen()
{
    fp = fopen("Account_details.txt", "r");
    if (fp == NULL)
    {
        printf("Error Cannot open file\n");
        exit(1);
    }
    fseek(fp, 0, SEEK_END);
    if (0 == ftell(fp))
    {
        printf("NO RECORDS!! File is empty\n");
        printf("Create new Account\n");
    }
    else
    {
        rewind(fp);
        char sName[SIZE];
        char sPhoneNo[SIZE];
        long long int llAccountNo;
        float fBalance;
        long long int llCardNo;
        int iPin;
        int iBlocked;
        while ((fscanf(fp, USER_FORMAT_IN, sName, sPhoneNo, &llAccountNo, &fBalance, &llCardNo, &iPin, &iBlocked)) != EOF)
        {
            addUser(sName, sPhoneNo, llAccountNo, fBalance, llCardNo, iPin, iBlocked);
        }
    }
}

//function name: checkValidString
//return type: bool
//parameters: char sName[]
//uses: to check valid string

bool checkValidString(char sName[])
{
    for (int i = 0; sName[i] != '\0'; i++)
    {
        if (!isalpha(sName[i]) || ispunct(sName[i]))
        {
            return false;
        }
    }
    return true;
}

//function name: checkValidPhoneNo
//return type: bool
//parameters: char sPhoneNo[]
//uses:  to check valid phone no
bool checkValidPhoneNo(char sPhoneNo[])
{
    int iCount = 0;
    for (int i = 0; sPhoneNo[i] != '\0'; i++)
    {
        if (!isdigit(sPhoneNo[i]) || ispunct(sPhoneNo[i]))
        {
            return false;
        }
        iCount++;
    }
    if (iCount == 10)
    {
        return true;
    }

    return false;
}
//function name: printError
//return type: void
//parameters: char *msg
//uses:  to print error message
void printError(char *msg)
{
    // printf("\n--------------------------------------\n");
    printf("\n**************************************\n");
    printf("Error : %s\n", msg);
    printf("**************************************\n\n");
    // printf("--------------------------------------\n\n");
}

//function name: printSuccess
//return type: void
//parameters: char *msg
//uses: to print success message
void printSuccess(char *msg)
{
    printf("\n--------------------------------------\n");
    // printf("\n***************************************\n");
    printf("%s\n", msg);
    // printf("***************************************\n\n");
    printf("--------------------------------------\n\n");
}
//function name:  readAccountNo
//return type:bool
//parameters: long long int *llAccountNo
//uses: to read account number
bool readAccountNo(long long int *llAccountNo)
{
    char *eptr;
    char sAccountNo[10];
    scanf("%s", sAccountNo);
    int i = 0;
    for (i = 0; sAccountNo[i] != '\0'; i++)
    {
        if (isalpha(sAccountNo[i]) || ispunct(sAccountNo[i]))
        {
            return false;
        }
    }
    if (i == 10)
    {
        *llAccountNo = strtoll(sAccountNo, &eptr, 10);
        return true;
    }
    else
    {
        return false;
    }
}

//function name: readCardNo
//return type:bool
//parameters: long long int *llCardNo
//uses: to read card number
bool readCardNo(long long int *llCardNo)
{
    char *eptr;
    char sCardNo[8];
    scanf("%s", sCardNo);
    int i = 0;
    for (i = 0; sCardNo[i] != '\0'; i++)
    {
        if (isalpha(sCardNo[i]) || ispunct(sCardNo[i]))
        {
            return false;
        }
    }
    if (i == 8)
    {
        *llCardNo = strtoll(sCardNo, &eptr, 10);
        return true;
    }
    else
    {
        return false;
    }
}

//function name:  readPin
//return type:bool
//parameters:int *iPin
//uses: to read pin

bool readPin(int *iPin)
{
    char sPin[5];
    scanf("%s", sPin);
    int i = 0;
    for (i = 0; sPin[i] != '\0'; i++)
    {
        if (isalpha(sPin[i]) || ispunct(sPin[i]))
        {
            return false;
        }
    }
    if (i == 4)
    {
        *iPin = atoi(sPin);
        return true;
    }
    else
    {
        return false;
    }
}
//function name:  readAmt
//return type:bool
//parameters:int *iAmt
//uses: to read amount
bool readAmt(int *iAmt)
{
    char sAmt[9];
    scanf("%s", sAmt);
    int i = 0;
    for (i = 0; sAmt[i] != '\0'; i++)
    {
        if (isalpha(sAmt[i]) || ispunct(sAmt[i]))
        {
            return false;
        }
    }

    *iAmt = atoi(sAmt);
    return true;
}