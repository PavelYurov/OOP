#include<iostream>
#include<Windows.h>
#include<string.h>

//можно ли было сделать без копирования?
class A1
{
protected:
	int a1;
	std::string name;
	virtual void show_a1() {
		std::cout << "a1 = " << a1;
	}
public:
	A1(int V1 = 0) {
		a1 = V1;
		name = "A1";
		std::cout << "\nClass " << name <<" constructor";
	}
	virtual void print() {
		std::cout << "\nVariable of "<< name <<" class"; 
	}
	virtual void show() {
		std::cout << std::endl;
		show_a1();
	}
};


class B1 : virtual public A1
{
protected:
	int b1;
	virtual void show_b1() {
		std::cout << "b1 = " << b1;
	}
public:
	B1(int V1 = 0, int V2 = 0) : A1(V2) {
		b1 = V1;
		name = "B1";
		std::cout <<"\nClass "<<name<<" constructor";
	}
	virtual void print() {
		std::cout << "\nVariable of "<<name<<" class";
	}
	virtual void show() {
		std::cout << std::endl;
		show_b1(); std::cout << ", ";
		show_a1();
	}
};

class B2 : virtual public A1
{
protected:
	int b2;
	virtual void show_b2() {
		std::cout << "b2 = " << b2;
	}
public:
	B2(int V1 = 0, int V2 = 0) : A1(V2) {
		b2 = V1;
		name = "B2";
		std::cout << "\nClass " << name << " constructor";
	}
	virtual void print() {
		std::cout << "\nVariable of " << name << " class";
	}
	virtual void show() {
		std::cout << std::endl;
		show_b2(); std::cout << ", ";
		show_a1();
	}
};

class B3 : virtual public A1
{
protected:
	int b3;
	virtual void show_b3() {
		std::cout << "b3 = " << b3;
	}
public:
	B3(int V1 = 0, int V2 = 0) : A1(V2) {
		b3 = V1;
		name = "B3";
		std::cout << "\nClass " << name << " constructor";
	}
	virtual void print() {
		std::cout << "\nVariable of " << name << " class";
	}
	virtual void show() {
		std::cout << std::endl;
		show_b3(); std::cout << ", ";
		show_a1();
	}
};

class C1 : virtual public B1,B2,B3
{
protected:
	int c1;
	virtual void show_c1() {
		std::cout << "c1 = " << c1;
	}
public:
	C1(int V1 = 0, int V2 = 0, int V3 = 0, int V4 = 0,int V5 = 0) : A1(V5), B1(V2, V5), B2(V3, V5), B3(V4, V5) {
		c1 = V1;
		name = "C1";
		std::cout << "\nClass " << name << " constructor";
	}
	virtual void print() {
		std::cout << "\nVariable of " << name << " class";
	}
	virtual void show() {
		std::cout << std::endl;
		show_c1(); std::cout << ", ";
		show_b1(); std::cout << ", ";
		show_b2(); std::cout << ", ";
		show_b3(); std::cout << ", ";
		show_a1();
	}
};

class C2 : virtual public B1, B2, B3
{
protected:
	int c2;
	virtual void show_c2() {
		std::cout << "c2 = " << c2;
	}
public:
	C2(int V1 = 0, int V2 = 0, int V3 = 0, int V4 = 0, int V5 = 0) : A1(V5),B1(V2, V5), B2(V3, V5), B3(V4, V5) {
		c2 = V1;
		name = "C2";
		std::cout << "\nClass " << name << " constructor";
	}
	virtual void print() {
		std::cout << "\nVariable of " << name << " class";
	}
	virtual void show() {
		std::cout << std::endl;
		show_c2(); std::cout << ", ";
		show_b1(); std::cout << ", ";
		show_b2(); std::cout << ", ";
		show_b3(); std::cout << ", ";
		show_a1();
	}
};
class C3 : virtual public B1, B2, B3
{
protected:
	int c3;
	virtual void show_c3() {
		std::cout << "c3 = " << c3;
	}
public:
	C3(int V1 = 0, int V2 = 0, int V3 = 0, int V4 = 0, int V5 = 0) : A1(V5), B1(V2, V5), B2(V3, V5), B3(V4, V5) {
		c3 = V1;
		name = "C3";
		std::cout << "\nClass " << name << " constructor";
	}
	virtual void print() {
		std::cout << "\nVariable of " << name << " class";
	}
	virtual void show() {
		std::cout << std::endl;
		show_c3(); std::cout << ", ";
		show_b1(); std::cout << ", ";
		show_b2(); std::cout << ", ";
		show_b3(); std::cout << ", ";
		show_a1();
	}
};

int main()
{
	B1 test1(1, 2);
	B2 test2(3, 4);
	B3 test3(5, 6);
	C1 test4(7, 8,9,10,20);
	C2 test5(11, 12, 13, 14,21);
	C3 test6(15, 16, 17, 18,22);
	A1* ptr = &test1; ptr->show(); ptr->print();
	ptr = &test2; ptr->show(); ptr->print();
	ptr = &test3; ptr->show(); ptr->print();
	ptr = &test4; ptr->show(); ptr->print();
	ptr = &test5; ptr->show(); ptr->print();
	ptr = &test6; ptr->show(); ptr->print();
	system("pause");
	return 0;
}
