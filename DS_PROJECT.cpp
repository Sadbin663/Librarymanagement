#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>

struct Book {
    int id;
    std::string bookName;
    std::string authorName;
    std::string date;
    Book* next;
};

struct Student {
    int id;
    std::string sName;
    std::string sClass;
    int sRoll;
    std::string bookName;
    std::string date;
    Student* next;
};

Book* booksHead = nullptr;
Student* issuedBooksHead = nullptr;

void addBook();
void booksList();
void del();
void issueBook();
void issueList();
void displayWelcomePage();
void displayMenu();

int main() {
    int ch;

    while (true) {
        system("cls");
        displayWelcomePage();
        displayMenu();
        std::cin >> ch;

        switch (ch) {
            case 0:
                return 0;

            case 1:
                addBook();
                break;

            case 2:
                booksList();
                break;

            case 3:
                del();
                break;

            case 4:
                issueBook();
                break;

            case 5:
                issueList();
                break;

            default:
                std::cout << "Invalid Choice...\n\n";
        }

        std::cout << "Press Any Key To Continue...";
        std::cin.get();
        std::cin.get();
    }

    return 0;
}

void displayWelcomePage() {
    std::cout << "\n-----------------------------------------------------\n";
    std::cout << "             Welcome to Library Management System\n";
    std::cout << "-----------------------------------------------------\n";
}

void displayMenu() {
    std::cout << "\n-------------------- Menu Options --------------------\n";
    std::cout << "1. Add Book\n";
    std::cout << "2. View Books List\n";
    std::cout << "3. Remove Book\n";
    std::cout << "4. Issue Book\n";
    std::cout << "5. View Issued Books List\n";
    std::cout << "0. Exit\n";
    std::cout << "-----------------------------------------------------\n";
    std::cout << "Enter your choice: ";
}

void addBook() {
    Book* newBook = new Book;
    char myDate[12];
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    std::sprintf(myDate, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    newBook->date = myDate;
    newBook->next = nullptr;

    std::cout << "Enter book id: ";
    std::cin >> newBook->id;

    std::cout << "Enter book name: ";
    std::cin.ignore();
    std::getline(std::cin, newBook->bookName);

    std::cout << "Enter author name: ";
    std::getline(std::cin, newBook->authorName);

    if (booksHead == nullptr) {
        booksHead = newBook;
    } else {
        Book* temp = booksHead;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newBook;
    }

    std::ofstream outFile("books.txt", std::ios::app);
    outFile.write(reinterpret_cast<const char*>(newBook), sizeof(Book));
    outFile.close();

    std::cout << "Book Added Successfully\n";
}

void booksList() {
    std::ifstream inFile("books.txt", std::ios::binary);
    Book* temp = booksHead;
    std::cout << "<== Available Books ==>\n\n";
    std::cout << std::setw << "Book id" << std::setw(30) << "Book Name" << std::setw(20) << "Author" <<std::setw(20)<< "Date\n\n";

    while (temp != nullptr) {
        std::cout << std::setw << temp->id << std::setw(30) << temp->bookName << std::setw(20) << temp->authorName << std::setw(30) << temp->date << "\n";
        temp = temp->next;
    }

    inFile.close();
}

void del() {
    int id;
    bool found = false;
    std::cout << "<== Remove Books ==>\n\n";
    std::cout << "Enter Book id to remove: ";
    std::cin >> id;

    Book* current = booksHead;
    Book* prev = nullptr;

    while (current != nullptr) {
        if (current->id == id) {
            found = true;
            if (prev == nullptr) {
                booksHead = current->next;
            } else {
                prev->next = current->next;
            }
            delete current;
            break;
        }
        prev = current;
        current = current->next;
    }

    if (found) {
        std::cout << "\n\nDeleted Successfully.\n";
        std::remove("books.txt");
        std::ofstream outFile("books.txt", std::ios::binary);
        Book* temp = booksHead;
        while (temp != nullptr) {
            outFile.write(reinterpret_cast<const char*>(temp), sizeof(Book));
            temp = temp->next;
        }
        outFile.close();
    } else {
        std::cout << "\n\nRecord Not Found!\n";
    }
}

void issueBook() {
    char myDate[12];
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    std::sprintf(myDate, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

    Student* newStudent = new Student;
    newStudent->date = myDate;
    newStudent->next = nullptr;

    int id;
    bool found = false;
    std::cout << "<== Issue Books ==>\n\n";
    std::cout << "Enter Book id to issue: ";
    std::cin >> id;

    std::ifstream inFile("books.txt", std::ios::binary);
    Book* temp = booksHead;
    while (temp != nullptr) {
        if (temp->id == id) {
            newStudent->bookName = temp->bookName;
            found = true;
            break;
        }
        temp = temp->next;
    }
    inFile.close();

    if (!found) {
        std::cout << "No book found with this id\n";
        std::cout << "Please try again...\n\n";
        delete newStudent;
        return;
    }

    std::ofstream outFile("issue.txt", std::ios::app);

    std::cout << "Enter Student Name: ";
    std::cin.ignore();
    std::getline(std::cin, newStudent->sName);

    std::cout << "Enter Student Class: ";
    std::getline(std::cin, newStudent->sClass);

    std::cout << "Enter Student Roll: ";
    std::cin >> newStudent->sRoll;

    if (issuedBooksHead == nullptr) {
        issuedBooksHead = newStudent;
    } else {
        Student* temp = issuedBooksHead;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newStudent;
    }

    outFile.write(reinterpret_cast<const char*>(newStudent), sizeof(Student));
    outFile.close();

    std::cout << "Book Issued Successfully\n\n";
}

void issueList() {
    std::cout << "<== Book Issue List ==>\n\n";
    std::cout << std::left << std::setw(10) << "S.id" << std::setw(30) << "Name" << std::setw(20) << "Class"
              << std::setw(10) << "Roll" << std::setw(30) << "Book Name" << "Date\n\n";

    std::ifstream file("issue.txt", std::ios::binary);
    Student* temp = issuedBooksHead;
    while (temp != nullptr) {
        std::cout << std::left << std::setw(10) << temp->id << std::setw(30) << temp->sName << std::setw(20) << temp->sClass
                  << std::setw(10) << temp->sRoll << std::setw(30) << temp->bookName << temp->date << "\n";
        temp = temp->next;
    }
    file.close();
}

