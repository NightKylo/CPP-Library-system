#include <iostream>
#include <conio.h>
#include "includes/books.h"

const std::string BOOK_PATH = "C:\\Users\\Anwender\\Documents\\Programmieren\\C++\\lib\\books";
const std::string USER_PATH = "C:\\Users\\Anwender\\Documents\\Programmieren\\C++\\lib\\users";
const int NEW_LINES = 50;
auto const RETURN_DAYS = 7;

void login(const bool &debug);
void lib(const User &user, const bool &debug);
void fill_paragraph(void);
void print_book(const Book &book);
void print_headers(void);

int main(void){
    login(false);
    return 0;
}

void login(const bool &debug){
    int id;
    std::string pin;
    bool correct_id = false;
    bool correct_pin = false;

    User_Container users = get_all_users(USER_PATH);

    if(users.size() == 0){
        std::cout << "    ERROR LOADING USERS TERMINATING...." << std::endl;
        return;
    }
    else if(debug){
        std::cout << "    USERS LOADED: " << users.size() << "\n" << std::endl;
    }

    std::cout << "    Welcome to the library\n\n    ID\n>>> ";
    std::cin >> id;
    std::cout << "    PIN\n>>> ";
    std::cin >> pin;

    for(User_Container::iterator iter = users.begin(); iter != users.end(); ++iter){
        User user = *iter;
        if(id == user.id){
            correct_id = true;
            if(pin == user.pin){
                correct_pin = true;
                std::cout << "    STARTING LIB" << std::endl;
                decrement_day(BOOK_PATH);
                lib(user, debug);
                break;
            }
        }
    }

    if(!correct_id){
        std::cout << "\x1b[A\x1b[A\x1b[A" << ">>> " <<  id << " <-- False ID\n    PIN\n>>> " << pin << std::endl;
    }
    if(!correct_pin){
        std::cout << "\x1b[A" << ">>> " <<  pin << " <-- Wrong PIN" << std::endl;
        getch();
    }
}

