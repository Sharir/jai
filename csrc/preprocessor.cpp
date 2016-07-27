#include "preprocessor.h"

using namespace std;

#define METHOD_1 0

string processFile(string file)
{
	int len = file.length(), s_len;
	string ret, str;

	size_t start, pos, block_end, temp;
	bool waiting_for_newline = false;
	int newlines = 0;
	
	// IF IN SCOPE OF COMMENT, IGNORE STRING. IF IN STRING, IGNORE COMMENT.
	for (int i = 0 ; i < len ; i++)
	{
		switch (file[i])
		{
		case '"':
			do
			{
				block_end = file.find('"', i + 1);
			} while (file[block_end - 1] == '\\');

			str = file.substr(i, block_end - i + 1);
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

				for (int j = 0; j < newlines ; j++) //add the new lines to the file after squeezing multiline strings and deleting comments so line number in errors would be accurate
					ret += '\n';

				newlines = 0;
			}
			break;

		case '/': //possible comment
			if (i + 1 < len)
			{
				if (file[i + 1] == '/') //one-line comment
				{
					pos = file.find('\n', i + 2);
					if (pos != string::npos)
					{
						i = pos - 1;
						continue;
					}
					else
						return ret;
				}
				else if (file[i + 1] == '*') // "/*" - beggining of comment block
				{
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

		ret += file[i];
	}

	return ret;
}