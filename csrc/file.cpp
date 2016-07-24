#include <iostream>
#include <string>

using namespace std;

string readFile(string name) {
	FILE* f;
	char* str;
	int size;
	int i = 0;

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

	string ret(str);
	free(str);
	return ret;
}

bool writeFile(string name, string data) {
	cout << name << "\n";
	FILE* f;

	f = fopen(name.c_str(), "w");
	if (!f) return false;

	fwrite(data.c_str(), 1, data.length(), f);
	fclose(f);

	return true;
}
