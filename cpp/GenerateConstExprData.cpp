#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <mutex>
#include <cstring>
#include <atomic>
using namespace std;

#include <BetterExceptions.h>

#include <QueueEventLink.h>
#include <QueueEventDispatcher.h>
//using namespace mutua::events;

// EASTL
#include <EASTL/unordered_map.h>
#include <EASTL/vector.h>
#include <EASTL/string.h>


// EASTL new operators
//////////////////////
void* operator new[](size_t size, const char* pName, int flags, unsigned     debugFlags, const char* file, int line) {
    return malloc(size);
}
void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line) {
    return malloc(size);
}


template <unsigned _keyLengthMin, unsigned _keyLengthMax, unsigned _quantity>
class UniqueRandomKeyGenerator {
private:

	const char*     alphabet;
	const unsigned  alphabetLength;

	static constexpr unsigned outputRatio = 102400;	// meaning 1/outputRatio -- every x keys we will output 1 character


	mutex generatedKeysGuard;
	eastl::unordered_map<eastl::string, bool> generatedKeys;

	mutua::events::QueueEventLink<void, char[_keyLengthMax+1], 0, 8> generatedRandomKeyEvent;


	void produceUniqueKeys(unsigned quantity) {
		unsigned instanceId = rand() % outputRatio;	// used to identify when to output, so that all threads will have an even output
		unsigned count = 0;
		unsigned attemptedKeys = 0;
		while (count < quantity) {

			// reserve an slot for the key
			char (*keySlot)[17];
			unsigned eventId = generatedRandomKeyEvent.reserveEventForReporting(keySlot);

			GENERATE_ANOTHER_KEY:

			unsigned keyLength = (rand() % (_keyLengthMax-_keyLengthMin)) + _keyLengthMin;
			for (unsigned i=0; i<keyLength; i++) {
				(*keySlot)[i] = alphabet[rand() % alphabetLength];
			}
			(*keySlot)[keyLength] = '\0';
			eastl::string key((const char*)keySlot);
			attemptedKeys++;

			// validate the key
			{
				std::scoped_lock<std::mutex> lock(generatedKeysGuard);
				if (generatedKeys[key]) {
					// key already exists
					goto GENERATE_ANOTHER_KEY;
				} else {
					generatedKeys[key] = true;
					count++;
				}
			}

			// enqueue the produced key
			generatedRandomKeyEvent.reportReservedEvent(eventId);

			if (count%102400 == instanceId) {
				cout << "." << flush;
			}
		}

	}


public:

	template <typename _keyConsumerCallback>
	UniqueRandomKeyGenerator(unsigned numberOfProducers, const char* alphabet, _keyConsumerCallback&& keyConsumerCallbackLambda)
			: alphabet(alphabet)
			, alphabetLength(strlen(alphabet))
			, generatedKeys(_quantity)
			, generatedRandomKeyEvent("generatedRandomKeyEvent") {

        // set the generic function pointer from the const pointer to lambda
        // (this is OK because our lambda consumer does not change 'this')
        union {
            void (_keyConsumerCallback::*constFuncPtr)   (const char (&) [17]) const;
            void (_keyConsumerCallback::*genericFuncPtr) (const char (&) [17]);
        };
        constFuncPtr = &_keyConsumerCallback::operator();

		generatedRandomKeyEvent.setAnswerlessConsumer(genericFuncPtr, {&keyConsumerCallbackLambda});	// nice pointer to lambda function
		mutua::events::QueueEventDispatcher generatedRandomKeyEventDispatcher(generatedRandomKeyEvent, 1, 0, true, false, true, false, false);

		// generate producers & wait for them to do their work
		thread threads[numberOfProducers];
		for (unsigned i=0; i<numberOfProducers; i++) {
			threads[i] = thread(&UniqueRandomKeyGenerator::produceUniqueKeys, this, _quantity/numberOfProducers);
		}
		for (unsigned i=0; i<numberOfProducers; i++) {
			threads[i].join();
		}

		generatedRandomKeyEventDispatcher.stopWhenEmpty();
	}

	template <typename _keyConsumerCallback>
	void traverseGeneratedKeys(_keyConsumerCallback&& keyConsumerCallbackLambda) {
		unsigned count = 0;
		for (const auto & [key, value] : generatedKeys) {
		    keyConsumerCallbackLambda(key.data());
		    count++;
			if (count%102400 == 0) {
				cout << "." << flush;
			}
		}
	}

};



