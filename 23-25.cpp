#include <iostream>
#include<vector>
#include <fstream>
#include <list>
#include <cmath>
#include <math.h>
#include <string.h>

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
	};
};

class condition_exeption : MyExeption {
public:
	condition_exeption() : MyExeption("conditions not allow to do this") {}
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

class memory_exeption : MyExeption {
public:
	memory_exeption() : MyExeption("no memory allocated") {}
	virtual ~memory_exeption() { print_exeption(); }
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
		day = 0, month = 0, year = 0;
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
	file() {
		domain = "NULL";
		name = "NULL";
		dchange = date(1, 1, 1);
		dlastread = date(1, 1, 1);
		dcreate = date(1,1,1);
	}
	virtual ~file() {}


	//операторы необходимые для работы push и других методов
	bool operator<(const file& a) {
		if (dchange < a.dchange) return true; else if (dchange == a.dchange && dlastread < a.dlastread) return true;
		else if (dchange == a.dchange && dlastread == a.dlastread && path_depth > a.path_depth) return true; else return false;
	}

	bool operator>(const file& a) {
		return !(*this < a) && !(*this == a);
	}
	bool operator==(const file& a) {
		return a.domain == domain && a.name == name && a.dchange == dchange && a.dlastread == dlastread && a.dcreate == a.dcreate;
	}
	bool operator==(const file& a) const {
		return a.domain == domain && a.name == name && a.dchange == dchange && a.dlastread == dlastread && a.dcreate == a.dcreate;
	}

	friend std::ostream& operator<<(std::ostream& out, const file& f) {
		out << "name: " << f.name << " path: " << f.domain << " date of last change: " << f.dchange << " date of last read: " << f.dlastread << " date of creation: " << f.dcreate << std::endl;
		return out;
	}
};

//==========================================================================================================================================================================================================
template <class T>
class Element
{//элемент связного списка
private:
	//указатель на предыдущий и следующий элемент
	Element* next;
	Element* prev;
	//информация, хранимая в поле
	T field;
public:
	Element(T value = NULL, Element<T>* next_ptr = NULL, Element<T>* prev_ptr = NULL)
	{
		field = value;
		next = next_ptr;
		prev = prev_ptr;
	}

	~Element() {
		std::cout << "element " << field << " deleted" << std::endl;
	}

	//доступ к полю *next
	virtual Element* getNext() { return next; }
	virtual void setNext(Element* value) { next = value; }
	//доступ к полю *prev
	virtual Element* getPrevious() { return prev; }
	virtual void setPrevious(Element* value) { prev = value; }
	//доступ к полю с хранимой информацией field
	virtual T getValue() { return field; }
	virtual void setValue(T value) { field = value; }
	friend std::ostream& operator<< (std::ostream& ustream, Element<T>& obj) {
		ustream << obj.field;
		return ustream;
	};
};

template <class T>
class LinkedListParent
{
protected:
	//достаточно хранить начало и конец
	Element<T>* head;
	Element<T>* tail;
	//для удобства храним количество элементов
	int num;

	virtual void erase_all() { //метод очистки памяти
		Element<T>* tmp = head;
		while (tmp->getNext() != NULL) {
			tmp = tmp->getNext();
			delete tmp->getPrevious();
		}
		if (tmp != NULL) delete tmp;
		num = 0;
		head = tail = NULL;
	}

public:
	virtual int Number() { return num; }
	virtual Element<T>* getBegin() { return head; }
	virtual Element<T>* getEnd() { return tail; }
	LinkedListParent()
	{
		//конструктор без параметров
		head = tail = NULL;
		num = 0;
	}

	//чисто виртуальная функция: пока не определимся с типом списка, не сможем реализовать добавление
	virtual Element<T>* push(T value) = 0;

	//чисто виртуальная функция: пока не определимся с типом списка, не сможем реализовать удаление
	virtual Element<T>* pop() = 0;

	virtual ~LinkedListParent()
	{
		if (head != NULL) erase_all(); //деструктор - освобождение памяти

	}
	//получение элемента по индексу
	virtual Element<T>* operator[](int i)
	{
		//индексация
		if (i<0 || i>num) throw(size_exeption());
		int k = 0;
		//ищем i-й элемент - вставем в начало и отсчитываем i шагов вперед
		Element<T>* cur = head;
		for (k = 0; k < i; k++)
		{
			cur = cur->getNext();
		}
		return cur;
	}

	friend std::ostream& operator<< (std::ostream& ustream, LinkedListParent<T>& obj) {
		if (typeid(ustream).name() == typeid(std::ofstream).name())
		{
			ustream << obj.num << std::endl;
			for (Element<T>* current = obj.getBegin(); current != NULL; current = current->getNext())
				ustream << current->getValue() << std::endl;
			return ustream;
		}
		ustream << "\nLength: " << obj.num << std::endl;
		int i = 0;
		for (Element<T>* current = obj.getBegin(); current != NULL; current = current->getNext(), i++)
			ustream << "arr[" << i << "] = " << current->getValue() << std::endl;
		return ustream;
	};
	friend std::istream& operator>> (std::istream& ustream, LinkedListParent<T>& obj) {
		//чтение из файла и консоли совпадают
		int len;
		ustream >> len;
		//очистить память
		obj.erase_all();
		T v = NULL;
		for (int i = 0; i < len; i++)
		{
			ustream >> v;
			obj.push(v);
		}
		return ustream;
	};
};


template<typename ValueType>
class ListIterator : public std::iterator<std::input_iterator_tag, ValueType>
{
private:
	Element<ValueType>* ptr;
public:

