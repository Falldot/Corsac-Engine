#define TEST_BLOCK
#define CORSAC_TEST_ONLY_RESULT
#define CORSAC_TEST_TIME_OFF

#include "Test.h"


int main()
{
	auto assert = Corsac::Block("Global Test Block");

	assert.set_comment_color(YELLOW);

	assert.equal("test0", 0, 1);
	assert.equal("test1", 0, 1);
	assert.equal("test2", 0, 1);
	assert.equal("test3", 0, 1);
	assert.equal("test4", 0, 1)->nt();
	assert.equal("test5", 1, 1)->add_comment("Chek Comment");
	assert.equal("test6", 1, 1)->add_comment("Chek Castom color", STANDART);
	assert.equal("test7", 1, 1);

	assert.add_block("Test Block 1", TestingBlock{
		assert->equal("test8", 1, 1);
		assert->equal("test9", 1, 0);
		assert->equal("test10", 1, 0);

		assert->add_block_comment("32142142");
	});

	assert.add_block("Test Block 1", TestingBlock{
		assert->equal("test8", 1, 1);
		assert->equal("test9", 1, 0);
		assert->equal("test10", 1, 0);
		assert->equal("test8", 1, 1);
		assert->equal("test9", 1, 0);
		assert->equal("test10", 1, 0);
		assert->equal("test8", 1, 1);
		assert->equal("test9", 1, 0);
		assert->equal("test10", 1, 0);

		assert->add_block_comment("32142142");
	});
	assert.add_block("Test Block 1", TestingBlock{

		assert->add_block_comment("32142142");
	});

assert.add_block("Test Block 1", TestingBlock{
	});

assert.add_block("Test Block 1", TestingBlock{
		assert->equal("test8", 1, 1);
		assert->equal("test9", 1, 0);
		assert->equal("test10", 1, 0);
		assert->equal("test8", 1, 1);
		assert->equal("test9", 1, 0);
		assert->equal("test10", 1, 0);
		assert->equal("test8", 1, 1);
		assert->equal("test9", 1, 0);
		assert->equal("test10", 1, 0);

		assert->add_block_comment("32142142");
	});



	assert.start();

	return 0;
}