#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#define MemoryCell struct _memorycell
#define S_MEMORYCELL sizeof(MemoryCell)
#define S_CHAR sizeof(char)

MemoryCell {
	MemoryCell *next;
	MemoryCell *previous;
	unsigned char value;
};

MemoryCell *nextCell(MemoryCell *current) {
	if (current->next == NULL) {
		current->next = (MemoryCell*)malloc(S_MEMORYCELL);
		current->next->previous = current;
		current->next->next = NULL;
		current->next->value = 0;
	}
	return current->next;
}

MemoryCell *previousCell(MemoryCell *current) {
	if (current->previous == NULL) {
		current->previous = (MemoryCell*)malloc(S_MEMORYCELL);
		current->previous->next = current;
		current->previous->previous = NULL;
		current->previous->value = 0;
	}
	return current->previous;
}

void freeMemory(MemoryCell *current) {
	while (current->next != NULL)
		current = current->next;
	while (current->previous != NULL) {
		current = current->previous;
		free(current->next);
	}
	free(current);
}

int usage(char *self) {
	printf("Usage: %s program.bf\n", self);
	return 1;
}

char *extendString(char *string, char new) {
	int l = strlen(string);
	string = realloc(string, l + 2);
	string[l] = new;
	string[l + 1] = 0;
	return string;
}

int forward(char *source, int i) {
	while (source[i] != ']')
		if (source[i] == '[')
			i = forward(source, i + 1) + 1;
		else
			i++;
	return i;
}

int backward(char *source, int i) {
	while (source[i] != '[')
		if (source[i] == ']')
			i = backward(source, i - 1) - 1;
		else
			i--;
	return i;
}

int main(int argc, char **argv) {
	if (argc != 2)
		return usage(argv[0]);

	char c = 0, *source = malloc(S_CHAR);
	source[0] = 0;
	int i = 0, sourceLength = 0;

	MemoryCell *memory = (MemoryCell*)malloc(S_MEMORYCELL);
	memory->next = NULL;
	memory->previous = NULL;
	memory->value = 0;

	FILE *f = fopen(argv[1], "r");
	while ( (c = fgetc(f)) != EOF )
		source = extendString(source, c);
	fclose(f);
	sourceLength = strlen(source);
	c = 0;

	for (i = 0; i < sourceLength; i++) {
		switch (source[i]) {
			case '>':
				memory = nextCell(memory);
				break;
			case '<':
				memory = previousCell(memory);
				break;
			case '+':
				memory->value++;
				break;
			case '-':
				memory->value--;
				break;
			case '.':
				fputc(memory->value, stdout);
				break;
			case ',':
				c = fgetc(stdin);
				if (c != EOF) {
					memory->value = c;
					c = 0;
				}
				break;
			case '[':
				if (memory->value == 0)
					i = forward(source, ++i);
				break;
			case ']':
				if (memory->value != 0)
					i = backward(source, --i);
				break;
			default:
				break;
		}
		if (c == EOF)
			break;
	}

	freeMemory(memory);
	free(source);

	return 0;
}
