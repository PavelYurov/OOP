/*#include<iostream>
#include<stdio.h>
#include<locale.h>
#include<math.h>

struct cord {
	double x;
	double y;
};

cord operator * (cord a, double value) {
	cord c = a;
	c.x *= value; c.y *= value;
	return c;
}
cord operator * ( double value, cord a) {
	cord c = a;
	c.x *= value; c.y *= value;
	return c;
}

class otrezok {
private:
	cord cords;
public:
	otrezok() {
		cords.x = NULL;
		cords.y = NULL;
	}
	otrezok(double x, double y) {
		cords.x = x;
		cords.y = y;
	}
	otrezok(cord cords) {
		this->cords = cords;
	}
	~otrezok() {} //std::cout << "деконструктор работает"; }
	otrezok operator * (double value) {
		otrezok a(cords);
		a.cords = a.cords * value;
		return a;
	}
	otrezok operator -() {
		otrezok a(cords);
		a.cords.x = a.cords.x * -1;
		return a;
	}
	otrezok operator +() {
		otrezok a(cords);
		a.cords.y = a.cords.y * -1;
		return a;
	}
	friend otrezok operator * (double value, otrezok a);
	friend void printf(otrezok a);
};

otrezok operator * (double value, otrezok a) {
	otrezok b(a.cords);
	b.cords = a.cords * value;
	return b;
}
void printf(otrezok a) {
	std::cout << a.cords.x << "\t" << a.cords.y << std::endl;
}
int main() {
	setlocale(LC_ALL, "Russian");
	otrezok a(10,2);
	printf(a);
	a = a * 2.0f;
	printf(a);
	a = 0.25f * a;
	printf(a);
	a = -a;
	printf(a);
	a = -a;
	a = +a;
	printf(a);
}*/

#include<iostream>
#include<Windows.h>
class myarrayparent {
protected:
	int capacity;
	int count;
	double* ptr;

	void copy(double* arr, int len) {
		delete[]ptr;
		ptr = new double[capacity];
		count = 0;
		for (int i = 0; i < len; i++)
			push(arr[i]);
	}
public:
	myarrayparent(int dimension = 100) {
		std::cout << "\n";
		ptr = new double[dimension];
		capacity = dimension;
		count = 0;
	}
	myarrayparent(double* arr, int  len) {
		capacity = len;
		copy(arr, len);
	}
	~myarrayparent() {
		delete[]ptr;
		ptr = NULL;
	}
	int Capacity() { return capacity; }
	int Size() { return count; }
	double Getcomponent(int index) {
		try {
			if (index < 0 || index >= count) {
				throw('e');
			}
		}
		catch (const char s) {
			std::cerr << "get > index error" << std::endl;
			system("pause");
			return -1;
		}
		return ptr[index];
	}
	void Setcomponent(int index, double value) {
		try {
			if (index < 0 || index >= count) {
				throw('e');
			}
			ptr[index] = value;
		}
		catch (const char s) {
			std::cerr << "set > index error" << std::endl;
			system("pause");
		}
	}
	virtual void push(double value) {
		try {
			if (count >= capacity) {
				throw('e');
			}
			ptr[count] = value;
			count++;
		}
		catch (const char s) {
			std::cout << "push > index error" << std::endl;
			system("pause");
		}

	}
	void remove_last() {
		try {
			if (count < 0) {
				throw('e');
			}
			count--;
		}
		catch (const char s) {
			std::cout << "remove > index error" << std::endl;
			system("pause");
		}
	}
	double operator[](int index) {
		return Getcomponent(index);
	}
	myarrayparent& operator = (const myarrayparent& a) {
		capacity = a.capacity;
		copy(a.ptr, a.count);
		return *this;
	}
	myarrayparent(const myarrayparent& v) {
		capacity = v.capacity;
		copy(v.ptr, v.count);
	}
	void print() {
		std::cout << "{";
		for (int i = 0; i < count; i++) {
			std::cout << ptr[i] << " ";
		}
		std::cout << "}" << std::endl;
	}
	virtual int indexof(double value) {
		for (int i = 0; i < count; i++) {
			if (ptr[i] == value) {
				return i;
			}
		}
		return -1;
	}

};


