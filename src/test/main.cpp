#include "Test.h"

int main()
{
	auto t = CorsacTest("Chek");


	t.equal("test0", 0, 1);

	auto tt = t.add_block("test");
	tt->equal("test1", 1, 1);

	tt->equal("test2", 1, 1);

	t.start();

	return 0;
}