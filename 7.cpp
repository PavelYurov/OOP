#include <iostream>
#include <fstream>
#include <typeinfo>
#include <Windows.h>
#include <time.h>
#include <math.h>

//#include<functional>
#include <vector>

bool is_in_range(int x, int y, int max_x, int max_y);

class Exception : public std::exception
{
protected:
	//сообщение об ошибке
	char* str;
public:
	Exception(const char* s){
		str = new char[strlen(s) + 1];
		strcpy_s(str, strlen(s) + 1, s);
	}
	Exception(const Exception& e){
		str = new char[strlen(e.str) + 1];
		strcpy_s(str, strlen(e.str) + 1, e.str);
	}
	~Exception(){
			delete[] str;
	}
	//функцию вывода можно будет переопределить в производных 
	//классах, когда будет ясна конкретика
	virtual void print(){
		std::cerr << "Exception: " << str;
	}
};

class WrongType : public Exception {
	WrongType(const char* string = "Wrong Type was gotten") :Exception(string) {

	}

	virtual void print() {
		Exception::print();
	}
};

class IndexOutOfBoundsException : public Exception {
protected:
	int index_x;
	int index_y;
public:
	IndexOutOfBoundsException(int index_x, int index_y, const char* string = "attempt to access index out of bounds") : Exception(string) {
		this->index_x = index_x;
		this->index_y = index_y;
	}
	virtual void print() {
		Exception::print();
		std::cerr << std::endl << "wrong index: " << index_x << " " <<index_y << std::endl;
	}
};
class WrongDimensionsException : public Exception {
protected:
	int dim_1;
	int dim_2;
public:
	WrongDimensionsException(int dim_1,
		int dim_2,
		const char* string = "wrong demension initialization") : Exception(string) {
		this->dim_1 = dim_1;
		this->dim_2 = dim_2;
	}
	virtual void print() {
		Exception::print();
		std::cerr << std::endl << "got dimensions: " << dim_1 << " x " << dim_2 << std::endl;
	}
};
class WrongSizeException : public WrongDimensionsException {
protected:
	int expected_size_h;
	int expected_size_w;
	int gotten_size_h;
	int gotten_size_w;
public:
	WrongSizeException(
		int expected_size_h,
		int expected_size_w,
		int gotten_size_h,
		int gotten_size_w,
		const char* string = "attempt to access index out of bounds") : WrongDimensionsException(gotten_size_h,gotten_size_w,string) {
		this->expected_size_h = expected_size_h;
		this->expected_size_w = expected_size_w;
		this->gotten_size_h = gotten_size_h;
		this->gotten_size_w = gotten_size_w;
	}
	virtual void print() {
		Exception::print();
		std::cerr << std::endl << "expected matrix:" << expected_size_h << " x " << expected_size_w << std::endl;
		std::cerr << std::endl << "gotten matrix:" << gotten_size_h << " x " << gotten_size_w << std::endl;
	}
};


template <class T>
class BaseMatrix{
protected:
	T** ptr;
	int height;
	int width;
	void rebuild(int Height, int Width) {
		if (Height <= 0 || Width <= 0)
			throw WrongDimensionsException(Height, Width);
		if (ptr != NULL) {
			for (int i = 0; i < height; i++)
				delete[] ptr[i];
			delete[] ptr;
			ptr = NULL;
		}
		height = Height;
		width = Width;
		ptr = new T * [height];
		for (int i = 0; i < height; i++) {
			ptr[i] = new T[width];
		}
	}
public:
	BaseMatrix<T>(int Height = 2, int Width = 2){
		if (Height <= 0 || Width <= 0)
			throw WrongDimensionsException(Height, Width);
		height = Height;
		width = Width;
		ptr = new T* [height];
		for (int i = 0; i < height; i++) {
			ptr[i] = new T[width];
		}
	}
	BaseMatrix<T>(const BaseMatrix<T>& M){
		height = M.height;
		width = M.width;
		ptr = new T* [height];
		for (int i = 0; i < height; i++){
			ptr[i] = new T[width];
			for (int j = 0; j < width; j++) {
				ptr[i][j] = M.ptr[i][j];
			}
		}
	}
	~BaseMatrix<T>()
	{
		if (ptr != NULL){
			for (int i = 0; i < height; i++)
				delete[] ptr[i];
			delete[] ptr;
			ptr = NULL;
		}
	}
	void print()
	{
		for (int i = 0; i < height; i++)
		{
			std::cout << "||";
			for (int j = 0; j < width; j++) {
				std::cout << " " << ptr[i][j];
			}
			std::cout << "||\n";
		}
	}
	T& operator()(int row, int column)
	{
		if (row < 0 || column < 0 || row >= height || column >= width) {
			throw IndexOutOfBoundsException(row,column);								//2
		}
		return ptr[row][column];
	}
	template <class V>
	friend std::ostream& operator << (std::ostream& ustream, BaseMatrix<V> obj);
	template <class V>
	friend std::istream& operator >> (std::istream& ustream, BaseMatrix<V>& obj);
};

