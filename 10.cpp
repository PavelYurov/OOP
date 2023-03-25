/*
Задание 6.1
1. Реализовать структуру для хранения представленных в задании
данных (класс, представляющий Связный список, является шаблоном класса, в
main его работа демонстрируется на примере хранения объекта структуры/класса из Вашего варианта, см. табл. 6.1 и код 6.1). Информация в элемент
структуры вводится со стандартного потока ввода (предусмотреть автоматическое заполнение).
2. В классе Связный Список ввести деструктор. Поля класса должны
быть защищёнными (protected). Ввести открытый интерфейс (public) для работы с ними. Унаследовать класс списка от базового класса Связный список с
ключевым словом public. Переопределить функции push() и pop() – для добавления и извлечения элементов из списка. Написать функцию вставки нового
элемента в произвольное место списка insert(), удаления remove(), поиска элемента find() по значению. Реализовать указанный вид поиска: под функцией
фильтра предполагается поиск элементов, которые удовлетворяют условию
element.field >, < value, под поиском – только element.info==value (element.info –
поле info в элементе списка element, value – искомое значение). Фильтр возвращает новый список элементов, удовлетворяющих условию. Функцию фильтра и
поиска реализовать в итеративной и рекурсивной формах. Класс, объекты которого хранятся в списке, указан в таблице 1.


Структура «Велосипед».
Минимальный набор полей: марка, тип, тип тормозов, количество
колес, диаметр колеса, наличие амортизаторов, детский или
взрослый.
Добавление: в конец (стек)
Удаление: с конца (стек)
Поиск по марке, фильтр по диаметру колеса.

Задание 6.2
1. Кроме фильтра по указанным полям, необходимо реализовать универсальный фильтр по произвольному полю, который принимает указатель на
функцию. Эта функция возвращает 1, если элемент удовлетворяет условию и 0
– в противном случае.
2. Выделите память для списка динамически (используя указатель на
базовый класс), а затем, в конце работы кода, освободите её. Проверьте правильную работу деструкторов базового и производного классов (используйте
виртуальный деструктор). Преобразуйте указатель на базовый класс в указатель
типа производного класса с помощью оператора dynamic_cast. Проверьте работу деструкторов в случае выполнения операции освобождения памяти для преобразованного указателя.
3. Напишите функции сохранения списка файл* save() и извлечения
списка из файла* load(). Для ввода/вывода элемента списка в/из файла используйте переопределённые функции ввода/вывода в поток (сделайте различие между вводом/выводом в консоль и вводом/выводом в файл)
*/

#include<iostream>
#include<fstream>
#include <typeinfo>


class Exception : public std::exception {
protected:
	char* str;
public:
	Exception(const char* str) {
		if (!(this->str = new char[strlen(str) + 1])){
			throw "not enough memory";
		}
		strcpy_s(this->str, strlen(str) + 1, str);
	}
	Exception(const Exception& ptr) {
		if (!(str = new char[strlen(ptr.str) + 1])) {
			throw "not enough memory";
		}
		strcpy_s(this->str, strlen(ptr.str) + 1, ptr.str);
	}
	~Exception() {
		delete[]str;
		str = NULL;
	}
	virtual void print() {
		std::cerr << "\nException was caught: " << str;
 	}
};

class Exception_Memory : Exception {
public:
	Exception_Memory(const char* str = "not enought memory") : Exception(str) {

	}
	Exception_Memory(const Exception_Memory& ptr) : Exception(ptr) {

	}
};

class Exception_Index : public Exception {
protected:
	int max_index;
	int index;
public:
	Exception_Index(int index, int max_index, const char* str = "not enought memory") : Exception(str) {
		this->index = index;
		this->max_index = max_index;
	}
	Exception_Index(const Exception_Index& ptr) : Exception(ptr) {
		this->index = ptr.index;
		this->max_index = ptr.max_index;
	}
	virtual void print() {
		Exception::print();
		std::cerr << "\nexpected [0, " << max_index << "], but got " << index;
	}
};

template<class T>
class LinkedList;
template<class T>
class Queue;
template<class T>
class List;

template<class T>
class Element
{
protected:
	Element* next;
	Element* prev;
	T info;
public:
	/*Element(T data)
	{
		next = prev = NULL;
		info = data;
	}*/
	Element(const T* data, Element* Prev = NULL, Element* Next = NULL)
	{
		next = Next;
		prev = Prev;
		info = *data;
	}
	Element(const Element& el)
	{
		next = el.next;
		prev = el.prev;
		info = el.info;
	}
	~Element() {
		std::cout << "\nexiting " << info;
	}
	template<class T1>
	friend std::ostream& operator<<(std::ostream& s, Element<T1>& el);
	template<class T1>
	friend class LinkedList;
	template<class T1>
	friend class Queue;
	template<class T1>
	friend class List;
	template<class T1>
	friend std::ostream& operator<<(std::ostream& s, LinkedList<T1>& el);
};