	ListIterator(Element<ValueType>* p = NULL) { ptr = p; }
	ListIterator(const ListIterator& it) { ptr = it.ptr; }

	bool operator!=(ListIterator const& other) const {
		return ptr != other.ptr;
	}
	bool operator==(ListIterator const& other) const {
		return ptr == other.ptr;
	}

	Element<ValueType>& operator*()							//чтение элемента по итератору
	{
		if (ptr == NULL) throw(nullptr_exeption());
		return *ptr;
	}
	Element<ValueType>& operator*() const
	{
		if (ptr == NULL) throw(nullptr_exeption());
		return *ptr;
	}
	ListIterator& operator++() {							//операторы для перемещения по списку
		if (ptr == NULL) throw(nullptr_exeption());
		ptr = ptr->getNext();
		return *this;
	}
	ListIterator& operator++(int v) {
		if (ptr == NULL) throw(nullptr_exeption());
		ptr = ptr->getNext();
		return *this;
	}
	ListIterator& operator--() {
		if (ptr == NULL) throw(nullptr_exeption());
		ptr = ptr->getPrevious();
		return *this;
	}
	ListIterator& operator--(int v) {
		if (ptr == NULL) throw(nullptr_exeption());
		ptr = ptr->getPrevious();
		return *this;
	}
	ListIterator& operator=(const ListIterator& it) {
		ptr = it.ptr;
		return *this;
	}
	ListIterator& operator=(Element<ValueType>* p) {
		ptr = p;
		return *this;
	}
};

template <class T>
class IteratedLinkedList : public LinkedListParent<T>
{
public:
	IteratedLinkedList() : LinkedListParent<T>() {}

	virtual ~IteratedLinkedList() {
	}

	virtual IteratedLinkedList<T>& operator=(IteratedLinkedList<T>& lst) {
		this->erase_all();
		for (ListIterator<T> iter = lst.begin(); iter != NULL; iter++) {
			this->push((*iter).getValue());
		}
		return *this;
	}
	virtual ListIterator<T> begin() {
		return ListIterator<T>(LinkedListParent<T>::head);
	}
	virtual ListIterator<T> end() {
		return ListIterator<T>(LinkedListParent<T>::tail);
	}

	friend std::ostream& operator<< (std::ostream& ustream, IteratedLinkedList<T>& obj) {			//вывод в поток через итераторы
		if (typeid(ustream).name() == typeid(std::ofstream).name())
		{
			ustream << obj.num << std::endl;
			for (ListIterator<T> iter = obj.begin(); iter != NULL; iter++)
			{
				ustream << *iter << std::endl;
			}
			return ustream;
		}
		ustream << "\nLength: " << obj.num << std::endl;
		int i = 0;
		for (ListIterator<T> iter = obj.begin(); iter != NULL; iter++, i++)
			ustream << "arr[" << i << "] = " << *iter << std::endl;
		return ustream;
	};
};

template<class T>
class IteratedQueue : public IteratedLinkedList<T> {										//класс очереди
public:
	IteratedQueue() : IteratedLinkedList<T>() {}

	virtual ~IteratedQueue() {}

	virtual Element<T>* push(T value) {														//метод добавления элемента в конец списка
		Element<T>* tmp;
		if (!(tmp = new Element<T>(value, NULL, this->tail))) throw(memory_exeption());
		this->num++;
		if (this->head == NULL) {
			this->head = tmp;
			this->tail = tmp;
		}
		else {

			this->tail->setNext(tmp);
			this->tail = tmp;
		}
		return tmp;

	};
	virtual Element<T>* pop() {																//метод удаления элемента с конца списка.
		if (this->head == NULL) throw(nullptr_exeption());
		this->num--;
		Element<T>* tmp = this->head;
		if (this->head == this->tail) {
			this->head = this->tail = NULL;
		}
		else {
			tmp->getNext()->setPrevious(NULL);
			this->head = tmp->getNext();
			tmp->setNext(NULL);
		}

		return tmp;
	};

	virtual Element<T>* insert(T value, int pos) {											//метод создания элемента в произврльном месте списка
		if (pos < 0 || pos > this->num + 2) throw(size_exeption()); else
			if (pos == this->num) return push(value); else {
				this->num++;
				Element<T>* cur = this->operator[](pos);
				Element<T>* tmp;
				if (!(tmp = new Element<T>(value, cur, cur->getPrevious()))) throw(memory_exeption());
				if (pos == 0) {
					this->head = tmp;
				}
				else {
					tmp->setPrevious(cur->getPrevious());
					tmp->getPrevious()->setNext(tmp);
				}
				cur->setPrevious(tmp);
				return tmp;
			}

	}
	virtual Element<T>* insert(Element<T>* pos, T value) {									//метод создания элемента в произврльном месте списка 
		if (pos == NULL) return IteratedQueue<T>::push(value);
		Element<T>* tmp;
		if (!(tmp = new Element<T>(value, pos))) throw(memory_exeption());
		if (pos->getPrevious() != NULL) {
			pos->getPrevious()->setNext(tmp);
			tmp->setPrevious(pos->getPrevious());
		}
		else {
			this->head = tmp;
		}
		pos->setPrevious(tmp);
		this->num++;
		return tmp;

	}

