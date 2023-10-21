#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <boost/regex.hpp>
using namespace std;
using namespace boost;
void output(regex reg, string s,string content,ofstream & file) {
	file << s << endl;
	for (smatch sm; regex_search(content, sm, reg);)
	{
		//cout << sm.str() << '\n';
		file << sm.str() << '\n';
		content = sm.suffix();
	}
}
void operatorsOutput(regex reg, string s, string content, ofstream& file) {
	file << s << endl;
	for (smatch sm; regex_search(content, sm, reg);)
	{
		//cout << sm.str() << '\n';
		if (sm.str()[0]!='"') file << sm.str() << '\n';
		content = sm.suffix();
	}
}
void run(string content, ofstream &file) {
	regex oneLineComm("//[^\\n\\r]*");
	//regex preprocDir("#[ ]*(define|import|include|elif|else|ifndef|error|if|ifdef|pragma|line|undef|using|endif)");
	//regex libraries("[ ]+<(cassert|cctype|cerrno|cfloat|ciso646|climits|clocale|cmath|csetjmp|csignal|cstdarg|cstddef|cstdio|cstdlib|cstring|ctime|ccomplex|cfenv|cinttypes|cstdalign|cstdbool|cstdint|ctgmath|cwchar|cwctype|algorithm|bitset|complex|deque|exception|fstream|functional|iomanip|ios|iosfwd|iostream|istream|iterator|limits|list|locale|map|memory|new|numeric|ostream|queue|set|sstream|stack|stdexcept|streambuf|string|typeinfo|utility|valarray|vector|array|atomic|chrono|condition_variable|forward_list|future|initializer_list|mutex|random|ratio|regex|scoped_allocator|system_error|thread|tuple|typeindex|type_traits|unordered_map|unordered_set|bits\\/stdc\\+\\+\\.h)>");
	regex javaLibraries("[ ]+java\\.util\\.Scanner|java\\.lang\\.Boolean|java\\.lang\\.Math|java\\.lang\\.Double|java\\.lang\\.System|java\\.lang\\.\\*|java\\.util\\.\\*");
	regex multilineComm("/[*](.|[\\r\\n])*?[*]/");
	//regex floats("(?<!\\w)[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?");
	regex floats("[-+]?(?![xX])([0-9]*\\.[0-9]+|[0-9]+([eE][-+]?[0-9]+)+)");
	//regex integers("[-+]?[0-9]+");
	regex integers("(?<![\\w\\.\\-\\+])[-+]?[0-9]+(?![xX\\.Ee])");
	//regex integers("(?<![(\\.\\d)|(0[xX])])[0-9]+(?![(\\.\\d)|[xX]])");
	regex hexes("0[xX][0-9a-fA-F]+");
	regex reservedWords("\\b(?:abstract|assert|boolean|break|byte|case|catch|char|class|const|continue|default|do|double|else|enum|extends|final|finally|float|for|goto|if|implements|import|instanceof|int|interface|long|native|new|package|private|protected|public|return|short|static|strictfp|super|switch|synchronized|this|throw|throws|transient|try|void|volatile|while)\\b");
	regex operators("\"[^\"\\n]*(<=|>=|\\+\\=|\\-\\=|\\*\\=|\\/\\=|(?<![/*])/(?![/*])|\\=\\=|\\!\\=|\\+\\+|\\-\\-|\\&\\&|\\|\\||\\!|\\+|\\-|\\%|\\=|\\*)[^\"\\n]*\"|(<=|>=|\\+\\=|\\-\\=|\\*\\=|\\/\\=|(?<![/*])/(?![/*])|\\=\\=|\\!\\=|\\+\\+|\\-\\-|\\&\\&|\\|\\||\\!|\\+|\\-|\\%|\\=|\\*)");
	regex brackets("(\\{|\\}|\\(|\\))");
	regex errorsSym("[^A-Z^a-z^0-9^(\\=\\-\\+\\*\\/\\'\"\\{\\}/\\\\\\(\\)\\!\\&\\.\\,\\;\\<\\>\\s\\[\\])\\%\\:]");
	regex missedSemicolon("[^;{\\s}/*]$");
	regex varIdentification("\\b(const|var|float|int|char|boolean|String|byte)[ ]+[a-zA-z][a-zA-z0-9]*[ ]*=[ ]*[^;=\\n]+");
	regex strings("\"([^\"\\\\]|\\\\.)*\"");
	regex chars("'(\\\\[tbnrf'\"\\\\]|\\w|/)'");
	output(oneLineComm, "One line comments:", content, file);
	output(javaLibraries, "Libraries:", content,file);
	output(multilineComm, "Multiline comments:", content,file);
	output(floats, "Floats:", content,file);
	output(integers, "Integers:", content,file);
	output(hexes, "Hexes:", content,file);
	output(reservedWords, "Reserved words:", content,file);
	operatorsOutput(operators, "Operators:", content,file);
	output(brackets, "Brackets:", content,file);
	output(errorsSym, "Wrong symbols:", content,file);
	string temp = regex_replace(content, oneLineComm, "");
	temp = regex_replace(temp, multilineComm, "");
	output(missedSemicolon, "; missed error:", temp,file);
	output(varIdentification, "Variable identification:", content,file);
	output(strings, "Strings:", content,file);
	output(chars, "Chars:", content,file);
}
int main() {
	ifstream file("test.txt");
	ofstream fileout("output.txt");
	stringstream buffer;
	buffer << file.rdbuf();
	string content = buffer.str();
	run(content,fileout);
	file.close();
	
}