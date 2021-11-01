#define TEST_BLOCK
#include "Test.h"


int main()
{
	auto t = CorsacTest("Chek");


	t.equal("test0", 0, 1);

	t.add_block("test1", Block{
		assert->equal("1 and 1", 1, 1);
	});

	t.start();


	return 0;
}