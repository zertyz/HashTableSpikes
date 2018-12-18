#include <iostream>
#include <random>
#include <mutex>
using namespace std;


// EASTL
#include <EASTL/unordered_map.h>
#include <EASTL/vector.h>
#include <EASTL/string.h>

#include <QueueEventLink.h>
#include <QueueEventDispatcher.h>
//using namespace mutua::events;

template <char* _alphabet, unsigned _keyLengthMin, unsigned _keyLengthMax, unsigned _quantity>
class UniqueRandomKeyGenerator {
private:

	auto keyConsumerCallback;

	mutex generatedKeysGuard;
	eastl::unordered_map<eastl::string, bool> generatedKeys;

	mutua::events::QueueEventLink<void, char[_keyLengthMax+1], 0, 8> generatedRandomKeyEvent;
	mutua::events::QueueEventDispatcher<generatedRandomKeyEvent>     generatedRandomKeyEventDispatcher;


	void produceUniqueKeys() {
		eastl::string      str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
		unsigned           alphabetLength = str.length();
		std::random_device rd;
		std::mt19937       generator(rd());
		unsigned count = 0;
		unsigned i = 0;
		while (count < _quantity) {

			// reserve an slot for the key

			GENERATE_ANOTHER_KEY:

			unsigned keyLength = (i % (_keyLengthMax-_keyLengthMin)) + _keyLengthMin;
			unsigned begin     = (i % (alphabetLength-keyLength));
			std::shuffle(str.begin()+begin, str.begin()+keyLength, generator);
			eastl::string key = str.substr(begin, keyLength);
			i++;

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

			if (count%102400 == 0) {
				cout << "." << flush;
			}
		}

	}

	// answerless event consumer -- just call the lambda 'keyConsumerCallback'
	void onGeneratedRandomKey(char *key) {
		keyConsumerCallback(key);
	}


public:

	UniqueRandomKeyGenerator(unsigned numberOfProducers, auto keyConsumerCallback)
			: keyConsumerCallback(keyConsumerCallback)
			, generatedKeys(_quantity)
			, generatedRandomKeyEvent("generatedRandomKeyEvent", &UniqueRandomKeyGenerator::onGeneratedRandomKey, {this})
			, generatedRandomKeyEventDispatcher(generatedRandomKeyEvent, 1, 0, true, false, true, false, false) {

		cout << "Generating " << _quantity << " random keys... " << flush;

		// generate producers & wait for them to do their work
		constexpr unsigned threadsLength = 4;
		thread threads[threadsLength];
		for (unsigned i=0; i<threadsLength; i++) {
			threads[i] = thread(&UniqueRandomKeyGenerator::produceUniqueKeys);
		}
		for (unsigned i=0; i<threadsLength; i++) {
			threads[i].join();
		}

		cout << "\n\n";
	}


};

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

}