	virtual Element<T>* push_ordered(T value) {												//добавление элементов по убыванию
		Element<T>* tmp;
		if (!(tmp = new Element<T>(value))) throw(memory_exeption());
		if (this->head == NULL) {
			this->head = tmp;
			this->tail = tmp;
		}
		else {
			ListIterator<T> iter = this->begin();
			while (iter != NULL) {
				if ((*iter).getValue() < value) { break; }
				iter++;
			}
			if (iter != NULL)
				this->insert(&*iter, value);
			else
				IteratedQueue<T>::push(value);
		}
		return tmp;

	};


	virtual Element<T>* deleteat(int pos) {													//удаление элемента на определенной позиции (через итератор)
		if (pos < 0 || this->num < pos) throw size_exeption();
		this->num--;
		ListIterator<T> iter = this->begin();
		for (int i = 0; i < pos; i++, iter++);
		if ((*iter).getNext() != NULL)
			(*iter).getNext()->setPrevious((*iter).getPrevious());
		else
			this->tail = (*iter).getPrevious();
		if ((*iter).getPrevious() != NULL)
			(*iter).getPrevious()->setNext((*iter).getNext());
		else
			this->head = (*iter).getNext();
		(*iter).setNext(NULL); (*iter).setPrevious(NULL);
		return &*iter;
	}
};

template<class T>
class SortedQueue : public IteratedQueue<T> {												//отсортированный по убыванию список
public:
	SortedQueue() : IteratedQueue<T>() {
	}
	virtual ~SortedQueue() {}

	virtual Element<T>* push(T value) {														//добавление элеменьа по убыванию
		Element<T>* tmp;
		if (!(tmp = new Element<T>(value))) throw(memory_exeption());
		if (this->head == NULL) {
			this->head = tmp;
			this->tail = tmp;
		}
		else {
			ListIterator<T> iter = this->begin();
			while (iter != NULL) {
				if ((*iter).getValue() < value) { break; }
				iter++;
			}
			if (iter != NULL)
				this->insert(&*iter, value);
			else
				IteratedQueue<T>::push(value);
		}
		return tmp;

	};
};

template <class T>
IteratedQueue<T>* filter(IteratedQueue<T>* lst, bool (*f)(T)) {									//универсальный фильтр
	if (f == NULL || lst == NULL) throw(nullptr_exeption());
	IteratedQueue<T>* tmp = new IteratedQueue<T>;
	for (ListIterator<T> iter = lst->begin(); iter != NULL; iter++) {
		if (f((*iter).getValue()))
			tmp->push((*iter).getValue());
	}
	return tmp;
}

template <class T>
IteratedQueue<T>* filter(IteratedLinkedList<T>* lst, bool (*f)(T)) {							//универсальный фильтр
	if (f == NULL || lst == NULL) throw(nullptr_exeption());
	IteratedQueue<T>* tmp = new IteratedQueue<T>;
	for (ListIterator<T> iter = lst->begin(); iter != NULL; iter++) {
		if (f((*iter).getValue()))
			tmp->push((*iter).getValue());
	}
	return tmp;
}
//==========================================================================================================================================================================================================

enum node_type {
	SINGLE = 0,
	MULTI
};

//узел
template<class K, class T>
class Node
{
protected:
	T data;
	SortedQueue<T> mdata;
	node_type type;
	//не можем хранить Node, но имеем право хранить указатель
	Node* left;
	Node* right;
	Node* parent;
	int height;
	K key;
public:
	//доступные извне переменные и функции
	virtual void setData(T d) { if(type == SINGLE)  data = d; else mdata.push(d); }
	virtual T getData() { return (type == SINGLE) ? data : (*mdata.begin()).getValue(); }
	virtual SortedQueue<T>& getMData() { if (type == MULTI) return mdata; else throw condition_exeption(); }
	virtual void setMData(SortedQueue<T>& d) { if (type == MULTI) mdata = d; else throw condition_exeption(); }
	virtual void setKey(K k) { key = k; }
	virtual K getKey() { return key; }
	int getHeight() { return height; }
	virtual Node* getLeft() { return left; }
	virtual Node* getRight() { return right; }
	virtual Node* getParent() { return parent; }
	virtual void setLeft(Node* N) { left = N; }
	virtual void setRight(Node* N) { right = N; }
	virtual void setParent(Node* N) { parent = N; }
	//Конструктор. Устанавливаем стартовые значения для указателей
	Node<K,T>(K key, T n, node_type type = SINGLE)
	{
		this->type = type;
		if (type == SINGLE) data = n; else mdata.push(n);
		left = right = parent = NULL;
		height = 1;
		this->key = key;
	}
	Node<K,T>(node_type type = SINGLE)
	{
		this->type = type;
		left = NULL;
		right = NULL;
		parent = NULL;
		data = T();
		key = K();
		height = 1;
	}
	virtual void print()
	{
		std::cout << "\n" << data;
	}
	virtual void setHeight(int h)
	{
		height = h;
	}

	virtual Node<K,T>* pred() {
		Node<K,T>* Current = this;
		if (left != NULL)
		{
			for (Current = left; Current->getRight() != NULL; Current = Current->getRight());
			return Current;
		}
		else
		{

			K value = Current->getKey();
			if (Current->getParent() != NULL && Current->getParent()->getKey() < value)
			{
				return Current->getParent();
			}
			while (Current->getParent() != NULL) {
				Current = Current->getParent();
				if (Current->getKey() < value) { return Current; }
				
			}
			return NULL;//дошли до корня, не найдя следующий элемент

		}
	}
	virtual Node<K,T>* sucs() {
		Node<K,T>* Current = this;
		if (right != NULL)
		{
			for (Current = right; Current->getLeft() != NULL; Current = Current->getLeft());
			return Current;
		}
		else
		{

			K val = Current->getKey();
			if (Current->getParent() != NULL && Current->getParent()->getKey() > val)
			{
				return Current->getParent();
			}
			K value = Current->getKey();
			while (Current->getParent() != NULL) {
				Current = Current->getParent();
				if (Current->getKey() > value) { return Current; }
			}
			return NULL;//дошли до корня, не найдя следующий элемент

		}
	}

