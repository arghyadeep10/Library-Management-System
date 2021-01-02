#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

/*Description of this program:

A student has two text files unique to his/her student ID.
1. Text File -> xleria00xacd.txt (current records) (here the x is a global variable which will be inserted into the string name, to generate new student IDs)
2. Text File -> xleria00xtra.txt (all records) (here the x is a global variable which will be inserted into the string name, to generate new student IDs)

xleria000xacd is the student ID of a student

librarian.c is the librarian side program. When a student visits the librarian and borrows or returns a book, we will accordingly make a change
to that student's records by changing the values in his/her unique text files.

A student can ask the librarian to do 2 things :
1. Issue a new book
2. Return a borrowed book

Whenever a new book is issued or an old book is returend, it needs to be printed in records history text file (i.e xleria00xtra.txt) of that student.

A librarian needs to login using his ID and password. Both will be encrypted using a hash function.
The login details of librarians will be stored in librarian.txt

When a librarian wishes to login we will first load all librarian data from the librarian.txt to an array of structures.
Then we ask him for his login id and password and check first whether it is a valid login id. If yes then we first encrypt
the entered password using the hash function and check whether it matches to ciphertext password stored alongside that login id
If yes, then we grant access, else we print Wrong Login ID OR password and ask him whether he wishes to continue or exit

We will use the word student and customer interchangeably

*/

const int N = 5; //N is the size of login_id



typedef struct
{
    char *login_id; //login_id of librarian
    char *psswd_bk; //password of librarian
}
librarian;

typedef struct
{
    char bookid[100];
    char book_name[100];
    char author_name[100];
    char genre[100];
    char issue_status[100]; //("issued" for issued) or ("not_issued" for not issued)
    char issue_student_id[100]; //(if book isn't issued then it will be NIL)
    char date_issue[11]; //date is a 10 character string of the type yyyy-mm-dd
    char date_due[11]; //date is a 10 character string of the type yyyy-mm-dd
    //if book isn't issued then issue date and due date will be NIL
}
book;

typedef struct
{
    char name[100];
    char phone[100];
    char email[100];
    char govt_id[100];
    char age[100];
    char address[1000];
    char pd[100]; //the pd here will the ciphertext
    char book_count[1]; //book_count max is 4
}
customer;

int display_student(char STUDENTIT[100]);
int static_display_book(char BOOKID[100]);
int display_book(char BOOKID[100]);
int student_display_menu();
int book_display_menu();
int count_all_books();
bool hash_check(char plaintext[], char *cipher);
int create_libr_account();
bool login();
int issue(char acno[]);
int return_book(char acno[]);
int entry_book();
int view_all_books();
int student_operations(char acno[]);
int edit_book(char BKID[100]);
int search_book_by_name(char bk_nm[100]);
int view_issued();


int main(void)
{

    int hola;

    do
    {
    printf("\n\nWELCOME TO XLERIA LIBRARY (LIBRARIAN PORTAL) !\n\n");
    printf("Kindly enter the serial number of the option you wish to execute\n\n");
    printf("1.Login \n2.Create new Librarian Profile \n3.Exit\n");

    scanf(" %d",&hola);

    if(hola==1) //librarian wishes to login
    {
         bool login_status = login();

            if(login_status == false)
            {
                printf("Try again \n");
            }

            else //this else block will contain all statements that are to be executed when librarian logs in successfully
            {
                int op;

                do
                {
                printf("You are now logged in\n");
                printf("Select the option that you wish to execute :\n");
                printf("1.Issue and Return Books\n");
                printf("2.Entry data for new book\n");
                printf("3.View Details of a student\n");
                printf("4.View Details of a book\n");
                printf("5.View all books\n");
                printf("6.View total number of books in library\n");
                printf("7.Edit a Book's detail\n");
                printf("8.View all the currently issued books\n");
                printf("9.Exit\n");

                scanf(" %d",&op);
                if(op == 9)
                {
                    printf("Goodbye !\n");
                    return 0;
                }
                else if(op == 1)
                {
                    printf("Enter Student/Customer ID.: \n");
                    char acno[100];
                    scanf(" %s",acno);
                    student_operations(acno);
                }
                else if(op == 2)
                {
                    entry_book();
                }
                else if(op == 3)
                {
                    student_display_menu();
                }
                else if(op == 4)
                {
                    book_display_menu();
                }
                else if(op == 5)
                {
                    view_all_books();
                }
                else if(op == 6)
                {
                    count_all_books();
                }
                else if(op == 7)
                {
                    printf("Enter Book ID of the book that you want to edit\n");
                    char BOOK_ID[100];
                    scanf(" %s",BOOK_ID);
                    edit_book(BOOK_ID);
                }
                else if(op == 8)
                {
                    view_issued();
                }
                else
                {
                   printf("\n\n>>Invalid Input. Try again\n\n");
                }
                }
                while(op!=9);
            }

    }
    else if(hola==2)
    {
        create_libr_account();
    }
    else if(hola==3)
    {
        return 0;
    }
    else
    {
        printf("\n\n>>Invalid Input. Try again \n\n");
    }
}
while(hola!=3);


    return 0;
}


