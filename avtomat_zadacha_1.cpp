#include<iostream>
#include<vector>
#include<time.h>
#include<math.h>
#include<random>
#include<thread>
#include<conio.h>
#include<fstream>
#include<typeinfo>



template <class T, class V>
class generation;
template<class V>
class ai1;

template <class T>
class Obj {
protected:
	std::vector<T> genom;
	long long genom_len;
	double fitness;
	double fitness_mean;
public:
	template <class T1, class V1>
	friend class generation;

	template <class V1>
	friend class ai1;

	Obj(const std::vector<T>& genom) {
		//std::cout << "\nobj1";
		this->genom_len = genom.size();
		for(long long i = 0; i <genom_len;++i) this->genom.push_back(genom[i]);
		fitness = 0;
		fitness_mean = 0;
	}

	Obj() {
		//std::cout << "\nobj1";
		genom_len = 0;
		fitness = 0;
		fitness_mean = 0;
	}

	Obj(const Obj<T>& a) {
		//std::cout << "\nobj1";
		genom = a.genom;
		genom_len = a.genom_len;
		fitness = a.fitness;
		fitness_mean = a.fitness_mean;
	}

	Obj<T>& operator=(const Obj<T> a) {
		genom = a.genom;
		genom_len = a.genom_len;
		fitness = a.fitness;
		fitness_mean = a.fitness_mean;
		return this;
	}

	virtual void fitness_mean_calc(const double min_fitness, const double max_fitness) {
		fitness_mean = (fitness - min_fitness) / (max_fitness - min_fitness);
	}

	virtual ~Obj() {
		//std::cout << "\nobj1 exit";
	}

	virtual long long get_size() { return genom_len; }

	T& operator[](const long long index) {
		if (index < 0 || index > genom_len) throw "\n>array size error";
		T res = genom[index];
		return res;
	}
	template <class T1>
	friend std::ostream& operator<<(std::ostream& s, Obj<T1>& o);
};

template <class T>
std::ostream& operator<<(std::ostream& s, Obj<T>& o) {
	if (typeid(s).name() == typeid(std::ofstream).name()) {
		s << std::endl;
		for (long long i = 0; i < o.genom_len; ++i)
			s << o.genom[i] << " ";
	}
	else {
		s << std::endl << "genom: " << o.genom_len << std::endl;
		for (long long i = 0; i < o.genom_len; ++i)
			s << o.genom[i] << " ";
		s << "fitness: " << o.fitness << " fitness normalized: " << o.fitness_mean;
	}
	return s;
}

template<class T, class V>
class generation {
protected:
	unsigned long long number;
	Obj<T>** arr;
	long long obj_amount;
	Obj<T>** best_arr;
	long long best_amount;
	int mchance;

	double best_fitness;

	void qsortRecursive(Obj<T>** mas,const long long size) {
		if (size <= 0) throw "\n>array len exeption";
		if (mas == NULL) throw "\nnull ptr in array";
		long long i = 0;
		long long  j = size - 1;
		if (mas[size/2] == NULL) throw "\nnull ptr in array";
		Obj<T>* mid = new Obj<bool>(*mas[size / 2]);
		do {
			if (mas[i] == NULL) throw "\nnull ptr in array";
			while (mas[i]->fitness_mean > mid->fitness_mean) {
				i++;
				if (mas[i] == NULL) throw "\nnull ptr in array";
			}
			if (mas[j] == NULL) throw "\nnull ptr in array";
			while (mas[j]->fitness_mean < mid->fitness_mean) {
				j--;
				if (mas[j] == NULL) throw "\nnull ptr in array";
			}
			if (i <= j) {
				std::swap(mas[j], mas[i]);
				i++;
				j--;
			}
		} while (i <= j);
		if (j > 0) {
			qsortRecursive(mas, j + 1);
		}
		if (i < size) {
			qsortRecursive(&mas[i], size - i);
		}
		delete mid;
	}
	void fresh_memory() {
	if (arr != NULL)
		for (long long i = 0; i < obj_amount; ++i) {
			if (arr[i] != NULL) delete arr[i];
		}
	if (arr != NULL) delete[] arr;
	arr = NULL;
	}
public:
	generation() {
		best_fitness = 0;
		mchance = 5;
		obj_amount = 0;
		arr = NULL;
		number = 0;
		best_amount = 1;
		best_arr = NULL;
	}

	generation(const long long number, Obj<T>** arr, const long long obj_amount) {
		if (arr == NULL) throw "\n>null array ptr exeption";
		best_amount = 1;
		best_fitness = 0;
		mchance = 5;
		best_arr = NULL;
		this->number = number;
		this->obj_amount = obj_amount;

		if (obj_amount <= 0) throw "\n>less than 1 object exeption";
		this->arr = arr;

		/*if (!(this->arr = new Obj<T>*[obj_amount])) throw "\n>zeromemory exception";

		for (long long i = 0; i < obj_amount; ++i) {
			if (arr[i] == NULL) throw "\n>null ptr in array";
			this->arr[i] = new Obj<T> (*arr[i]);
		}*/
	}