template <unsigned _numberOfKeys, typename _keyOutputLambda>
void generateRandomFile(string fileName, string fileHeader, _keyOutputLambda&& keyOutputLambda, string fileFooter) {
	cout << "Generating " << _numberOfKeys << " unique random textual keys for '" << fileName << "'... " << flush;
	std::ofstream outfile (fileName, std::ofstream::binary);
	outfile << fileHeader;
	UniqueRandomKeyGenerator<4, 16, _numberOfKeys> keyGenerator
				(std::thread::hardware_concurrency(), "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", [&] (const char (&producedKey)[17]) {
		static unsigned count = 0;
		if (count > 0) {
			outfile << ", ";
		}
		if (count % 4 == 0) {
			outfile << "\n\t";
		}
		keyOutputLambda(outfile, count++, (char*)producedKey);
	});
	outfile << fileFooter;
	outfile.close();
	cout << " Done.\n" << flush;
}

template <unsigned _numberOfKeys,
          typename _keyOutput1Lambda,
		  typename _keyOutput2Lambda>
void generateRandomFile(string fileName,
                        string section1Header, _keyOutput1Lambda&& keyOutput1Lambda, string section1Footer,
						string section2Header, _keyOutput2Lambda&& keyOutput2Lambda, string section2Footer) {

	cout << "Generating " << _numberOfKeys << " unique random textual keys for section 1 of '" << fileName << "'... " << flush;
	std::ofstream outfile (fileName, std::ofstream::binary);
	outfile << section1Header;
	UniqueRandomKeyGenerator<4, 16, _numberOfKeys> keyGenerator
				(std::thread::hardware_concurrency(), "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", [&] (const char (&producedKey)[17]) {
		static unsigned count = 0;
		if (count > 0) {
			outfile << ", ";
		}
		if (count % 4 == 0) {
			outfile << "\n\t";
		}
		keyOutput1Lambda(outfile, count++, (const char*)producedKey);
	});
	outfile << section1Footer;
	cout << " Done.\n" << flush;

	cout << "Reapplying the " << _numberOfKeys << " already generated unique random textual keys for section 2 of '" << fileName << "'... " << flush;
	outfile << section2Header;
	keyGenerator.traverseGeneratedKeys([&](const char* key) {
		static unsigned count = 0;
		if (count > 0) {
			outfile << ", ";
		}
		if (count % 4 == 0) {
			outfile << "\n\t";
		}
		keyOutput2Lambda(outfile, count++, (const char*)key);
	});
	outfile << section2Footer;
	outfile.close();
	cout << " Done.\n" << flush;

}

/** Designed to create static .h files */
void createStaticFile(string fileName, string contents) {
	cout << "Creating static file '" << fileName << "'... " << flush;
	std::ofstream outfile (fileName, std::ofstream::binary);
	outfile << contents;
	outfile.close();
	cout << " Done.\n" << flush;
}

/** Creates a pair of .h/.cpp files with random keys  */
template <unsigned _numberOfKeys, typename _keyOutputLambda>
void createRandomKeysSourceFiles(string hFileName,   string hFileContents,
                                 string cppFileName, string cppFileHeader, _keyOutputLambda&& keyOutputLambda, string cppFileFooter) {

	// h file
	createStaticFile(hFileName, hFileContents);
	// cpp file
	generateRandomFile<_numberOfKeys>(cppFileName, cppFileHeader, keyOutputLambda, cppFileFooter);
}

