#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>

/*Description of this program:
This is the student/customer side program.

A student/customer has two text files unique to his/her a/c no.
1. Text File -> xleria00xacd.txt (current records) (here the x is a global variable which will be inserted into the string name, to generate new student IDs)
2. Text File -> xleria00xtra.txt (all records) (here the x is a global variable which will be inserted into the string name, to generate new student IDs)

A student/customer can :
1. Create a student Account and generate a student ID
2. Login to an student Account with a student ID

After successful creation of a new account, we will generate a new student ID for that student/customer of the form xleria000xaca.
We will then create two new text files using that student ID of the form xleria00xacd.txt and xleria00xtra.txt
This text file will store all the details of the student/customer, starting from his name, mobile, email, Govt ID No.

If student/customer logins to his account, he can do the following things :
1. See his current records which shows the books he has issued and not returned
2. See all the books he has issued in past
*/
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

int display_student(char STUDENTIT[100]);
int static_display_book(char BOOKID[100]);
int display_book(char BOOKID[100]);
int student_display_menu();
int book_display_menu();
int count_all_books();
int view_all_books();
bool hash_check(char plaintext[], char *cipher);
int create_account();
bool login();
int edit_student(char STU_ID[100]);
int search_book_by_name(char bk_nm[100]);
int view_all_past_data(char STUD_ID[100]);

char stu_id_cust[100]; //this global string will be used to store the student's id after login to access various
                  //functions




int main(void)
{

    int a;

    do
    {
    printf("WELCOME TO XLERIA LIBRARY STUDENT/CUSTOMER PORTAL\n\n");
    printf("Press 1 for Login\n");
    printf("Press 2 for Creating a new Account\n");
    printf("Press 3 to Exit\n");

    scanf("%d",&a);
    if(a == 1)
    {
        bool login_status = login();

        if(login_status == false)
        {
            printf("\n\n>>Try again \n\n");
        }

        else if(login_status == true)
        {
            int resw;
            do
            {
                //all the stuff that a student can do
                printf("\n1.Open Student Dashboard");
                printf("\n2.View Book Details");
                printf("\n3.View all Books in library");
                printf("\n4.Edit Student Details");
                printf("\n5.View all past book issues and returns with library");
                printf("\n6.Exit\n");
                scanf("%d",&resw);

                if(resw == 6)
                {
                    printf("Goodbye !\n");
                    return 0;
                }

                else if(resw == 1)
                {
                    display_student(stu_id_cust);
                }

                else if(resw == 2)
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

                            static_display_book(BOOKID);
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
                            printf("\nGoodbye !\n");
                            return 0;
                        }
                    }
                    while(op != 3);

                    return 0;
                    /*char bkgid[100];
                    printf("\nEnter BOOK ID: ");
                    scanf("%s",bkgid);
                    static_display_book(bkgid);
                    */
                }

                else if(resw == 3)
                {
                    view_all_books();
                }

                else if(resw == 4)
                {
                    edit_student(stu_id_cust);
                }

                else if(resw == 5)
                {
                    view_all_past_data(stu_id_cust);
                }

                else
                {
                    printf("\n\n>>Wrong Input. Try again\n\n");
                }

            }
            while(resw!=6); //while he doesn't press exit
        }

    }
    else if(a == 2)
    {
        create_account();
    }
    else if(a == 3)
    {
        printf("Goodbye !\n");
        return 0;
    }
    else
    {
        printf("\n\n>>Wrong Input. Try again later\n\n");
    }

    }
    while(a!=3);

    return 0;
}