	//template<class K, class T> 
	friend std::ostream& operator<< (std::ostream& stream, Node<K,T>& N){
		if (&N == NULL) throw nullptr_exeption();
		if(N.type == SINGLE)
			stream << "\nNode Key: " << N.key  << ", data: " << N.data << ", height:" << N.height;
		else {
			stream << "\nNode Key: " << N.key << ", data: ";
			for (ListIterator<T> iter = N.getMData().begin(); iter!= NULL; iter++)
				stream << *iter << ", ";
			stream << "height:" << N.height;
		}
		return stream;
	}
};


template<class K, class T>
void print(Node<K,T>* N) {
	std::cout << "\n" << N->getKey() << " " << N->getData() << " " << N->getHeight();
}

template<class K, class T>
bool ecv(Node<K, T>* N) {
	return N->getData() == T();
}

template<class K, class T>
bool ecv2(Node<K, T>* N, Node<K, T>& keyvalue) {
	return N->getKey() == keyvalue.getKey() && N->getData() == keyvalue.getData();
}

template<class K, class T>
class treeiterator : public std::iterator<std::input_iterator_tag, T> {
protected:
	Node<K,T>* ptr;
public:
	treeiterator(Node<K,T>* p = NULL) {
		ptr = p;
	}
	treeiterator(const treeiterator<K, T>& p) {
		ptr = p.ptr;
	}
	Node<K,T>* operator=(const treeiterator<K, T>& a) { return ptr = a.ptr; }
	Node<K,T>* operator=(Node<K,T>* p) { return ptr = p; }
	Node<K,T>* operator*() { return ptr; }
	treeiterator<K, T>& operator++() { (ptr == NULL) ? throw nullptr_exeption() : ptr = ptr->sucs(); return *this; }
	treeiterator<K, T>& operator++(int k) { (ptr == NULL) ? throw nullptr_exeption() : ptr = ptr->sucs(); return *this; }
	treeiterator<K, T>& operator--() { (ptr == NULL) ? throw nullptr_exeption() : ptr = ptr->pred(); return *this; }
	treeiterator<K, T>& operator--(int k) { (ptr == NULL) ? throw nullptr_exeption() : ptr = ptr->pred(); return *this; }
	bool operator==(treeiterator<K, T>& p) { return ptr == p.ptr; }
	bool operator!=(treeiterator<K, T>& p) { return ptr != p.ptr; }
};

template<class K, class T>
class Tree
{
protected:
	//корень - его достаточно для хранения всего дерева
	Node<K,T>* root;
public:
	//доступ к корневому элементу
	virtual Node<K,T>* getRoot() { return root; }
	//конструктор дерева: в момент создания дерева ни одного узла нет, корень смотрит в никуда
	Tree<K, T>() { root = NULL; }
	//рекуррентная функция добавления узла. Устроена аналогично, но вызывает сама себя - добавление в левое или правое поддерево
	virtual Node<K,T>* Add_R(Node<K,T>* N)
	{
		return Add_R(N, root);
	}
	virtual Node<K,T>* Add_R(Node<K,T>* N, Node<K,T>* Current)
	{
		if (N == NULL) return NULL;
		if (root == NULL)
		{
			root = N;
			return N;
		}
		if (Current->getKey() > N->getKey())
		{
			//идем влево
			if (Current->getLeft() != NULL)
				Current->setLeft(Add_R(N, Current->getLeft()));
			else
				Current->setLeft(N);
			Current->getLeft()->setParent(Current);
		}
		if (Current->getKey() < N->getKey())
		{
			//идем вправо
			if (Current->getRight() != NULL)
				Current->setRight(Add_R(N, Current->getRight()));
			else
				Current->setRight(N);
			Current->getRight()->setParent(Current);
		}
		if (Current->getKey() == N->getKey()) {
			Current->setData(N->getData());
		}												
			//нашли совпадение
		//для несбалансированного дерева поиска
		return Current;
	}
	//функция для добавления числа. Делаем новый узел с
	virtual Node<K,T>* Add(K k, T n)
	{
		Node<K, T>* N = new Node<K, T>;
		N->setData(n);
		N->setKey(k);
		return Add_R(N);
	}
	virtual Node<K, T>* Min(Node<K, T>* Current = NULL)
	{
		//минимум - это самый "левый" узел. Идём по дереву всегда влево
		if (root == NULL) return NULL;
		if (Current == NULL)
			Current = root;
		while (Current->getLeft() != NULL)
			Current = Current->getLeft();
		return Current;
	}
	virtual Node<K, T>* Max(Node<K, T>* Current = NULL)
	{
		//минимум - это самый "правый" узел. Идём по дереву всегда вправо
		if (root == NULL) return NULL;
		if (Current == NULL)
			Current = root;
		while (Current->getRight() != NULL)
			Current = Current->getRight();
		return Current;
	}
	//поиск узла в дереве. Второй параметр - в каком поддереве искать, первый - что искать
	virtual Node<K, T>* Find(K Key, Node<K, T>* Current)
	{
		//база рекурсии
		if (Current == NULL) return NULL;
		if (Current->getKey() == Key) return Current;
		//рекурсивный вызов
		if (Current->getKey() > Key) return Find(Key, Current->getLeft());
		if (Current->getKey() < Key) return Find(Key, Current->getRight());
	}
	virtual std::vector<Node<K,T>*> FindV(T data) {
		std::vector<Node<K, T>*> tmp;
		for (treeiterator<K, T> iter(begin()); *iter != NULL; iter++) {
			if ((*iter)->getData() == data) tmp.push_back(*iter);
		}
		return tmp;
	}
	virtual std::vector<Node<K, T>*> Find(bool (*f)(Node<K,T>*), Node<K, T>* Current)
	{
		std::vector<Node<K, T>*> tmp;
		for (treeiterator<K, T> iter(begin()); *iter != NULL; iter++) {
			if (f(*iter)) tmp.push_back(*iter);
		}
		return tmp;
	}
	//три обхода дерева
	virtual void PreOrder(Node<K, T>* N, void (*f)(Node<K, T>*))
	{
		if (N != NULL)
			f(N);
		if (N != NULL && N->getLeft() != NULL)
			PreOrder(N->getLeft(), f);
		if (N != NULL && N->getRight() != NULL)
			PreOrder(N->getRight(), f);
	}
	//InOrder-обход даст отсортированную
	virtual void InOrder(Node<K, T>* N, void
	(*f)(Node<K, T>*))
	{
		if (N != NULL && N->getLeft() != NULL)
			InOrder(N->getLeft(), f);
		if (N != NULL)
			f(N);
		if (N != NULL && N->getRight() != NULL)
			InOrder(N->getRight(), f);
	}
	virtual void PostOrder(Node<K, T>* N, void
	(*f)(Node<K, T>*))
	{
		if (N != NULL && N->getLeft() != NULL)
			PostOrder(N->getLeft(), f);
		if (N != NULL && N->getRight() != NULL)
			PostOrder(N->getRight(), f);
		if (N != NULL)
			f(N);
	}
	virtual int getheight(Node<K, T>* n) {
		if (n == NULL) return 0;
		return std::max({ n->getHeight(), getheight(n->getLeft()),getheight(n->getRight()) });
	}

