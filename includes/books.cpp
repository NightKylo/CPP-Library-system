#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <string.h>
#include <sstream>

auto const STRING_NOT_FOUND = std::string::npos;
const std::string BOOK_PATH = "books";
const std::string USER_PATH = "users";

struct User{
    int id;
    std::string firstname;
    std::string lastname;
    std::string birth;
    std::string pin;
};

struct Book
{
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

Book_Container get_all_books(const std::string &book_path){
    std::ifstream file;
    std::string line;
    Book_Container con;
    std::vector<std::string> values = {"borrower_id", "name", "author", "id", "days_until_return"};

    file.open(book_path, std::ios::binary | std::ios::in);
    Book book;
    while(true){
        std::getline(file , line);
        if(file.fail())
            break;
        if(line.find(":") != STRING_NOT_FOUND){
            for(int i = 0; i < values.size(); i++){
                if(line.find(values[i]) != STRING_NOT_FOUND){
                    std::string value;
                    for(int v = 0; v < line.length(); v++){
                        if(v > line.find(":") && v != line.length() - 1){
                            value += line[v];
                        }
                    }
                    
                    //std::cout << "String: " << value << "\nLen: " << value.length() << std::endl;
                    //std::cout << values[i] << std::endl;
                    if(strcmp(values[i].c_str(), "borrower_id") == 0){
                        book.borrower_id = std::stoi(value);
                    }
                    else if(strcmp(values[i].c_str(), "name") == 0){
                        book.name = value;
                    }
                    else if(strcmp(values[i].c_str(), "author") == 0){
                        book.author = value;
                    }
                    else if(strcmp(values[i].c_str(), "days_until_return") == 0){
                        book.days_until_return = std::stoi(value);
                    }
                    else if(strcmp(values[i].c_str(), "id") == 0){
                        book.id = std::stoi(value);
                    }
                    break;
                }
            } 
        }
        else if(line.find("}") != STRING_NOT_FOUND){
            con.push_back(book);
            //cout << "Added user at " << user_num << endl;
        }
    }

    file.close();
    return con;
}

User_Container get_all_users(const std::string &path){
    std::ifstream file;
    std::string line;
    User_Container con;
    std::vector<std::string> values = {"id", "firstname", "lastname", "birth", "pin"};
    file.open(path, std::ios::binary | std::ios::in);
    User user;
    while(true){
        std::getline(file , line);
        if(file.fail())
            break;
        if(line.find(":") != STRING_NOT_FOUND){
            for(int i = 0; i < values.size(); i++){
                if(line.find(values[i]) != STRING_NOT_FOUND){
                    std::string value;
                    for(int v = 0; v < line.length(); v++){
                        if(v > line.find(":") && v != line.length() - 1){
                            value += line[v];
                        }
                    }
                    
                    //std::cout << "String: " << value << "\nLen: " << value.length() << std::endl;
                    if(strcmp(values[i].c_str(), "id") == 0){
                        user.id = stoi(value);
                    }
                    else if(strcmp(values[i].c_str(), "firstname") == 0){
                        user.firstname = value;
                    }
                    else if(strcmp(values[i].c_str(), "lastname") == 0){
                        user.lastname = value;
                    }
                    else if(strcmp(values[i].c_str(), "birth") == 0){
                        user.birth = value;
                    }
                    else if(strcmp(values[i].c_str(), "pin") == 0){
                        user.pin = value;
                    }
                    break;
                }
            } 
        }
        else if(line.find("}") != STRING_NOT_FOUND){
            con.push_back(user);
            //cout << "Added user at " << user_num << endl;
        }
    }

    //std::cout << "    USERS LOADED: " << con.size() << "\n" << std::endl;
    file.close();
    return con;
}

Book get_book_by_id(const std::string &path, const int &id){
    Book_Container books = get_all_books(BOOK_PATH);
    for(Book_Container::iterator iter = books.begin(); iter != books.end(); ++iter){
        Book book = *iter;
        if(book.id == id){
            return book;
        }
    }
    Book error;
    return error;
}

User get_user_by_id(const std::string &path, const int &id){
    User_Container all_users = get_all_users(path);
    for(User_Container::iterator iter = all_users.begin(); iter != all_users.end(); ++iter){
        User user = *iter;
        if(user.id == id){
            return user;
        }
    }
    User error;
    return error;
}

void update_users(const std::string &path, const User &user){
    User_Container old_users = get_all_users(path);
    User_Container new_users;
    for(User_Container::iterator iter = old_users.begin(); iter != old_users.end(); ++iter){
        User u = *iter;
        if(u.id == user.id){
            new_users.push_back(user);
        }
        else{
            new_users.push_back(u);
        }
    }
    write_users(path, new_users);
}

void update_books(const std::string &path, const Book &book){
    Book_Container old_books = get_all_books(path);
    Book_Container new_books;
    for(Book_Container::iterator iter = old_books.begin(); iter != old_books.end(); ++iter){
        Book b = *iter;
        if(b.id == book.id){
            new_books.push_back(book);
        }
        else{
            new_books.push_back(b);
        }
    }
    write_books(path, new_books);
}

void decrement_day(const std::string &path){
    Book_Container books = get_all_books(path);
    Book_Container new_books;
    for(Book_Container::iterator iter = books.begin(); iter != books.end(); ++iter){
        Book book = *iter;
        if(book.borrower_id != -1){
            book.days_until_return--;
        }
        new_books.push_back(book);
    }
    write_books(path, new_books);
}

void write_users(const std::string &path, const User_Container &users){
    std::ofstream file;
    file.open(path);
    std::vector<std::string> user_strings (users.size());
    for(User_Container::const_iterator iter = users.begin(); iter != users.end(); ++iter){
        User user = *iter;
        std::ostringstream stream;
        stream << "{\n    id:" << user.id << "\n    firstname:" << user.firstname << "\n    lastname:" << user.lastname << "\n    birth:" << user.birth << "\n    pin:" << user.pin << "\n}\n";
        user_strings.push_back(stream.str());
    }
    std::string user_string;
    
    for(std::vector<std::string>::iterator iter = user_strings.begin(); iter != user_strings.end(); ++iter){
        user_string += *iter;
    }

    file << user_string;
    file.close();
}

void write_books(const std::string &path, const Book_Container &books){
    std::ofstream file;
    file.open(path);
    std::vector<std::string> user_strings (books.size());
    for(Book_Container::const_iterator iter = books.begin(); iter != books.end(); ++iter){
        Book book = *iter;
        std::ostringstream stream;
        stream << "{\n    id:" << book.id << "\n    name:" << book.name << "\n    author:" << book.author << "\n    days_until_return:" << book.days_until_return << "\n    borrower_id:" << book.borrower_id << "\n}\n";
        user_strings.push_back(stream.str());
    }
    std::string user_string;
    
    for(std::vector<std::string>::iterator iter = user_strings.begin(); iter != user_strings.end(); ++iter){
        user_string += *iter;
    }

    file << user_string;
    file.close();
}