int create_account()
{
    FILE *ptr = fopen("cust_detail.txt","r");
    char character = getc(ptr);
    fclose(ptr);

    int cust_count = character - '0';

    char name[100];
    char phone[100];
    char email[100];
    char govt_id[100];
    char age[100];
    char address[1000];
    char plaintext[100];
    char temp_c_pd[100];

    printf("\nName: ");
    scanf("%s",name);
    printf("\nPhone: ");
    scanf("%s",phone);
    printf("\nEmail ID: ");
    scanf("%s",email);
    printf("\nGovt ID: ");
    scanf("%s",govt_id);
    printf("\nAge: ");
    scanf("%s",age);
    printf("\nAddress: ");
    scanf("%s",address);
    printf("\nPassword: ");
    scanf("%s",plaintext);
    printf("\nConfirm Password: ");
    scanf("%s",temp_c_pd);

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


            //first let increment the number of customers in cust_details.txt
            cust_count++;
            FILE *fptr = fopen("cust_detail.txt","w");
            fputc(character+1,fptr);
            fclose(fptr);

            //lets generate a/c no. for this customer. Lets also generate the 2 txt files that will be required.
            char *acd = malloc(14);
            sprintf(acd, "xleria%04iacd", cust_count);
            char *tra = malloc(14);
            sprintf(tra, "xleria%04iacdtra", cust_count);


            char *acdtxt = malloc(17);
            sprintf(acdtxt, "xleria%04iacd.txt", cust_count);
            char *tratxt = malloc(17);
            sprintf(tratxt, "xleria%04iacdtra.txt", cust_count);

            FILE *ptracd = fopen(acdtxt,"w");
            FILE *ptrtra = fopen(tratxt,"w");
            fclose(ptracd);
            fclose(ptrtra);

            //lets input the ac data inside the acd.txt file
            FILE *file_acd = fopen(acdtxt,"w");
            fprintf(file_acd,"%s\n",plaintext);
            fprintf(file_acd,"%s\n",name);
            fprintf(file_acd,"%s\n",phone);
            fprintf(file_acd,"%s\n",email);
            fprintf(file_acd,"%s\n",govt_id);
            fprintf(file_acd,"%s\n",age);
            fprintf(file_acd,"%s\n",address);
            fprintf(file_acd,"%s\n","0"); //current book count
            fprintf(file_acd,"%s\n","NIL"); //book id of book 1
            fprintf(file_acd,"%s\n","NIL"); //book id of book 2
            fprintf(file_acd,"%s\n","NIL"); //book id of book 3
            fprintf(file_acd,"%s\n","NIL"); //book id of book 4

            fclose(file_acd);

            printf("\n\nYOUR STUDENT PROFILE HAS BEEN SUCCESSFULLY CREATED\n");
            printf("Your student ID is : %s\n",acd);


    }
    else
    {
        printf("\n\n>>Password and Confirm Password do not match. Try again later. \n\n");
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
        strcpy(stu_id_cust,LOG_ID);
        return true;
    }
    else if(status == false)
    {
        printf("\n\n>>The Login ID or password entered by you is incorrect\n\n");
        return false;
    }

    return false;
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

        printf(">>Do you wish to view the details of this above student ? (Y/N)");
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
        printf("1.View a book's detail\n");
        printf("2.Exit\n");
        scanf("%d",&op);

        if(op==1)
        {
            printf("Enter bookID that you want to issue : \n");
            char BOOKID[100];
            scanf("%s",BOOKID);

            display_book(BOOKID);
        }
    }
    while(op == 1);

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

