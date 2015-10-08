#include "state.h"

bool State::eval(const char *program) {
	assert(program != NULL);

	// while the command is not null
	for (char c = *(program++); c != 0; c = *(program++)) {
		if (!evalChar(c)) return false;
	}

	return true;
}

bool State::evalChar(const char c) {
	if (evalState == CHAR_CODE) {
		push(new StackMember(c));
		evalState = STANDARD;
	} else if ('0' <= c && c <= '9') {
		if (evalState != IN_NUMBER) {
			evalState = IN_NUMBER;
			intValue = 0;
		}

		intValue = intValue * 10 + c - '0';
	} else {
		if (evalState == IN_NUMBER) push(intValue);
		evalState = STANDARD;

		StackMember *top;
		StackMember *second;
		switch (c) {
			case '*':
				top = pop();
				second = pop();
				assert(top->type == INTEGER && second->type == INTEGER);

				push(top->data.integer * second->data.integer);

				delete top;
				delete second;
				break;

			case '.':
				top = pop();
				assert(top->type == INTEGER);
				printf("%i", top->data.integer);
				delete top;
				break;

			case ' ':
				break;

			default:
				error("command %c not recognized\n", c);
				return false;
		}
	}

	return true;
}

void State::push(StackMember *stackMember) {
	stackMember->next = topOfStack;
	topOfStack = stackMember;
}

StackMember *State::pop() {
	assert(topOfStack != NULL);

	StackMember *popped = topOfStack;
	topOfStack = popped->next;
	return popped;
}