template<class T1>
std:: ostream& operator<<(std::ostream& s, Element<T1>& el)
{
	s << el.info;
	return s;
}


template<class T>
class LinkedList
{
protected:
	Element<T>* head;
	Element<T>* tail;
	int count;
public:
	LinkedList()
	{
		head = tail = NULL;
		count = 0;
	}
	LinkedList(T* arr, int len) {
		count = 0;
		if (len > 0) {
			Element<T>* cur;
			if(!(cur = new Element<T>(&arr[0])))throw Exception_Memory();
			head = cur;
			++count;
			for (int i = 1; i < len; ++i) {
				if(!(cur->next = new Element<T>(&arr[i],cur)))throw Exception_Memory();
				cur = cur->next;
				++count;
			}
			tail = cur;
		}
		else {
			head = tail = NULL;
		}
	}
	virtual Element<T>* pop() = 0;

	virtual Element<T>* push(T value) = 0;

	Element<T>& operator[](int index)
	{
		Element<T>* current = head;
		if (count <= index || index < 0) throw Exception_Index(index, count);
		for (int i = 0;current != NULL && i < index;current = current->next, i++);
		return *current;
	}

	virtual bool isEmpty() {
		return (LinkedList<T>::count == 0);
	}

	template<class T1>
	friend std::ostream& operator<<(std::ostream& s, LinkedList<T1>& el);

	virtual ~LinkedList()
	{
		std::cout << "\nBase class destructor";
		Element<T>* next_element = head;
		while (next_element != NULL) {
			Element<T>* cur = next_element;
			next_element = next_element->next;
			delete cur;
		}
		head = NULL; tail = NULL;
		count = 0;
	}
};

template<class T1>
std::ostream& operator<<(std::ostream& s, LinkedList<T1>& el)
{
	s << "\n";
	Element<T1>* current;
	for (current = el.head; current != NULL; current = current->next)
		s << *current << "; ";
	return s;
}

template<class T>
class Queue : public LinkedList<T>
{
public:
	Queue<T>() : LinkedList<T>() {}
	Queue<T>(T* arr, int len) : LinkedList<T>(arr, len) {}
	virtual Element<T>* push(T value)
	{
		if (LinkedList<T>::head == NULL) //if(count==0)
		{
			//пустой список
			if(!(LinkedList<T>::head = new Element<T>(value)))throw Exception_Memory();;
			LinkedList<T>::tail = LinkedList<T>::head;
		}
		else
		{
			//элементы уже есть
			if(!(LinkedList<T>::tail->next = new Element<T>(value,LinkedList<T>::tail)))throw Exception_Memory();;
			//LinkedList<T>::tail->next->prev = ;
			LinkedList<T>::tail = LinkedList<T>::tail->next;
		}
		LinkedList<T>::count++;
		return LinkedList<T>::tail;
	}
	virtual Element<T>* pop()
	{
		if (LinkedList<T>::head == NULL)
			return NULL;
		Element<T>* res = LinkedList<T>::head;
		if (LinkedList<T>::head == LinkedList<T>::tail)
			LinkedList<T>::head = LinkedList<T>::tail = NULL;
		else
			LinkedList<T>::head = LinkedList<T>::head->next;
		LinkedList<T>::count--;
		return res;
	}
	virtual ~Queue() { std::cout << "\nQueue class destructor"; }
};