bool hash_check(char plaintext[], char *cipher) //the hash function accepts the plaintext password and the ciphertext to which we should tally
{
    if(strlen(plaintext)!=strlen(cipher))
    {
        return false;
    }

    char p;
    char *key = "YTNSHKVEFXRBAUQZCLWDMIPGJO";
    int j=0;

    while(plaintext[j]!='\0')
    {
        if(plaintext[j]>=65&&plaintext[j]<=90) //the character is upper case letter
        {
        p=plaintext[j];
        plaintext[j] = key[p-65];
        j++;
        }
        else if(plaintext[j]>=97&&plaintext[j]<=122) //the character is lower case letter
        {
        p=toupper(plaintext[j]);
        plaintext[j] = tolower(key[p-65]);
        j++;
        }
        else
        {
        j++;
        }
    }

    if(strcmp(plaintext,cipher)==0)
    {
        return true;
    }

    return false;
}


int create_libr_account()
{
    FILE *ptr = fopen("libr_detail.txt","r");
    char character = getc(ptr);
    fclose(ptr);

    int libr_count = character - '0';

    char name[100];
    char phone[100];
    char email[100];
    char govt_id[100];
    char age[100];
    char address[1000];
    char plaintext[100];
    char temp_c_pd[100];

    printf("\nName: ");
    scanf(" %s",name);
    printf("\nPhone: ");
    scanf(" %s",phone);
    printf("\nEmail ID: ");
    scanf(" %s",email);
    printf("\nGovt ID: ");
    scanf(" %s",govt_id);
    printf("\nAge: ");
    scanf(" %s",age);
    printf("\nAddress: ");
    scanf(" %s",address);
    printf("\nPassword: ");
    scanf(" %s",plaintext);
    printf("\nConfirm Password: ");
    scanf(" %s",temp_c_pd);

    if(strcmp(plaintext,temp_c_pd)==0)
    {
            //lets convert the plaintext into ciphertext
            char p;
            char *key = "YTNSHKVEFXRBAUQZCLWDMIPGJO";
            int j=0;

            while(plaintext[j]!='\0')
            {
                if(plaintext[j]>=65&&plaintext[j]<=90) //the character is upper case letter
                {
                p=plaintext[j];
                plaintext[j] = key[p-65];
                j++;
                }
                else if(plaintext[j]>=97&&plaintext[j]<=122) //the character is lower case letter
                {
                p=toupper(plaintext[j]);
                plaintext[j] = tolower(key[p-65]);
                j++;
                }
                else
                {
                j++;
                }
            }


            //first let increment the number of librarians in libr_details.txt
            libr_count++;
            FILE *fptr = fopen("libr_detail.txt","w");
            fputc(character+1,fptr);
            fclose(fptr);

            //lets generate a/c no. for this librarian. Lets also generate the 2 txt files that will be required.
            char *acd = malloc(14);
            sprintf(acd, "xleria%04ilib", libr_count);

            char *acdtxt = malloc(17);
            sprintf(acdtxt, "xleria%04ilib.txt", libr_count);


            FILE *ptracd = fopen(acdtxt,"w");
            fclose(ptracd);

            //lets input the ac data inside the libr.txt file
            FILE *file_acd = fopen(acdtxt,"w");
            fprintf(file_acd,"%s\n",plaintext);
            fprintf(file_acd,"%s\n",name);
            fprintf(file_acd,"%s\n",phone);
            fprintf(file_acd,"%s\n",email);
            fprintf(file_acd,"%s\n",govt_id);
            fprintf(file_acd,"%s\n",age);
            fprintf(file_acd,"%s\n",address);


            fclose(file_acd);

            printf("Your librarian ID is : %s\n",acd);

    }
    else
    {
        printf("\n\n>>Password and Confirm Password do not match. Try again. \n\n");
        return 0;
    }

    return 0;

}


bool login()
{
    printf("\nLogin ID : ");
    char LOG_ID[100];
    scanf("%s",LOG_ID);

    printf("\nPassword : ");
    char PS[100];
    scanf("%s",PS);

    //open the file with entered LOG_ID
    char *acdtxt = malloc(17);
    sprintf(acdtxt, "%s.txt", LOG_ID);

    FILE *opn = fopen(acdtxt,"r");
    if(opn == NULL)
    {
        printf("\n\n>>The Login ID entered by you is incorrect\n\n");
        return false;
    }

    char CPD[100];
    fscanf(opn,"%s",CPD);

    fclose(opn);

    bool status = hash_check(PS,CPD);

    if(status == true)
    {
        printf("\n\n>>Login Successful\n\n");
        return true;
    }
    else if(status == false)
    {
        printf("\n\n>>The Login ID or password entered by you is incorrect\n\n");
        return false;
    }

    return false;
}

