#include <iostream>
#include <map>
#include <unordered_map>
#include <thread>
#include <random>
#include <algorithm>

#include <mutex>
#define synchronized(m)     for(std::unique_lock<std::recursive_mutex> lk(m); lk; lk.unlock())

#include <boost/test/unit_test.hpp>

#include <MutuaTestMacros.h>
#include <BetterExceptions.h>
#include <TimeMeasurements.h>
using namespace mutua::cpputils;

#include <AlgorithmComplexityAndReentrancyAnalysis.h>
#include <SplitRun.h>
using namespace mutua::testutils;

// ska
#include "flat_hash_map/unordered_map.hpp"
#include "flat_hash_map/bytell_hash_map.hpp"

// EASTL
#include <EASTL/unordered_map.h>
#include <EASTL/vector.h>
#include <EASTL/string.h>

// BBHash (BB's minimum perfect hash function)
#include "BBHash/MinimumPerfectMap.h"


#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE HashTableSpikes
#include <boost/test/unit_test.hpp>

struct TestCaseObjects {
    int x = 0;
    TestCaseObjects() {
        BOOST_TEST_MESSAGE("Setting up before test testing objects");
    	cerr << "Starting up before test..." << endl;
    }
    ~TestCaseObjects() {
        BOOST_TEST_MESSAGE("Tearing down after test testing objects");
    	cerr << "Finishing down after test..." << endl;
    }
};

struct TestSuiteObjects {
    int x = 0;
    TestSuiteObjects() {
        //BOOST_TEST_MESSAGE("Setting up before class testing objects");
    	cerr << "Starting up before class..." << endl;
    }
    ~TestSuiteObjects() {
        //BOOST_TEST_MESSAGE("Tearing down after class testing objects");
    	cerr << "Finishing down after class..." << endl;
    }
};

BOOST_GLOBAL_FIXTURE(TestSuiteObjects);
BOOST_FIXTURE_TEST_SUITE(testSuiteName, TestCaseObjects);

BOOST_AUTO_TEST_CASE(testCase1) {
	cerr << "Just running testCase1" << endl;
    x = 1;
    BOOST_CHECK(x == 1);
}
BOOST_AUTO_TEST_CASE(testCase2) {
	cerr << "Just running testCase2" << endl;
    BOOST_CHECK(x == 1);
    BOOST_CHECK_EQUAL(2, x);
}

BOOST_AUTO_TEST_SUITE_END()

void* operator new[](size_t size, const char* pName, int flags, unsigned     debugFlags, const char* file, int line)
{
    return malloc(size);
}

void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
    return malloc(size);
}

// test & exploration functions
///////////////////////////////

