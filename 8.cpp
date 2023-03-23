#include <iostream>
#include<Windows.h>

class BaseString
{
protected:
	char* p;
	int len;
	int capacity;
public:
	BaseString(const char* ptr)
	{
		try {
			std::cout << "\nBase Constructor 1\n";
			len = strlen(ptr) + 1;
			capacity = 256;
			if (len > capacity)
				throw("too big string");
			
			if (!(p = new char[capacity])) {
				throw("not enough memory");
			}
			for (int i = 0; i < len; i++)
			{
				p[i] = ptr[i];
			}
			p[len - 1] = '\0';					//а если строка больше 256?
		}
		catch (const char* tmp_err) {
			std::cerr << tmp_err << std::endl;
			system("pause");
		}
	}
	BaseString(int Capacity = 256)
	{
		try {
			std::cout << "\nBase Constructor 0\n";
			capacity = Capacity;
			
			if (!(p = new char[capacity])) {
				throw("not enough memory");
			}
			len = 0;
		}
		catch (const char* tmp_err) {
			std::cerr << tmp_err << std::endl;
			system("pause");
		}
	}
	~BaseString()
	{
		std::cout << "\nBase Destructor\n";
		if (p != NULL)
			delete[] p;
		len = 0;
	}
	virtual int Length() {
		return len;
	}
	virtual int Capacity() {
		return capacity;
	}
	//char* get() {return p;}				//???
	char& operator[](int i) {
		return p[i];
	}
	BaseString& operator=(BaseString& s)
	{
		try {
			std::cout << "\nBase Operator = \n";
			len = s.Length();
			delete[]p;
			if (!(p = new char[s.capacity])) {
				throw("not enough memory");
			}
			capacity = s.capacity;
			for (int i = 0; i < s.Length(); i++)
			{
				p[i] = s[i];
			}
			//strcpy(p, s.get());
			p[len - 1] = '\0';
		}
		catch (const char* tmp_err) {
			std::cerr << tmp_err << std::endl;
			system("pause");
		}
		return *this;
	}
	BaseString(const BaseString& s)
	{
		try {
			std::cout << "\nBase Copy Constructor\n";
			len = s.len;
			delete[]p;
			if (!(p = new char[s.capacity])) {
				throw("not enough memory");
			}
			capacity = s.capacity;
			for (int i = 0; i < s.len - 1; i++)
			{
				p[i] = s.p[i];
			}
			p[len - 1] = '\0';
		}
		catch (const char* tmp_err) {
			std::cerr << tmp_err << std::endl;
			system("pause");
		}
	}
	virtual void print()
	{
		int i = 0;
		while (p[i] != '\0' && i < 256) {
			std::cout << p[i];
			i++;
		}
	}
};

class ChildString : public BaseString {
public:
	ChildString(const char* ptr) : BaseString(ptr) {
		std::cout << "\nChild Constructor 0\n";
	}
	ChildString(int capacity = 256) : BaseString(capacity) {
		std::cout << "\nChild Constructor 0\n";
	}
	ChildString(const BaseString& s) : BaseString(s) {
		std::cout << "\nChild <- Base Constructor 0\n";
	}
	ChildString(const ChildString& s) {
		try {
			std::cout << "\nChild Copy Constructor\n";
			len = s.len;
			delete[]p;
			if (!(p = new char[s.capacity])) {
				throw("not enough memory");
			}
			capacity = s.capacity;
			for (int i = 0; i < s.len- 1; i++)
			{
				p[i] = s.p[i];
			}
			p[len - 1] = '\0';
		}
		catch (const char* tmp_err) {
			std::cerr << tmp_err << std::endl;
			system("pause");
		}
	}
	~ChildString() {
		std::cout << "\nChild Destructor\n";
	}
	ChildString& operator=(const ChildString& s)
	{
		try {
			std::cout << "\nChild Operator = \n";
			len = s.len;
			delete[]p;
			capacity = s.capacity;
			if (!(p = new char[s.capacity])) {
				throw("not enough memory");
			}
			for (int i = 0; i < s.len; i++)
			{
				p[i] = s.p[i];
			}
			p[len - 1] = '\0';
		}
		catch (const char* tmp_err) {
			std::cerr << tmp_err << std::endl;
			system("pause");
		}
		return *this;
	}
	virtual int max_code_sum() {
		int max_v = 0;
		int v = 0;
		for (int i = 0; i < len - 2; i++) {
			if (p[i] != ' ' && p[i] != ',' && p[i] != '.' && p[i] != '\\' && p[i] != '/' && p[i] != '|') {
				//v += (int)p[i];
				v += static_cast<int>(p[i]);
			}
			else {
				max_v = max(max_v, v);
				v = 0;
			}
		}
		return max(max_v, v);
	}
};

int main() {
	{
		BaseString a("123 454");
		BaseString b(a);
		BaseString c;
		c = a;
		ChildString d("123 454 fid idf ллвла");
		ChildString e(d);
		ChildString f;
		f = d;
		a.print(); std::cout << std::endl;
		b.print(); std::cout << std::endl;
		c.print(); std::cout << std::endl;
		d.print(); std::cout << std::endl;
		e.print(); std::cout << std::endl;
		f.print(); std::cout << std::endl;
		std::cout << d.max_code_sum();
	}
	system("pause");
}