int issue(char acno[]) //the function accepts the student ID as argument
                       //acno is the student ID of the student/customer
{
    //if BOOKID exists then customer can issue the book (since each book has its own text file, use a file ptr for check)
    //if file_ptr == NULL, it means book doesn't exist
        //load data of this book from this book's text file (if it exists)
        //write the data for new book in student/customer's file. Also after issuing this book, if student doesn't have
        //all 4 books issued, then put NIL in place of book_id and book_name of empty fields.
        //write the data for issue of this book in this book's text file
    //refer to file sample_book.txt to understand the framework of a book's text file



    char issue_date[11];
    printf("Enter today's date (dd-mm-yyyy) (This will be issue date) : \n");
    scanf("%s",issue_date);

    char due_date[11];
    printf("Enter due date (dd-mm-yyyy) : \n");
    scanf("%s",due_date);


    printf("Enter bookID that you want to issue : \n");
    char BOOKID[100];
    scanf("%s",BOOKID);



    //first lets open the book's file and set issue status to issued and enter the studen't ID, issue_date and due_date
    //lets first generate the filename
    int size = strlen(BOOKID);
    char *filename = malloc(size + 4 + 1);
    sprintf(filename,"%s.txt",BOOKID);

    FILE *bookptr = fopen(filename,"r");
    if(bookptr == NULL)
    {
        printf("\n\n>>BookID is invalid or this book doesn't exist\n\n");
        return 1;
    }

    book book_details;
    //we will store details of this book in the variable book_details of type book
    fscanf(bookptr,"%s",book_details.bookid);
    fscanf(bookptr,"%s",book_details.book_name);
    fscanf(bookptr,"%s",book_details.author_name);
    fscanf(bookptr,"%s",book_details.genre);
    fscanf(bookptr,"%s",book_details.issue_status);

    fclose(bookptr);
    /*fscanf(bookptr,"%s",book_details.issue_student_id);
    fscanf(bookptr,"%s",book_details.date_issue);
    fscanf(bookptr,"%s",book_details.date_due);
    */

    if(strcmp(book_details.issue_status,"issued")==0)
    {
        printf("\n\n>>This book is currently issued to another student\n\n");
        return 1;
    }

    strcpy(book_details.issue_status,"issued");
    strcpy(book_details.issue_student_id,acno);
    strcpy(book_details.date_issue,issue_date);
    strcpy(book_details.date_due,due_date);

    //now lets write this new data to the book's file
    FILE *bookptrW = fopen(filename,"w");

    fprintf(bookptrW,"%s\n",book_details.bookid);
    fprintf(bookptrW,"%s\n",book_details.book_name);
    fprintf(bookptrW,"%s\n",book_details.author_name);
    fprintf(bookptrW,"%s\n",book_details.genre);
    fprintf(bookptrW,"%s\n",book_details.issue_status);
    fprintf(bookptrW,"%s\n",book_details.issue_student_id);
    fprintf(bookptrW,"%s\n",book_details.date_issue);
    fprintf(bookptrW,"%s\n",book_details.date_due);

    fclose(bookptrW);

    //now lets edit the student's file

    //first lets load the details of customer into a structure of type customer and name cust
    char *acnotxt = malloc(17);
    sprintf(acnotxt, "%s.txt", acno);

    FILE *file_pointer = fopen(acnotxt,"r");

    customer cust;

    fscanf(file_pointer,"%s",cust.name);
    fscanf(file_pointer,"%s",cust.phone);
    fscanf(file_pointer,"%s",cust.email);
    fscanf(file_pointer,"%s",cust.govt_id);
    fscanf(file_pointer,"%s",cust.age);
    fscanf(file_pointer,"%s",cust.address);
    fscanf(file_pointer,"%s",cust.pd);
    fscanf(file_pointer,"%s",cust.book_count);

    int temp = cust.book_count[0] - '0';
    /*
    cust.book_count[0] = cust.book_count[0] - '0';
    int temp = cust.book_count[0];
    */
    if(temp == 4)
    {
        printf("\n\n>>Cannot issue anymore books as customer currently holds 4 books\n\n");
        return 0;
    }

    //the customer currently holds cust.book_count[0] number of books

    book cust_books[4];
    //now lets load the books that customer currently holds (if he/she doesn't hold all 4 books, then those slots would be NIL)
    for(int i=0;i<temp;i++)
    {
       fscanf(file_pointer,"%s",cust_books[i].bookid);
    }
    //after issuing the new book, we just need to add these
    //already issued books at the end.
    //after putting the new issued book in the student's account
    //and these old books, we will be left with 4-(cust.book_count[0] + 1) empty book slots for that student
    //In these 4-(cust.book_count[0] + 1) empty slots we need to print NIL in the file of that student
    fclose(file_pointer);

    //at this point we have stored the details of this student/customer in our program memory inside the customer variable cust
    //we already have the student's complete file data in the variable cust of type customer
    //the bookid of books currently in posession of customer are stored in the array of structure called cust_books[]
    //of type book.

    strcpy(cust_books[temp].bookid,BOOKID);

    for(int i = temp + 1; i<4 ; i++)
    {
        strcpy(cust_books[i].bookid,"NIL");
    }

    int temp_print = temp + 1;
    char str[1];
    sprintf(str,"%d",temp_print);

    strcpy(cust.book_count,str); //incrementing the no. of books in possession of student

    //now lets write this new data in the text file of the student

    FILE *file_pointerW = fopen(acnotxt,"w");

    cust.book_count[0] = (char)cust.book_count[0];

    fprintf(file_pointerW, "%s\n", cust.name);
    fprintf(file_pointerW, "%s\n", cust.phone);
    fprintf(file_pointerW, "%s\n", cust.email);
    fprintf(file_pointerW, "%s\n", cust.govt_id);
    fprintf(file_pointerW, "%s\n", cust.age);
    fprintf(file_pointerW, "%s\n", cust.address);
    fprintf(file_pointerW, "%s\n", cust.pd);
    fprintf(file_pointerW, "%s\n", cust.book_count);
    fprintf(file_pointerW, "%s\n", cust_books[0].bookid);
    fprintf(file_pointerW, "%s\n", cust_books[1].bookid);
    fprintf(file_pointerW, "%s\n", cust_books[2].bookid);
    fprintf(file_pointerW, "%s\n", cust_books[3].bookid);
   /* for(int i=0;i<temp;i++)
    {
        fprintf(file_pointerW, "%s", cust.name);
    }

    fprintf(file_pointerW, "%s", cust.name);

    for(int i = temp+1; i<4; i++)
    {
        fprintf(file_pointerW, "%s", ");
    }
    */

    fclose(file_pointerW);

    //now we need to print this issue of the book in that student's tra file for future reference
    char acdtrai[100];
    sprintf(acdtrai,"%stra.txt",acno);

    FILE *traptr = fopen(acdtrai,"a"); //open in append mode as we want to add this issuance

    fprintf(traptr,"Item Type: Issue   | Book ID: %s  | Book Name: %s   | Issue Date: %s | Due Date: %s \n",BOOKID,book_details.book_name,issue_date,due_date);

    fclose(traptr);


    return 0;
}


