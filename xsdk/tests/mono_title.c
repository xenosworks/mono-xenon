#include "mono_test.h"

#ifndef MONO_TEST_NAME
#error MONO_TEST_NAME must name the managed assembly beside the title
#endif

int main(void)
{
	return mono_test_run(MONO_TEST_NAME);
}
