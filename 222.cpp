#include <fstream>
#include "basic.h"

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

	virtual void erase_all() {
		Element<T>* tmp = head;
		while (tmp->getNext() != NULL) {
			tmp = tmp->getNext();
			delete tmp->getPrevious();
		}
		if(tmp != NULL) delete tmp;
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
		if(head != NULL) erase_all(); //деструктор - освобождение памяти
		
	}
	//получение элемента по индексу - какова асимптотическая оценка этого действия ? О(n)
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
	//ListIterator() { ptr = NULL; }
	//ListIterator(ValueType* p = NULL) { ptr = p; }

	ListIterator(Element<ValueType>* p = NULL) { ptr = p; }
	ListIterator(const ListIterator& it) { ptr = it.ptr; }

	bool operator!=(ListIterator const& other) const {
		return ptr != other.ptr;
	}
	bool operator==(ListIterator const& other) const {
		return ptr == other.ptr;
	}//need for BOOST_FOREACH
	Element<ValueType>& operator*()
	{																	//if (ptr == NULL) throw(nullptr_exeption()); ломает прогу. почему не знаю...  ((удалить потом комментарий))
		return *ptr;
	}

	Element<ValueType>& operator*() const
	{																	//if (ptr == NULL) throw(nullptr_exeption()); ломает прогу. почему не знаю...  ((удалить потом комментарий))
		return *ptr;
	}
	ListIterator& operator++() {
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
protected:
	//ListIterator<T> iterator;
public:
	IteratedLinkedList() : LinkedListParent<T>() {
		//std::cout << "\nIteratedLinkedList constructor";
	}

	//IteratedLinkedList(IteratedLinkedList<T>& lst) : LinkedListParent<T>(lst) {}

	virtual ~IteratedLinkedList() {
		//std::cout << "\nIteratedLinkedList destructor";
	}
	virtual ListIterator<T> begin() {
		return ListIterator<T>(LinkedListParent<T>::head);
	}
	virtual ListIterator<T> end() {
		return ListIterator<T>(LinkedListParent<T>::tail);
	}


	 friend std::ostream& operator<< (std::ostream& ustream, IteratedLinkedList<T>& obj) {
		if (typeid(ustream).name() == typeid(std::ofstream).name())
		{
			ustream << obj.num << std::endl;
			for (ListIterator<T> iter = obj.begin(); iter != NULL; iter++)
				ustream << *iter << std::endl;
			return ustream;
		}
		ustream << "\nLength: " << obj.num << std::endl;
		int i = 0;
		for (ListIterator<T> iter = obj.begin(); iter != NULL; iter++, i++)
			ustream << "arr[" << i << "] = " << *iter << std::endl;
		return ustream;
	};
	 friend std::istream& operator>> (std::istream& ustream, IteratedLinkedList<T>& obj) {
		//чтение из файла и консоли совпадают
		int len;
		ustream >> len;
		//здесь надо очистить память под obj, установить obj.num = 0
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

template<class T>
class IteratedQueue : public IteratedLinkedList<T> {
public:
	IteratedQueue() : IteratedLinkedList<T>() {}
	/*IteratedQueue(IteratedQueue<T>& queue) : IteratedLinkedList<T>() {
		this->erase_all();
		ListIterator<T> iter = queue.begin();
		for (; iter != NULL; iter++) {
			push((*iter).getValue());
		}
	}*/
	/*IteratedQueue(const IteratedQueue<T>& queue) : IteratedLinkedList<T>(){
		for (Element<T>* ptr = queue.head; ptr != NULL; ptr = ptr->getNext()) {
			push(ptr->getValue());
		}
	}*/

	virtual ~IteratedQueue(){}

	/*template<class T>
	IteratedQueue<T>& operator= (const IteratedQueue<T>& lst) {
		if (this == lst) return this;
		this->erase_all();
		ListIterator<T> iter = lst.begin();
		for (; iter != NULL; iter++) {
			push((*iter).getValue());
		}
		return this;
	}*/

	virtual Element<T>* push(T value) {
		Element<T>* tmp;
		if (!(tmp = new Element<T>(value,NULL, this->tail))) throw(memory_exeption());
		this->num++;
		if(this->head == NULL){
			this->head = tmp;
			this->tail = tmp;
		}
		else {

			this->tail->setNext(tmp);
			this->tail = tmp;
		}
		return tmp;
		
	};
	virtual Element<T>* pop() {
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

	virtual Element<T>* insert(T value, int pos) {
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
	virtual Element<T>* insert(Element<T>* pos, T value) {
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
};

template<class T>
class SortedQueue : public IteratedQueue<T> {
public:
	SortedQueue() : IteratedQueue<T>(){
	}
	virtual ~SortedQueue(){}

	virtual Element<T>* push(T value) {
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
			this->insert(&*iter, value);
		}
		return tmp;

	};
};

template <class T>
IteratedQueue<T>* filter(IteratedLinkedList<T>* lst, bool (*f)(T)) {
	if (f == NULL || lst == NULL) throw(nullptr_exeption());
	IteratedQueue<T>* tmp = new IteratedQueue<T>;
	for (ListIterator<T> iter = lst->begin(); iter != NULL; iter++) {
		if (f((*iter).getValue()))
			tmp->push((*iter).getValue());
	}
	return tmp;
}


int main() {

	try {
		/*
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
		std::cout << std::endl; pop<int>(lst1, 11);
		std::cout << std::endl << std::endl << std::endl;
		for (auto i : lst1) {
			std::cout << i << " ";
		}
		std::cout << std::endl;

		std::list<int> tmp = filter<int>(lst1, P);

		for (auto i : tmp) {
			std::cout << i << " ";
		}
		std::cout << std::endl;*/

		SortedQueue<int> queue;
		queue.push(2);
		queue.push(1);
		queue.push(5);
		queue.push(3);
		queue.push(4);
		queue.push(6);
		queue.push(-7);
		queue.push(-14);
		queue.push(-21);
		queue.push(-28);
		queue.push(-35);
		queue.push(-42);
		queue.push(-49);
		queue.push(-56);




		std::cout << "!"<< *queue[1]->getPrevious() <<"!" << std::endl;
		std::cout << queue << std::endl;
		Element<int>* tmp = queue.pop();
		std::cout << *tmp << std::endl;
		std::cout << queue << std::endl;
		delete tmp;
		//std::cin >> queue;
		//queue.insert(queue[4],9);
		//queue.insert(queue[5],9);
		queue.push(0);
		std::cout << queue << std::endl;
		
		IteratedQueue<int>* newqueue1 = filter<int>(&queue, P);
		std::cout << *newqueue1 << std::endl;

		IteratedQueue<int>* newqueue2 = filter<int>(newqueue1, P1);
		std::cout << *newqueue2 << std::endl;

		delete newqueue1;
		newqueue1 = filter<int>(newqueue2, P2);
		std::cout << *newqueue1 << std::endl;

		delete newqueue1;
		delete newqueue2;

	}
	catch (...) {}

	try {
		std::cout << "=============================================================" << std::endl;
		SortedQueue<file> queue;
		file fff("d:/s", "fff", { 1,1,1 }, { 1,1,2 }, { 1,1,1 });
		file rrr("d:", "rrr", { 1,1,1 }, { 1,1,1 }, { 1,1,1 });
		file mmm("d:/s/a/", "mmm", { 1,1,1 }, { 1,1,1 }, { 1,1,1 });
		file ggg("d:/aaa/", "ggg", { 1,1,1 }, { 1,1,1 }, { 1,1,1 });
		file ddd("d:/", "ddd", { 1,1,1 }, { 1,1,1 }, { 1,1,1 });
		file ttt("d:/", "ttt", { 1,1,1 }, { 2,1,1 }, { 1,1,1 });
		file hhh("d:/assr/", "hhh", { 5,3,2 }, { 1,1,2 }, { 1,1,2 });
		file aaa("d:/4/ras13d/", "aaa", { 1,4,2 }, { 1,1,2 }, { 1,1,2 });
		queue.push(fff);
		queue.push(rrr);
		queue.push(mmm);
		queue.push(ggg);
		queue.push(ttt);
		queue.push(hhh);
		queue.push(aaa);
		queue.push(ddd);

		std::cout << std::endl;
		std::cout << queue;
		std::cout << std::endl;


	}
	catch (...) {}
	return 0;
}