int return_book(char acno[]) //the function accepts the student ID as argument
{
    printf("Enter bookID that you want to return : \n");
    char BOOKID[100];
    scanf("%s",BOOKID);

    char ret_dt[100];
    printf("Enter today's date (dd-mm-yyyy) (This will be the return date) : \n");
    scanf(" %s",ret_dt);


    //first lets open the book's file and set issue status to not_issued and enter the studen't ID, issue_date and due_date
    //lets first generate the filename
    int size = strlen(BOOKID);
    char *filename = malloc(size + 4 + 1);
    sprintf(filename,"%s.txt",BOOKID);

    FILE *bookptr = fopen(filename,"r");
    if(bookptr == NULL)
    {
        printf("\n\n>>BookID is invalid or this book doesn't exist\n\n");
        return 1;
    }

    book book_details;
    //we will store details of this book in the variable book_details of type book
    fscanf(bookptr,"%s",book_details.bookid);
    fscanf(bookptr,"%s",book_details.book_name);
    fscanf(bookptr,"%s",book_details.author_name);
    fscanf(bookptr,"%s",book_details.genre);
    fscanf(bookptr,"%s",book_details.issue_status);

    fclose(bookptr);
    /*fscanf(bookptr,"%s",book_details.issue_student_id);
    fscanf(bookptr,"%s",book_details.date_issue);
    fscanf(bookptr,"%s",book_details.date_due);
    */

    if(strcmp(book_details.issue_status,"not_issued")==0)
    {
        printf("\n\n>>This book is currently not issued to anyone\n\n");
        return 1;
    }

    strcpy(book_details.issue_status,"not_issued");
    strcpy(book_details.issue_student_id,"NIL");
    strcpy(book_details.date_issue,"NIL");
    strcpy(book_details.date_due,"NIL");

    //now lets write this new data to the book's file
    FILE *bookptrW = fopen(filename,"w");

    fprintf(bookptrW,"%s\n",book_details.bookid);
    fprintf(bookptrW,"%s\n",book_details.book_name);
    fprintf(bookptrW,"%s\n",book_details.author_name);
    fprintf(bookptrW,"%s\n",book_details.genre);
    fprintf(bookptrW,"%s\n",book_details.issue_status);
    fprintf(bookptrW,"%s\n",book_details.issue_student_id);
    fprintf(bookptrW,"%s\n",book_details.date_issue);
    fprintf(bookptrW,"%s\n",book_details.date_due);

    fclose(bookptrW);

    //now lets make changes to file of the student, by removing this book's id from the student's current holdings
    //and also decrease the no. of books currently being hold by the student by 1.

   //first lets load the details of customer into a structure of type customer and name cust
    char *acnotxt = malloc(17);
    sprintf(acnotxt, "%s.txt", acno);

    FILE *file_pointer = fopen(acnotxt,"r");

    customer cust;

    fscanf(file_pointer,"%s",cust.name);
    fscanf(file_pointer,"%s",cust.phone);
    fscanf(file_pointer,"%s",cust.email);
    fscanf(file_pointer,"%s",cust.govt_id);
    fscanf(file_pointer,"%s",cust.age);
    fscanf(file_pointer,"%s",cust.address);
    fscanf(file_pointer,"%s",cust.pd);
    fscanf(file_pointer,"%s",cust.book_count);

    int temp = cust.book_count[0] - '0';
    /*
    cust.book_count[0] = cust.book_count[0] - '0';
    int temp = cust.book_count[0];
    */


    //the customer currently holds cust.book_count[0] number of books

    book cust_books[4];
    //now lets load the books that customer currently holds (if he/she doesn't hold all 4 books, then those slots would be NIL)
    for(int i=0;i<4;i++)
    {
       fscanf(file_pointer,"%s",cust_books[i].bookid);
    }
    //after returning this book, we just need to remove it from this list

    fclose(file_pointer);

    //at this point we have stored the details of this student/customer in our program memory inside the customer variable cust
    //we already have the student's complete file data in the variable cust of type customer
    //the bookid of books currently in posession of customer are stored in the array of structure called cust_books[]
    //of type book.

    //now we need to go through the array of structures of type book called cust_books[] to find a record with the
    //book id = BOOKID. If found, then we will change it to NIL and finally decrease the current number of books
    //in possession of customer/student by 1.

    int flag = 0;
    for(int i=0;i<4;i++)
    {
        if(strcmp(cust_books[i].bookid,BOOKID)==0)
        {
            flag = 1;
            strcpy(cust_books[i].bookid,"NIL");
            printf("\n\n>>Book successfully returned to Library\n\n");
            break;
        }
        else
        {
            flag = 0;
        }
    }

    if(flag == 0)
    {
       printf("\n\n>>Book is not currently being issued by entered student\n\n");
       return 1;
    }

    else if(flag == 1)
    {
        int temp_print = temp - 1;
        char str[1];
        sprintf(str,"%d",temp_print);

        strcpy(cust.book_count,str); //decrementing the no. of books in possession of student

        //now lets write this new data in the text file of the student

        FILE *file_pointerW = fopen(acnotxt,"w");

        cust.book_count[0] = (char)cust.book_count[0];

        fprintf(file_pointerW, "%s\n", cust.name);
        fprintf(file_pointerW, "%s\n", cust.phone);
        fprintf(file_pointerW, "%s\n", cust.email);
        fprintf(file_pointerW, "%s\n", cust.govt_id);
        fprintf(file_pointerW, "%s\n", cust.age);
        fprintf(file_pointerW, "%s\n", cust.address);
        fprintf(file_pointerW, "%s\n", cust.pd);
        fprintf(file_pointerW, "%s\n", cust.book_count);
        fprintf(file_pointerW, "%s\n", cust_books[0].bookid);
        fprintf(file_pointerW, "%s\n", cust_books[1].bookid);
        fprintf(file_pointerW, "%s\n", cust_books[2].bookid);
        fprintf(file_pointerW, "%s\n", cust_books[3].bookid);

        fclose(file_pointerW);

    }

    //now we need to print this return of the book in that student's tra file for future reference
    char acdtrai[100];
    sprintf(acdtrai,"%stra.txt",acno);

    FILE *traptr = fopen(acdtrai,"a"); //open in append mode as we want to add this issuance

    fprintf(traptr,"Item Type: Return   | Book ID: %s  | Book Name: %s    | Return Date: %s \n",BOOKID,book_details.book_name,ret_dt);

    fclose(traptr);


    return 0;

}


