#include "WordFinder.h"
using namespace std;

// Global variables
ofstream program_log;
map <string, string> memoizedMem;
string separatedMemoizedWords,
	   NULL_STRING = " NULL ",
	   dictionaryName = "resources/diction10k.txt";

unordered_set<string> loadDictionary(string filename) {
	// Checks if a word is in the file
        
    string line;
    ifstream Myfile;
	int lineNumber = 0;
    Myfile.open (filename);
	unordered_set<string> dictionary;
	program_log << "		Loading up \"" << filename <<"\" into a dictionary...\n";
	
	// Get the line in the dict
    getline(Myfile, line);
	lineNumber += 1;
	
    if( Myfile.is_open() ) {
        do {
			if (lineNumber > 1) {
				line.pop_back();
			}
			
			// Adding word to the dict
			dictionary.insert(line);
			if ( dictionary.find(line) != dictionary.end() ) {
				program_log << "			Successfully added \"" << line << "\" to the dictionary...\n";
			}
			
			// Get the line in the dict
            getline(Myfile, line);
			lineNumber += 1;
        } while(!Myfile.eof());
        Myfile.close();
		return dictionary;
    }
    else
    program_log<<"Unable to open this file."<<endl;

    return dictionary;
}

string checkForWordsIterative(string phrase, unordered_set<string> dictionary) {
	/*  Checks if a string can be separated with spaces into words
	 	using an iterative bottom-up method
	*/ 
	
	string prefix, suffix, separatedIterativeWords, subString;
	int lenOfPhrase = strlen(phrase.c_str());
	
	if ( dictionary.find(phrase) != dictionary.end() ) {
		return phrase;
	}
	for (int i = 1; i < lenOfPhrase; i++) {
			
		prefix = phrase.substr(0, i);
			
		program_log << "	Checking substring: " << prefix << "...\n";
			
		if ( dictionary.find(prefix) != dictionary.end() ) {
			suffix = phrase.substr(i, lenOfPhrase);
			subString = checkForWordsIterative(suffix, dictionary);
			if (subString != NULL_STRING) {
				separatedIterativeWords = prefix + " " + subString;
				return separatedIterativeWords;
			}
		}
	}
	return NULL_STRING;
}

string checkForWordsMemoized(string phrase, unordered_set<string> dictionary) {
	/* Checks if a string can be separated with spaces into words
	   using the recursive iterative method
	*/
	
	string prefix,
		   suffix,
		   separatedMemoizedWords,
		   subString;
	
	// Check if phrase is a word from the dict
	if ( dictionary.find(phrase) != dictionary.end() ) {
		return phrase;
	}
	// Check if phrase is in memory, if so, return the value of the memory at that key
	else if ( memoizedMem.find(phrase) != memoizedMem.end() ) {
		return memoizedMem[phrase];
	}
	// Loops through every letter in the phrase
	int lenOfPhrase = strlen(phrase.c_str());
	for ( int i = 1; i < lenOfPhrase; i++ ) {
		prefix = phrase.substr(0, i);
		if ( dictionary.find(prefix) != dictionary.end() ) {
			suffix = phrase.substr(i, lenOfPhrase);
			subString = checkForWordsMemoized(suffix, dictionary);
			if (subString != NULL_STRING) {
				separatedMemoizedWords = prefix + " " + subString;
				return separatedMemoizedWords;
			}
		}
	}
	memoizedMem[phrase] = NULL_STRING;
	return NULL_STRING;
}
void runVersion( string version, string line, unordered_set<string> dictionary ) {
	/* Runs the specific version of the checkForWords 
	   function and handles output formatting accordingly
	*/
	string success = "YES, can be split\n",
		   failure = "NO, cannot be split\n\n",
		   sentence;
	cout << version << " attempt:" << "\n";
	program_log << "In " << version << " attempt..\n";
	if ( version == "memoized") {
		sentence = checkForWordsMemoized(line, dictionary);
	}
	if ( version == "iterative") {
		sentence = checkForWordsIterative(line, dictionary);
	}
	if (sentence != NULL_STRING) {
		cout << success << sentence << "\n\n";
	}
	else {
		cout << failure;
	}
}

int main() {
	//  checks the file of strings to see if
	//  each line can be made into a set of words
	
	int strings, result;
	string line;
	
	try {
		cout << "\n";
		
		program_log.open ("program_log.txt");
		program_log << "Reading from standard in...\n\n";
		
		unordered_set<string> dictionary = loadDictionary(dictionaryName);
		
		getline (cin,line);
		stringstream(line) >> strings;
		
		for (int i = 1; i <= strings; i++) {
		
			getline (cin,line);
			
			program_log << "Checking string " << line << " (" << i << " of " << strings << ")...\n\n";
			cout << "phrase number: " << i << "\n" << line << "\n\n";
		
			runVersion("iterative", line, dictionary);
		
			runVersion("memoized", line, dictionary);
		}
		program_log << "Done Running!\n";
		program_log.close();
		return 0;
	}
	catch (...) {
		return 1;
	}
}