//1. Сортировка выбором.
//2. Сортировка слиянием.
//Написать функцию, которая читает массив из файла, сортирует его
//указанным методом, записывает отсортированный массив в новый файл.
//Функция, выполняющая сортировку, должна быть выполнена в виде шаблона
//функции.


#include<iostream>
#include<fstream>

#define N 1000

/*
template<class T>
void copy(T from[], T to[], unsigned long long size)
{
	unsigned long long int i = 0;
	for (i = 0; i < size; i++)
	{
		to[i] = from[i];
	}
}*/
/*
template<class T>
void print(T* arr) {
	long len = _msize(arr) / sizeof(T);
	std::cout << "\n";
	for (int i = 0; i < len; ++i) std::cout << arr[i] << " ";
}
*/
template<class T>
void sort_with_file(void (*f)(T*), std::ifstream& file_in, std::ofstream& file_out) {
	if (f == NULL) throw "nullptrfunction";
	if (file_out.is_open() && file_in.is_open()) {
		unsigned long long lenght;
		file_in >> lenght;
		T* arr;
		if (!(arr  = new T[lenght])) throw "memory error";
		unsigned long long reallen = 0;
		for (unsigned long long int i = 0; i < lenght && !file_in.eof(); ++i) {
			file_in >> arr[i];
			reallen++;
		} 
		if (reallen != lenght) throw "len error";
		file_out << lenght << "\n";
		f(arr);
		for (unsigned long long int i = 0; i < lenght && arr != NULL; ++i) {
			file_out << arr[i] << " ";
		}
		if(!arr)
			delete[] arr;
	}
	else throw "file not opened";
}

template <class T>
void do_not_sort(T* arr) {
}


template <class T>
void selection_sort(T* arr) {
	if (!arr) throw "nullptr array";
	unsigned long long len = _msize(arr) / sizeof(T);
	for (unsigned long long i = 0; i < len; ++i) {
		unsigned long long min_i = i;
		for (unsigned long long j = i; j < len; ++j) {
			if (arr[min_i] > arr[j]) {
				min_i = j;
			}
		}
		std::swap(arr[i], arr[min_i]);
	}
}


template <class T>
T* merge_sort_body(T* arr_l, T* arr_r, unsigned long long len_l, unsigned long long len_r) {
	if (len_l == 0 && len_r == 0) throw "len error";
	if (len_l < 0 || len_r < 0) throw "len error";
	if ((arr_l == NULL && len_l > 0) || (arr_r == NULL && len_r > 0)) throw "error input array";
	T* ptr_sorted_l = NULL;
	T* ptr_sorted_r = NULL;
	if (len_l + len_r > 1) {
		if (len_l >= 1) {
			unsigned long long len = len_l / 2;
			ptr_sorted_l = merge_sort_body<T>(arr_l, &arr_l[len], len, len_l - len);
		}
		if (len_r >= 1) {
			unsigned long long len = len_r / 2;
			ptr_sorted_r = merge_sort_body<T>(arr_r, &arr_r[len], len, len_r - len);
		}
	}
	if (len_l + len_r == 1) {
		T* ptr_ret;
		if(!(ptr_ret = new T[len_l + len_r])) throw "memory error";
		for (int i = 0; i < len_l; ++i) {
			ptr_ret[i] = arr_l[i];
		}
		for (int i = 0; i < len_r; ++i) {
			ptr_ret[i] = arr_r[i];
		}
		if(ptr_sorted_l)
			delete[]ptr_sorted_l;
		if (ptr_sorted_r)
			delete[]ptr_sorted_r;
		//print<T>(ptr_ret);
		return ptr_ret;
	}
	else {
		T* array_res;
		if(!(array_res = new T[len_l + len_r])) throw "memory error";
		unsigned long long i_l = 0, i_r = 0;
	
		while (i_r < len_r && i_l < len_l) {
			if (ptr_sorted_l[i_l] < ptr_sorted_r[i_r]) {
				array_res[i_l + i_r] = ptr_sorted_l[i_l];
				i_l++;
			}
			else {
				array_res[i_l + i_r] = ptr_sorted_r[i_r];
				i_r++;
			}
		}
		if (i_r >= len_r) {
			while (i_l < len_l) {
				array_res[i_l + i_r] = ptr_sorted_l[i_l];
				i_l++;
			}
		}
		else {
			while (i_r < len_r) {
				array_res[i_l + i_r] = ptr_sorted_r[i_r];
				i_r++;
			}
		}

		if (ptr_sorted_l)
			delete[]ptr_sorted_l;
		if (ptr_sorted_r)
			delete[]ptr_sorted_r;
		//print<T>(array_res);
		return array_res;
	}
}

template <class T>
void merge_sort(T* arr) {
	unsigned long long len = _msize(arr) / sizeof(T);
	T* answer;
	answer = merge_sort_body<T>(arr, NULL, len, 0);
	
	if (answer) {
		for (int i = 0; i < len; ++i) {
			arr[i] = answer[i];
		}
		delete[]answer;
	}
}


/*
// Алгоритм глупой сортировки Stupid sort

template<class T>
T* stupidsort(T* a, size_t len) {
	// копируем массив - будем сортировать его копию
	T* arr = new T[len];
	::copy(a, arr, len);
	size_t i = 1;
	T temp;
	for (; i < len; i++)
	{
		if (arr[i - 1] > arr[i]) {
			// меняем элементы местами
			temp = arr[i - 1];
			arr[i - 1] = arr[i];
			arr[i] = temp;
			// начинаем идти сначала
			i = 0;
		}
	}
	return arr;
}*/
int main()
{
	
	int arr1[N];
	for (int i = 0; i < N; i++)
	{
		srand(i);
		arr1[i] = (int)(sin((double)rand()) * N);
	}
	std::ofstream fout("arr1.txt");
	if (fout)
	{
		fout << N << "\n";
		for (int i = 0; i < N; i++)
			fout << arr1[i] << " ";
		fout.close();
	}
	
	std::ifstream fin1("arr1.txt");
	std::ofstream fres1("res1.txt");
	std::ofstream fres2("res2.txt");

	void (*f)(int*);
	f = merge_sort<int>;
	void (*g)(int*);
	g = selection_sort<int>;

	time_t time1, time2;
	time1 = clock();

	sort_with_file<int>(f, fin1, fres1);
	fin1.close();
	std::ifstream fin2("arr1.txt");
	sort_with_file<int>(g, fin2, fres2);

	time2 = clock();
	std::cout << ((double)(time2)-time1) / CLOCKS_PER_SEC;
	fin2.close(); fres1.close(); fres2.close();

	/*int* random; int len = 0;
	if (fin)
	{
		fin >> len;
		random = new int[len];
		for (int i = 0; i < len; i++)
			fin >> random[i];
		time_t time1, time2;
		time1 = clock();
		int* res1 = stupidsort<int>(random, N);
		time2 = clock();
		std::cout << ((double)(time2)-time1) / CLOCKS_PER_SEC;
		std::ofstream fres1("res1.txt");
		if (fres1)
		{
			fres1 << N << "\n";
			for (int i = 0; i < N; i++)
				fres1 << res1[i] << " ";
			fres1.close();
		}
		delete[] res1;
		delete[] random;
		fin.close();
	}*/
	return 0;
}