template <class T>
class ChildMatrix : public BaseMatrix<T> {
public:
	ChildMatrix<T>(int Height = 2, int Width = 2) : BaseMatrix<T>(Height, Width) {

	}
	ChildMatrix<T>(const BaseMatrix<T>& ptrM) : BaseMatrix<T>(ptrM) {

	}
	ChildMatrix<T>(const ChildMatrix<T>& ptrM) {
		this->height = ptrM.height;
		this->width = ptrM.width;
		this->ptr = new T* [this->height];
		for (int i = 0; i < this->height; i++) {
			this->ptr[i] = new T[this->width];
			for (int j = 0; j < this->width; j++) {
				this->ptr[i][j] = ptrM.ptr[i][j];
			}
		}
	}
	~ChildMatrix<T>() {

	}
	ChildMatrix<T>& operator=(const ChildMatrix<T>& M) {
		if (this->ptr != NULL) {
			for (int i = 0; i < this->height; i++)
				delete[] this->ptr[i];
			delete[] this->ptr;
			this->ptr = NULL;
		}
		this->height = M.height;
		this->width = M.width;
		this->ptr = new T* [this->height];
		for (int i = 0; i < this->height; i++) {
			this->ptr[i] = new T[this->width];
			for (int j = 0; j < this->width; j++) {
				this->ptr[i][j] = M.ptr[i][j];
			}
		}
		return *this;
	}
	/*void fill(const std::function<int()> f = [=]()noexcept->int {return rand() % 1000; }) {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				ptr[i][j] = f();
			}
		}
	}*/
	void fill(int range) {
		for (int i = 0; i < this->height; i++) {
			for (int j = 0; j < this->width; j++) {
				this->ptr[i][j] = static_cast<T>((rand() % abs(range)));
			}
		}
	}
	ChildMatrix<T> expand() {
		ChildMatrix<T> res(this->height * this->width, 1);
		int p = 0;
		for (int i = 0; i < this->height + this->width; i++) {
			if (i % 2 == 0) {
				for (int j = 0; j <= i; j++) {
					if(is_in_range(i - j, j, this->height, this->width)){
						res.ptr[p][0] = this->ptr[i - j][j]; p++;
					}
					
				}
			}else {
				for (int j = 0; j <= i; j++) {
					if (is_in_range(j, i-j, this->height, this->width)) {
						res.ptr[p][0] = this->ptr[j][i - j]; p++;
					}

				}
			}
		}
		return res;
	}

	std::vector<T> expand_v_2() {
		std::vector<T> res;
		for (int i = 0; i < this->height + this->width; i++) {
			if (i % 2 == 0) {
				for (int j = 0; j <= i; j++) {
					if (is_in_range(i - j, j, this->height, this->width)) {
						res.push_back(this->ptr[i - j][j]);
					}

				}
			}
			else {
				for (int j = 0; j <= i; j++) {
					if (is_in_range(j, i - j, this->height, this->width)) {
						res.push_back(this->ptr[i - j][j]);
					}

				}
			}
		}
		return res;
	}
};


template <class T>
std::ostream& operator << (std::ostream& ustream, const BaseMatrix<T> obj)
{
	//ustream<<my_manip;
	if (typeid(ustream).name() == typeid(std::ofstream).name())
	{
		ustream << obj.height << " " << obj.width << "\n";
		for (int i = 0; i < obj.height; i++)
		{
			for (int j = 0; j < obj.width; j++)
				ustream << obj.ptr[i][j] << "\n";
		}
		return ustream;
	}
	for (int i = 0; i < obj.height; i++)
	{
		ustream << "||";
		for (int j = 0; j < obj.width; j++)
			ustream << " "<< obj.ptr[i][j] ;
		ustream << "||\n";
	}
	return ustream;
}

template <class T>
std::istream& operator >> (std::istream& ustream, BaseMatrix<T>& obj)
{
	if (typeid(ustream) == typeid(std::ifstream)) {
		int h, w;
		ustream >> h >> w;
		obj.rebuild(h,w);
	}
	for (int i = 0; i < obj.height; i++) {
		for (int j = 0; j < obj.width; j++) {
			ustream >> obj.ptr[i][j];
		}
	}
	return ustream;
}
std::ostream& my_manip(std::ostream& s)
{
	s.precision(4);
	s.fill('%');
	s.width(10);
	return s;
}
bool is_in_range(int x, int y, int max_x, int max_y) {
	return (x >= 0 && x < max_x && y >= 0 && y < max_y);
}
int main()
{
	srand(time(NULL));
	{
		try {

			BaseMatrix<int> Wrong(-1, 1);
			//std::cin >> Wrong;
			std::cout << Wrong(2, 0);

		}
		catch (Exception& e)
		{
			std::cerr << "\nException has been caught: "; e.print();
		}
		/*catch (WrongDimensionsException e)
		{
			std::cerr << "\nException has been caught: "; e.print();
		}
		catch (WrongSizeException e)
		{
			std::cerr << "\nException has been caught: "; e.print();
		}
		catch (IndexOutOfBoundsException e)
		{
			std::cerr << "\nException has been caught: "; e.print();
		}*/
		std::cout << "\n";
		BaseMatrix<int> M(3, 2);
		std::cin >> M;
		std::ofstream fout("out.txt");
		if (fout.is_open())
		{
			fout << M;
			fout.close();
		}
		std::ifstream fin("out.txt");
		BaseMatrix<int> M1(3, 2);
		if (fin)
		{
			fin >> M1;
			fin.close();
		}
		std::cout << M1;
		std::cout << std::endl;
		ChildMatrix<int> M2(M1);
		std::cout << std::endl << "M2:" << std::endl << M2;
		ChildMatrix<int> M3;
		std::cout << std::endl << "M3:" << std::endl << M3;
		M3.fill(100);
		std::cout << std::endl << "M3 filled:" << std::endl << M3;
		ChildMatrix<int> M4 = M2;
		std::cout << std::endl << "M4:" << std::endl << M4;
		M4 = M4.expand();
		std::cout << std::endl << "M4 expand:" << std::endl << M4;
		
		/*std::vector<int> a = M4.expand_v_2();
		for (int i = 0; i < a.size(); i++) {
			std::cout << a[i] << " ";
		}*/

	}
	//system("pause");
	return 0;
}