	generation(const long long obj_amount) {
		if (obj_amount <= 0) throw "\n>less than 1 object exeption";
		best_amount = 1;
		best_fitness = 0;
		mchance = 5;
		best_arr = NULL;
		number = 1;
		this->obj_amount = obj_amount;

		if (!(this->arr = new Obj<T>*[obj_amount])) throw "\n>zeromemory exception";
	}

	virtual ~generation() {
		std::cout << "\ngeneration " << number << "\t max fitness: " << best_fitness;
		if (arr != NULL)
			for (long long i = 0; i < obj_amount; ++i) {
				if (arr[i] != NULL) delete arr[i];
			}
		if (best_arr != NULL)
			for (long long i = 0; i < best_amount; ++i) {
				if (best_arr[i] != NULL) delete best_arr[i];
			}
		if (arr!=NULL) delete[] arr;
		if (best_arr != NULL) delete[] best_arr;

	}
	virtual double find_max_fitness() {
		double max_fitness = 0;
		for (long long i = 0; i < obj_amount; ++i) {
			if (arr[i]->fitness > max_fitness) {
				max_fitness = arr[i]->fitness;
			}
		}
		return max_fitness;
	}

	virtual double find_min_fitness() {
		double min_fitness = arr[0]->fitness;
		for (long long i = 1; i < obj_amount; ++i) {
			if (arr[i]->fitness < min_fitness) {
				min_fitness = arr[i]->fitness;
			}
		}
		return min_fitness;
	}

	virtual void train(V (*f)(Obj<T>* const)) = 0;
	virtual void select(const long long best_amount) = 0;
	virtual generation<T,V>* crossover() = 0;
	virtual void work() = 0;


	virtual void save(const long long ba, const char* str = "out.txt") {
		if (ba <= 0) throw "\n>less that 1 objext to save";
		std::ofstream f(str);
		if (f.is_open()) {
			f << obj_amount << " ";
			f << arr[0]->genom_len;
			for (long long i = 0; i < ba; ++i) {
				f << *(arr[i]);
			}
			f.close();

		}
		else throw "\n>file not opened";
	}

	virtual void load(const char* str = "out.txt") {
		if (arr != NULL) throw "\n>not empty array have already created";
		std::ifstream f(str);
		if (f.is_open()) {
			f >> obj_amount;
			if (!(this->arr = new Obj<T>*[obj_amount])) throw "\n>zeromemory exception";
			long long gen_len = 0;
			f >> gen_len;

			for (long long i = 0; i < obj_amount; ++i) {
				std::vector<T> tmp;
				for (long long j = 0; j < gen_len; ++j) {
					T tmp1;
					f >> tmp1;
					tmp.push_back(tmp1);
				}
				arr[i] = new Obj<T>(tmp);
			}

			f.close();
		}
	}
};

template<class V>
class ai1 : public generation<bool,V> {
protected:
	long long random_coeff;
	Obj<bool>* create_random_obj(const long long len) {
		if (len <= 0) throw "\n>len exception";
		std::vector<bool> tmp;
		for (long long i = 0; i < len; ++i) tmp.push_back(rand() % 2);//
		return new Obj<bool>(tmp);
	}
	void mutate(std::vector<bool>& p) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dist(0, 1000);
		int proc = dist(gen);
		if (proc <= this->mchance*10) {
			for (long long i = 0; i < p.size(); ++i) {
				int proc_c = dist(gen);
				if (proc_c <= this->mchance) {
					p[i] = static_cast<bool>(rand() % 2);
				}
			}
		}
	}
