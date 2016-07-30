#include <iostream>
#include <string>

using namespace std;

char* readFile(string name) {
	FILE* f;
	char* str;
	int size;
	int i = 0;
	int c;

	f = fopen(name.c_str(), "r");
	if (!f) return NULL;

	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);

	str = new char[size + 1];

	while((c = fgetc(f)) != EOF)
		*(str + i++) = (char)c;

	*(str + i) = '\0';

	fclose(f);

	return str;
}

bool writeFile(string name, string data) {
	FILE* f;

	f = fopen(name.c_str(), "w");
	if (!f) return false;

	fwrite(data.c_str(), 1, data.length(), f);
	fclose(f);

	return true;
}