/** <pre>
 * GenerateConstExprData.cpp
 * =========================
 * created by luiz, Dec 18, 2018
 *
 * Generate the source files containing constexpr test data for 'HashTableSpikes'.
 *
 * The constexpr data needed by 'HashTableSpikes' is so big that, currently, gcc and even clang
 * are not able to generate it at compile time, so, this executable was designed to fill this gap.
*/
int main() {

	cout << "Started GenerateConstExprData with " << std::thread::hardware_concurrency() << " producers and 1 consumer for each file\n";
	cout << "###################################"                  "#"                      "#######################################\n\n" << flush;

	// frozen_test_data.h
	generateRandomFile<2048>("frozen_test_data.h",
		"// this file was generated by 'GenerateConstExprData.cpp'\n\n"
		"static constexpr frozen::unordered_map<frozen::string, int, 2'048> frozenMap = {",
		[] (ofstream& outfile, unsigned i, const char* key) { outfile << "{\"" << key << "\", " << i << "}"; },
		"\n};");

	// ConstexprRandomUniqueKeys.cpp
//	generateRandomFile<2'048'000>("ConstexprRandomUniqueKeys.cpp",
//		"// this file was generated by 'GenerateConstExprData.cpp'\n"
//		"\n"
//		"#include <array>\n"
//		"#include <string>\n"
//		"#include <EASTL/string.h>\n"
//		"\n"
//		"namespace ConstexprRandomUniqueKeys {\n"
//		"\n"
//		"static constexpr unsigned stdStringKeysLength = 2'048'000;\n"
//		"\n"
//		"static constexpr std::array<std::string_view, stdStringKeysLength> stdStringKeys = {	// ConstexprRandomUniqueKeys::stdStringKeys",
//		[] (ofstream& outfile, unsigned i, const char* key) { outfile << "\"" << key << "\""; },
//		"\n};	// ConstexprRandomUniqueKeys::stdStringKeys \n\n",
//		"\n"
//		"static constexpr unsigned eastlStringKeysLength = 2'048'000;\n"
//		"\n"
//		"static constexpr std::array<eastl::string_view, eastlStringKeysLength> eastlStringKeys = {	// ConstexprRandomUniqueKeys::eastlStringKeys",
//		[] (ofstream& outfile, unsigned i, const char* key) { outfile << "\"" << key << "\""; },
//		"\n};	// ConstexprRandomUniqueKeys::eastlStringKeys \n\n"
//		"}	// namespace ConstexprRandomUniqueKeys");


	// ConstexprRandomUniqueStdStringKeys
	/////////////////////////////////////
	#define _ConstexprRandomUniqueStdStringKeysLength 	4'096'000
	createRandomKeysSourceFiles<_ConstexprRandomUniqueStdStringKeysLength>(

		"ConstexprRandomUniqueStdStringKeys.h",
		//////////////////////////////////////
		"// this file was generated by 'GenerateConstExprData.cpp'\n"
		"\n"
		"#include <string>\n"
		"\n"
		"struct ConstexprRandomUniqueStdStringKeys {\n"
		"	static constexpr unsigned                              keysLength = " TO_STR(_ConstexprRandomUniqueStdStringKeysLength) ";\n"
		"	static const std::array<std::string_view, keysLength>  keys;\n"
		"};",

		"ConstexprRandomUniqueStdStringKeys.cpp",
		////////////////////////////////////////
		"// this file was generated by 'GenerateConstExprData.cpp'\n"
		"\n"
		"#include <array>\n"
		"#include <string>\n"
		"#include \"ConstexprRandomUniqueStdStringKeys.h\"\n"
		"\n"
		"static constexpr std::array<std::string_view, ConstexprRandomUniqueStdStringKeys::keysLength> _keys = {",
		[] (ofstream& outfile, unsigned i, const char* key) { outfile << "\"" << key << "\""; },
		"\n};	// _keys \n"
		"\n"
		"const std::array<std::string_view, ConstexprRandomUniqueStdStringKeys::keysLength> ConstexprRandomUniqueStdStringKeys::keys       = _keys;\n");

	#undef _ConstexprRandomUniqueStdStringKeysLength

	// ConstexprRandomUniqueEastlStringKeys
	///////////////////////////////////////
	#define _ConstexprRandomUniqueEastlStringKeysLength 	4'096'000
	createRandomKeysSourceFiles<_ConstexprRandomUniqueEastlStringKeysLength>(

		"ConstexprRandomUniqueEastlStringKeys.h",
		///////////////////////////////////////
		"// this file was generated by 'GenerateConstExprData.cpp'\n"
		"\n"
		"#include <EASTL/string.h>\n"
		"\n"
		"struct ConstexprRandomUniqueEastlStringKeys {\n"
		"	static constexpr unsigned                                keysLength = " TO_STR(_ConstexprRandomUniqueEastlStringKeysLength) ";\n"
		"	static const std::array<eastl::string_view, keysLength>  keys;\n"
		"};",

		"ConstexprRandomUniqueEastlStringKeys.cpp",
		//////////////////////////////////////////
		"// this file was generated by 'GenerateConstExprData.cpp'\n"
		"\n"
		"#include <array>\n"
		"#include <string>\n"
		"#include \"ConstexprRandomUniqueEastlStringKeys.h\"\n"
		"\n"
		"static constexpr std::array<eastl::string_view, ConstexprRandomUniqueEastlStringKeys::keysLength> _keys = {",
		[] (ofstream& outfile, unsigned i, const char* key) { outfile << "\"" << key << "\""; },
		"\n};	// _keys \n"
		"\n"
		"const std::array<eastl::string_view, ConstexprRandomUniqueEastlStringKeys::keysLength> ConstexprRandomUniqueEastlStringKeys::keys       = _keys;\n");

	#undef _ConstexprRandomUniqueEastlStringKeysLength

	// MapEASTLStringKeys.h
}
