<h1>CS50 Final Project</h1>

<h3>
Project Name: Library Management System
</h3>
<h3>
Type: Command Line Program
</h3>
<h3>
Written in: C Programming Language
</h3>
<h3>
Number of Lines of Code: 2318
</h3>

<h2>Why I chose this Project?</h2>
Answer:
Most libraries around the world, still follow the manual system. Generally in
this manual system, the customer/student needs to fill up his details on a card
attached to the book that he borrowed. He also needs to do the same on his
library card which every time needs to be searched manually by the librarian.
This takes unnecessarily a lot of time and hence results in long queues.
Moreover when a librarian wishes to look at specific details like :
A student/customer’s info
Books currently issued by the library
and more, it can be quite painful to look over all these records manually.
A Library management system solves all these problems. Issuing new books,
taking return of books, searching for a book, searching for student’s info and
viewing list of books that are already issued becomes a breeze.
Moreover during current COVID-19 situations, when libraries will slowly open,
it is imperative to reduce queues at the counter and make the whole process
of book issue and return fast and efficient.

<h2>Reasons for Choosing C:</h2>
There were many reasons for choosing C for this project. Some of them were :
Fast processing speed
Low Level Abstraction and greater control over the computer’s system.
Especially the low level access to computer memory using pointers.
Extensive File I/O
I know that many programming languages allow us to do the same thing and
may require less effort and time during development because of the high level
abstractions that they provide. To develop anything in C, you almost always
need to write your code from scratch. So I felt, as a beginner to programming,
creating a project in C would really help with the Fundamentals of C and
programming itself and also give me the opportunity to showcase my new
found knowledge of this programming language that I obtained from the CS50
course itself.

<h2>Overview of the project:</h2>

My project is divided into separate programs, the first one is a librarian side
solution that allows the librarian to :
1. Issue and Accept Return of books
2. View a student’s details
3. View a book’s details
4. Entry a new book in the system
5. Edit a book’s data
6. View the books that are currently issued by the library.

The student/client side solution allows a student to :
1. Create a new profile or Login to an existing profile
2. View their Dashboard (which shows their details and all the books that are currently in their possession
3. Edit their details
4. Search for books in the Library
5. View all past book issues and returns made with the Library


<h2>Design and Implementation:</h2>
The whole motivation behind choosing C, as I mentioned above was to push
myself out of my comfort zone by
building everything from scratch. I didn't want to use the power of high-levl
abstraction of a programming
language like Python or a database like MySQL.
Instead I created my own database system using entirely File Handling. I hence
created files that not only
stored data for individual items (like students,librarians, and books) but also to
store counter values to keep
track of how many students, librarian and books there were in the system. At
first it seemed quite impossible
since it was not only the extraction process, but the parsing of the data just
extracted and updating the data
after every action. However slowly over the course of many weeks, I tried to
figure out a mental picture of how
it would work.
I realized that the most important aspect was : Structuring my files properly so
that efficient parsing of file
data can be done. Moreover maintaining that structure during updation of
data.
After figuring out the structure for storing the data (in a permanent manner),
the next challenge
was to create an UI. Using do while loops, I structured my menus in order to
provide a seamless
experience for my users.
The next most vital aspect was data encryption and hashing for facilitation of
login using passwords.
To ensure safety, all the passwords are stored in form of ciphertext in the files.


<h3>How the file management System works</h3>
To store permanent global data we need some default text files that work as counters. For e.g:

1. book_detail.txt (This text file contains the total number of books in the system) [Initially the value should be zero]

2. cust_detail.txt (This text file contains the total number of students in library system) [Initially the value should be zero]

3. libr_detail.txt (This text file contains the total number of librarians in library system) [Initially the value should be zero]

Then there are some text files that store an array of information. For e.g:

1. books.txt (This text file contains the ids of all books in the system)

Apart from this a unique text file is created for each student, librarian and each book so as to store data pertaining to that object only.

1. The data for each book is stored in a unique text file for the book whose name is "book_id".txt

2. Each student has his separate text file of type : "student_id"acd.txt for his details and "student_id"tra.txt for his book issue/return with the library

3. Each librarian has his separate text file of type : "librarian_login_id"lib.txt for his details.