	virtual treeiterator<K, T> begin() {
		treeiterator<K, T> tmp(Min());
		return tmp;
	}
	virtual treeiterator<K, T> end() {
		treeiterator<K, T> tmp(Max());
		return tmp;
	}

	//template<class K, class T>
	friend std::ostream& operator<<(std::ostream& stm, Tree<K, T>& tree) {
		
		for (treeiterator<K, T> iter(tree.begin());*iter != NULL;iter++) {
			stm << **iter << " ";
		}
		return stm << std::endl;
	}

};

template<class K, class T>
class balanced_tree : public Tree<K, T> {
protected:
	//определение разности высот двух поддеревьев
	int bfactor(Node<K, T>* p)
	{
		int hl = 0;
		int hr = 0;
		if (p->getLeft() != NULL)
			hl = p->getLeft()->getHeight();
		if (p->getRight() != NULL)
			hr = p->getRight()->getHeight();
		return (hr - hl);
	}

	//при добавлении узлов в них нет информации о балансе, т.к. не ясно, куда в дереве они попадут
	//после добавления узла рассчитываем его высоту (расстояние до корня) и редактируем высоты в узлах, где это
	//значение могло поменяться
	void fixHeight(Node<K, T>* p)
	{
		int hl = 0;
		int hr = 0;
		if (p->getLeft() != NULL)
			hl = p->getLeft()->getHeight();
		if (p->getRight() != NULL)
			hr = p->getRight()->getHeight();
		p->setHeight((hl > hr ? hl : hr) + 1);
	}

	//краеугольные камни АВЛ-деревьев - процедуры поворотов
	Node<K, T>* RotateRight(Node<K, T>* p) // правый поворот вокруг p
	{
		Node<K, T>* q = p->getLeft();

		p->setLeft(q->getRight());
		q->setRight(p);

		if (p == Tree<K, T>::root) {
			Tree<K, T> ::root = q;
		}
		else {
			if (p->getParent()->getLeft() == p) {
				p->getParent()->setLeft(q);
			}
			else {
				p->getParent()->setRight(q);
			}
		}

		q->setParent(p->getParent());
		p->setParent(q);
		if (p->getLeft() != NULL) { p->getLeft()->setParent(p); }

		fixHeight(p);
		fixHeight(q);
		return q;
	}

	Node<K, T>* RotateLeft(Node<K, T>* q) // левый поворот вокруг q
	{
		Node<K, T>* p = q->getRight();

		q->setRight(p->getLeft());
		p->setLeft(q);

		if (q == Tree<K, T>::root) {
			Tree<K, T> ::root = p;
		}
		else {
			if (q->getParent()->getLeft() == q) {
				q->getParent()->setLeft(p);
			}
			else {
				q->getParent()->setRight(p);
			}
		}

		p->setParent(q->getParent());
		q->setParent(p);
		if (q->getRight() != NULL) q->getRight()->setParent(q);

		fixHeight(q);
		fixHeight(p);
		return p;
	}