int entry_book() //remember to append to booklist.txt
{
    //FURTHER TODO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //ask for name,id,author's name and genre of book
    //create a text file for this book using the book_id
    //in this text file book's issue status as n
    //set the remaining fields, like issue_date,due_date,student_id of issuing student as NIL
    //refer to file sample_book.txt to understand the framework of a book's text file
    char BOOK_ID[100];
    char BOOK_NAME[100];
    char AUTHOR_NAME[100];
    char GENRE[100];
    printf("Enter the following details for book entry: \n");
    printf("\nBook ID: ");
    scanf("%s",BOOK_ID);
    printf("\nBook Name: ");
    scanf("%s",BOOK_NAME);
    printf("\nAuthor Name: ");
    scanf("%s",AUTHOR_NAME);
    printf("\nGenre: ");
    scanf("%s",GENRE);

    //create the filename
    int size = strlen(BOOK_ID);
    char *filename = malloc(size + 4 + 1);
    sprintf(filename,"%s.txt",BOOK_ID);

    //create a new txt file for this book
    FILE *ptr = fopen(filename,"w");

    fprintf(ptr,"%s\n",BOOK_ID);
    fprintf(ptr,"%s\n",BOOK_NAME);
    fprintf(ptr,"%s\n",AUTHOR_NAME);
    fprintf(ptr,"%s\n",GENRE);
    fprintf(ptr,"%s\n","not_issued"); //book issue status is set to "not_issued" when book is created
    fprintf(ptr,"%s\n","NIL");
    fprintf(ptr,"%s\n","NIL");
    fprintf(ptr,"%s\n","NIL");

    fclose(ptr);


    //increment count of total books in library by 1 by accessing the book_detail.txt file:

    FILE *incr = fopen("book_detail.txt","r");
    int *a = malloc(sizeof(int));
    fscanf(incr,"%d",a);

    *a = *a + 1;

    fclose(incr);


    FILE *incrW = fopen("book_detail.txt","w");

    fprintf(incrW,"%d\n",*a);
    fclose(incrW);

    //add this book's id to books.txt
    FILE *bk = fopen("books.txt","a");
    fprintf(bk,"%s\n",BOOK_ID);
    fclose(bk);

    return 0;
}