int edit_student(char STU_ID[100])
{
    char acdtxt[100];
    sprintf(acdtxt,"%s.txt",STU_ID);
    FILE *ptr = fopen(acdtxt,"r");

    if(ptr == NULL)
    {
        printf("\n\n>>Student doesn't exist\n\n");
        return 1;
    }

    //load the student's info
    customer edit_st;
    fscanf(ptr,"%s",edit_st.pd);
    fscanf(ptr,"%s",edit_st.name);
    fscanf(ptr,"%s",edit_st.phone);
    fscanf(ptr,"%s",edit_st.email);
    fscanf(ptr,"%s",edit_st.govt_id);
    fscanf(ptr,"%s",edit_st.age);
    fscanf(ptr,"%s",edit_st.address);
    fscanf(ptr,"%s",edit_st.book_count);
    fclose(ptr);

    //provide options for editing
    int hola;
    do
    {
        printf("\n############ Welcome to student detail editing window ################\n");
        printf("Select the option from the below list\n");
        printf("1.Edit name\n");
        printf("2.Edit phone\n");
        printf("3.Edit email\n");
        printf("4.Edit govt_id\n");
        printf("5.Edit age\n");
        printf("6.Edit address\n");
        printf("7.Edit password\n");
        printf("8.Edit all of the above details\n");
        printf("9.Exit\n");

        scanf(" %d",&hola);

        if(hola == 1)
        {
            printf("Enter new name\n");
            scanf(" %s",edit_st.name);
        }
        else if(hola == 2)
        {
            printf("Enter new phone\n");
            scanf(" %s",edit_st.phone);
        }
        else if(hola == 3)
        {
            printf("Enter new email\n");
            scanf(" %s",edit_st.email);
        }
        else if(hola == 4)
        {
            printf("Enter new govt_id\n");
            scanf(" %s",edit_st.govt_id);
        }
        else if(hola == 5)
        {
            printf("Enter new age\n");
            scanf(" %s",edit_st.age);
        }
        else if(hola == 6)
        {
            printf("Enter new address\n");
            scanf(" %s",edit_st.address);
        }
        else if(hola == 7)
        {
            char plaintext[100];
            char temp_c_pd[100];
            printf("\nEnter new Password: ");
            scanf("%s",plaintext);
            printf("\nConfirm Password: ");
            scanf("%s",temp_c_pd);

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
                strcpy(edit_st.pd,plaintext);
            }

        }

        else if(hola ==8)
        {
            printf("Enter new name\n");
            scanf(" %s",edit_st.name);

            printf("Enter new phone\n");
            scanf(" %s",edit_st.phone);

            printf("Enter new email\n");
            scanf(" %s",edit_st.email);

            printf("Enter new govt_id\n");
            scanf(" %s",edit_st.govt_id);

            printf("Enter new age\n");
            scanf(" %s",edit_st.age);

            printf("Enter new address\n");
            scanf(" %s",edit_st.address);

            char plaintext[100];
            char temp_c_pd[100];
            printf("\nEnter new Password: ");
            scanf("%s",plaintext);
            printf("\nConfirm Password: ");
            scanf("%s",temp_c_pd);

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
                strcpy(edit_st.pd,plaintext);
            }


        }
        else if(hola == 9)
        {
            printf("Goodbye !\n");
            break;
        }
        else
        {
            printf("\n\n>>Wrong Input, Try again\n\n");
        }
    }
    while(hola!=9);

    printf("\n##################################################################\n");

    //rewrite details into book
    FILE *ptrw = fopen(acdtxt,"w");
    if(ptrw==NULL)
    {
        printf("\n\n>>Oops and error occured\n\n");
        return 1;
    }

    fprintf(ptrw,"%s\n",edit_st.pd);
    fprintf(ptrw,"%s\n",edit_st.name);
    fprintf(ptrw,"%s\n",edit_st.phone);
    fprintf(ptrw,"%s\n",edit_st.email);
    fprintf(ptrw,"%s\n",edit_st.govt_id);
    fprintf(ptrw,"%s\n",edit_st.age);
    fprintf(ptrw,"%s\n",edit_st.address);
    fprintf(ptrw,"%s\n",edit_st.book_count);

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

int view_all_past_data(char STUD_ID[100])
{
    char tratxt[100];
    sprintf(tratxt,"%stra.txt",STUD_ID);

    FILE *pointer = fopen(tratxt,"r");

    printf("\n**************** List of all past issues and returns with the library *******************\n");

    char ch;
    while((ch = fgetc(pointer)) !=EOF)
    {
        printf("%c",ch);
    }
    /*
    char line[300];
    while(fscanf(pointer,"%s",line)!=EOF)
    {
        printf("%s\n",line);
    }
    */

    printf("\n*****************************************************************************************\n");

    fclose(pointer);

    return 0;
}

