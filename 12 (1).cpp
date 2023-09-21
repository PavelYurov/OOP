#include <list>
#include <iostream>
#include <cmath>
#include <math.h>
#include <string.h>



//классы исключений

class MyExeption : std::exception{
protected:
	std::string str;
public:
	MyExeption(const std::string& str) {
		this->str = str;
	}
	virtual ~MyExeption(){}
	virtual void print_exeption() {
		std::cerr << str << std::endl;
	};
};

class condition_exeption : MyExeption {
public:
	condition_exeption() : MyExeption("conditions not allow to do this"){}
	virtual ~condition_exeption() { print_exeption(); }
};

class size_exeption : MyExeption {
public:
	size_exeption() : MyExeption("wrong size or out of bounds") {}
	virtual ~size_exeption() { print_exeption(); }
};

class nullptr_exeption : MyExeption {
public:
	nullptr_exeption() : MyExeption("pointer to 0") {}
	virtual ~nullptr_exeption() { print_exeption(); }
};

//задание
//Минимальный набор полей : полный
//адрес, краткое имя, дата последнего
//изменения, дата последнего чтения, дата
//создания





class date { //класс даты дд.мм.гггг 
protected:
	int day;
	int month;
	long year;
public:
	date(int day, int month, long year) {
		if (day > 31 || day < 1 || month > 12 || month < 1) throw(condition_exeption());
		this->day = day;
		this->month = month;
		this->year = year;
	}
	date() { //default
		day = 0, month = 0, year = 0;
	}

	date& operator=(const date& a) {
		if(a.day == 0) throw(condition_exeption());
		day = a.day;
		month = a.month;
		year = a.year;
		return *this;
	}

	bool operator>(const date& a) { // некоторые операторы для класса дат
		if (year > a.year)
			return true;
		else
			if (year == a.year && month > a.month)
				return true;
			else
				if (year == a.year && month == a.month && day > a.day)
					return true;
				else
					return false;
	}

	bool operator<(const date& a) {
		if (year > a.year)
			return false;
		else
			if (year == a.year && month > a.month)
				return false;
			else
				if (year == a.year && month == a.month && day >= a.day)
					return 0;
				else
					return 1;
	}

	bool operator>(const date& a) const {
		if (year > a.year)
			return true;
		else
			if (year == a.year && month > a.month)
				return true;
			else
				if (year == a.year && month == a.month && day > a.day)
					return true;
				else
					return false;
	}

	bool operator<(const date& a) const {
		if (year > a.year)
			return false;
		else
			if (year == a.year && month > a.month)
				return false;
			else
				if (year == a.year && month == a.month && day >= a.day)
					return 0;
				else
					return 1;
	}

	bool operator==(const date& a) {
		return a.day == day && a.month == month && a.year == year;
	}

	bool operator==(const date& a) const {
		return a.day == day && a.month == month && a.year == year;
	}

	friend std::ostream& operator<<(std::ostream& out, const date& a) {
		out << a.day << "." << a.month << "." << a.year;
		return out;
	}
};

class file {
protected:
	std::string domain;
	std::string name;
	date dchange;
	date dlastread;
	date dcreate;
	int path_depth = 0;
public:
	file(std::string domain,
		std::string name,
		date dchange,
		date dlastread,
		date dcreate) {
		this->dchange = dchange; this->dcreate = dcreate; this->dlastread = dlastread; this->domain = domain; this->name = name;
		for (auto i : domain) {
			if (i == '/') path_depth++;
		}
	}
	virtual ~file(){}

	//Дата последнего
	//изменения, дата
	//последнего чтения;
	//полный адрес(по возрастанию)


	//операторы необходимые для работы push и других методов
	bool operator<(const file& a) {
		if (dchange < a.dchange) return true; else if (dchange == a.dchange && dlastread < a.dlastread) return true;
		else if (dchange == a.dchange && dlastread == a.dlastread && path_depth > a.path_depth) return true; else return false;
	}
	bool operator==(const file& a) {
		return a.domain == domain && a.name == name && a.dchange == dchange && a.dlastread == dlastread && a.dcreate == a.dcreate;
	}
	bool operator==(const file& a) const{
		return a.domain == domain && a.name == name && a.dchange == dchange && a.dlastread == dlastread && a.dcreate == a.dcreate;
	}