int view_all_books()
{
    printf("=========== BOOKS PRESENT IN THE LIBRARY ===========\n\n");
    FILE *bk = fopen("books.txt","r");

    char bk_id[100];
    while(fscanf(bk,"%s",bk_id) != EOF)
    {
        static_display_book(bk_id);
    }

    fclose(bk);

    printf("===================================================\n\n");
    return 0;
}


int student_operations(char acno[])
{
    int a;

    do
    {
    printf("Select from the below options :\n");
    printf("1.Issue a new book to a student\n");
    printf("2.Accept return of a book from a student\n");
    printf("3.View details of a student/customer\n");
    printf("4.View details of a book\n");
    printf("5.View total number of books in library\n");
    printf("6.Exit\n");

    scanf("%d",&a);

    if(a==1)
    {
        issue(acno);
    }
    else if(a==2)
    {
        return_book(acno);
    }
    else if(a==3)
    {
        student_display_menu();
    }
    else if(a==4)
    {
        book_display_menu();
    }
    else if(a==5)
    {
        count_all_books();
    }
    else if(a==6)
    {
        printf("\n\nGoodbye !\n\n");
        return 0;
    }
    else
    {
       printf("\n\n>>Invalid Input. Try again later\n\n");
    }

    }
    while(a!=6);

    return 0;
}


int display_student(char STUDENTID[100])
{
    //TODO
    /*
    1. Load the student's details including the book ids
    2. Display all the details and when you need to display a book, call the static_display_book() function
       along with all the books whose book ids are not NIL
    */

    //first lets load the details of customer into a structure of type customer and name cust
    char *acnotxt = malloc(17);
    sprintf(acnotxt, "%s.txt", STUDENTID);

    FILE *file_pointer = fopen(acnotxt,"r");

    customer cust;

    fscanf(file_pointer,"%s",cust.pd);
    fscanf(file_pointer,"%s",cust.name);
    fscanf(file_pointer,"%s",cust.phone);
    fscanf(file_pointer,"%s",cust.email);
    fscanf(file_pointer,"%s",cust.govt_id);
    fscanf(file_pointer,"%s",cust.age);
    fscanf(file_pointer,"%s",cust.address);
    fscanf(file_pointer,"%s",cust.book_count);

    book cust_books[4];
    //now lets load the books that customer currently holds (if he/she doesn't hold all 4 books, then those slots would be NIL)
    for(int i=0;i<4;i++)
    {
       fscanf(file_pointer,"%s",cust_books[i].bookid);
    }

    fclose(file_pointer);

    //lets display all the above data along with detailed book data for each book:
    printf("***************** STUDENT DETAILS *******************\n");
    printf("Name: %s\n",cust.name);
    printf("Phone: %s\n",cust.phone);
    printf("Email ID: %s\n",cust.email);
    printf("GOVT ID: %s\n",cust.govt_id);
    printf("Age: %s\n",cust.age);
    printf("Address: %s\n",cust.address);
    printf("The number of books currently in possession of student: %s\n",cust.book_count);

    printf("DETAILS of the Books currently in possession of student :\n");

    for(int i=0;i<4;i++)
    {
        if(strcmp(cust_books[i].bookid,"NIL")!=0)
        {
            static_display_book(cust_books[i].bookid);
        }
    }

    printf("*****************************************************\n");


    return 0;
}

int static_display_book(char BOOKID[100])
{
    //first lets open the book's file and set issue status to issued and enter the studen't ID, issue_date and due_date
    //lets first generate the filename
    int size = strlen(BOOKID);
    char *filename = malloc(size + 4 + 1);
    sprintf(filename,"%s.txt",BOOKID);

    FILE *bookptr = fopen(filename,"r");
    if(bookptr == NULL)
    {
        printf("\n\n>>BookID is invalid or this book doesn't exist\n\n");
        return 1;
    }

    book book_details;
    //we will store details of this book in the variable book_details of type book
    fscanf(bookptr,"%s",book_details.bookid);
    fscanf(bookptr,"%s",book_details.book_name);
    fscanf(bookptr,"%s",book_details.author_name);
    fscanf(bookptr,"%s",book_details.genre);
    fscanf(bookptr,"%s",book_details.issue_status);
    fscanf(bookptr,"%s",book_details.issue_student_id);
    fscanf(bookptr,"%s",book_details.date_issue);
    fscanf(bookptr,"%s",book_details.date_due);

    fclose(bookptr);

    //now lets print the details of this book

    printf("********* BOOK DETAILS *********\n");
    printf("BOOK ID: %s\n",book_details.bookid);
    printf("BOOK NAME: %s\n",book_details.book_name);
    printf("AUTHOR NAME: %s\n",book_details.author_name);
    printf("GENRE: %s\n",book_details.genre);
    printf("ISSUE STATUS of the book: %s\n",book_details.issue_status);

    if(strcmp(book_details.issue_status,"not_issued")==0)
    {
        printf("*********************************\n");
        return 0;
    }
    else
    {
        printf("%s\n",book_details.issue_student_id);
        printf("%s\n",book_details.date_issue);
        printf("%s\n",book_details.date_due);
        printf("*********************************\n");

        return 0;
    }


    return 0;
}

