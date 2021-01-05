#ifndef BOOKS_H_
#define BOOKS_H_

#include <string>
#include <vector>

auto const STRING_NOT_FOUND = std::string::npos;

struct User{
    int id;
    std::string firstname;
    std::string lastname;
    std::string birth;
    std::string pin;
};

struct Book{
    int id = -1;
    std::string name;
    std::string author;
    int days_until_return;
    int borrower_id = -1;
};

typedef std::vector<Book> Book_Container;
typedef std::vector<User> User_Container;

Book_Container get_all_books(const std::string &book_path);
User_Container get_all_users(const std::string &path);
Book get_book_by_id(const std::string &path, const int &id);
User get_user_by_id(const std::string &path, const int &id);
void update_users(const std::string &path, const User &user);
void update_books(const std::string &path, const Book &book);
void decrement_day(const std::string &path);
void write_users(const std::string &path, const User_Container &users);
void write_books(const std::string &path, const Book_Container &books);

#endif