template<class T>
class List : public LinkedList<T> {
public:
	virtual Element<T>* push(T value)
	{
		if (LinkedList<T>::head == NULL) //if(count==0)
		{
			//пустой список
			if (!(LinkedList<T>::head = new Element<T>(&value)))throw Exception_Memory();;
			LinkedList<T>::tail = LinkedList<T>::head;
		}
		else
		{
			//элементы уже есть
			if (!(LinkedList<T>::tail->next = new Element<T>(&value, LinkedList<T>::tail)))throw Exception_Memory();;
			//LinkedList<T>::tail->next->prev = ;
			LinkedList<T>::tail = LinkedList<T>::tail->next;
		}
		LinkedList<T>::count++;
		return LinkedList<T>::tail;
	}
	List<T>() : LinkedList<T>() {}
	List<T>(T* arr, int len) : LinkedList<T>(arr, len) {}
	virtual Element<T>* pop()
	{
		if (LinkedList<T>::head == NULL)
			return NULL;
		Element<T>* res = LinkedList<T>::tail;
		if (LinkedList<T>::head == LinkedList<T>::tail)
			LinkedList<T>::head = LinkedList<T>::tail = NULL;
		else
			LinkedList<T>::tail = LinkedList<T>::tail->prev;
		LinkedList<T>::count--;
		return res;
	}
	virtual Element<T>* insert(int index,const T& data) {
		if (index < 0 || index > LinkedList<T>::count) throw Exception_Index(index, LinkedList<T>::count+1);
		Element<T>* ptr;
		if(!(ptr = new Element<T>(&data))) throw Exception_Memory();
		if (index == 0) {
			ptr->next = LinkedList<T>::head;
			LinkedList<T>::head->prev = ptr;
			LinkedList<T>::head = ptr;
		}
		else if (index == LinkedList<T>::count) {
			ptr->prev = LinkedList<T>::tail;
			LinkedList<T>::tail->next = ptr;
			LinkedList<T>::tail = ptr;
		}
		else {
			Element<T>* ptr_prev = &(this->operator[](index - 1));
			Element<T>* ptr_next = &(this->operator[](index));
			ptr_prev->next = ptr;
			ptr_next->prev = ptr;
			ptr->next = ptr_next;
			ptr->prev = ptr_prev;
		}
		LinkedList<T>::count++;
		return ptr;
	}

	virtual void remove(int index) {
		if (index < 0 || index >= LinkedList<T>::count) throw Exception_Index(index, LinkedList<T>::count);
		Element<T>* ptr_to_delete = &(this->operator[](index));
		if (index == 0) {
			LinkedList<T>::head = LinkedList<T>::head->next;
			LinkedList<T>::head->prev = NULL;
		}
		else if (index == LinkedList<T>::count - 1) {
			LinkedList<T>::tail = LinkedList<T>::tail->prev;
			LinkedList<T>::tail->next = NULL;
		}
		else {
			Element<T>* ptr_next = &(this->operator[](index + 1));
			Element<T>* ptr_prev = &(this->operator[](index - 1));
			ptr_prev->next = ptr_next;
			ptr_next->prev = ptr_prev;
		}
		--LinkedList<T>::count;
		delete ptr_to_delete;
	}
	virtual List<T>* find(bool (*f)(const T&, const T&),const T& find_data) {
		List<T>* l = new List<T>();
		for (Element<T>* ptr = LinkedList<T>::head; ptr != NULL; ptr = ptr->next) if (f(ptr->info,find_data)) l->push(ptr->info);
		return l;
		
	}
	virtual bool save(const char* root) {
		std::ofstream f(root);
		if (f.is_open()) {
			for (Element<T>* ptr = LinkedList<T>::head; ptr != NULL; ptr = ptr->next) f << "\n"<< ptr->info ;
			f.close();
			return true;
		}
		else return false;
	}
	virtual bool load(const char* root) {
		std::ifstream f(root);
		if (f.is_open()) {
			while (!f.eof()) {
				T a;
				f >> a;
				this->push(a);
			}
			f.close();
			return true;
		}
		else return false;
	}
	virtual ~List() { std::cout << "\nList class destructor"; }
};


class my_class
{
public:
	int data;
	my_class(int v = 0) { data = v; }
	~my_class() { std::cout << "\nmy_class destructor"; }
	friend std::ostream& operator<<(std::ostream& s, my_class& value);
};
std::ostream& operator<<(std::ostream& s, my_class& value)
{
	s << value.data;
	return s;
}

/*Структура «Велосипед».
Минимальный набор полей: марка, тип, тип тормозов, количество
колес, диаметр колеса, наличие амортизаторов, детский или
взрослый.
Добавление: в конец (стек)
Удаление: с конца (стек)
Поиск по марке, фильтр по диаметру колеса.*/

enum Bike_Type
{
	nullt = 0,
	road,
	highway,
	hybrid,
	mountain,
	electro
};
enum Brake_Type
{
	nullb = 0,
	type_1,
	type_c
};
enum age {
	child = 0,
	abult
};

class Byke {
protected:
	char* mark;
	Bike_Type type;
	Brake_Type brake;
	int wheel;
	float diam;
	bool amort;
	age ag;
public:
	Byke() {
		if(!(this->mark == new char[1])) throw Exception_Memory();
		this->mark[0] = '~';
		this->type = static_cast<Bike_Type>(0);
		this->brake = static_cast<Brake_Type>(0);
		this->wheel = 0;
		this->diam = 0.0f;
		this->amort = false;
		this->ag = abult;
	}
	Byke(const char* mark,
		Bike_Type type,
		Brake_Type brake,
		int wheel,
		float diam,
		bool amort,
		age ag
	) {
		if (!(this->mark == new char[strlen(mark)+1])) throw Exception_Memory();
		strcpy_s(this->mark, strlen(mark) + 1, mark);
		this->type = static_cast<Bike_Type>(type);
		this->brake = static_cast<Brake_Type>(brake);
		this->wheel = wheel;
		this->diam = diam;
		this->amort = amort;
		this->ag = ag;
	}

