#ifndef CORSAC_BENCHMARK
#define CORSAC_BENCHMARK

#include <vector>
#include <functional>
#include <iostream>
#include <chrono>
#include <string>
#include <algorithm>
#include <numeric>

namespace corsac{
	class BenchmarkManager;

	class Bench
	{
		private:
			std::string name;
			std::vector<unsigned int> amount_operations;
			std::vector<unsigned int> times;

		public: 
			std::function<int()> bench;
			
			Bench(std::string name, std::function<int()> bench)
			{
				this->bench = bench;
				this->name = name;
			}

			void add_time(unsigned int time)
			{
				this->times.push_back(time);
			}

			void add_operation(unsigned int amount_operation)
			{
				this->amount_operations.push_back(amount_operation);
			}

			void start_bench(int amount)
			{
				for(unsigned int c = 0; c < amount; c++)
				{
					std::cout << "\r " << this->name << " time test " << c << "/" << amount;
					auto start = std::chrono::steady_clock::now();
					this->bench();
					auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start);						

					this->add_time(elapsed.count());
				}

			
				unsigned int amount_operation = 0;

				for(unsigned int c = 0; c < amount; c++)
				{
					std::cout << "\r " << this->name << " operations test " << c+1 << "/" << amount;
					auto start = std::chrono::steady_clock::now();
					for(auto end = std::chrono::steady_clock::now();
						std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() < 1000;
						end = std::chrono::steady_clock::now())
					{
						this->bench();
						amount_operation++;
					}
					this->amount_operations.push_back(amount_operation);

				}
				std::cout << "\r                         \r";
			}


			friend class BenchmarkManager;
	};


	class BenchmarkManager
	{
		private:
			std::vector<Bench> benchs;
			unsigned int amount = 100;
		public:
			BenchmarkManager(){}

			BenchmarkManager(unsigned int amount)
			{
				this->amount = amount;
			}

			void set_amount(int amount)
			{
				this->amount = amount;
			}

			void create_bench(std::string name, std::function<int()> bench)
			{
				this->benchs.push_back(Bench(name, bench));
			}

			void start_benchmarks()
			{
				int amount_bench = static_cast<float>(this->benchs.size());
				for(int i = 0; i < this->benchs.size(); i++)
				{
					this->benchs[i].start_bench(this->amount);

					std::sort(
						this->benchs[i].amount_operations.begin(),
						this->benchs[i].amount_operations.end()
						);

					std::sort(
						this->benchs[i].times.begin(),
						this->benchs[i].times.end()
						);
				}
			}

			void print_result()
			{
				for(int i = 0; i < this->benchs.size(); i++)
				{

					unsigned int average_operation = accumulate(	this->benchs[i].amount_operations.begin(), 
																	this->benchs[i].amount_operations.end(), 
																	0)/this->benchs[i].amount_operations.size();

					unsigned int average_time = accumulate(	this->benchs[i].times.begin(), 
															this->benchs[i].times.end(), 
															0)/this->benchs[i].times.size();

					this->benchs[i].amount_operations;
					std::cout << this->benchs[i].name << ": " << std::endl;
					
					std::cout << "   Time:" << std::endl;
					std::cout 	<< "      Min: " 
								<< this->benchs[i].times[0] 
								<< "ns"
								<< std::endl;

					std::cout 	<< "      Max: " 
								<< this->benchs[i].times[this->benchs[i].times.size()-1] 
								<< "ns"
								<< std::endl;

					std::cout 	<< "      Average: " 
								<< average_time 
								<< "ns"
								<< std::endl;

					std::cout 	<< "      Median: " 
								<< this->benchs[i].times[this->benchs[i].times.size()/2-1] 
								<< "ns"
								<< std::endl;



					std::cout << "   Operations:" << std::endl;
					std::cout 	<< "      Min: " 
								<< this->benchs[i].amount_operations[0] 
								<< "op/s"
								<< std::endl;
					
					std::cout 	<< "      Max: " 
								<< this->benchs[i].amount_operations[this->benchs[i].amount_operations.size()-1] 
								<< "op/s"
								<< std::endl;

					std::cout 	<< "      Average: " 
								<< average_operation
								<< "op/s"
								<< std::endl;

					std::cout 	<< "      Median: " 
								<< this->benchs[i].amount_operations[this->benchs[i].amount_operations.size()/2-1]
								<< "op/s"
								<< std::endl;

					

				}
			}
	};

	
}

#endif