int display_book(char BOOKID[100])
{
    //first lets open the book's file and set issue status to issued and enter the studen't ID, issue_date and due_date
    //lets first generate the filename
    int size = strlen(BOOKID);
    char *filename = malloc(size + 4 + 1);
    sprintf(filename,"%s.txt",BOOKID);

    FILE *bookptr = fopen(filename,"r");
    if(bookptr == NULL)
    {
        printf("\n\n>>BookID is invalid or this book doesn't exist\n\n");
        return 1;
    }

    book book_details;
    //we will store details of this book in the variable book_details of type book
    fscanf(bookptr,"%s",book_details.bookid);
    fscanf(bookptr,"%s",book_details.book_name);
    fscanf(bookptr,"%s",book_details.author_name);
    fscanf(bookptr,"%s",book_details.genre);
    fscanf(bookptr,"%s",book_details.issue_status);
    fscanf(bookptr,"%s",book_details.issue_student_id);
    fscanf(bookptr,"%s",book_details.date_issue);
    fscanf(bookptr,"%s",book_details.date_due);

    fclose(bookptr);

    //now lets print the details of this book

    printf("********* BOOK DETAILS *********\n");
    printf("BOOK ID: %s\n",book_details.bookid);
    printf("BOOK NAME: %s\n",book_details.book_name);
    printf("AUTHOR NAME: %s\n",book_details.author_name);
    printf("GENRE: %s\n",book_details.genre);
    printf("ISSUE STATUS of the book: %s\n",book_details.issue_status);

    if(strcmp(book_details.issue_status,"not_issued")==0)
    {
        printf("*********************************\n");
        return 0;
    }
    else
    {
        printf("Book issued to student with student ID: %s\n",book_details.issue_student_id);
        printf("Issue Date: %s\n",book_details.date_issue);
        printf("Due Date: %s\n",book_details.date_due);
        printf("*********************************\n");

        printf("\n\nDo you wish to view the details of this above student ? (Y/N) ");
        char ch;
        scanf(" %c",&ch);
        if(ch == 'Y')
        {
            display_student(book_details.issue_student_id);
            return 0;
        }
        else
        {
            return 0;
        }
    }

}

int student_display_menu()
{
    int op;
    do
    {
        printf("Select from the options below : \n");
        printf("1.View a student's detail\n");
        printf("2.Exit\n");
        scanf("%d",&op);

        if(op==1)
        {
            printf("Enter student ID: \n");
            char STUDENTID[100];
            scanf("%s",STUDENTID);

            display_student(STUDENTID);
        }
    }
    while(op == 1);

    return 0;
}

int book_display_menu()
{
    int op;
    do
    {
        printf("Select from the options below : \n");
        printf("1.Search a book by its ID\n");
        printf("2.Search a book by its name\n");
        printf("3.Exit\n");
        scanf("%d",&op);

        if(op==1)
        {
            printf("Enter bookID that you want to view : \n");
            char BOOKID[100];
            scanf("%s",BOOKID);

            display_book(BOOKID);
        }
        else if(op == 2)
        {
            printf("Enter name of book that you want to view (for ease of use the search is case insensitive) : \n");
            char bk_nm[100];
            scanf("%s",bk_nm);

            search_book_by_name(bk_nm);
        }
        else if(op == 3)
        {
            printf("\n\nGoodbye !\n\n");
            return 0;
        }
    }
    while(op != 3);

    return 0;
}

int count_all_books()
{
    FILE *incr = fopen("book_detail.txt","r");
    int *a = malloc(sizeof(int));
    fscanf(incr,"%d",a);

    printf("The total number of books in library is : %d\n",*a);
    fclose(incr);

    return *a;
}

