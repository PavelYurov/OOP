#include <iostream>
#include <cmath>
#include <math.h>
#include <string.h>
#include <map>
#include<set>
#include <vector>

//классы исключений

class MyExeption : std::exception {
protected:
	std::string str;
public:
	MyExeption(const std::string& str) {
		this->str = str;
	}
	virtual ~MyExeption() {}
	virtual void print_exeption() {
		std::cerr << str << std::endl;
	}
};

class condition_exeption : MyExeption {
public:
	condition_exeption() : MyExeption("conditions not allow to do this") {}
	virtual ~condition_exeption() { print_exeption(); }
};

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
		day = -1, month = -1, year = -1;
	}
	date(const date& tmp) {
		day = tmp.day; month = tmp.month; year = tmp.year;
	}

	date& operator=(const date& a) {
		if (a.day == 0) throw(condition_exeption());
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

	bool operator !=(const date& a) {
		return (a.day != day || a.month != month || a.year != year);
	}
	bool operator !=(const date& a) const {
		return (a.day != day || a.month != month || a.year != year);
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

//поиск элемента по значению
template<class Key, class T>
typename std::map<Key, T>::iterator find_key(std::map<Key, T>& vmap, const T value) {
	typename std::map<Key, T>::iterator iter = vmap.begin();
	for (; iter != vmap.end(); iter++)
		if (iter->second == value) break;
	return iter;
}

//поиск по ключу
template<class Key, class T>
typename std::map<Key, T>::iterator find_element(std::map<Key, T>& vmap, const Key k) {
	typename std::map<Key, T>::iterator iter = vmap.begin();
	for (; iter != vmap.end(); iter++) 
		if (iter->first == k) break;
	return iter;
}

//вывод
template<class Key, class T>
void show(std::map<Key, T>& vmap) {
	std::cout << std::endl;
	for (typename std::map<Key, T>::iterator iter = vmap.begin(); iter != vmap.end(); iter++) {
		std::cout << "[" << iter->first << "]: " << iter->second << std::endl;
	}
}

//универсальный фильтр с предикатом f
template<class Key, class T>
std::map<Key, T> filter(std::map<Key, T>& vmap, bool (*f)(const T&)) {
	std::map<Key, T> tmp;
	typename std::map<Key, T>::iterator iter = vmap.begin();
	for (; iter != vmap.end(); iter++) {
		if (f(iter->second)) tmp[iter->first] = iter->second;
	}
	return tmp;
}

//добавление элемента с проверкой на повторение
template<class Key, class T>
void add(std::map<Key, T>& map, const Key& key, const T& value) {
	if (find_element<Key, T>(map, key) != map.end()) throw condition_exeption();
	map[key] = value;
}

template<class Key, class T>
std::set<T> uniq_val_set(std::map<Key, T> map) {
	std::set<T> tmp;
	for (typename std::map<Key, T>::iterator iter = map.begin(); iter != map.end(); iter++)
		tmp.insert(iter->second);
	return tmp;
}

template<class Key, class T>
std::vector<T> uniq_val_vector(std::map<Key, T> map) {
	std::vector<T> tmp;
	for (T i : (uniq_val_set<Key, T>(map))) {
		tmp.push_back(i);
	}
	return tmp;
}

//|=========================================|=|=========================================|
//|=========================================|=|=========================================|
//|=========================================|=|=========================================|

//поиск элемента по значению
template<class Key, class T>
typename std::multimap<Key, T> find_key(std::multimap<Key, T>& vmap, const T value) {
	typename std::multimap<Key, T>::iterator iter = vmap.begin();
	std::multimap<Key, T> tmp;
	for (; iter != vmap.end(); iter++)
		if (iter->second == value) tmp.insert({ iter->first,iter->second });
	return tmp;
}

//поиск по ключу
template<class Key, class T>
typename std::multimap<Key, T> find_element(std::multimap<Key, T>& vmap, const Key k) {
	typename std::multimap<Key, T>::iterator iter = vmap.begin();
	std::multimap<Key, T> tmp;
	for (; iter != vmap.end(); iter++)
		if (iter->first == k) tmp.insert({ iter->first,iter->second });;
	return tmp;
}

//вывод
template<class Key, class T>
void show(std::multimap<Key, T>& vmap) {
	std::cout << std::endl;
	for (typename std::multimap<Key, T>::iterator iter = vmap.begin(); iter != vmap.end(); iter++) {
		std::cout << "[" << iter->first << "]: " << iter->second << std::endl;
	}
}

//универсальный фильтр с предикатом f
template<class Key, class T>
std::multimap<Key, T> filter(std::multimap<Key, T>& vmap, bool (*f)(const T&)) {
	std::multimap<Key, T> tmp;
	typename std::multimap<Key, T>::iterator iter = vmap.begin();
	for (; iter != vmap.end(); iter++) {
		if (f(iter->second)) tmp.insert({ iter->first, iter->second });
	}
	return tmp;
}

/*
//добавление элемента с проверкой на повторение
template<class Key, class T>																	//не имеет смысла, так как противоречит смыслу multimap
void add(std::multimap<Key, T>& map, const Key& key, const T& value) {
	if (find_element<Key, T>(map, key) != std::multimap<Key,T>()) throw condition_exeption();
	map.insert({key,value});
}
*/

template<class Key, class T>
std::set<T> uniq_val_set(std::multimap<Key, T> map) {
	std::set<T> tmp;
	for (typename std::multimap<Key, T>::iterator iter = map.begin(); iter != map.end(); iter++)
		tmp.insert(iter->second);
	return tmp;
}

template<class Key, class T>
std::vector<T> uniq_val_vector(std::multimap<Key, T> map) {
	std::vector<T> tmp;
	for (T i : (uniq_val_set<Key, T>(map))) {
		tmp.push_back(i);
	}
	return tmp;
}



bool P(const date& a) {
	return a > date(1, 1, 2000);
}

int main() {

	try {
		std::map<std::string, date> file;
		file["tmp.exe"] = date(1, 11, 2001);
		file["msvdf.msi"] = date(31, 2, 1984);
		file["ddrsd-kill.dll"] = date(26, 12, 1991);
		file["rootkick.troyan"] = date(31, 2, 1984);
		file["system"] = date(15, 3, 2077);
		file["wsappx"] = date(12, 8, 2020);

		add<std::string, date>(file, "tmp.cpp", date(1, 11, 2001));

		//add<std::string, date>(file, "tmp.exe", date(14, 10, 2205));

		std::map<std::string, date> file2 = filter<std::string, date>(file, P);


		std::cout << find_key<std::string, date>(file, date(1, 11, 2001))->first << std::endl;
		std::cout << find_element<std::string, date>(file, "tmp.exe")->second << std::endl;

		show<std::string, date>(file);
		std::cout << std::endl;
		show<std::string, date>(file2);

		for (date i : uniq_val_set<std::string, date>(file)) {
			std::cout << i << " ";
		}
		std::cout << std::endl;
		for (date i : uniq_val_vector<std::string, date>(file)) {
			std::cout << i << " ";
		}

		std::cout << "\n=======================================\n";
	}catch(...){}

	try{
		std::multimap<std::string, date> file;
		file.insert({ { "tmp.exe",date(1, 11, 2001) },
			{ "tmp.exe",date(1, 11, 2003) },
			{ "tmp.exe",date(1, 11, 2001) },
			{ "tmp.exe",date(18, 3, 2015) },
			{"new_file.txt", date(1,11,2001)},
			{"msvdf.msi",date(31, 2, 1984)},
			{"ddrsd-kill.dll",date(26, 12, 1991)},
			{"rootkick.troyan",date(31, 2, 1984)},
			{"system",date(15, 3, 2077)},
			{"wsappx",date(12, 8, 2020)} });
		std::multimap<std::string, date> file2 = filter<std::string, date>(file, P);
		show<std::string, date>(file);
		std::cout << std::endl;
		show<std::string, date>(file2);
		for (date i : uniq_val_set<std::string, date>(file)) {
			std::cout << i << " ";
		}
		std::cout << std::endl;
		for (date i : uniq_val_vector<std::string, date>(file)) {
			std::cout << i << " ";
		}
		std::cout << std::endl;
		auto a1 = find_key<std::string, date>(file, date(1, 11, 2001));
		auto a2 = find_element<std::string, date>(file, "tmp.exe");
		show(a1); show(a2);

		//add<std::string, date>(file, "tmp1.exe", date(1, 1, 1));	//не имеет смысла для multimap
		//show<std::string, date>(file);

	}catch(...){}

	return 0;
}
