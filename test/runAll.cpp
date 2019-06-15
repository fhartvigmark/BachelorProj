#include <gtest/gtest.h>

//Method to setup env variables for OpenMP
void setup(char **argv) {
	char *hasOMPCancel = getenv("OMP_CANCELLATION");

	//If variable is not set, set it and restart program
	if (hasOMPCancel == nullptr) {
		printf("Setting environment variables");
		setenv("OMP_CANCELLATION", "true", 1);

		int output = execvp(argv[0], argv);

		printf("Setting environment variablesfailed with code %d\n",output);
		exit(1);
	}
}

int main(int argc, char **argv) {
	//setup(argv);

  	::testing::InitGoogleTest(&argc, argv);
  	return RUN_ALL_TESTS();
}