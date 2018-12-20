#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <mutex>
#include <cstring>
#include <atomic>
using namespace std;


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


#include <QueueEventLink.h>
#include <QueueEventDispatcher.h>
//using namespace mutua::events;

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
	generateRandomFile<2'048'000>("ConstexprRandomUniqueKeys.cpp",
		"// this file was generated by 'GenerateConstExprData.cpp'\n"
		"\n"
		"#include <array>\n"
		"#include <string>\n"
		"#include <EASTL/string.h>\n"
		"\n"
		"namespace ConstexprRandomUniqueKeys {\n"
		"\n"
		"static constexpr std::array<std::string_view, 2'048'000> stdStringKeys = {	// ConstexprRandomUniqueKeys::stdStringKeys",
		[] (ofstream& outfile, unsigned i, const char* key) { outfile << "\"" << key << "\""; },
		"\n};	// ConstexprRandomUniqueKeys::stdStringKeys \n\n",
		"\n"
		"static constexpr std::array<eastl::string_view, 2'048'000> eastlStringKeys = {	// ConstexprRandomUniqueKeys::eastlStringKeys",
		[] (ofstream& outfile, unsigned i, const char* key) { outfile << "\"" << key << "\""; },
		"\n};	// ConstexprRandomUniqueKeys::eastlStringKeys \n\n"
		"}	// namespace ConstexprRandomUniqueKeys");


	// MapEASTLStringKeys.h
}