	//балансировка поддерева узла p - вызов нужных поворотов в зависимости от показателя баланса
	Node<K, T>* Balance(Node<K, T>* p) // балансировка узла p
	{
		fixHeight(p);
		if (bfactor(p) == 2)
		{
			if (bfactor(p->getRight()) < 0) RotateRight(p->getRight());
			return RotateLeft(p);
		}
		if (bfactor(p) == -2)
		{
			if (bfactor(p->getLeft()) > 0) RotateLeft(p->getLeft());
			return RotateRight(p);
		}

		return p; // балансировка не нужна
	}
	//меняют местами узлы с соседними
	void swap_R(Node<K, T>* n) {

		if (n == NULL) throw nullptr_exeption();
		Node<K, T>* right = n->getRight();

		if (right == NULL) throw nullptr_exeption();
		Node<K, T>* tmp1 = n->getLeft();
		Node<K, T>* tmp2 = n->getParent();
		Node<K, T>* tmp3 = right->getLeft();
		Node<K, T>* tmp4 = right->getRight();

		int tmp5 = n->getHeight();
		n->setHeight(right->getHeight());
		right->setHeight(tmp5);

		if (n == this->root) {
			this->root = right;
			right->setParent(NULL);
		}
		else {
			(n == tmp2->getLeft()) ? tmp2->setLeft(right) : tmp2->setRight(right);
			right->setParent(tmp2); 
		}
		n->setParent(right);
		right->setRight(n);
		if(tmp1!=NULL) tmp1->setParent(right);
		right->setLeft(tmp1);
		n->setLeft(tmp3);
		if (tmp3 != NULL) tmp3->setParent(n);
		if (tmp4 != NULL) tmp4->setParent(n);
		n->setRight(tmp4);
		
	}
	void swap_L(Node<K, T>* n) {

		if (n == NULL) throw nullptr_exeption();
		Node<K, T>* left = n->getLeft();

		if (left == NULL) throw nullptr_exeption();
		Node<K, T>* tmp1 = n->getRight();
		Node<K, T>* tmp2 = n->getParent();
		Node<K, T>* tmp3 = left->getLeft();
		Node<K, T>* tmp4 = left->getRight();

		if (n == this->root) {
			this->root = left;
			left->setParent(NULL);
		}
		else {
			(n == tmp2->getLeft()) ? tmp2->setLeft(left) : tmp2->setRight(left);
			left->setParent(tmp2);
		}
		n->setParent(left);
		left->setLeft(n);
		if (tmp1 != NULL) tmp1->setParent(left);
		left->setRight(tmp1);
		n->setLeft(tmp3);
		if (tmp3 != NULL) tmp3->setParent(n);
		if (tmp4 != NULL) tmp4->setParent(n);
		n->setRight(tmp4);
	}

public:
	balanced_tree() : Tree<K, T>() {}
	virtual ~balanced_tree() {}


	virtual Node<K, T>* Add_R(Node<K, T>* N)
	{
		return Add_R(N, Tree<K, T>::root);
	}

	//рекуррентная функция добавления узла. Устроена аналогично, но вызывает сама себя - добавление в левое или правое поддерево
	virtual Node<K, T>* Add_R(Node<K, T>* N, Node<K, T>* Current)
	{
		//вызываем функцию push_R из базового класса
		Node<K, T>* pushedNode = Tree<K, T>::Add_R(N, Current);
		//применяем к добавленному узлу балансировку
		if (Current != NULL)
			return Balance(Current);
		return pushedNode;
	}

	//функция для добавления числа. Делаем новый узел с этими данными и вызываем нужную функцию добавления в дерево
	virtual Node<K, T>* Add(K k,T n)
	{
		Node<K, T>* N = new Node<K, T>;
		N->setData(n);
		N->setKey(k);
		return Add_R(N);
	}

	//удаление узла
	virtual void Remove(Node<K, T>* N) {
		if (N == NULL) throw nullptr_exeption();
		Node<K, T>* left = N->getLeft();
		Node<K, T>* right = N->getRight();

		if (left == NULL && right == NULL) {
			if (N == this->root) {
				this->root = NULL;
			}
			else {
				(N->getParent()->getLeft() == N) ? N->getParent()->setLeft(NULL) : N->getParent()->setRight(NULL);
			}
			delete N;
			return;
		}
		if (left != NULL && right == NULL) {
			if (N == this->root) {
				this->root = left;
			}
			else {
				(N->getParent()->getLeft() == N) ? N->getParent()->setLeft(left) : N->getParent()->setRight(left);
				left->setParent(N->getParent());
			}
			delete N;
			return;
		}
		if (left == NULL && right != NULL) {
			if (N == this->root) {
				this->root = right;
			}
			else {
				(N->getParent()->getLeft() == N) ? N->getParent()->setLeft(right) : N->getParent()->setRight(right);
				right->setParent(N->getParent());
			}
			delete N;
			return;
		}
		if (right->getHeight() >= left->getHeight()) {
			swap_R(N);
		}
		else {
			swap_L(N);
		}
		Remove(N);
	}

