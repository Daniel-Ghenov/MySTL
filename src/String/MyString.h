#ifndef MYSTD_STRING_MYSTRING_H
#define MYSTD_STRING_MYSTRING_H
#include <cassert>
#include "Helper.h"

namespace mystd {

const int STRING_UPSIZE_BY = 2;
const int STRING_DEFAULT_CAP = 4;
const int STRING_DOWNSIZE_BY = 4;
const int STRING_CIN_BUFF_SIZE = 1024;

const int CHAR_SIZE = 256;

class string{
private:
    char* _data = nullptr;
    size_t _size = 0;
    size_t _capacity = 0;


public:

    static const size_t npos = -1;  //return-value for "no instance found"


    string();   //Big 4 and Constructors
    string(const char* content);
    explicit string(size_t capacity);
    ~string();
    string(const string& other);
    string& operator=(const string& other);
    string& operator=(const char* other);
    string& operator=(string&& other) noexcept;
    string(string&& other) noexcept;



    char& operator[](size_t size);  //Access Data
    const char& operator[](size_t size) const;
    const char& at(size_t size) const;
    char& at(size_t size);
    const char& back() const;
    const char& front() const;
    char& back();
    char& front();
    const char* data() const;
    const char* c_str() const;

    size_t size() const;    //Capacity
    size_t length() const;
    size_t capacity() const;
    void resize(size_t size);
    void reserve(size_t size);
    void clear();
    void shrink_to_fit();


    string& operator+=(const string& other);    //Modifiers
    string& append(const string& other);
    string& push_back(char c);
    string& assign(const string& other);
    void swap(string& other);
    void pop_back();

    string substr(size_t startpos, size_t endpos);  //String Operations

    bool operator==(const string& other) const; //Boolean Operations
    bool operator!=(const string& other) const;
    bool operator<=(const string& other) const;
    bool operator>=(const string& other) const;
    bool operator<(const string& other) const;
    bool operator>(const string& other) const;

    bool operator==(const char* other) const; //Boolean Operations
    bool operator!=(const char* other) const;
    bool operator<=(const char* other) const;
    bool operator>=(const char* other) const;
    bool operator<(const char* other) const;
    bool operator>(const char* other) const;

    bool empty() const;
    operator bool() const;
    int compare(const string& other) const;


    friend std::ostream& operator<<(std::ostream& os, const string& str);   //Stream Operations
    friend std::istream& operator>>(std::istream& is, string& str);
    friend std::istream& getline(std::istream& is, string& str, char delim);
    friend string operator+ (const string& lhs, const string& rhs);


private:

    void short_copy(const char* content, size_t size = sizeof(string));
    void short_copy(const string& other);
    void copy_from(const char* content);
    void copy_from(const string& other);
    void move(string&& other);
    void free() noexcept;


    bool is_short() const;
    size_t size_mask() const;
    size_t capacity_mask() const;
    void set_size(size_t size);
    void set_capacity(size_t capacity);
    void set_short_size(size_t size);
    size_t short_size()const;
    char* data();


    static const size_t mostSigBit = (static_cast<size_t>(1) << (sizeof(_capacity) * 8) - 1);   //bitsize mask for the most significant bit for ease of use

};

std::ostream& operator<<(std::ostream& os, const string& str);
std::istream& operator>>(std::istream& is, string& str);

std::istream& getline(std::istream& is, string& str, char delim = '\n');

string operator+ (const string& lhs, const string& rhs);

} // namespace mystd

#endif // MYSTD_STRING_MYSTRING_H
