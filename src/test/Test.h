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

		std::vector<std::function<bool()>> tests;
		std::vector<std::string> names;

	public:
		CorsacTest(){};

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

		template <typename T, typename C>
		CorsacTest* is(std::string name, T f, C t){
			this->tests.push_back([=]()mutable{
				if(typeid(f).name() == typeid(t).name())
					return true;

				return false;
			});

			this->names.push_back(name);
			return this;
		}


		/*
		*
		*
		*
		*/
		template <typename iter, typename T>
		CorsacTest* is_contein(std::string name, iter start, iter end, T search)
		{
			this->tests.push_back([=]()mutable{
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

			this->names.push_back(name);
			return this;
		}

		void start()
		{
			unsigned int start_time = clock();
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

			for(auto i = 0; i < this->tests.size(); i++)
			{
				if(this->tests[i]()){
					SetConsoleTextAttribute(hConsole, 10);
					std::cout << "   ok: ";
				}
				else{
					SetConsoleTextAttribute(hConsole, 12);
					std::cout << "   er: ";
				}

				std::cout << this->names[i] <<std::endl;
			}

			unsigned int end_time = clock(); 
			unsigned int search_time = end_time - start_time; 
			SetConsoleTextAttribute(hConsole, 6);
			std::cout << "Test time: " << search_time/1000.0 << std::endl;
		}

};

#endif