	//удаление узла 2
	virtual void Remove_(Node<K, T>* N) {
		if (N == NULL) throw nullptr_exeption();
		Node<K, T>* left = N->getLeft();
		Node<K, T>* right = N->getRight();

		if (left == NULL && right == NULL) {
			if (N == this->root) {
				this->root = NULL;
			}
			else {
				(N->getParent()->getLeft() == N) ? N->getParent()->setLeft(NULL) : N->getParent()->setRight(NULL);
			}
			delete N;
			return;
		}
		if (left != NULL && right == NULL) {
			if (N == this->root) {
				this->root = left;
			}
			else {
				(N->getParent()->getLeft() == N) ? N->getParent()->setLeft(left) : N->getParent()->setRight(left);
				left->setParent(N->getParent());
			}
			delete N;
			return;
		}
		if (left == NULL && right != NULL) {
			if (N == this->root) {
				this->root = right;
			}
			else {
				(N->getParent()->getLeft() == N) ? N->getParent()->setLeft(right) : N->getParent()->setRight(right);
				right->setParent(N->getParent());
			}
			delete N;
			return;
		}
		if (right->getHeight() >= left->getHeight()) {
			Node<K, T>* sucs = N->sucs();
			N->setKey(sucs->getKey());
			N->setData(sucs->getData());
			Remove_(sucs);
		}
		else {
			Node<K, T>* pred = N->pred();
			N->setKey(pred->getKey());
			N->setData(pred->getData());
			Remove_(pred);
		}
		fixHeight(this->root);
	}
	T operator[](K key) {
		return this->Find(key,this->root)->getData();
	}
};


template<class K, class T>
class multi_tree : public balanced_tree<K,T>{
protected:
	
public:
	multi_tree() : balanced_tree<K,T>(){}
	~multi_tree(){}
	virtual Node<K, T>* Add(K k, T n)
	{
		Node<K, T>* N = new Node<K, T>(MULTI);
		N->setData(n);
		N->setKey(k);
		return this->Add_R(N);
	}
	SortedQueue<T>& operator[](K key) {
		return this->Find(key,this->getRoot())->getMData();
	}
};

template<class K, class T>
class splay_tree : public balanced_tree<K,T> {
protected:
	Node<K, T>* splay(Node<K, T>* n) {
		if (n == NULL) return NULL;
		Node<K, T>* prnt;
		if (!(prnt = n->getParent())) { this->root = n; return n; }
		Node<K, T>* grprnt;
		if (!(grprnt = prnt->getParent())) {
			if (n == prnt->getLeft())
				balanced_tree<K, T>::RotateRight(prnt);
			else
				balanced_tree<K, T>::RotateLeft(prnt);
			this->root = n;
			return n;
		}
		if (grprnt->getLeft() == prnt) {
			if (prnt->getLeft() == n) {
				balanced_tree<K, T>::RotateRight(grprnt);
				balanced_tree<K, T>::RotateRight(prnt);
			}
			else {
				balanced_tree<K, T>::RotateLeft(prnt);
				balanced_tree<K, T>::RotateRight(grprnt);
			}
		}
		else {
			if (prnt->getLeft() == n) {
				balanced_tree<K, T>::RotateRight(prnt);
				balanced_tree<K, T>::RotateLeft(grprnt);
			}
			else {
				balanced_tree<K, T>::RotateLeft(grprnt);
				balanced_tree<K, T>::RotateLeft(prnt);
			}
		}
		
		return splay(n);
	}
public:
	splay_tree() : balanced_tree<K, T>() {}
	~splay_tree() {}
	virtual Node<K, T>* Find(K Key, Node<K,T>* n) {
		Node<K, T>* N = Tree<K, T>::Find(Key, n);
		if (N == NULL) return NULL;
		return splay(N);
	}

	virtual Node<K, T>* Add(K k, T n)
	{
		Node<K, T>* N = new Node<K, T>;
		N->setData(n);
		N->setKey(k);
		Tree<K,T>::Add_R(N);
		return splay(N);
	}

