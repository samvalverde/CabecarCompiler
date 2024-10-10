#ifndef CPARSER_H
#define CPARSER_H

#include <string>

using namespace std;

namespace ParserLib
{
	class CParser {
		public:
			int Parse(string fileName);
	};
}
#endif

