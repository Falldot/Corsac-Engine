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
#include <io.h>
#include <fcntl.h>

struct CorsacBlock
{
	std::function<bool()> test;
	std::string name;
	std::string comment;
	int comment_color;

	CorsacBlock(std::string name, std::function<bool()> func)
	{
		this->test = func;
		this->name = name;
	}
};

class CorsacTest
{
	private:
		std::string name_block;
		std::vector<CorsacBlock> tests;
		std::vector<CorsacTest> blocks;
		int amount_space;
		int comment_color;

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
				auto cheked_test = this->tests[i];
				auto result = cheked_test.test();

				this->space_print(this->amount_space+3);

				if(result){
					this->set_color(GREEN);
					std::cout << "\xfb:    ";
				}
				else{
					this->set_color(RED);
					std::cout << "X:    ";
					error++;
				}

				std::cout << cheked_test.name <<std::endl;

				if(cheked_test.comment.size() > 0){

					this->space_print(this->amount_space+4);
					this->set_color(cheked_test.comment_color);
					std::cout << "> " << cheked_test.comment << std::endl;
				}
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

		void set_comment_color(int color)
		{
			this->comment_color = color;
		}

		void add_block(std::string name, std::function<void(CorsacTest*)> func_block)
		{
			func_block(this->create_block(name));		
		}

		CorsacTest*  add(std::string name, std::function<bool()> test)
		{
			this->tests.push_back(CorsacBlock(name, test));
		
			return this;
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

		CorsacTest*  nt()
		{
			auto condition = this->tests[this->tests.size()-1].test;
			this->tests[this->tests.size()-1].test = [=]()mutable{ return !condition();};
		
			return this;
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
		*	@param name		- Имя теста
		*	@param start 	- Итератор начала массива
		*	@param end 		- Итератор конца массива
		*	@paramsearch 	- Элемент который ищется в массиве  
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

		void add_comment(std::string comment)
		{
			this->tests[this->tests.size()-1].comment = comment;
			this->tests[this->tests.size()-1].comment_color = this->comment_color;
		}

		void add_comment(std::string comment, int color)
		{
			this->tests[this->tests.size()-1].comment = comment;
			this->tests[this->tests.size()-1].comment_color = color;
		}

		void start()
		{
			int error = this->start_test();
			this->set_color(GREEN);
			std::cout << " \xfb:    " << this->amount_test()-error << std::endl;
			
			this->set_color(RED);
			std::cout << " X:    " << error << std::endl;

			this->set_color(STANDART);
		}
		
};

#endif