void lib(const User &user, const bool &debug){
    fill_paragraph();
    Book_Container books = get_all_books(BOOK_PATH);

    if(books.size() == 0){
        std::cout << "    ERROR LOADING BOOKS >> TERMINATING...." << std::endl;
    }
    else if(debug){
        std::cout << "    BOOKS LOADED " << books.size() << "\n" << std::endl;
    }

    int key;
    std::cout << "    What do you want to do?\n\n    [0] Show borrowed books\n    [1] Borrow book\n    [2] Return book\n    [3] Show all books\n    [4] Personal information\n    [5] Logout\n>>> ";
    std::cin >> key;

    Book_Container borrowed_books;
    bool he_borrowed_book = false;

    if(key == 0){
        fill_paragraph();
        std::cout << "    You have currently borrowed these books:\n    " << std::endl;
        for(Book_Container::iterator iter = books.begin(); iter != books.end(); ++iter){
            Book book = *iter;
            if(book.borrower_id == user.id){
                he_borrowed_book = true;
            }
        }

        if(he_borrowed_book){
            print_headers();
            for(Book_Container::iterator iter = books.begin(); iter != books.end(); ++iter){
                Book book = *iter;
                if(book.borrower_id == user.id){
                    print_book(book);
                }
            }
        }
        else{
            std::cout << "    You don't have any books borrowed" << std::endl;
        }
        getch();
    }
    else if(key == 1){
        fill_paragraph();
        std::string yn;
        std::string in;
        int book_id;
        std::cout << "    Enter the id of the book you want to borrow ('c' to cancel)\n>>> ";
        std::cin >> in;
        if(in.find("c") == 0){
            lib(user, debug);
            return;
        }

        book_id = std::stoi(in);

        Book book = get_book_by_id(BOOK_PATH, book_id);
        if(book.borrower_id == -1 && book.id != -1){
            std::cout << std::endl;
            print_headers();
            print_book(book);
            std::cout << "\n    Is this correct(y/n): ";
            std::cin >> yn;
            if(yn.find("y") != STRING_NOT_FOUND){
                book.borrower_id = user.id;
                book.days_until_return = RETURN_DAYS;
                fill_paragraph();
                update_books(BOOK_PATH, book);
                std::cout << "    You succesfully borrowed '" << book.name <<  "' from '" << book.author << "'." << std::endl;
                getch();
            }
        }
        else if(book.borrower_id == user.id){
            fill_paragraph();
            std::cout << "    You have already borrowed this book. Type 1 in main menu to see your borrowed books." << std::endl;
            getch();
        }
        else if(book.borrower_id != -1){
            fill_paragraph();
            std::cout << "    This book is not available at the moment. It will be available in " << book.days_until_return << " days." << std::endl;
            getch();
        }
        else if(book.id == -1){
            fill_paragraph();
            std::cout << "    It exists no book with id " << book_id << std::endl;
            getch();
        }
        else{
            fill_paragraph();
            std::cout << "    An error occured, while trying to borrow book with id " << book.id << std::endl;
            return;
        }
    }
    else if(key == 2){
        fill_paragraph();
        std::string in;
        int return_book_id;
        std::cout << "    Which book do you want to return (ID, 'c' to cancel) ?\n>>> ";
        std::cin >> in;
        if(in.find("c") == STRING_NOT_FOUND){
            return_book_id = stoi(in);
            Book book = get_book_by_id(BOOK_PATH, return_book_id);
            if(book.borrower_id != user.id){
                fill_paragraph();
                std::cout << "    You can't return this book, because you didn't borrow it." << std::endl;
                getch();
            }
            else{
                std::cout << std::endl;
                print_headers();
                print_book(book);
                std::string yn;
                std::cout << "\n    Is that correct (y/n): ";
                std::cin >> yn;
                if(yn == "y") {           
                    book.borrower_id = -1;
                    book.days_until_return = 0;
                    update_books(BOOK_PATH, book);
                    fill_paragraph();
                    std::cout << "\n    You succesfully returned '" << book.name << "' from '" << book.author << "'." << std::endl;
                    getch();
                }
            }
        }
    }
    else if(key == 3){
        fill_paragraph();
        print_headers();
        Book_Container books = get_all_books(BOOK_PATH);
        for(Book_Container::iterator iter = books.begin(); iter != books.end(); ++iter){
            print_book(*iter);
        }
        getch();
    }
    else if(key == 4){
        fill_paragraph();
        int key2;
        std::string info;
        std::string info_long;
        std::string edit;

        info += "    id:";
        while(info.length() < 19)
        {
            info += ' ';
        }
        info += std::to_string(user.id);
        info_long += info;
        info = "";
        
        info += "\n    firstname:";
        while(info.length() < 20)
        {
            info += ' ';
        }
        info += user.firstname;
        info_long += info;
        info = "";
        
        info += "\n    lastname:";
        while(info.length() < 20)
        {
            info += ' ';
        }
        info += user.lastname;
        info_long += info;
        info = "";
        
        info += "\n    birth:";
        while(info.length() < 20)
        {
            info += ' ';
        }
        info += user.birth;
        info_long += info;
        info = "";
        
        info += "\n    pin:";
        while(info.length() < 20)
        {
            info += ' ';
        }
        info += user.pin;
        info_long += info + "\n\n";
        std::cout << info_long << "    [0] Edit\n    [1] Continue\n>>> ";
        std::cin >> key2;

        if(key2 == 0){
            fill_paragraph();
            std::cout << "\n    What do you want to edit (The parameter name)\n>>> ";
            std::cin >> edit;
            std::string new_value;
            User new_user = user;

            fill_paragraph();
            if(edit.find("firstname") != STRING_NOT_FOUND || edit.find("Firstname") != STRING_NOT_FOUND){
                std::cout << "\n    New Firstname\n>>> ";
                std::cin >> new_value;
                new_user.firstname = new_value;
                fill_paragraph();
                std::cout << "    Your new firstname is " << new_user.firstname << std::endl;
                getch();
            }
            else if(edit.find("lastname") != STRING_NOT_FOUND || edit.find("Lastname") != STRING_NOT_FOUND){
                std::cout << "\n    New Lastname\n>>> ";
                std::cin >> new_value;
                new_user.lastname = new_value;
                fill_paragraph();
                std::cout << "    Your new lastname is " << new_user.lastname << std::endl;
                getch();
            }
            else if(edit.find("pin") != STRING_NOT_FOUND || edit.find("PIN") != STRING_NOT_FOUND || edit.find("Pin") != STRING_NOT_FOUND){
                std::cout << "\n    New PIN\n>>> ";
                std::cin >> new_value;
                new_user.pin = new_value;
                fill_paragraph();
                std::cout << "    Your new PIN is " << new_user.pin << std::endl;
                getch();
            }
            else{
                fill_paragraph();
                std::cout << "    You can't edit the param '" << edit << "'." << std::endl;
                getch();
            }
            update_users(USER_PATH, new_user);
            lib(new_user, debug);
            return;
        }
        else if(key2 != 1)
            {
                fill_paragraph();
                std::cout << "    There is no option with index " << key2 << std::endl;
                getch();
            }
    }
    else if(key == 5){
        return;
    }
    else{
        fill_paragraph();
        std::cout << "    There is no option with index " << key << std::endl;
        getch();
    }
    
    lib(user, debug);
}

void fill_paragraph(void){
    std::string new_lines;
    for(int i = 0; i < NEW_LINES; i++){
        new_lines += '\n';
    }
    std::cout << new_lines << std::endl;
}

void print_book(const Book &book){
    std::string out = "    ";
    out += std::to_string(book.id);
    while (out.length() < 15)
    {
        out += ' ';
    }
    out += book.name;
    while (out.length() < 65)
    {
        out += ' ';
    }
    out += book.author;
    while (out.length() < 100)
    {
        out += ' ';
    }
    out += std::to_string(book.days_until_return) + " Days";
    std::cout << out << std::endl;
}

void print_headers(void){
    std::cout << "    ID";
    for(int i = 0; i < 9; i++){
        std::cout << " ";
    }
    std::cout << "Name";
    for(int i = 0; i < 46; i++){
        std::cout << " ";
    }
    std::cout << "Author";
    for(int i = 0; i < 29; i++){
        std::cout << " ";
    }
    std::cout << "Return time" << std::endl;
}