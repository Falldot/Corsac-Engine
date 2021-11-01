#ifdef TEST_ENABLE

#define Test [=]()mutable

#endif

#ifdef TEST_BLOCK

#define TestBlock [=](CorsacTest* assert)mutable

#endif

#ifndef CORSAC_TEST
#define CORSAC_TEST

#define STANDART 6
#define GREEN 10
#define RED 12
#define YELLOW 14

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

		CorsacTest* create_block(std::string name)
		{

			this->blocks.push_back(CorsacTest(name, this->amount_space+3));

			return &this->blocks[this->blocks.size()-1];
		}

		void set_color(int color)
		{

			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

			SetConsoleTextAttribute(hConsole, color);
		}

		int start_test()
		{
			unsigned int start_time = clock();
			this->set_color(STANDART);
			this->space_print(this->amount_space);
			std::cout << this->name_block  << ": "<< std::endl;

			int error = 0;

			for(auto i = 0; i < this->tests.size(); i++)
			{
				this->space_print(this->amount_space+3);


				auto result = this->tests[i]();

				if(result){
					this->set_color(GREEN);
					std::cout << "ok:    ";
				}
				else{
					this->set_color(RED);
					std::cout << "er:    ";
					error++;
				}

				std::cout << this->names[i] <<std::endl;
			}

			for(auto i = 0; i < this->blocks.size(); i++)
			{
				error += this->blocks[i].start_test();
			}


			unsigned int end_time = clock(); 
			unsigned int search_time = end_time - start_time; 
			this->set_color(YELLOW);


			this->space_print(this->amount_space);
			std::cout << "Test time: " << search_time/1000.0 << std::endl;
			this->set_color(STANDART);
			return error;
		}


	public:
		CorsacTest(std::string name, int amount_space = 0){
			this->name_block = name;
			this->amount_space = amount_space;
		};

		int amount_test(){
			int sum = this->tests.size();
			for (int i = 0; i < this->blocks.size(); ++i)
			{
				sum += this->blocks[i].amount_test(); 
			}

			return sum;
		}

		void add_block(std::string name, std::function<void(CorsacTest*)> func_block)
		{
			func_block(this->create_block(name));		
		}

		template <typename T, typename C>
		CorsacTest* equal_ref(std::string name, T* f, C* t){
			this->add(name, [=]()mutable
			{
				if(f == t)
					return true;

				return false;
			});

			return this;
		}

		template <typename T, typename C>
		CorsacTest* equal_type(std::string name, T f, C t){
			this->add(name, [=]()mutable
			{
				if(typeid(f).name() == typeid(t).name())
					return true;

				return false;
			});

			return this;
		}

		template <typename T, typename C>
		CorsacTest* equal(std::string name, T f, C t){
			this->add(name, [=]()mutable
			{
				if(f == t)
					return true;

				return false;
			});

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
		CorsacTest* is_false(std::string name, T f)
		{
			this->add(name, [=]()mutable
			{
				return f == false;
			});
			return this;
		}

		template <typename T>
		CorsacTest* is_true(std::string name, T f)
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
		*	@param name	- Имя теста
		*	@start 	- Итератор начала массива
		*	@end 	- Итератор конца массива
		*	@search - Элемент который ищется в массиве  
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
			int error = this->start_test();
			this->set_color(GREEN);
			std::cout << " ok:    " << this->amount_test()-error << std::endl;
			
			this->set_color(RED);
			std::cout << " er:    " << error << std::endl;

			this->set_color(STANDART);
		}
		
};

#endif