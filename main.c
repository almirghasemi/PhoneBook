/* This is the program is a console phone book.*/


/* It uses the following libraries. */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


/* It consists of these 5 functions below. */
void AddContact();
void DeleteContact();
void ShowContacts();
void Export();
void Import();


/* It has one global variable which indicates current number of contacts which is 0 initially.*/
int NumberOfContacts = 0;

/* It also has a global structure to represent a phone book.
This structure is made of:
    1. a string for first name.
    2. a string for last name.
    3. a string for phone number.
Since the number of contacts are going to change as the time goes, we are going to use dynamic allocation
of memory. Therefore, we also define a pointer of type PhoneBook structure.*/
struct PhoneBook {
char FirstName[20];
char LastName[20];
char PhoneNumber[15];
}*Contact;


/* We use AddContact function to add the information of a new contact.*/
void AddContact(){

    NumberOfContacts++; /* When we add a contact, first the number of contact is increased by 1 */

    /* if it is the first contact, we want to allocate memory for the first time using malloc().
     but how many bytes? as many as PhoneBook structure requires which can be obtained using sizeof().
     Contact pointer points to this region of memory in heap.*/
    if (NumberOfContacts == 1){Contact = malloc(sizeof(struct PhoneBook));}

    /*if it is not the first contact, we want to reallocate memory that had been allocated already, using realloc()*/
    else {Contact = realloc(Contact , sizeof(struct PhoneBook)*(NumberOfContacts));}

    /* In both cases above, if Contact pointer becomes null which means that either malloc() or realloc() has failed to allocate or reallocate memory,
    then our program shows the following massage and terminates the program*/
    if (!Contact){puts("\n Error in Memory Allocation! "); exit(1);}

    /*When memory is allocated successfully, we can read data and put it into members of our structure.*/
    puts("Enter the first name: ");
    gets((Contact+NumberOfContacts-1)->FirstName); /*reads the first name. one must consider that Nth contact is at (N-1)th pointer offset.
                                                            It does not do boundary checks nor spelling check.*/

    /* After getting the first name, we want to capitalize its first letter and having other letters in small forms.*/
    for(int i = 0 ; i < 20 ; i++){
        if (i==0){(Contact+NumberOfContacts-1)->FirstName[i] = toupper( (Contact+NumberOfContacts-1)->FirstName[i]);}
        else {(Contact+NumberOfContacts-1)->FirstName[i] = tolower( (Contact+NumberOfContacts-1)->FirstName[i]);}}

    /* Then we get the last name, and do the same procedure */
    puts("Enter the last name: ");
    gets((Contact+NumberOfContacts-1)->LastName);
    for(int i = 0 ; i < 20 ; i++){
        if (i==0){(Contact+NumberOfContacts-1)->LastName[i] = toupper( (Contact+NumberOfContacts-1)->LastName[i]);}
        else {(Contact+NumberOfContacts-1)->LastName[i] = tolower( (Contact+NumberOfContacts-1)->LastName[i]);}}

    /* Next, we get the phone number, it does not do boundary checking nor format checking*/
    puts("Enter the phone: ");
    gets((Contact+NumberOfContacts-1)->PhoneNumber);

    /*Then, we are going to sort the contact list based on their first names using qsort()*/
    qsort(Contact,NumberOfContacts, sizeof (struct PhoneBook), strcmp);

    /*At last, contact is added*/
     puts("\nContact Added Successfully\n\n");
        }

/* As the name indicates, it deletes a contact.*/
void DeleteContact(){

    /* To delete a contact, we must introduce a mechanism that user selects which contact to be deleted.
    In this case, we simply use contact`s index to select and delete it. */

    int index; /* An index variable */

    /* If there are no contacts, there is nothing to delete*/
    if (NumberOfContacts==0){puts("\nThere is no contact to delete\n");return;}

    /* If there is only one contact, there is no need to select. it will be deleted immediately.*/
    if (NumberOfContacts==1){puts("\nYour Only contact was deleted\n");NumberOfContacts--;free(Contact); return;}

    /*Otherwise...*/
    puts("\nWhich contact you want to delete?\nEnter its row number\n");

    ShowContacts();

    /*Reading the index number from user and suppressing the next carriage return character*/
    while(1){
    scanf("%i%*c", &index);

    /*If the entered number is wrong, user must try again.*/
    if(index<=0 || index>(NumberOfContacts)){puts("\nWrong Number\nTry Again");continue;}break;}

    /* Upon successful reading of index, the supposed contact first letter of its first name is going to get a large value,
    such that when it is sorted, it will be placed at the end of list.*/
    (Contact+index-1)->FirstName[0]=0b01111111;

    /* This sorting puts the deleting contact at the end of list.*/
    qsort(Contact,NumberOfContacts, sizeof (struct PhoneBook), strcmp);

    /* By reallocating and reducing the size of allocated memory, the last contact which is the deleting contact is going to be chopped of from the list*/
    Contact = realloc(Contact , sizeof(struct PhoneBook)*(NumberOfContacts-1));

    /* Reducing number of contacts by one*/
    NumberOfContacts--;

    /* Print the massage.*/
    puts("\nContact Deleted Successfully\n");
    return;

}

/* It displays the list of contacts*/
void ShowContacts(){

    /* If there is no contact, there is nothing to display.*/
    if(NumberOfContacts == 0 ){puts("\nThere is no Contact to show!\n"); return;}

    puts("\n");

    /*Iterating over sorted contacts and displaying their information.*/
    for(int counter = 0; counter<NumberOfContacts; counter++)
    {
        printf("\t%i. %s %s: %s\n", counter+1, (Contact+counter)->FirstName, (Contact+counter)->LastName, (Contact+counter)->PhoneNumber);
    }
    puts("\n");
}