void hashTableExperiments() {

    cout << endl << endl;
    cout << "Hash Table Experiments:" << endl;
    cout << "====================== " << endl << endl;

    #define _numberOfElements 2'048'000
    #define _threads          4

    //setHeapTraceBaseline();

    cout << "Generating keys... " << flush;
    std::vector<string> keys = std::vector<string>(_numberOfElements);
    std::string         str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
    std::random_device  rd;
    std::mt19937        generator(rd());
    for (int i=0; i<_numberOfElements; i++) {
        std::shuffle(str.begin(), str.end(), generator);
        string key = str.substr(0, 16);
        keys[i]    = key;
        if (i%102400 == 0) {
            cout << "." << flush;
        }
    }

    cout << endl << endl << flush;

    //getHeapTraceInfo("Random keys allocation costs");

    class StandardMapStringIndexExperiments: public AlgorithmComplexityAndReentrancyAnalysis {
    public:
        std::vector<string>&  keys;
        std::map<string, int> map;
        std::mutex            writeGuard;
        std::mutex*           readGuard;

        StandardMapStringIndexExperiments(std::vector<string>& keys)
                : AlgorithmComplexityAndReentrancyAnalysis("StandardMapStringIndexExperiments", _numberOfElements)
        		, readGuard(nullptr)
                , keys(keys) {
            map  = std::map<string, int>();
        }

        void resetTables(EResetOccasion occasion) override {
            map.clear();
        }

        // algorithms under analysis & test
        ///////////////////////////////////

        void insertAlgorithm(unsigned int i) override {
            std::lock_guard<std::mutex> lock(writeGuard);
        	readGuard = &writeGuard;
            map[keys[i]] = ((int)i);
            readGuard = nullptr;
        }

        void selectAlgorithm(unsigned int i) override {
        	if (readGuard != nullptr) std::lock_guard<std::mutex> lock(*readGuard);
            if (map[keys[i]] != ((int)i)) {
                cerr << "Select: item #" << i << ", on the insert phase, should be " << ((int)i) << " but is " << map[keys[i]] << endl << flush;
            }
        }

        void updateAlgorithm(unsigned int i) override {
            std::lock_guard<std::mutex> lock(writeGuard);
        	readGuard = &writeGuard;
            map[keys[i]] = -((int)i);
            readGuard = nullptr;
        }

        void deleteAlgorithm(unsigned int i) override {
            std::lock_guard<std::mutex> lock(writeGuard);
        	readGuard = &writeGuard;
            int value = map[keys[i]];
            map.erase(keys[i]);
            if (value != -((int)i)) {
                cerr << "Delete: item #" << i << ", on the update phase, should be " << -((int)i) << " but was " << value << endl << flush;
            }
            readGuard = nullptr;
        }
    };
    //setHeapTraceBaseline();
    {
        StandardMapStringIndexExperiments standardMapStringIndexExperiments = StandardMapStringIndexExperiments(keys);
        // reentrancy tests
        for (int i=11; i<=10; i++) {
            if (i%10 == 0) {
                standardMapStringIndexExperiments.testReentrancy(_numberOfElements, true);
//                getHeapTraceInfo("StandardMapStringIndexExperiments first pass allocation costs");
            } else {
                standardMapStringIndexExperiments.testReentrancy(_numberOfElements, false);
                cout << "." << flush;
            }
        }
        // complexity analysis
        for (int i=11; i<=10; i++) {
            if (i%10 == 0) {
                standardMapStringIndexExperiments.analyseComplexity(false, _threads, _threads, _threads, _threads, true);
            } else {
                standardMapStringIndexExperiments.analyseComplexity(false, _threads, _threads, _threads, _threads, false);
                cout << "." << flush;
            }
        }
    }
//    getHeapTraceInfo("StandardMapStringIndexExperiments total allocation costs");

    class UnorderedMapStringIndexExperiments: public AlgorithmComplexityAndReentrancyAnalysis {
    public:
        std::vector<string>&            keys;
        std::unordered_map<string, int> map;
        std::mutex                      writeGuard;
        std::mutex*                     readGuard;

        UnorderedMapStringIndexExperiments(std::vector<string>& keys)
                : AlgorithmComplexityAndReentrancyAnalysis("UnorderedMapStringIndexExperiments", _numberOfElements)
				, readGuard(nullptr)
                , keys(keys) {
            map  = std::unordered_map<string, int>();
        }

        void resetTables(EResetOccasion occasion) override {
            map.clear();
        }

        // algorithms under analysis & test
        ///////////////////////////////////

        void insertAlgorithm(unsigned int i) override {
            std::lock_guard<std::mutex> lock(writeGuard);
        	readGuard = &writeGuard;
            map[keys[i]] = ((int)i);
            readGuard = nullptr;
        }

        void selectAlgorithm(unsigned int i) override {
        	if (readGuard != nullptr) std::lock_guard<std::mutex> lock(*readGuard);
            if (map[keys[i]] != ((int)i)) {
                cerr << "Select: item #" << i << ", on the insert phase, should be " << ((int)i) << " but is " << map[keys[i]] << endl << flush;
            }
        }

        void updateAlgorithm(unsigned int i) override {
            std::lock_guard<std::mutex> lock(writeGuard);
        	readGuard = &writeGuard;
            map[keys[i]] = -((int)i);
            readGuard = nullptr;
        }

        void deleteAlgorithm(unsigned int i) override {
            std::lock_guard<std::mutex> lock(writeGuard);
        	readGuard = &writeGuard;
            int value = map[keys[i]];
            map.erase(keys[i]);
            if (value != -((int)i)) {
                cerr << "Delete: item #" << i << ", on the update phase, should be " << -((int)i) << " but was " << value << endl << flush;
            }
            readGuard = nullptr;
        }
    };
//    setHeapTraceBaseline();
    {
        UnorderedMapStringIndexExperiments unorderedMapStringIndexExperiments = UnorderedMapStringIndexExperiments(keys);
        // reentrancy tests
        for (int i=11; i<=10; i++) {
            if (i%10 == 0) {
                unorderedMapStringIndexExperiments.testReentrancy(_numberOfElements, true);
//                getHeapTraceInfo("UnorderedMapStringIndexExperiments first pass allocation costs");
            } else {
                unorderedMapStringIndexExperiments.testReentrancy(_numberOfElements, false);
                cout << "." << flush;
            }
        }
        // complexity analysis
        for (int i=11; i<=10; i++) {
            if (i%10 == 0) {
                unorderedMapStringIndexExperiments.analyseComplexity(false, _threads, _threads, _threads, _threads, true);
            } else {
                unorderedMapStringIndexExperiments.analyseComplexity(false, _threads, _threads, _threads, _threads, false);
                cout << "." << flush;
            }
        }
    }
//    getHeapTraceInfo("UnorderedMapStringIndexExperiments total allocation costs");

    class SkaUnorderedMapStringIndexExperiments: public AlgorithmComplexityAndReentrancyAnalysis {
    public:
        std::vector<string>&            keys;
        ska::unordered_map<string, int> map;
        std::mutex                      writeGuard;
        std::mutex*                     readGuard;

        SkaUnorderedMapStringIndexExperiments(std::vector<string>& keys)
                : AlgorithmComplexityAndReentrancyAnalysis("SkaUnorderedMapStringIndexExperiments", _numberOfElements)
        		, readGuard(nullptr)
                , keys(keys) {
            map  = ska::unordered_map<string, int>();
        }

        void resetTables(EResetOccasion occasion) override {
            map.clear();
        }

        // algorithms under analysis & test
        ///////////////////////////////////

        void insertAlgorithm(unsigned int i) override {
            std::lock_guard<std::mutex> lock(writeGuard);
        	readGuard = &writeGuard;
            map[keys[i]] = ((int)i);
            readGuard = nullptr;
        }

        void selectAlgorithm(unsigned int i) override {
        	if (readGuard != nullptr) std::lock_guard<std::mutex> lock(*readGuard);
            if (map[keys[i]] != ((int)i)) {
                cerr << "Select: item #" << i << ", on the insert phase, should be " << ((int)i) << " but is " << map[keys[i]] << endl << flush;
            }
        }

        void updateAlgorithm(unsigned int i) override {
            std::lock_guard<std::mutex> lock(writeGuard);
        	readGuard = &writeGuard;
            map[keys[i]] = -((int)i);
            readGuard = nullptr;
        }

        void deleteAlgorithm(unsigned int i) override {
            std::lock_guard<std::mutex> lock(writeGuard);
        	readGuard = &writeGuard;
            int value = map[keys[i]];
            map.erase(keys[i]);
            if (value != -((int)i)) {
                cerr << "Delete: item #" << i << ", on the update phase, should be " << -((int)i) << " but was " << value << endl << flush;
            }
            readGuard = nullptr;
        }
    };
//    setHeapTraceBaseline();
    {
        SkaUnorderedMapStringIndexExperiments skaUnorderedMapStringIndexExperiments = SkaUnorderedMapStringIndexExperiments(keys);
        // reentrancy tests
        for (int i=11; i<=10; i++) {
            if (i%10 == 0) {
                skaUnorderedMapStringIndexExperiments.testReentrancy(_numberOfElements, true);
//                getHeapTraceInfo("SkaUnorderedMapStringIndexExperiments total allocation costs");
            } else {
                skaUnorderedMapStringIndexExperiments.testReentrancy(_numberOfElements, false);
                cout << "." << flush;
            }
        }
        // complexity analysis
        for (int i=11; i<=10; i++) {
            if (i%10 == 0) {
                skaUnorderedMapStringIndexExperiments.analyseComplexity(false, _threads, _threads, _threads, _threads, true);
            } else {
                skaUnorderedMapStringIndexExperiments.analyseComplexity(false, _threads, _threads, _threads, _threads, false);
                cout << "." << flush;
            }
        }
    }
//    getHeapTraceInfo("SkaUnorderedMapStringIndexExperiments total allocation costs");

    class SkaByteLLMapStringIndexExperiments: public AlgorithmComplexityAndReentrancyAnalysis {
    public:
        std::vector<string>&              keys;
        ska::bytell_hash_map<string, int> map;
        std::mutex                        writeGuard;
        std::mutex*                       readGuard;

        SkaByteLLMapStringIndexExperiments(std::vector<string>& keys)
                : AlgorithmComplexityAndReentrancyAnalysis("SkaByteLLMapStringIndexExperiments", _numberOfElements, _numberOfElements, _numberOfElements)
        		, readGuard(nullptr)
                , keys(keys) {
            map  = ska::bytell_hash_map<string, int>();
        }

        void resetTables(EResetOccasion occasion) override {
            map.clear();
        }

        // algorithms under analysis & test
        ///////////////////////////////////

        void insertAlgorithm(unsigned int i) override {
            std::lock_guard<std::mutex> lock(writeGuard);
        	readGuard = &writeGuard;
            map[keys[i]] = ((int)i);
            readGuard = nullptr;
        }

        void selectAlgorithm(unsigned int i) override {
        	if (readGuard != nullptr) std::lock_guard<std::mutex> lock(*readGuard);
            if (map[keys[i]] != ((int)i)) {
                cerr << "Select: item #" << i << ", on the insert phase, should be " << ((int)i) << " but is " << map[keys[i]] << endl << flush;
            }
        }

        void updateAlgorithm(unsigned int i) override {
            std::lock_guard<std::mutex> lock(writeGuard);
        	readGuard = &writeGuard;
            map[keys[i]] = -((int)i);
            readGuard = nullptr;
        }

        void deleteAlgorithm(unsigned int i) override {
            std::lock_guard<std::mutex> lock(writeGuard);
        	readGuard = &writeGuard;
            int value = map[keys[i]];
            map.erase(keys[i]);
            if (value != -((int)i)) {
                cerr << "Delete: item #" << i << ", on the update phase, should be " << -((int)i) << " but was " << value << endl << flush;
            }
            readGuard = nullptr;
        }
    };
//    setHeapTraceBaseline();
    {
        SkaByteLLMapStringIndexExperiments skaByteLLMapStringIndexExperiments = SkaByteLLMapStringIndexExperiments(keys);
        // reentrancy tests
        for (int i=11; i<=10; i++) {
            if (i%10 == 0) {
                skaByteLLMapStringIndexExperiments.testReentrancy(_numberOfElements, true);
//                getHeapTraceInfo("SkaByteLLMapStringIndexExperiments total allocation costs");
            } else {
                skaByteLLMapStringIndexExperiments.testReentrancy(_numberOfElements, false);
                cout << "." << flush;
            }
        }
        // complexity analysis
        for (int i=10; i<=10; i++) {
            if (i%10 == 0) {
                skaByteLLMapStringIndexExperiments.analyseComplexity(false, _threads, _threads, _threads, _threads, true);
            } else {
                skaByteLLMapStringIndexExperiments.analyseComplexity(false, _threads, _threads, _threads, _threads, false);
                cout << "." << flush;
            }
        }
    }
//    getHeapTraceInfo("SkaByteLLMapStringIndexExperiments total allocation costs");

    class EastlUnorderedMapStringIndexExperiments: public AlgorithmComplexityAndReentrancyAnalysis {
    public:
        eastl::vector<eastl::string>             keys;
        eastl::unordered_map<eastl::string, int> map;
        std::mutex                               writeGuard;
        std::mutex*                              readGuard;

        EastlUnorderedMapStringIndexExperiments(std::vector<string>& keys)
                : AlgorithmComplexityAndReentrancyAnalysis("EastlUnorderedMapStringIndexExperiments", _numberOfElements, _numberOfElements, _numberOfElements)
        		, readGuard(nullptr) {
        	this->keys = eastl::vector<eastl::string>(keys.size());
        	for (int i=0; i<keys.size(); i++) {
        		this->keys[i] = eastl::string(keys[i].c_str());
        	}
            map  = eastl::unordered_map<eastl::string, int>();
        }

        void resetTables(EResetOccasion occasion) override {
            map.clear();
        }

        // algorithms under analysis & test
        ///////////////////////////////////

        void insertAlgorithm(unsigned int i) override {
            std::lock_guard<std::mutex> lock(writeGuard);
        	readGuard = &writeGuard;
            map[keys[i]] = ((int)i);
            readGuard = nullptr;
        }

        void selectAlgorithm(unsigned int i) override {
        	if (readGuard != nullptr) std::lock_guard<std::mutex> lock(*readGuard);
            if (map[keys[i]] != ((int)i)) {
                cerr << "Select: item #" << i << ", on the insert phase, should be " << ((int)i) << " but is " << map[keys[i]] << endl << flush;
            }
        }

        void updateAlgorithm(unsigned int i) override {
            std::lock_guard<std::mutex> lock(writeGuard);
        	readGuard = &writeGuard;
            map[keys[i]] = -((int)i);
            readGuard = nullptr;
        }

        void deleteAlgorithm(unsigned int i) override {
            std::lock_guard<std::mutex> lock(writeGuard);
        	readGuard = &writeGuard;
            int value = map[keys[i]];
            map.erase(keys[i]);
            if (value != -((int)i)) {
                cerr << "Delete: item #" << i << ", on the update phase, should be " << -((int)i) << " but was " << value << endl << flush;
            }
            readGuard = nullptr;
        }
    };
//    setHeapTraceBaseline();
    {
    	EastlUnorderedMapStringIndexExperiments eastlUnorderedMapStringIndexExperiments = EastlUnorderedMapStringIndexExperiments(keys);
        // reentrancy tests
        for (int i=11; i<=10; i++) {
            if (i%10 == 0) {
            	eastlUnorderedMapStringIndexExperiments.testReentrancy(_numberOfElements, true);
//                getHeapTraceInfo("EastlUnorderedMapStringIndexExperiments total allocation costs");
            } else {
            	eastlUnorderedMapStringIndexExperiments.testReentrancy(_numberOfElements, false);
                cout << "." << flush;
            }
        }
        // complexity analysis
        for (int i=10; i<=10; i++) {
            if (i%10 == 0) {
            	eastlUnorderedMapStringIndexExperiments.analyseComplexity(false, _threads, _threads, _threads, _threads, true);
            } else {
            	eastlUnorderedMapStringIndexExperiments.analyseComplexity(false, _threads, _threads, _threads, _threads, false);
                cout << "." << flush;
            }
        }
    }
//    getHeapTraceInfo("EastlUnorderedMapStringIndexExperiments total allocation costs");

#undef _numberOfElements
#undef _threads
}