	friend std::ostream& operator<<(std::ostream& out, const file& f) {
		out << "name: " << f.name << " path: " << f.domain << " date of last change: " << f.dchange << " date of last read: " << f.dlastread << " date of creation: " << f.dcreate << std::endl;
		return out;
	}
};



template <class T>
void push(std::list<T>& lst, const T& element) {
	typename std::list<T>::iterator iter = lst.begin();
	while (iter != lst.end()) {
		if (*iter < element) break;
		iter++;
	}
	lst.insert(iter, element);
}

template <class T>
T pop(std::list<T>& lst) {
	if (lst.size() == 0)  throw(size_exeption());
	T tmp(*lst.begin());
	lst.pop_front();
	return tmp;
}

template <class T>
std::list<T> filter(std::list<T>& lst, bool (*f)(T&)) {
	if (lst.size() == 0) throw size_exeption();
	if (f == NULL) throw(nullptr_exeption());
	std::list<T> tmp;
	typename std::list<T>::iterator iter = lst.begin();
	for (;iter != lst.end();iter++) {
		if (f(*iter)) tmp.push_back(*iter);
	}
	return tmp;
}

template<class T>
void show(std::list<T> lst) {
	typename std::list<T>::iterator iter = lst.begin();
	for (; iter != lst.end(); iter++) {
		std::cout << *iter << " ";
	}
}

bool P(int& a) {
	return (a < 0) && (a % 7 == 0);
}



int main() {
	
	try {
		
		file fff("d:/s", "fff", { 1,1,1 }, { 1,1,2 }, { 1,1,1 });
		file rrr("d:", "rrr", { 1,1,1 }, { 1,1,1 }, { 1,1,1 });
		file mmm("d:/s/a/", "mmm", { 1,1,1 }, { 1,1,1 }, { 1,1,1 });

		file ggg("d:/aaa/", "ggg", { 1,1,1 }, { 1,1,1 }, { 1,1,1 });
		file ddd("d:/", "ddd", { 1,1,1 }, { 1,1,1 }, { 1,1,1 });
		file ttt("d:/", "ttt", { 1,1,1 }, { 2,1,1 }, { 1,1,1 });
		file hhh("d:/assr/", "hhh", { 5,3,2 }, { 1,1,2 }, { 1,1,2 });
		file aaa("d:/4/ras13d/", "aaa", { 1,4,2 }, { 1,1,2 }, { 1,1,2 });
		std::list<file> lst = { fff };
		push<file>(lst, ggg);
		push<file>(lst, ttt);
		push<file>(lst, aaa);
		push<file>(lst, hhh);
		push<file>(lst, ddd);

		push<file>(lst, rrr);
		push<file>(lst, mmm);


		for (auto i : lst) {
			std::cout << i << std::endl;
		}
		std::cout << std::endl;

		std::cout << pop(lst) << std::endl;

		std::cout << std::endl;

		for (auto i : lst) {
			std::cout << i << std::endl;
		}
		std::cout << std::endl << "================================================" << std::endl;

		std::list<int> lst1 = { -7, -14, -21 };
		for (int i = 10; i > -10; i--) {
			push<int>(lst1, i);
		}
		push<int>(lst1, 0);
		for (auto i : lst1) {
			std::cout << i << " ";
		}
		std::cout << std::endl << "\t" << pop<int>(lst1) << std::endl;
		std::cout << std::endl << std::endl << std::endl;
		for (auto i : lst1) {
			std::cout << i << " ";
		}
		std::cout << std::endl;

		std::list<int> tmp = filter<int>(lst1, P);

		show<int>(tmp);
		
		std::cout << std::endl;

	}
	catch (...) {}
	return 0;
}


