#ifdef TEST_ENABLE

#define Testing [=]()mutable

#endif

#ifdef TEST_BLOCK

#define TestBlock [=](Corsac::Block* assert)mutable

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


/*
	@desc - Структура теста.

	@field test 			- Лямбда функция с тестом.
	@field name 			- Имя теста.
	@field comment 			- Комментарий к тесту.
	@field comment_color 	- Цвет комментария.
*/		
namespace Corsac{

	struct Test
	{
		std::function<bool()> test;
		std::string name;
		std::string comment;
		

		int comment_color;
		int block_comment;

		Test(std::string name, std::function<bool()> func)
		{
			this->test = func;
			this->name = name;
		}
	};

	class Block
	{
		private:
			std::string name_block;
			
			std::vector<Test> tests;
			std::vector<Block> blocks;


			int amount_space;
			int comment_color;


			/*
				@desc - Ставит нужное кол-во пробелов.

				@param amount - Кол-во пробелов.
			*/		
			void space_print(int amount){
				for (int i = 0; i <= amount; i++)
				{
					std::cout << " ";
				}
			}

			/*
				@desc - Добавляет новый блок тестов.

				@param name - Имя подблока.
			*/		
			Block* create_block(std::string name)
			{
				this->blocks.push_back(Block(name, this->amount_space+3));

				return &this->blocks[this->blocks.size()-1];
			}

			/*
				@desc - Устанавливает цвет текста.

				@param color - Номер цвета.
			*/		
			void set_color(int color)
			{

				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

				SetConsoleTextAttribute(hConsole, color);
			}


			/*
				@desc - Запускает тесты и подтесты.
			*/
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
			/*
				@param name 	- Имя блока теста.
				@amount_space 	- Задает кол-во отступов.
			*/
			Block(std::string name, int amount_space = 0){
				this->name_block = name;
				this->amount_space = amount_space;
			};


			/*
				@desc - Возвращает кол-во тестов.
			*/		
			int amount_test(){
				int sum = this->tests.size();
				for (int i = 0; i < this->blocks.size(); ++i)
				{
					sum += this->blocks[i].amount_test(); 
				}

				return sum;
			}

			/*
				@desc - Устонавливает базовый цвет текста комментария.

				@param color - Номер цвета в консоли.
			*/		
			void set_comment_color(int color)
			{
				this->comment_color = color;
			}


			/*
				@desc - Добавляет под блок для тестирования.

				@note - Может использовать макрос TestBlock для сокращения записи, 
						для этого надо прописать #define TEST_BLOCK.
				@exemple - assert.add_block("Block1", TestBlock{ 
								assert.equal("Equal", 1, 1);
							});


				@param name - Имя теста.
				@param test - Лямбда функция с набором тестом.
			*/		
			void add_block(std::string name, std::function<void(Block*)> func_block)
			{
				func_block(this->create_block(name));		
			}


			/*
				@desc - Добавляет кастомый тест.

				@note - функция должна возвращать булево значение.
				@note - Может использовать макрос Test для сокращения записи, 
						для этого надо прописать #define TEST_ENABLE.
				@exemple - assert.add("custom test", Test{ 
								...code
							});


				@param name - Имя теста.
				@param test - Лямбда функция с тестом, возвращает булево.
			*/
			Block*  add(std::string name, std::function<bool()> test)
			{
				this->tests.push_back(Test(name, test));
			
				return this;
			}

			/*
				@desc - Сравнивает ссылку двух аргументов.

				@param name 	- Имя теста.
				@param f 		- Ссылка на первый аргумент.
				@param t 		- Ссылка на второй аргумент.
			*/
			template <typename T, typename C>
			Block* equal_ref(std::string name, T* f, C* t){
				this->add(name, [=]()mutable
				{
					if(f == t)
						return true;

					return false;
				});

				return this;
			}

			/*
				@desc - Сравнивает два аргумента.

				@note - Тип аргументов должен иметь перегрузку, чтобы их можно было сравнить

				@param name 	- Имя теста.
				@param f 		- Первый сравниваемый аргумент.
				@param t 		- Второй сравниваемый аргумент.
			*/	
			template <typename T, typename C>
			Block* equal(std::string name, T f, C t){
				this->add(name, [=]()mutable
				{
					if(f == t)
						return true;

					return false;
				});

				return this;
			}

			/*
				@desc - Инвертирует ожидаемый результат теста.
			*/	
			Block* nt()
			{
				auto condition = this->tests[this->tests.size()-1].test;
				this->tests[this->tests.size()-1].test = [=]()mutable{ return !condition();};
			
				return this;
			}

			/*
				@desc - Проверяет на ложь.

				@param name 	- Имя теста.
				@param f 		- Проверяемый аргумент.
			*/	
			template <typename T>
			Block* is_false(std::string name, T f)
			{
				this->add(name, [=]()mutable
				{
					return f == false;
				});
				return this;
			}

			/*
				@desc - Проверяет на правду.

				@param name 	- Имя теста.
				@param f 		- Проверяемый аргумент.
			*/		
			template <typename T>
			Block* is_true(std::string name, T f)
			{
				this->add(name, [=]()mutable
				{
					return f == true;
				});
				return this;
			}

			/*
				@desc - Сравнивает типы элементов.

				@param name 	- Имя теста.
				@param f 		- Первый аргумент.
				@param t 		- Второй аргумент с которым надо сравнить.
			*/
			template <typename T, typename C>
			Block* is(std::string name, T f, C t)
			{
				this->add(name, [=]()mutable
				{
					if(typeid(f).name() == typeid(t).name())
						return true;

					return false;
				});

				return this;
			}


			/*
				@desc - Проверяет контеинер на наличие в нем переданного элемента.

				@param name		- Имя теста.
				@param start 	- Итератор начала массива.
				@param end 		- Итератор конца массива.
				@param search 	- Элемент который ищется в массиве.  
			*/
			template <typename iter, typename T>
			Block* is_contein(std::string name, iter start, iter end, T search)
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


			/*
				@desc - Добавляет комментарий после теста.

				@param comment - Имя теста.  
			*/
			void add_comment(std::string comment)
			{
				this->tests[this->tests.size()-1].comment = comment;
				this->tests[this->tests.size()-1].comment_color = this->comment_color;
			}

			/*
				@desc - Добавляет комментарий после теста.

				@param comment 	- Имя теста.
				@param color 	- Получает номер цвета текста.  
			*/
			void add_comment(std::string comment, int color)
			{
				this->tests[this->tests.size()-1].comment = comment;
				this->tests[this->tests.size()-1].comment_color = color;
			}


			/*
				@desc - Запускает тесты и подтесты.
			*/		
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
}
#endif