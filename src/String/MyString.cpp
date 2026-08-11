#include "MyString.h"

namespace mystd {

string::string(const char* content){
    if(!content)
        throw std::logic_error("Cannot initialize with nullptr");

    size_t len = strlen(content);

    if(len < sizeof(string)){   //short
        set_short_size(len);
        strcpy_s((char*)this, len + 1, content);
    }

    else{   //normal initialization
        set_size(len + 1);
        _data = new char[size() * STRING_UPSIZE_BY];
        strcpy(_data, content);
        set_capacity(size() * STRING_UPSIZE_BY);
    }
}
string::string(size_t capacity){
    if(capacity >= sizeof(string)){
        _data = new char[capacity];
        _data[0] = '\0';
        set_capacity(capacity);
    }
}
string::~string(){
    free();
}

string::string(const string& other){
    copy_from(other);
}

string& string::operator=(const string& other){

    if(this != &other){
        free();
        copy_from(other);
    }
    return *this;
}

string& string::operator=(const char* other){   //operator overload so that converting constructor is not used

    free();
    copy_from(other);
    return *this;
}

string& string::operator=(string&& other) noexcept {
    if(this != &other){
        this->free();
        this->move(std::move(other));
    }
    return *this;
}

string::string(string&& other) noexcept{
    move(std::move(other));
}



void string::reserve(size_t size){

    if(size > this->size()){
        resize(size);
    }
}

size_t string::capacity() const{
    if(is_short()){
        return sizeof(string);
    }
    return (_capacity << 1 )| capacity_mask();   //bitshifting _capacity and OR it with our mask to get the actual capacity
}

size_t string::size() const {
    if(is_short()){
        return short_size();
    }
    return _size >> 1 | size_mask(); //the most significant byte of _size is actually the most significant byte of _capacity
    //so we bitshift once and Or with sizeMask
}


size_t string::length() const{
    return size();
}

char* string::data(){   //private function for returning the data;
    if(is_short())       //used so we don't check for shortness every time
        return (char*)this;
    return _data;
}
const char* string::data() const{   //the same as c_str
    if(is_short())
        return (char*)this;
    return _data;
}

const char* string::c_str() const{  //in std:: implementations c_str() == data()
    if(is_short())
        return (char*)this;
    return _data;
}

void string::resize(size_t size){

    if(size < sizeof(string)){
        const char* currData = data();
        short_copy(currData);    //if our new string can be represented as short
                                //we copy it as a short string
        if(!is_short())
            delete[] currData;

        return;
    }

    char* newData = new char[size];
    strcpy_s(newData, size, data());

    if(!is_short()){ //if we resize from being short we shouldn't delete the current data
        delete[] _data;
    }
    _data = newData;
    set_size(size);
    set_capacity(size * STRING_UPSIZE_BY);

}
void string::clear(){
    free();
}

void string::shrink_to_fit(){
    set_capacity(size());
}


char& string::operator[](size_t size){
    return data()[size];
}

const char& string::operator[](size_t size) const{
    return data()[size];
}

const char& string::at(size_t size) const{
    if(size > this->size())
        throw std::out_of_range("Out of Bounds");

    return (*this)[size];
}

char& string::at(size_t size){
    if(size > _size)
        throw std::out_of_range("Out of Bounds");

    return (*this)[size];
}
const char& string::front() const{
    if(_data == nullptr)
        throw std::logic_error("String has been Deleted");

    return (*this)[0];
}

char& string::front() {
    if(_data == nullptr)
        throw std::logic_error("String has been Deleted");

    return (*this)[0];
}

const char& string::back() const {
    if(_data == nullptr)
        throw std::logic_error("String has been Deleted");


    return (*this)[_size];
}

char& string::back(){
    if(_data == nullptr)
        throw std::logic_error("String has been Deleted");

    return (*this)[_size];
}




string& string::operator+=(const string& other){

    if(is_short() && size() + other.size() < capacity()){
        strcat((char*)this, other.data());   //if we can keep our string short we just concatenate inside the short string
    }
    else if(size() + other.size() + 1 < capacity()){
        strcat(data(), other.data());    //if we have the capacity we just concatenate
    }
    else{
        char* newString = new char[(size() + other.size() + 1) * STRING_UPSIZE_BY];
        strcpy(newString, data());
        strcat(newString, other.data()); //else we create a new string and make it the concatenation of our two

        if(!is_short())
            delete[] _data;

        _data = newString;
        set_size(size() + other.size());
        set_capacity(size() * STRING_UPSIZE_BY);
    }

    return *this;
}

string& string::append(const string& other){
    operator+=(other);
    return *this;
}

string& string::push_back(char c){
    if(size() == capacity())
        resize(capacity() * STRING_UPSIZE_BY);

    (*this)[_size++] = c;
    return *this;
}

string& string::assign(const string& other){
    operator=(other);
    return *this;
}


void string::swap(string& other){
    string tempStr = other;
    other = *this;
    *this = tempStr;
}

void string::pop_back(){
    _data[_size--] = '\0';
    if(_size < _capacity / STRING_DOWNSIZE_BY){
        resize(_capacity / STRING_UPSIZE_BY);
    }
}

string string::substr(size_t startpos, size_t len = npos){
    if(size() < startpos + len)
        throw std::out_of_range("Out of Bounds");


    string str;
    if(len < sizeof(string)){   //if our new string can be represented as short we should do so
        str.short_copy(data() + startpos, len);
        return str;
    }

    str.set_capacity(len * STRING_UPSIZE_BY);   //else we create a new string and return it
    char* newStr = new char[str.capacity()];
    strcpy_s(newStr, len,  data() + startpos);
    str._data = newStr;
    str.set_size(len);

    return str;
}


int string::compare(const string& other) const{   //comparison operators
    return strcmp(data(), other.data());
}

bool string::operator==(const string& other) const{
    return(strcmp(data(), other.data()) == 0);
}
bool string::operator!=(const string& other) const{
    return(strcmp(data(), other.data()) != 0);
}
bool string::operator<=(const string& other) const{
    return(strcmp(data(), other.data()) <= 0);
}
bool string::operator>=(const string& other) const{
    return(strcmp(data(), other.data()) >= 0);
}
bool string::operator<(const string& other) const{
    return(strcmp(data(), other.data()) <= 0);
}
bool string::operator>(const string& other) const{
    return(strcmp(data(), other.data()) >= 0);
}

bool string::operator==(const char* other) const{
    return(strcmp(data(), other) == 0);
}
bool string::operator!=(const char* other) const{
    return(strcmp(data(), other) != 0);
}
bool string::operator<=(const char* other) const{
    return(strcmp(data(), other) <= 0);
}
bool string::operator>=(const char* other) const{
    return(strcmp(data(), other) >= 0);
}
bool string::operator<(const char* other) const{
    return(strcmp(data(), other) <= 0);
}
bool string::operator>(const char* other) const{
    return(strcmp(data(), other) >= 0);
}

bool string::empty() const{
    return size() == 0;
}
string::operator bool() const{
    return (_data == nullptr || empty());
}

std::ostream& operator<<(std::ostream& os, const string& str){
    os<<str.data();
    return os;
}

std::istream& operator>>(std::istream& is, string& str){

    char buffer[STRING_CIN_BUFF_SIZE];

    is>>buffer;
    str = buffer;
    return is;
}

std::istream& getline(std::istream& is, string& str, char delim){

    char buffer[STRING_CIN_BUFF_SIZE];

    is.getline(buffer, STRING_CIN_BUFF_SIZE, delim);
    str = buffer;
    return is;

}

string operator+ (const string& lhs, const string& rhs){
    size_t newsize = (lhs.size() + rhs.size() + 1);
    newsize = (newsize < sizeof(string))? newsize : newsize * STRING_UPSIZE_BY;    //if the new string can be stored as a short string
                                                                            //we will not upsize it so we can not use dynamic memory allocation
    string newString (newsize);
    strcpy(newString.data(), lhs.data());
    strcat(newString.data(), rhs.data());
    return newString;
}



void string::free() noexcept {
    if(!is_short())
        delete[] _data;
    _data = nullptr;
    set_capacity(0);
    set_size(0);
}
void string::copy_from(const string& other){
    if(other.is_short()){
        short_copy(other);
        return;
    }

    set_capacity(other.capacity());
    set_size(other.size());
    _data = new char[capacity()];
    strcpy(_data, other._data);
}

void string::copy_from(const char* content){  //overload so we do not go trough converting constructor

    size_t strsize = strlen(content);
    if(strsize < sizeof(string)){
        short_copy(content, strsize);
        return;
    }

    set_capacity(strsize * STRING_UPSIZE_BY);
    _data = new char[capacity()];
    strcpy_s(_data, strsize, content);
    set_size(strsize);

}
void string::move(string&& other){

    if(other.is_short()){
        short_copy(other);
        other.free();
    }
    else{
        this->_data = other._data;
        this->_size = other._size;
        this->_capacity = other._capacity;
        other._data = nullptr;
        other.free();
    }
}

void string::short_copy(const string& other){//copy function for shortStrings;
    assert(other.is_short());

    set_short_size(other.size());
    strcpy_s((char*)this, sizeof(string) ,other.data());

}

void string::short_copy(const char* content, size_t size){   //parameters used so we can be ensured that data will not overflow when using short strings
    assert(size <= sizeof(string));

    if(!is_short()){
        delete[] _data;
    }
    set_short_size(size);
    strcpy_s((char*)this, size, content);   //if we are in a short string we directly copy onto the memory of the string
}


size_t string::size_mask() const{
    return ((_capacity ^ mostSigBit) & mostSigBit);
    //The most significant bit in _capacity is actually the most significant bit of our size but reversed so we XOR it
}
size_t string::capacity_mask() const{
    return _size & 1;
        //the least significant byte of _size is the least significant bit of capacity

}

void string::set_size(size_t size){

    _size = ( size << 1) | (_size & 1);
    //bitshifting the new size and adding the old sizes last bit which is the _capacity bit


    _capacity = ((_capacity << 1) >> 1) | ((size ^ mostSigBit) & mostSigBit);
    //bitshift capacity so the most significant bit is 0 and then we OR with the most significant bit of _size
    //we also XOR the most significant bit of size so it is reversed
    //That way when the two most significant bits of _capacity are "00" => size > capacity => we are in a short string

}

void string::set_capacity(size_t capacity){

    _size = ((_size >> 1) << 1) | (capacity & 1);
    //setting the least significant bit of _size to be the least significant bit of capacity

    _capacity = capacity >> 1 | (_capacity & mostSigBit);
    //shifting capacity so we keep the bit of _size

}

bool string::is_short() const{
    return ((_capacity >> ((sizeof(_capacity) * 8) - 2)) == 0) ;   //checking if the two most significant bits of _capacity are "00"
}

void string::set_short_size(size_t size){

    assert(size < sizeof(string));  //when we are in a short string the size will be stored in the last byte of String
    //which is the most significant byte in _capacity because we are in little endian

    _capacity = ((_capacity << 8) >> 8) | ((sizeof(string) - size - 1) << ((sizeof(_capacity) * 8) - 8));  //flushing the last 8 bits of _capacity and setting them to the bits of size
    //since short sized strings are at max 23 characters long the 6 bits of the most significant byte of _capacity can hold them all
    //we have sizeof(string - size - 1) so that when the size is 24 the last character is actually /0

}
size_t string::short_size()const{

    return sizeof(string) - (_capacity >> ((sizeof(_capacity) * 8) - 8)) - 1;
    //the size is stored as sizeof(string) - size - 1;
}

} // namespace mystd
