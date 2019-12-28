#include <iostream>
#include "header_declare.h"

int main() {

	int recieve_value;
	int error_question = 0;

	error_question = CONNECTOR();
	
	if (error_question == 1) return 1;

	INTERFAACE_DISPLAY();

	while (1) {
		recieve_value = Repetitional_function();
		if (recieve_value == 1) {
			break;
		}
	}

	return 0;
}



