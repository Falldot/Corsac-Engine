#define TEST_BLOCK
#include "Test.h"


int main()
{
	auto t = CorsacTest("Chek");

	t.equal("test0", 0, 1);
	t.equal("test0", 0, 1);
	t.equal("test0", 0, 1);
	t.equal("test0", 0, 1);
	t.equal("test0", 0, 1);
	t.equal("test0", 1, 1);
	t.equal("test0", 1, 1);
	t.equal("test0", 1, 1);

	t.add_block("test1", TestBlock{
		assert->equal("1 and 1", 1, 1);
		assert->equal("1 and 1", 1, 0);

		assert->equal("1 and 1", 1, 0);
	});


	t.start();


	return 0;
}