	bool filter1(const Byke& data, Byke& filt) {
		return (data.diam > filt.diam);
	}
	bool filter2(const Byke& data, Byke& filt) {
		return (data.diam < filt.diam);
	}
	bool filter3(const Byke& data, Byke& filt) {
		return (data.diam >= filt.diam);
	}
	bool filter4(const Byke& data, Byke& filt) {
		return (data.diam <= filt.diam);
	}
	bool filter5(const Byke& data, Byke& filt) {
		return (data.mark == filt.mark);
	}

	~Byke() {
		delete[]mark;
	}
	friend std::ostream& operator<<(std::ostream& s, Byke& value);
	friend std::istream& operator>>(std::istream& s, Byke& value);
};

std::ostream& operator<<(std::ostream& s, Byke& value) {
	//if (typeid(s).name() == typeid(std::ofstream).name())
	s << value.mark << " " << value.type << " " << value.brake << " " << value.wheel << " " << value.diam << " " << value.amort << " " << value.ag << std::endl;
	return s;
}
std::istream& operator>>(std::istream& s, Byke& value) {
	s >> value.mark; //>> value.type >> value.brake >> value.wheel >> value.diam >> value.amort >> value.ag;
	return s;
}

/*class List_Byke : List<Byke> {
	List_Byke() :List<Byke>() {}
	List_Byke(Byke* arr, int len) : List<Byke>(arr, len) {}
	virtual ~List_Byke() { std::cout << "\nByke list class destructor"; }
	virtual 
};*/

bool test_filter1(const int& data, const int& filt) {
	return (data == filt);
}
bool test_filter2(const int& data,const int& filt) {
	return (data >= filt);
}

int main()
{
	{
		/*std::cout << "Only object\n";
		List<int> Q;
		for (int i = 0; i < 5; i++){
			Q.push(i);
			delete Q.pop(); 
		}*/
	}
	{
		int arr[5];
		for (int i = 0; i < 5; i++)
			arr[i] = i;
		List<int> Q(arr,5);
		Q.insert(1,11);
		std::cout << Q;
		Q.insert(1, 12);
		std::cout << Q;
		Q.remove(2);
		std::cout << Q;
		bool (*f)(const int&, const int&);
		f = test_filter1;
		List<int>* Q1 = Q.find(f,3);
		std::cout << *Q1;
		delete Q1;
		f = test_filter2;
		Q1 = Q.find(f, 3);
		std::cout << *Q1;
		delete Q1;
		Q.save("out.txt");
		Q.insert(1, 30);
		std::cout << Q;
		Q.load("out.txt");
		std::cout << Q;
	}
	/*//if (true)
	//{
	//	std::cout << "Only object\n";
	//	Queue<int> Q;
	//	for (int i = 0; i < 100; i += 5)
	//		Q.push(i);
	//		for (; !Q.isEmpty(); std::cout << Q.pop()->info << ", ");
	//	std::cout << "\n";
	//}
	//if (true)
	//{
	//	std::cout << "\nBase pointer 1\n";
	//	LinkedList<int>* ptr = new Queue<int>;
	//	for (int i = 1; i < 100; i += 5)
	//		ptr->push(i);
	//	std::cout << *ptr << "\n";
	//	delete ptr;
	//	//добавить виртуальные деструкторы
	//}
	//if (true)
	//{
	//	std::cout << "\nBase pointer 2\n";
	//	LinkedList<int>* ptr; Queue<int>* ptr2 = new
	//		Queue<int>; ptr = ptr2;
	//	for (int i = 1; i < 100; i += 5)
	//		ptr->push(i);
	//	std::cout << *ptr << "\n";
	//	delete ptr;
	//	//добавить виртуальные деструкторы
	//}
	//if (true)
	//{
	//	std::cout << "\nQueue pointer\n";
	//	Queue<int>* ptr = new Queue<int>;
	//	for (int i = 2; i < 100; i += 5)
	//		ptr->push(i);
	//	std::cout << *ptr << "\n";
	//	delete ptr;
	//}
	//if (true)
	//{
	//	std::cout << "\nmy_class Queue test\n";
	//	Queue<my_class> Q;
	//	for (int i = 3; i < 100; i += 5)
	//		Q.push(my_class(i));
	//	std::cout << Q;
	//}return 0;*/


}