class myarraychild : public myarrayparent {
public:
	myarraychild(int dimension = 100) : myarrayparent(dimension) {

	}
	myarraychild(const myarrayparent& v) : myarrayparent(v) {
	}
	void removeat(int index) {
		try {
			if (index < 0 || index >= count) {
				throw('e');
			}
			for (int i = index; i < count; i++) {
				ptr[i] = ptr[i + 1];
			}
			ptr[count] = 0;
			count--;
		}
		catch (const char s) {
			std::cerr << "remove > index error" << std::endl;
			system("pause");
		}

	}
protected:
	virtual void remove_all_(double value) {
		while (indexof(value) != -1) {
			removeat(indexof(value));
		}
	}
public:
	myarraychild remove_all(double value) {
		myarraychild a(*this);
		a.remove_all_(value);
		return a;
	}
	void insert_at(double value, int index) {
		try {
			if (index < 0 || index > count) {
				throw("insert > index error");
			}
			if (count == capacity) {
				throw("insert > too much elements");
			}
			for (int i = count; i > index; i--) {
				ptr[i] = ptr[i - 1];
			}
			ptr[index] = value;
			count++;
		}
		catch (const char* err) {
			std::cerr << err << std::endl;
			system("pause");
		}
	}
};

class mysortedarray : public myarraychild {
protected:
	/*void sort(double* mas, int size) {
		int i = 0;
		int j = size - 1;
		double mid = mas[size / 2];
		do {
			while (mas[i] < mid) {
				i++;
			}
			while (mas[j] > mid) {
				j--;
			}
			if (i <= j) {
				double tmp = mas[i];
				mas[i] = mas[j];
				mas[j] = tmp;

				i++;
				j--;
			}
		} while (i <= j);
		if (j > 0) {
			sort(mas, j + 1);
		}
		if (i < size) {
			sort(&mas[i], size - i);
		}
	}*/
	virtual void remove_all_(double value) {
		for (int i = 0; i < count; i++) {
			if (ptr[i] == value) {
				removeat(i);
				i--;
			}
			if (ptr[i] > value) {
				break;
			}
		}
	}
public:
	mysortedarray(int dimension = 100) : myarraychild(dimension) {
		//sort(ptr, count);
	}
	mysortedarray(const myarraychild& v) : myarraychild(v) {
		//sort(ptr, count);
	}
	virtual void push(double value) {
		int i = 0;
		for (; i < count; i++) {
			if (ptr[i] >= value) {
				break;
			}
		}
		insert_at(value, i);
	}
	virtual int indexof(double value) {
		for (int i = 0; i < count; i++) {
			if (ptr[i] == value) {
				return i;
			}
			if (ptr[i] > value) {
				break;
			}
		}
		return -1;
	}
};

int main() {
	if (true) {
		myarrayparent arr;
		for (int i = 0; i < 10; i++) {
			arr.push((double)(i + 1));
			myarrayparent p1;
			p1 = arr;
			p1.print();
			std::cout << std::endl;
		}
		arr.push(10);
		arr.print();
		std::cout << arr.indexof(10) << std::endl;
		myarraychild arr2(arr);
		myarraychild arr3(arr2.remove_all(10)); arr2.print(); arr3.print(); std::cout << "\n============================\n";
		arr3.remove_last(); arr3.print();
		std::cout << "\n" << arr3.Size() << std::endl;
		arr3.insert_at(999,8); arr3.print(); std::cout << "\n============================\n";
		mysortedarray arr4(arr3); arr4.print(); arr4.push(1); arr4.push(1000); arr4.print();
		mysortedarray arr5 = arr4.remove_all(1); arr5.print();
	}
	system("pause");
	return 0;
}