#define TEST_BLOCK
#include "Test.h"

using namespace Corsac;

int main()
{
	auto assert = Block("Global Test Block");

	assert.set_comment_color(YELLOW);

	assert.equal("test0", 0, 1);
	assert.equal("test1", 0, 1);
	assert.equal("test2", 0, 1);
	assert.equal("test3", 0, 1);
	assert.equal("test4", 0, 1)->nt();
	assert.equal("test5", 1, 1)->add_comment("Chek Comment");
	assert.equal("test6", 1, 1)->add_comment("Chek Castom color", STANDART);
	assert.equal("test7", 1, 1);

	assert.add_block("Test Block 1", TestBlock{
		assert->equal("test8", 1, 1);
		assert->equal("test9", 1, 0);
		assert->equal("test10", 1, 0);
	});


	assert.start();


	return 0;
}