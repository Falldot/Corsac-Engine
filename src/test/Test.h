#ifdef TEST_ENABLE

#define Test [=]()mutable

#endif

#ifndef CORSAC_TEST
#define CORSAC_TEST

#include <functional>
#include <vector>
#include <windows.h> 
#include <string>
#include <iostream>
#include <ctime>
#include <typeinfo>

class CorsacTest
{
	private:
		std::string name_block;
		std::vector<std::function<bool()>> tests;
		std::vector<std::string> names;
		std::vector<CorsacTest> blocks;
		int amount_space;

		void space_print(int amount){
			for (int i = 0; i <= amount; i++)
			{
				std::cout << " ";
			}
		}

	public:
		CorsacTest(std::string name, int amount_space = 0){
			this->name_block = name;
			this->amount_space = amount_space;
		};

		CorsacTest* add_block(std::string name)
		{
			this->blocks.push_back(CorsacTest(name, this->amount_space+3));

			return &this->blocks[this->blocks.size()-1];
		}

		template <typename T, typename C>
		CorsacTest* equal_ref(std::string name, T* f, C* t){
			this->tests.push_back([=]()mutable{
				if(f == t)
					return true;

				return false;
			});

			this->names.push_back(name);

			return this;
		}

		template <typename T, typename C>
		CorsacTest* equal(std::string name, T f, C t){
			this->tests.push_back([=]()mutable{
				if(f == t)
					return true;

				return false;
			});

			this->names.push_back(name);

			return this;
		}

		void add(std::string name, std::function<bool()> test)
		{
			this->tests.push_back(test);
			this->names.push_back(name);
		}


		void nt()
		{
			auto condition = this->tests[this->tests.size()-1];
			this->tests[this->tests.size()-1] = [=]()mutable{ return !condition();};
		}

		template <typename T>
		CorsacTest* is_false(std::name, T f)
		{
			this->add(name, [=]()mutable
			{
				return f == false;
			});
			return this;
		}

		template <typename T>
		CorsacTest* is_true(std::name, T f)
		{
			this->add(name, [=]()mutable
			{
				return f == true;
			});
			return this;
		}

		template <typename T, typename C>
		CorsacTest* is(std::string name, T f, C t)
		{
			this->add(name, [=]()mutable
			{
				if(typeid(f).name() == typeid(t).name())
					return true;

				return false;
			});
			return this;
		}


		/**
		*	Проверка на наличие элемента в массиве
		*	 
		*	@param name 	- Имя теста
		*	@param @start 	- Итератор начала массива
		*	@param @end 	- Итератор конца массива
		*	@param @search - Элемент который ищется в массиве  
		*/
		template <typename iter, typename T>
		CorsacTest* is_contein(std::string name, iter start, iter end, T search)
		{
			this->add(name, [=]()mutable
			{
				bool find = false;

				for(; start != end; ++start)
				{
					if(*start == search){
						find = true;
						break;
					}
				}
            	return find;
			});	
			return this;
		}

		void start()
		{
			unsigned int start_time = clock();
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


			SetConsoleTextAttribute(hConsole, 6);
			this->space_print(this->amount_space);
			std::cout << this->name_block  << ": "<< std::endl;

			for(auto i = 0; i < this->tests.size(); i++)
			{
				this->space_print(this->amount_space+3);


				if(this->tests[i]()){
					SetConsoleTextAttribute(hConsole, 10);
					std::cout << "ok:		";
				}
				else{
					SetConsoleTextAttribute(hConsole, 12);
					std::cout << "er:		";
				}

				std::cout << this->names[i] <<std::endl;
			}

			for(auto i = 0; i < this->blocks.size(); i++)
			{
				this->blocks[i].start();
			}


			unsigned int end_time = clock(); 
			unsigned int search_time = end_time - start_time; 
			SetConsoleTextAttribute(hConsole, 6);


			this->space_print(this->amount_space);
			std::cout << "Test time: " << search_time/1000.0 << std::endl;
		}
};

#endif