int edit_book(char BKID[100])
{
    char acdtxt[100];
    sprintf(acdtxt,"%s.txt",BKID);
    FILE *ptr = fopen(acdtxt,"r");

    if(ptr == NULL)
    {
        printf("\n\n>>Book doesn't exist\n\n");
        return 1;
    }

    //load the book's info
    book edit_bk;
    fscanf(ptr,"%s",edit_bk.bookid);
    fscanf(ptr,"%s",edit_bk.book_name);
    fscanf(ptr,"%s",edit_bk.author_name);
    fscanf(ptr,"%s",edit_bk.genre);
    fscanf(ptr,"%s",edit_bk.issue_status);
    fscanf(ptr,"%s",edit_bk.issue_student_id);
    fscanf(ptr,"%s",edit_bk.date_issue);
    fscanf(ptr,"%s",edit_bk.date_due);
    fclose(ptr);

    //provide options for editing
    int hola;
    do
    {
        printf("\n############ Welcome to book editing window ################\n");
        printf("Select the option from the below list\n");
        printf("1.Edit Book's name\n");
        printf("2.Edit Author's name\n");
        printf("3.Edit Genre\n");
        printf("4.Edit all of the above 3 details\n");
        printf("5.Exit\n");

        scanf(" %d",&hola);

        if(hola == 1)
        {
            printf("Enter new book name\n");
            scanf(" %s",edit_bk.book_name);
        }
        else if(hola == 2)
        {
            printf("Enter new author name\n");
            scanf(" %s",edit_bk.author_name);
        }
        else if(hola == 3)
        {
            printf("Enter new genre\n");
            scanf(" %s",edit_bk.genre);
        }
        else if(hola ==4)
        {
            printf("Enter new book name\n");
            scanf(" %s",edit_bk.book_name);
            printf("Enter new author name\n");
            scanf(" %s",edit_bk.author_name);
            printf("Enter new genre\n");
            scanf(" %s",edit_bk.genre);
        }
        else if(hola == 5)
        {
            printf("\n\nGoodbye !\n\n");
            break;
        }
        else
        {
            printf("\n\n>>Wrong Input, Try again\n\n");
        }
    }
    while(hola!=5);

    printf("\n##################################################################\n");

    //rewrite details into book
    FILE *ptrw = fopen(acdtxt,"w");
    if(ptrw==NULL)
    {
        printf("\n\n>>Oops and error occured\n\n");
        return 1;
    }

    fprintf(ptrw,"%s\n",edit_bk.bookid);
    fprintf(ptrw,"%s\n",edit_bk.book_name);
    fprintf(ptrw,"%s\n",edit_bk.author_name);
    fprintf(ptrw,"%s\n",edit_bk.genre);
    fprintf(ptrw,"%s\n",edit_bk.issue_status);
    fprintf(ptrw,"%s\n",edit_bk.issue_student_id);
    fprintf(ptrw,"%s\n",edit_bk.date_issue);
    fprintf(ptrw,"%s\n",edit_bk.date_due);

    fclose(ptrw);

    return 0;
}


int search_book_by_name(char bk_nm[100])
{
    //first lets load details of id and names of all books

    //to do that lets find how many books are there in system
    FILE *bk_counter = fopen("book_detail.txt","r");

    int *a = malloc(sizeof(int));
    fscanf(bk_counter,"%d",a);
    int nof_books = *a; //nof_books stores the total no. of books

    fclose(bk_counter);

    book books[nof_books]; //we create an array of structure books to load only the book id and book name

    FILE *ptr = fopen("books.txt","r");

    for(int i=0;i<nof_books;i++)
    {
        fscanf(ptr,"%s",books[i].bookid);

        char acdtxti[100];
        sprintf(acdtxti,"%s.txt",books[i].bookid);
        FILE *bkptr = fopen(acdtxti,"r");

        char dummy[100];
        fscanf(bkptr,"%s",dummy);

        fscanf(bkptr,"%s",books[i].book_name);

        fclose(bkptr);
    }

    fclose(ptr);

    //by this time we have stored the ids and names of all books

    for(int i=0;i<nof_books;i++)
    {
        if(strcasecmp(bk_nm,books[i].book_name)==0)
        {
            printf("\n\nBook FOUND !\n");
            static_display_book(books[i].bookid);
            return 0;
        }
    }

    printf("\n\n>>Book NOT FOUND \n\n");
    return 0;

}

int view_issued()
{
    //first lets load details of id and names of all books

    //to do that lets find how many books are there in system
    FILE *bk_counter = fopen("book_detail.txt","r");

    int *a = malloc(sizeof(int));
    fscanf(bk_counter,"%d",a);
    int nof_books = *a; //nof_books stores the total no. of books

    fclose(bk_counter);

    book books[nof_books]; //we create an array of structure books to load only the book id and issue_status

    FILE *ptr = fopen("books.txt","r");

    for(int i=0;i<nof_books;i++)
    {
        fscanf(ptr,"%s",books[i].bookid);

        char acdtxti[100];
        sprintf(acdtxti,"%s.txt",books[i].bookid);
        FILE *bkptr = fopen(acdtxti,"r");

        char dummy[100];
        fscanf(bkptr,"%s",dummy);
        fscanf(bkptr,"%s",dummy);
        fscanf(bkptr,"%s",dummy);
        fscanf(bkptr,"%s",dummy);

        fscanf(bkptr,"%s",books[i].issue_status);

        fclose(bkptr);
    }

    fclose(ptr);

    //by this time we have stored the ids and issue_status of all books
    printf("\n################## BOOKS CURRENTLY ISSUED #######################\n");
    for(int i=0;i<nof_books;i++)
    {
        if(strcasecmp(books[i].issue_status,"issued")==0)
        {
            static_display_book(books[i].bookid);
        }
    }
    printf("\n#################################################################\n");
    return 0;

}
