#include "preprocessor.h"

using namespace std;

int count_occurences(string str, char ch, size_t beg = 0, size_t s_end = -1)
{
	int n = 0;

	if (s_end == -1)
		s_end = str.length();

	for (unsigned int i = beg ; i < s_end ; i++)
	{
		if (str[i] == ch)
			n++;
	}

	return n;
}

string prepare(string src)
{
	int len = src.length(), s_len;
	string ret, str;

	size_t start, pos, block_end, temp;
	bool waiting_for_newline = false;
	int newlines = 0;
	
	// TODO: support all newline types ('\r\n') - Windows
	for (int i = 0 ; i < len ; i++)
	{
		switch (src[i])
		{
		case '"':
			do
			{
				block_end = src.find('"', i + 1);
			} while (src[block_end - 1] == '\\');

			str = src.substr(i, block_end - i + 1);
			s_len = str.length();

			newlines = 0;
			for (int k = 0 ; k < s_len ; k++)
			{
				if (str[k] == '\n')
				{
					newlines++;
				}
			}
			waiting_for_newline = true;

			str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
			str.erase(std::remove(str.begin(), str.end(), '\t'), str.end());

			ret += str;
			i = block_end;
			continue;

		case '\n':
			if (waiting_for_newline)
			{
				waiting_for_newline = false;

				for (int j = 0 ; j < newlines ; j++) //add the new lines to the src after squeezing multiline strings and deleting comments so line number in errors would be accurate
					ret += '\n';

				newlines = 0;
			}
			break;

		case '/': //possible comment
			if (i + 1 < len)
			{
				if (src[i + 1] == '/') //one-line comment
				{
					pos = src.find('\n', i + 2);
					if (pos != string::npos)
					{
						i = pos - 1;
						continue;
					}
					else
						return ret;
				}
				else if (src[i + 1] == '*') // "/*" - beggining of comment block
				{
					pos = i;
					block_end = src.find("*/", pos + 2);

					if (block_end == string::npos)
					{
						printf("Preprocessor error: comment block is not terminated");
						exit(1);
					}

					pos = src.find("/*", pos + 2);
					while(pos != string::npos && pos < block_end)
					{
						block_end = src.find("*/", block_end + 2);

						if (block_end == string::npos)
						{
							printf("Preprocessor error: comment block is not terminated");
							exit(1);
						}

						pos = src.find("/*", pos + 2);
					}
					
					newlines = count_occurences(src, '\n', i + 2, block_end);
					waiting_for_newline = true;

					i = block_end + 1;
					continue;

					//algorithm idea - very basic:
					/*
						start - start of whole block.
						pos - start of current block (will be different than start in nested comment blocks).
						block_end - end of whole block (will be different through the run of the loop until absolute end is found).

						start = i;
						pos = i;
						block_end = find first occurence of "* /" (without the space);

						while ( find "/*" between pos and block_end == found)
						{
							pos = found;
							block_end = find "* /" from block_end + 2 and on
						}

						after everything - just i = block_end + 2
					*/
				}
			}
			break;
		}

		ret += src[i];
	}

	return ret;
}

string processFile(string src)
{
	string ret;
	
	ret = prepare(src); //inline multi-line strings & remove comments and blocks

	return ret;
}