// Memory footprint test cases
//////////////////////////////

#define DECLARE_MAP_ALGORITHM_ANALYSIS_AND_REENTRANCY_TEST_CLASS(_className, _mapType, _vectorType, _keyType, _outputFunction, _mapInitExpr)  \
    class _className: public AlgorithmComplexityAndReentrancyAnalysis {                                                          \
    public:                                                                                                                      \
		_vectorType<_keyType>&              keys;                                                                                \
        _mapType<_keyType, int>             map;                                                                                 \
        std::mutex                          writeGuard;                                                                          \
        std::mutex*                         readGuard;                                                                           \
                                                                                                                                 \
        _className(_vectorType<_keyType>& keys)                                                                                  \
                : AlgorithmComplexityAndReentrancyAnalysis(#_className, _numberOfElements, _numberOfElements, _numberOfElements) \
                , keys(keys)                                                                                                     \
                , readGuard(nullptr) {                                                                                           \
            map  = _mapType<_keyType, int>(_mapInitExpr);                                                                       \
        }                                                                                                                        \
                                                                                                                                 \
        void resetTables(EResetOccasion occasion) override {                                                                     \
            map.clear();                                                                                                         \
        }                                                                                                                        \
                                                                                                                                 \
        /* algorithms under analysis & test */                                                                                   \
        /* //////////////////////////////// */                                                                                   \
                                                                                                                                 \
        void insertAlgorithm(unsigned int i) override {                                                                          \
            std::lock_guard<std::mutex> lock(writeGuard);                                                                        \
        	readGuard = &writeGuard;                                                                                             \
            map[keys[i]] = ((int)i);                                                                                             \
            readGuard = nullptr;                                                                                                 \
        }                                                                                                                        \
                                                                                                                                 \
        void selectAlgorithm(unsigned int i) override {                                                                          \
        	if (readGuard != nullptr) std::lock_guard<std::mutex> lock(*readGuard);                                              \
            if (map[keys[i]] != ((int)i)) {                                                                                      \
            	_outputFunction("Select: item #" + to_string(i) + ", on the insert phase, should be " + to_string((int)i) +      \
            	                " but is " + to_string(map[keys[i]]) + "\n");                                                    \
            }                                                                                                                    \
        }                                                                                                                        \
                                                                                                                                 \
        void updateAlgorithm(unsigned int i) override {                                                                          \
            std::lock_guard<std::mutex> lock(writeGuard);                                                                        \
        	readGuard = &writeGuard;                                                                                             \
            map[keys[i]] = -((int)i);                                                                                            \
            readGuard = nullptr;                                                                                                 \
        }                                                                                                                        \
                                                                                                                                 \
        void deleteAlgorithm(unsigned int i) override {                                                                          \
            std::lock_guard<std::mutex> lock(writeGuard);                                                                        \
        	readGuard = &writeGuard;                                                                                             \
            int value = map[keys[i]];                                                                                            \
            map.erase(keys[i]);                                                                                                  \
            if (value != -((int)i)) {                                                                                            \
                _outputFunction("Delete: item #" + to_string(i) + ", on the update phase, should be " + to_string(-((int)i)) +   \
                                " but was " + to_string(value) + "\n");                                                          \
            }                                                                                                                    \
            readGuard = nullptr;                                                                                                 \
        }                                                                                                                        \
    }                                                                                                                            \

#define DECLARE_PERFECT_HASH_MAP_ALGORITHM_ANALYSIS_AND_REENTRANCY_TEST_CLASS(_className, _mapType, _vectorType, _keyType, _outputFunction, _seed)  \
    class _className: public AlgorithmComplexityAndReentrancyAnalysis {                                                          \
    public:                                                                                                                      \
		_vectorType<_keyType>&              keys;                                                                                \
        _mapType<_keyType, int>             map;                                                                                 \
                                                                                                                                 \
        _className(_vectorType<_keyType>& keys)                                                                                  \
                : AlgorithmComplexityAndReentrancyAnalysis(#_className, _numberOfElements, _numberOfElements, _numberOfElements) \
                , keys(keys)                                                                                                     \
                , map(_mapType<_keyType, int>(&keys[0], keys.size(), _seed)) {}                                              \
                                                                                                                                 \
        void resetTables(EResetOccasion occasion) override {                                                                     \
            map.clear();                                                                                                         \
        }                                                                                                                        \
                                                                                                                                 \
        /* algorithms under analysis & test */                                                                                   \
        /* //////////////////////////////// */                                                                                   \
                                                                                                                                 \
        void insertAlgorithm(unsigned int i) override {                                                                          \
            map[keys[i]] = ((int)i);                                                                                             \
        }                                                                                                                        \
                                                                                                                                 \
        void selectAlgorithm(unsigned int i) override {                                                                          \
            if (map[keys[i]] != ((int)i)) {                                                                                      \
            	_outputFunction("Select: item #" + to_string(i) + ", on the insert phase, should be " + to_string((int)i) +      \
            	                " but is " + to_string(map[keys[i]]) + "\n");                                                    \
            }                                                                                                                    \
        }                                                                                                                        \
                                                                                                                                 \
        void updateAlgorithm(unsigned int i) override {                                                                          \
            map[keys[i]] = -((int)i);                                                                                            \
        }                                                                                                                        \
                                                                                                                                 \
        void deleteAlgorithm(unsigned int i) override {                                                                          \
            int value = map[keys[i]];                                                                                            \
            map.erase(keys[i]);                                                                                                  \
            if (value != -((int)i)) {                                                                                            \
                _outputFunction("Delete: item #" + to_string(i) + ", on the update phase, should be " + to_string(-((int)i)) +   \
                                " but was " + to_string(value) + "\n");                                                          \
            }                                                                                                                    \
        }                                                                                                                        \
    }                                                                                                                            \

struct MemoryFootprintExperimentsObjects {

    // test case constants
    static constexpr unsigned int _numberOfElements = 4'096'000;
    static constexpr unsigned int _threads          = 4;

    // hash map test case instances
    DECLARE_MAP_ALGORITHM_ANALYSIS_AND_REENTRANCY_TEST_CLASS(StandardMapStringIndexExperiments,          std::map,             std::vector,   std::string,   MemoryFootprintExperimentsObjects::output, /* empty param */);
    static StandardMapStringIndexExperiments*          standardMapStringIndexExperiments;
    DECLARE_MAP_ALGORITHM_ANALYSIS_AND_REENTRANCY_TEST_CLASS(StandardUnorderedMapStringIndexExperiments, std::unordered_map,   std::vector,   std::string,   MemoryFootprintExperimentsObjects::output, _numberOfElements);
    static StandardUnorderedMapStringIndexExperiments* standardUnorderedMapStringIndexExperiments;
    DECLARE_MAP_ALGORITHM_ANALYSIS_AND_REENTRANCY_TEST_CLASS(SkaByteLLMapStringIndexExperiments,         ska::bytell_hash_map, std::vector,   std::string,   MemoryFootprintExperimentsObjects::output, _numberOfElements);
    static SkaByteLLMapStringIndexExperiments*         skaByteLLMapStringIndexExperiments;
    DECLARE_MAP_ALGORITHM_ANALYSIS_AND_REENTRANCY_TEST_CLASS(EastlUnorderedMapStringIndexExperiments,    eastl::unordered_map, eastl::vector, eastl::string, MemoryFootprintExperimentsObjects::output, _numberOfElements);
    static EastlUnorderedMapStringIndexExperiments*    eastlUnorderedMapStringIndexExperiments;

    // perfect hash map test case instances
    DECLARE_PERFECT_HASH_MAP_ALGORITHM_ANALYSIS_AND_REENTRANCY_TEST_CLASS(PerfectHashFunctionStringIndexExperiments,  BBHash::MinimumPerfectMap, std::vector, std::string, MemoryFootprintExperimentsObjects::output, _threads);
    static PerfectHashFunctionStringIndexExperiments*  perfectHashFunctionStringIndexExperiments;

    // test case data
    static   std::vector  <std::string>* stdStringKeys;		// keys for all by EASTL containers
    static eastl::vector<eastl::string>* eastlStringKeys;

    // output messages for boost tests
    static string testOutput;


    MemoryFootprintExperimentsObjects() {
    	static bool firstRun = true;
    	if (firstRun) {
			cerr << endl << endl;
			cerr << "Memory Footprint Experiments:" << endl;
			cerr << "============================ " << endl << endl;
			firstRun = false;
    	}
    }
    ~MemoryFootprintExperimentsObjects() {
    	BOOST_TEST_MESSAGE("\n" + testOutput);
    	testOutput = "";
        // the following lines are commented because we don't want our keys to be deleted between test cases
//    	if (stdStringKeys)   delete stdStringKeys;   stdStringKeys   = nullptr;
//    	if (eastlStringKeys) delete eastlStringKeys; eastlStringKeys = nullptr;
    }

    static void output(string msg, bool toCerr) {
    	if (toCerr) cerr << msg << flush;
    	testOutput.append(msg);
    }
    static void output(string msg) {
    	output(msg, true);
    }

    void assureStandardMapStringIndexExperiments() {
    	BOOST_ASSERT_MSG(!standardMapStringIndexExperiments, "tests must never leave leftovers -- containers must be deleted and set to 'nullptr' after use");
        assureStdStringKeys();
        standardMapStringIndexExperiments = new StandardMapStringIndexExperiments(*stdStringKeys);
    }
    void assureStandardUnorderedMapStringIndexExperiments() {
    	BOOST_ASSERT_MSG(!standardUnorderedMapStringIndexExperiments, "tests must never leave leftovers -- containers must be deleted and set to 'nullptr' after use");
        assureStdStringKeys();
        standardUnorderedMapStringIndexExperiments = new StandardUnorderedMapStringIndexExperiments(*stdStringKeys);
    }
    void assureSkaByteLLMapStringIndexExperiments() {
    	BOOST_ASSERT_MSG(!skaByteLLMapStringIndexExperiments, "tests must never leave leftovers -- containers must be deleted and set to 'nullptr' after use");
        if (skaByteLLMapStringIndexExperiments) return;
        assureStdStringKeys();
        skaByteLLMapStringIndexExperiments = new SkaByteLLMapStringIndexExperiments(*stdStringKeys);
    }
    void assureEastlUnorderedMapStringIndexExperiments() {
    	BOOST_ASSERT_MSG(!eastlUnorderedMapStringIndexExperiments, "tests must never leave leftovers -- containers must be deleted and set to 'nullptr' after use");
        assureEastlStringKeys();
        eastlUnorderedMapStringIndexExperiments = new EastlUnorderedMapStringIndexExperiments(*eastlStringKeys);
    }

    void assurePerfectHashFunctionStringIndexExperiments() {
    	BOOST_ASSERT_MSG(!perfectHashFunctionStringIndexExperiments, "tests must never leave leftovers -- containers must be deleted and set to 'nullptr' after use");
        if (perfectHashFunctionStringIndexExperiments) return;
        assureStdStringKeys();
        perfectHashFunctionStringIndexExperiments = new PerfectHashFunctionStringIndexExperiments(*stdStringKeys);
    }

#define GENERATE_KEYS(_keyStringType, _keysContainer, _vectorType)                                \
        HEAP_MARK();                                                                              \
        output("Generating " #_keyStringType " keys... ");                                        \
        _keysContainer = new _vectorType<_keyStringType>(_numberOfElements);                      \
        _keyStringType     str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"); \
        std::random_device rd;                                                                    \
        std::mt19937       generator(rd());                                                       \
        for (int i=0; i<_numberOfElements; i++) {                                                 \
            std::shuffle(str.begin(), str.end(), generator);                                      \
            _keyStringType key         = str.substr(0, 16);                                       \
            (*_keysContainer)[i] = key;                                                           \
            if (i%102400 == 0) {                                                                  \
                output(".");                                                                      \
            }                                                                                     \
        }                                                                                         \
        output("\n\n");                                                                           \
        HEAP_TRACE(#_keyStringType " " + to_string(_numberOfElements) + " Random keys", output);  \

    void assureStdStringKeys() {
        if (stdStringKeys) return;
        if (eastlStringKeys) {
        	output("Deleting no longer needed 'eastlStringKeys'.\n");
        	delete eastlStringKeys;
        	eastlStringKeys = nullptr;
        }
        GENERATE_KEYS(std::string, stdStringKeys, std::vector);
    }
    void assureEastlStringKeys() {
    	if (eastlStringKeys) return;
        if (stdStringKeys) {
        	output("Deleting no longer needed 'stdStringKeys'.\n");
        	delete stdStringKeys;
        	stdStringKeys = nullptr;
        }
        GENERATE_KEYS(eastl::string, eastlStringKeys, eastl::vector);
    }

#undef GENERATE_KEYS
};
// static initializers
  std::vector  <std::string>*                                                  MemoryFootprintExperimentsObjects::stdStringKeys                              = nullptr;
eastl::vector<eastl::string>*                                                  MemoryFootprintExperimentsObjects::eastlStringKeys                            = nullptr;
MemoryFootprintExperimentsObjects::StandardMapStringIndexExperiments*          MemoryFootprintExperimentsObjects::standardMapStringIndexExperiments          = nullptr;
MemoryFootprintExperimentsObjects::StandardUnorderedMapStringIndexExperiments* MemoryFootprintExperimentsObjects::standardUnorderedMapStringIndexExperiments = nullptr;
MemoryFootprintExperimentsObjects::SkaByteLLMapStringIndexExperiments*         MemoryFootprintExperimentsObjects::skaByteLLMapStringIndexExperiments         = nullptr;
MemoryFootprintExperimentsObjects::EastlUnorderedMapStringIndexExperiments*    MemoryFootprintExperimentsObjects::eastlUnorderedMapStringIndexExperiments    = nullptr;
MemoryFootprintExperimentsObjects::PerfectHashFunctionStringIndexExperiments*  MemoryFootprintExperimentsObjects::perfectHashFunctionStringIndexExperiments  = nullptr;
string                                                                         MemoryFootprintExperimentsObjects::testOutput                                 = "";
#undef DECLARE_MAP_ALGORITHM_ANALYSIS_AND_REENTRANCY_TEST_CLASS


//BOOST_TEST_GLOBAL_FIXTURE(MemoryFootprintExperimentsObjects);
BOOST_FIXTURE_TEST_SUITE(MemoryFootprintExperiments, MemoryFootprintExperimentsObjects);

#define DECLARE_REENTRANCY_TEST(_testName, _assureFunction, _mapContainer)                           \
BOOST_AUTO_TEST_CASE(_testName) {                                                                    \
    _assureFunction();                                                                               \
    HEAP_MARK();                                                                                     \
    for (int i=10; i<=10; i++) {                                                                     \
        if (i%10 == 0) {                                                                             \
            string reentrancyTestOutput = _mapContainer->testReentrancy(_numberOfElements, true);    \
            output(reentrancyTestOutput, false);                                                     \
        } else {                                                                                     \
        	_mapContainer->testReentrancy(_numberOfElements, false);                                 \
            output(".");                                                                             \
        }                                                                                            \
    }                                                                                                \
    HEAP_TRACE(#_testName, output);                                                                  \
    delete _mapContainer;                                                                            \
    _mapContainer = nullptr;                                                                         \
}                                                                                                    \

#define DECLARE_COMPLEXITY_ANALYSIS_TEST(_testName, _assureFunction, _mapContainer)                  \
BOOST_AUTO_TEST_CASE(_testName) {                                                                    \
	_assureFunction();                                                                               \
	HEAP_MARK();                                                                                     \
	for (int i=10; i<=10; i++) {                                                                     \
		if (i%10 == 0) {                                                                             \
			string complexityAnalysisOutput;                                                         \
			tie(complexityAnalysisOutput, std::ignore, std::ignore, std::ignore, std::ignore) = _mapContainer->analyseComplexity(false, _threads, _threads, _threads, _threads, true); \
			output(complexityAnalysisOutput, false);                                                 \
		} else {                                                                                     \
			_mapContainer->analyseComplexity(false, _threads, _threads, _threads, _threads, false);  \
			output(".");                                                                             \
		}                                                                                            \
	}                                                                                                \
	HEAP_TRACE(#_testName, output);                                                                  \
	delete _mapContainer;                                                                            \
	_mapContainer = nullptr;                                                                         \
}                                                                                                    \

/*DECLARE_REENTRANCY_TEST         (StandardMapStringIndexReentrancyTests,    assureStandardMapStringIndexExperiments, standardMapStringIndexExperiments);
DECLARE_COMPLEXITY_ANALYSIS_TEST(StandardMapStringIndexComplexityAnalysis, assureStandardMapStringIndexExperiments, standardMapStringIndexExperiments);

DECLARE_REENTRANCY_TEST         (StandardUnorderedMapStringIndexReentrancyTests,    assureStandardUnorderedMapStringIndexExperiments, standardUnorderedMapStringIndexExperiments);
DECLARE_COMPLEXITY_ANALYSIS_TEST(StandardUnorderedMapStringIndexComplexityAnalysis, assureStandardUnorderedMapStringIndexExperiments, standardUnorderedMapStringIndexExperiments);

DECLARE_REENTRANCY_TEST         (SkaByteLLMapStringIndexReentrancyTests,    assureSkaByteLLMapStringIndexExperiments, skaByteLLMapStringIndexExperiments);
DECLARE_COMPLEXITY_ANALYSIS_TEST(SkaByteLLMapStringIndexComplexityAnalysis, assureSkaByteLLMapStringIndexExperiments, skaByteLLMapStringIndexExperiments);
*/
DECLARE_REENTRANCY_TEST         (PerfectHashFunctionStringIndexReentrancyTests,    assurePerfectHashFunctionStringIndexExperiments, perfectHashFunctionStringIndexExperiments);
DECLARE_COMPLEXITY_ANALYSIS_TEST(PerfectHashFunctionStringIndexComplexityAnalysis, assurePerfectHashFunctionStringIndexExperiments, perfectHashFunctionStringIndexExperiments);

DECLARE_REENTRANCY_TEST         (EastlUnorderedMapStringIndexReentrancyTests,    assureEastlUnorderedMapStringIndexExperiments, eastlUnorderedMapStringIndexExperiments);
DECLARE_COMPLEXITY_ANALYSIS_TEST(EastlUnorderedMapStringIndexComplexityAnalysis, assureEastlUnorderedMapStringIndexExperiments, eastlUnorderedMapStringIndexExperiments);

#undef DECLARE_REENTRANCY_TEST
#undef DECLARE_COMPLEXITY_ANALYSIS_TEST

BOOST_AUTO_TEST_SUITE_END();

void memoryFootprintExperiments() {

    cout << endl << endl;
    cout << "Memory Footprint Experiments:" << endl;
    cout << "============================ " << endl << endl;

    #define _numberOfElements 4'096'000
    #define _threads          4

    class SkaByteLLMapStringIndexExperiments: public AlgorithmComplexityAndReentrancyAnalysis {
    public:
        std::vector<string>&              keys;
        ska::bytell_hash_map<string, int> map;
        std::mutex                        writeGuard;
        std::mutex*                       readGuard;

        SkaByteLLMapStringIndexExperiments(std::vector<string>& keys)
                : AlgorithmComplexityAndReentrancyAnalysis("SkaByteLLMapStringIndexExperiments", _numberOfElements, _numberOfElements, _numberOfElements)
        		, readGuard(nullptr)
                , keys(keys) {
            map  = ska::bytell_hash_map<string, int>(_numberOfElements);
        }

        void resetTables(EResetOccasion occasion) override {
            map.clear();
        }

        // algorithms under analysis & test
        ///////////////////////////////////

        void insertAlgorithm(unsigned int i) override {
            std::lock_guard<std::mutex> lock(writeGuard);
        	readGuard = &writeGuard;
            map[keys[i]] = ((int)i);
            readGuard = nullptr;
        }

        void selectAlgorithm(unsigned int i) override {
        	if (readGuard != nullptr) std::lock_guard<std::mutex> lock(*readGuard);
            if (map[keys[i]] != ((int)i)) {
                cerr << "Select: item #" << i << ", on the insert phase, should be " << ((int)i) << " but is " << map[keys[i]] << endl << flush;
            }
        }

        void updateAlgorithm(unsigned int i) override {
            std::lock_guard<std::mutex> lock(writeGuard);
        	readGuard = &writeGuard;
            map[keys[i]] = -((int)i);
            readGuard = nullptr;
        }

        void deleteAlgorithm(unsigned int i) override {
            std::lock_guard<std::mutex> lock(writeGuard);
        	readGuard = &writeGuard;
            int value = map[keys[i]];
            map.erase(keys[i]);
            if (value != -((int)i)) {
                cerr << "Delete: item #" << i << ", on the update phase, should be " << -((int)i) << " but was " << value << endl << flush;
            }
            readGuard = nullptr;
        }
    };
    {
//        setHeapTraceBaseline();
        cout << "Generating std::string keys... " << flush;
        std::vector<string> keys = std::vector<string>(_numberOfElements);
        std::string         str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
        std::random_device  rd;
        std::mt19937        generator(rd());
        for (int i=0; i<_numberOfElements; i++) {
            std::shuffle(str.begin(), str.end(), generator);
            string key = str.substr(0, 16);
            keys[i]    = key;
            if (i%102400 == 0) {
                cout << "." << flush;
            }
        }
        cout << endl << endl << flush;
//        getHeapTraceInfo("std::string Random keys allocation costs");

//        setHeapTraceBaseline();
        SkaByteLLMapStringIndexExperiments skaByteLLMapStringIndexExperiments = SkaByteLLMapStringIndexExperiments(keys);
        // reentrancy tests
        for (int i=11; i<=10; i++) {
            if (i%10 == 0) {
                skaByteLLMapStringIndexExperiments.testReentrancy(_numberOfElements, true);
//                getHeapTraceInfo("SkaByteLLMapStringIndexExperiments total allocation costs");
            } else {
                skaByteLLMapStringIndexExperiments.testReentrancy(_numberOfElements, false);
                cout << "." << flush;
            }
        }
        // complexity analysis
        for (int i=10; i<=10; i++) {
            if (i%10 == 0) {
                skaByteLLMapStringIndexExperiments.analyseComplexity(false, _threads, _threads, _threads, _threads, true);
            } else {
                skaByteLLMapStringIndexExperiments.analyseComplexity(false, _threads, _threads, _threads, _threads, false);
                cout << "." << flush;
            }
        }
    }
//    getHeapTraceInfo("SkaByteLLMapStringIndexExperiments total allocation costs");

    class EastlUnorderedMapStringIndexExperiments: public AlgorithmComplexityAndReentrancyAnalysis {
    public:
        eastl::vector<eastl::string>&            keys;
        eastl::unordered_map<eastl::string, int> map;
        std::mutex                               writeGuard;
        std::mutex*                              readGuard;

        EastlUnorderedMapStringIndexExperiments(eastl::vector<eastl::string>& keys)
                : AlgorithmComplexityAndReentrancyAnalysis("EastlUnorderedMapStringIndexExperiments", _numberOfElements, _numberOfElements, _numberOfElements)
        		, readGuard(nullptr)
        		, keys(keys) {
            map  = eastl::unordered_map<eastl::string, int>(_numberOfElements);
        }

        void resetTables(EResetOccasion occasion) override {
            map.clear();
        }

        // algorithms under analysis & test
        ///////////////////////////////////

        void insertAlgorithm(unsigned int i) override {
            std::lock_guard<std::mutex> lock(writeGuard);
        	readGuard = &writeGuard;
            map[keys[i]] = ((int)i);
            readGuard = nullptr;
        }

        void selectAlgorithm(unsigned int i) override {
        	if (readGuard != nullptr) std::lock_guard<std::mutex> lock(*readGuard);
            if (map[keys[i]] != ((int)i)) {
                cerr << "Select: item #" << i << ", on the insert phase, should be " << ((int)i) << " but is " << map[keys[i]] << endl << flush;
            }
        }

        void updateAlgorithm(unsigned int i) override {
            std::lock_guard<std::mutex> lock(writeGuard);
        	readGuard = &writeGuard;
            map[keys[i]] = -((int)i);
            readGuard = nullptr;
        }

        void deleteAlgorithm(unsigned int i) override {
            std::lock_guard<std::mutex> lock(writeGuard);
        	readGuard = &writeGuard;
            int value = map[keys[i]];
            map.erase(keys[i]);
            if (value != -((int)i)) {
                cerr << "Delete: item #" << i << ", on the update phase, should be " << -((int)i) << " but was " << value << endl << flush;
            }
            readGuard = nullptr;
        }
    };
    {
//        setHeapTraceBaseline();
        cout << "Generating eastl::string keys... " << flush;
        eastl::vector<eastl::string> keys = eastl::vector<eastl::string>(_numberOfElements);
        eastl::string                str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
        std::random_device           rd;
        std::mt19937                 generator(rd());
        for (int i=0; i<_numberOfElements; i++) {
            std::shuffle(str.begin(), str.end(), generator);
            eastl::string key = str.substr(0, 16);
            keys[i]           = key;
            if (i%102400 == 0) {
                cout << "." << flush;
            }
        }
        cout << endl << endl << flush;
//        getHeapTraceInfo("eastl::string Random keys allocation costs");

//        setHeapTraceBaseline();
    	EastlUnorderedMapStringIndexExperiments eastlUnorderedMapStringIndexExperiments = EastlUnorderedMapStringIndexExperiments(keys);
        // reentrancy tests
        for (int i=11; i<=10; i++) {
            if (i%10 == 0) {
            	eastlUnorderedMapStringIndexExperiments.testReentrancy(_numberOfElements, true);
//                getHeapTraceInfo("EastlUnorderedMapStringIndexExperiments total allocation costs");
            } else {
            	eastlUnorderedMapStringIndexExperiments.testReentrancy(_numberOfElements, false);
                cout << "." << flush;
            }
        }
        // complexity analysis
        for (int i=10; i<=10; i++) {
            if (i%10 == 0) {
            	eastlUnorderedMapStringIndexExperiments.analyseComplexity(false, _threads, _threads, _threads, _threads, true);
            } else {
            	eastlUnorderedMapStringIndexExperiments.analyseComplexity(false, _threads, _threads, _threads, _threads, false);
                cout << "." << flush;
            }
        }
    }
//    getHeapTraceInfo("EastlUnorderedMapStringIndexExperiments total allocation costs");

#undef _numberOfElements
#undef _threads
}


boost::unit_test::test_suite* init_unit_test_suite(int argc, char* args[]) {
	cerr << "Starting Boost Tests. Initializing static variables..." << endl;
	boost::unit_test::framework::master_test_suite().p_name.value = "my master test suite name";
	return 0;
}




int _main() {

//    try {
//        hashTableExperiments();
//    } catch (const std::exception& e) {
//        DUMP_EXCEPTION(e, "Error while running hashTableExperiments()");
//    }
//    heap_trace_allocated_bytes_baseline   = 0;
//    heap_trace_deallocated_bytes_baseline = 0;
//    getHeapTraceInfo("hashTableExperiments allocation totals");

    try {
        memoryFootprintExperiments();
    } catch (const std::exception& e) {
        DUMP_EXCEPTION(e, "Error while running hashTableExperiments()");
    }
//    heap_trace_allocated_bytes_baseline   = 0;
//    heap_trace_deallocated_bytes_baseline = 0;
//    getHeapTraceInfo("memoryFootprintExperiments allocation totals");

//    heap_trace_allocated_bytes_baseline   = 0;
//    heap_trace_deallocated_bytes_baseline = 0;
//    getHeapTraceInfo("Program allocation totals");
    return EXIT_SUCCESS;
}