/* This function stores the number of contacts in the first line of a file, and the contact information in the following lines in a .csv format
    The stored list is also sorted.*/
void Export(){

    int counter = 0; /* A local counter variable to iterate through contact list.*/

    FILE *fp; /* Defining a file pointer.*/

    fp = fopen("ContactList.txt","w"); /* We open a file and we definitely want to write to it.*/

    /*We can check to make sure the file is opened successfully.*/
    if (fp==NULL){puts("\nError in opening the file\n"); return;}

    fprintf(fp,"%i\n",NumberOfContacts); /* In its first line, we write the number of contacts and a newline.*/

    /* Then we write the contact information one by one in a line.
    Although it is convenient to use fprintf(), for faster writing,
    I use fputs() to write information which excludes null terminator here.
    Iterating until reaching the number of contacts*/
    while(counter<NumberOfContacts){

        /*Writing first name without null terminator.*/
        fputs((Contact+counter)->FirstName, fp);

        fputc(',',fp); /*Writing a ',' */

        /*Writing last name without null terminator.*/
        fputs((Contact+counter)->LastName, fp);

        fputc(',',fp);/*Writing a ',' */

        /*Writing phone number without null terminator.*/
        fputs((Contact+counter)->PhoneNumber, fp);

        fputc('\n',fp); /*Writing a new line character*/

        counter++; /* Going to next contact...*/

        fflush(fp); /*flushing the stream so that the contact is stored in file*/
        }

    fclose(fp); /* Closing the file when writing is finished*/

    /* Displaying massage of success*/
    puts("\nContact List exported successfully");
    return ;
    }

    /* Import function brings the contacts that had been stored previously*/
void Import(){

    int counter1 = 0; /* Counter1 is used to iterate over contacts in the file where contacts are stored.*/

    char buf[50];   /* buf array is defined to hold one line of information from file at a time. */

    FILE *fp; /* fp is a file pointer is used to open the file*/

    /* When a line in file is read, its data will be broken down to first name,
    last name and phone number which are stored in temp temporarily*/
    struct PhoneBook temp;

    fp = fopen("ContactList.txt", "r"); /*Opening the file in read mode*/

    if (fp == NULL){puts("\nNo File to Import\n");return;} /*If unsuccessful, it shows the massage and then skips the import procedure.*/

    /* The first line contains the number of contacts followed by
    a new line character. We read the number of contacts and skip the new line*/
    fscanf(fp,"%i%c", &NumberOfContacts);

    Contact = malloc(NumberOfContacts*(sizeof(struct PhoneBook))); /*When we know the number of contacts, we know how much memory to allocate*/

    /* Now we start to read line by line*/
    while (counter1<NumberOfContacts){

        fgets(buf,100,fp); /* fgets() function reads until either 99 characters are read OR a new line character '\n' is encountered which is the case.*/

        int counter2 = 0; /* counter2 is defined to iterate over elements of buffer. from beginning to the end of buffer.*/

        int counter3 = 0; /* counter3 is defined to iterate over elements between commas of the buffer.*/

        /* Starting from the beginning of the buffer, every character will be put in to first name of temp until ',' is reached*/
        while(buf[counter2] != ',')
        {   temp.FirstName[counter3] = buf[counter2];
            counter2++;
            counter3++;}

        temp.FirstName[counter3]='\0'; /*when comma is reached, instead of comma we put null terminator so that the string becomes null terminated.*/

        counter3 = 0; /* Resetting counter3 to zero again.*/

        counter2++; /* skipping over comma and going to next element after the first comma.*/

        /* Again starting from first element after the first comma, we put the characters of buffer into last name of temp until second comma is reached.*/
        while(buf[counter2] != ',')
        {   temp.LastName[counter3] = buf[counter2];
            counter2++;
            counter3++;}

        temp.LastName[counter3]='\0';  /*when second comma is reached, instead of comma we put null terminator so that the string becomes null terminated.*/

        counter3 = 0; /* Resetting counter3 to zero again.*/

        counter2++; /* skipping over comma and going to next element after the second comma.*/

        /* Again starting from first element after the second comma, we put the characters of buffer into phone number of temp until new line '\n' is reached.*/
        while(buf[counter2] != '\n')
        {   temp.PhoneNumber[counter3] = buf[counter2];
            counter2++;
            counter3++;}

        temp.PhoneNumber[counter3]='\0';  /*when new line is reached, instead of '\n' we put null terminator so that the string becomes null terminated.*/

        *(Contact+counter1)=temp; /* When temp holds the first name, last name and phone number of a contact, temp will be assigned to counter1 th element
        of Contact pointer. */

        counter1++; /*Going to next contact...*/

    }

    fclose(fp); /*When all lines are read, we close the file.*/
    return;

}

int main()
{
    char option; /* we will use option to determine what functions to call */

    Import(); /* Contacts are imported automatically, if there is existing file.*/

    while(1){

    /* Display options*/
    puts("1. Add Contact\n2. Delete Contact\n3. Show Contacts\n4. Export List\nSelect a number 1/2/3/4: ");

    /* Using getch() function to read a character because it is NOT line buffered nor echoed, which is suitable for interactive environment */
    option = getch();

    /*Using a switch-case and display options we call the corresponding function*/
    switch (option) {
    case '1': {AddContact();break;}
    case '2': {DeleteContact();break;}
    case '3': {ShowContacts();break;}
    case '4': {Export();break;}
    default : {puts("Invalid Input\n\n");}
    }
    }
    return 0;
}