	virtual Node<K,T>* Find(Node<K,T> keyvalue, bool (*f)(Node<K, T>*, Node<K, T>&), Node<K, T>* Current) {
		if (f == NULL) return Find(keyvalue, [](Node<K, T>* a, Node<K, T>& b)->bool {return a->getData() == b.getData() && a->getKey() == b.getKey(); }, Current);
		Node<K, T>* answer = NULL;
		for (treeiterator<K, T> iter = this->begin(); *iter != NULL; iter++) {
			if (f(*iter, keyvalue)) {
				answer = *iter;
				break;
			}
		}
		splay(answer);
		return answer;
	}

};
int main()
{
	
	balanced_tree<double,int> T;
	int arr[15];
	int i = 0;
	for (i = 0; i < 15; i++) arr[i] = (int)(100 *
		cos(15 * double(i + 1)));
	for (i = 0; i < 15; i++)
		T.Add(arr[i],i);
	Node<double,int>* M = T.Min();
	std::cout << "\nMin = " << M->getData() << "\tFind " << arr[3] << ": " << T.Find(arr[3], T.getRoot());
	void (*f_ptr)(Node<double,int>*); f_ptr = print;
	std::cout << "\n-----\nInorder:";
	T.InOrder(T.getRoot(), f_ptr);

	int k = 2;
	std::cout <<std::endl << *((T.Find(arr[k], T.getRoot()))->pred()) << std::endl << arr[k] << std::endl << *((T.Find(arr[k], T.getRoot()))->sucs());

	std::cout <<std::endl << T << *T.getRoot();

	std::cout << "\n==========================================================================================\n";

	try {
		file fff("d:/s", "fff", { 1,1,1 }, { 1,1,2 }, { 1,1,1 });
		file rrr("d:", "rrr", { 1,1,1 }, { 1,1,1 }, { 1,1,1 });
		file mmm("d:/s/a/", "mmm", { 1,1,1 }, { 1,1,1 }, { 1,1,1 });
		file ggg("d:/aaa/", "ggg", { 1,1,1 }, { 1,1,1 }, { 1,1,1 });
		file ddd("d:/", "ddd", { 1,1,1 }, { 1,1,1 }, { 1,1,1 });
		file ttt("d:/", "ttt", { 1,1,1 }, { 2,1,1 }, { 1,1,1 });
		file hhh("d:/assr/", "hhh", { 5,3,2 }, { 1,1,2 }, { 1,1,2 });
		file aaa("d:/4/ras13d/", "aaa", { 1,4,2 }, { 1,1,2 }, { 1,1,2 });
		Tree<std::string, file> tree;
		tree.Add("fff", fff);
		tree.Add("rrr", rrr);
		tree.Add("mmm", mmm);
		tree.Add("ggg", ggg);
		tree.Add("hhh", hhh);
		tree.Add("aaa", aaa);
		tree.Add("ttt", ttt);
		tree.Add("ddd", ddd);
		void (*f_ptrr)(Node<std::string, file>*); f_ptrr = print;
		std::cout << "\n-----\nInorder:";
		tree.InOrder(tree.getRoot(), f_ptrr);

		std::cout << std::endl << tree;
	}
	catch (...) {}


	std::cout << "\n==========================================================================================\n";

	try {
		balanced_tree<int, int> tree;
		for (int i = -20; i <= 20; i++) {
			tree.Add(i, i);
		}
		tree.Add(101, 20);
		std::cout << tree;
		std::cout << std::endl << tree.getRoot()->getHeight() << "\t" << tree.getRoot()->getLeft()->getHeight() << " " << tree.getRoot()->getRight()->getHeight() << std::endl;
		for (auto i : tree.FindV(20)) {
			std::cout << *i << " ";
		}

	}catch(...){}
	std::cout << "\n==========================================================================================\n";
	try {
		balanced_tree<int, int> tree;
		for (int i = -20; i <= 20; i++) {
			tree.Add(i, i);
		}
		std::cout << tree << std::endl;

		tree.Remove_(tree.Find(4,tree.getRoot()));
		tree.Remove_(tree.Find(10, tree.getRoot()));
		tree.Remove_(tree.Find(-16, tree.getRoot()));
		tree.Remove_(tree.Find(13, tree.getRoot()));

		std::cout << tree;
	}catch(...){}

	std::cout << "\n==========================================================================================\n";

	try {
		balanced_tree<int, int> tree;
		for (int i = -10; i <= 10; i++) {
			tree.Add(i, i % 2);
		}
		for (auto i : tree.Find(ecv,tree.getRoot())) {
			std::cout << *i << "\t";
		}
	} catch(...){}

	std::cout << "\n==========================================================================================\n";

	try {
		multi_tree<int, int> map;
		map.Add(1, 1);
		map.Add(2, 2);
		map.Add(3, 3);
		map.Add(3, 4);
		map.Add(3, 5);
		map.Add(4, 6);
		map.Add(4, 7);
		map.Add(5, 8);
		map.Add(5, 9);

		std::cout << map << std::endl << map[3];
	}catch(...){}

	std::cout << "\n==========================================================================================\n";

	try {
		file fff("d:/s", "fff", { 1,1,1 }, { 1,1,2 }, { 1,1,1 });
		file rrr("d:", "rrr", { 1,1,1 }, { 1,1,1 }, { 1,1,1 });
		file mmm("d:/s/a/", "mmm", { 1,1,1 }, { 1,1,1 }, { 1,1,1 });
		file ggg("d:/aaa/", "ggg", { 1,1,1 }, { 1,1,1 }, { 1,1,1 });
		file ddd("d:/", "ddd", { 1,1,1 }, { 1,1,1 }, { 1,1,1 });
		file ttt("d:/", "ttt", { 1,1,1 }, { 2,1,1 }, { 1,1,1 });
		file hhh("d:/assr/", "hhh", { 5,3,2 }, { 1,1,2 }, { 1,1,2 });
		file aaa("d:/4/ras13d/", "aaa", { 1,4,2 }, { 1,1,2 }, { 1,1,2 });
		multi_tree<std::string, file> tree;
		tree.Add("1", fff);
		tree.Add("2", rrr);
		tree.Add("1", mmm);
		tree.Add("1", ggg);
		tree.Add("2", hhh);
		tree.Add("2", aaa);
		tree.Add("1", ttt);
		tree.Add("3", ddd);

		std::cout << tree << std::endl << tree["2"];

	}
	catch (...) {};
	std::cout << "\n==========================================================================================\n";
	try {
		splay_tree<int, int> splay;
		for (int i = 0; i < 10; i++) {
			splay.Add(i, i);
			//std::cout << splay << std::endl;
		}
		std::cout << *splay.Find(4, splay.getRoot()) << std::endl;
		std::cout << *splay.Find(3, splay.getRoot()) << std::endl;
		std::cout << *splay.Find(1, splay.getRoot()) << std::endl;
		std::cout << *splay.Find(7, splay.getRoot()) << std::endl;
		std::cout << *splay.Find(5, splay.getRoot()) << std::endl;
		std::cout << splay[3] << std::endl;
		std::cout << splay[1] << std::endl;
		std::cout << splay[7] << std::endl;


		std::cout << splay << std::endl << *splay.getRoot() << std::endl;

		std::cout << *splay.Find(Node<int,int>(2,2),NULL,splay.getRoot());
	}
	catch (...) {}
	return 0;
}