public:
	ai1() : generation<bool,V>() {random_coeff = 5;} //std::cout << "\na1 created"; }

	ai1(const long long number, Obj<bool>** arr, const long long obj_amount,const long long random_coeff = 5) : generation<bool,V>(number,arr,obj_amount) {
		//std::cout << "\na1 created";
		if (random_coeff <= 0) throw "\n>random obj part error";
		this->random_coeff = random_coeff;}

	ai1(const long long obj_amount, const long long len, const long long random_coeff = 5) : generation<bool,V>(obj_amount) {
		//std::cout << "\na1 created";
		if (random_coeff <= 0) throw "\n>random obj part error";
		this->random_coeff = random_coeff;
		for (long long i = 0; i < obj_amount; ++i) {
			this->arr[i] = create_random_obj(len);
		}
	}

	virtual ~ai1() { }//std::cout << "\nai exiting"; }

	virtual void train(V (*f)(Obj<bool>* const)) { //нужна собственно задача
		for (long long i = 0; i < this->obj_amount; ++i) {
			long long answers = 0;
			//std::cout << std::endl<< arr[i]->genom.size() <<" " << arr[i]->genom_len;
			for (long long j = 0; j < this->arr[i]->genom_len; ++j) {
				if (this->arr[i]->genom[j] == 1) {
					answers++;
				}
			}
			this->arr[i]->fitness = static_cast<double>(answers) / this->arr[i]->genom_len;
			//std::cout << std::endl << arr[i]->fitness;
		}
		double a = this->find_max_fitness();
		double b = this->find_min_fitness();
		for (long long i = 0; i < this->obj_amount; ++i) {
			this->arr[i]->fitness_mean_calc(b, a);
		}
	}

	virtual void select(const long long best_amount) {
		this->best_amount = best_amount;
		if (best_amount <= 1 || best_amount % 2 != 0 || best_amount > this->obj_amount) throw "best amount exception";
		if (!(this->best_arr = new Obj<bool>*[best_amount])) throw "memory exception";
		this->qsortRecursive(this->arr, this->obj_amount);
		//std::cout << "\n==========================\n";
		for (long long i = 0; i < best_amount; ++i) {
			/*best_arr[i] = arr[i];
			arr[i] = NULL;*/
			
			this->best_arr[i] = new Obj<bool>(this->arr[i]->genom);

			//std::cout << std::endl << best_arr[i]->fitness;
		}
																																	//добавить вероятность выбора случайного в лучшие
		//best_fitness = best_arr[0]->fitness;

		this->best_fitness = this->arr[0]->fitness;
		this->fresh_memory();
	}

	virtual generation<bool,V>* crossover() {
		long long N = this->best_arr[0]->genom_len;
		Obj<bool>** arr_res;
		if (!(arr_res = new Obj<bool>*[this->obj_amount])) throw "\n>memory exeption";
		long long random_amount = (this->obj_amount - this->best_amount)/ this->random_coeff;

		for (long long i = 0; i < this->best_amount; ++i) {
			/*arr_res[i] = best_arr[i];
			best_arr[i] = NULL;*/

			arr_res[i] = new Obj<bool>(this->best_arr[i]->genom);
		}

		for (long long i = this->best_amount; i < random_amount + this->best_amount;++i) { //
			arr_res[i] = create_random_obj(N);
		}

		for (long long i = this->best_amount + random_amount; i < this->obj_amount; ++i) {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dist(0, N-1);
			long long index = dist(gen);
			//long long index = rand()%N;
			std::vector<bool>tmp;
			if (i % 2 == 0) {
				for (long long j = 0; j < index; ++j) {
					tmp.push_back(this->best_arr[(i) % this->best_amount]->genom[j]);
				}
				for (long long j = index; j < N; ++j) {
					tmp.push_back(this->best_arr[(i+1) % this->best_amount]->genom[j]);
				}
			}
			else {
				for (long long j = 0; j < index; ++j) {
					tmp.push_back(this->best_arr[(i + 1) % this->best_amount]->genom[j]);
				}
				for (long long j = index; j < N; ++j) {
					tmp.push_back(this->best_arr[(i) % this->best_amount]->genom[j]);
				}
			}
			mutate(tmp);
			arr_res[i] = new Obj<bool>(tmp);
			//std::cout << std::endl << i << "\t\t" << arr_res[i]->genom_len;
		}

		generation<bool,V>* ptr = new ai1<V>(this->number + 1, arr_res, this->obj_amount);
		/*for (int i = 0; i < obj_amount; ++i) {
			delete arr_res[i];
		}
		delete[]arr_res;*/
		return ptr;
	}

	virtual void work() {
		std::cout << "\ndo something \\_(I_I)_/";
	}

};


int calculate(Obj<bool>* const a) {
	long long answer = 0;
	for (long long i = 0; i < a->get_size(); ++i) {
		answer += static_cast<int>(a->operator[](i));
	}
	return answer;
}

int main() {
	srand(time(NULL));
	const int BEST_AMOUNT = 20;
	int (*f)(Obj<bool>* const);
	f = calculate;
	double timer_start = clock();
	generation<bool,int>* test1 = new ai1<int>(10000, 200, 3);

	while (!_kbhit()) {
		
		double timer = (double)clock();

		test1->train(f);

		test1->select(BEST_AMOUNT);

		generation<bool,int>* test2 = test1->crossover();

		delete test1;

		test1 = test2;

		std::cout << "\t time: " << ((double)clock() - timer) / CLOCKS_PER_SEC << std::endl << "\n======================================================";
	}
	test1->save(BEST_AMOUNT, "output.txt");
	delete test1;
	
	std::cout << std::endl << "total time: "  << ((double)clock() - timer_start) / CLOCKS_PER_SEC << std::endl;

	test1 = new ai1<int>();
	test1->load();
	test1->work();
	delete test1;

}
