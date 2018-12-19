#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <mutex>
#include <cstring>
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

	mutex generatedKeysGuard;
	eastl::unordered_map<eastl::string, bool> generatedKeys;

	mutua::events::QueueEventLink<void, char[_keyLengthMax+1], 0, 8> generatedRandomKeyEvent;


	void produceUniqueKeys(unsigned quantity) {
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

			if (count%102400 == 0) {
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
		constexpr unsigned threadsLength = numberOfProducers;
		thread threads[threadsLength];
		for (unsigned i=0; i<threadsLength; i++) {
			threads[i] = thread(&UniqueRandomKeyGenerator::produceUniqueKeys, this, _quantity/threadsLength);
		}
		for (unsigned i=0; i<threadsLength; i++) {
			threads[i].join();
		}

		generatedRandomKeyEventDispatcher.stopWhenEmpty();
	}


};

template <unsigned _numberOfKeys, typename _keyOutputLambda>
void generateRandomFile(string fileName, string fileHeader, _keyOutputLambda&& keyOutputLambda, string fileFooter) {
	cout << "Generating " << _numberOfKeys << " unique random textual keys for " << fileName << "... " << flush;
	std::ofstream outfile (fileName, std::ofstream::binary);
	outfile << fileHeader;
	UniqueRandomKeyGenerator<4, 16, _numberOfKeys>
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
		[] (ofstream& outfile, unsigned i, char* key) { outfile << "{\"" << key << "\", " << i << "}"; },
		"\n};");

	// MapStdStringKeys.h
	generateRandomFile<8'192'000>("MapStdStringKeys.h",
		"// this file was generated by 'GenerateConstExprData.cpp'\n\n"
		"#include <string>\n"
		"#include <vector>\n"
		"\n"
		"static constexpr std::vector<std::string> MapStdStringKeys = {",
		[] (ofstream& outfile, unsigned i, char* key) { outfile << "\"" << key << "\""; },
		"\n};");

	// MapEASTLStringKeys.h
}
