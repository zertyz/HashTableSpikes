#include <iostream>
#include <map>
#include <unordered_map>
#include <thread>
#include <random>
#include <algorithm>

#include <mutex>
#define synchronized(m)     for(std::unique_lock<std::recursive_mutex> lk(m); lk; lk.unlock())

#include <boost/test/unit_test.hpp>

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


#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE someModuleName
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

boost::unit_test::test_suite* init_unit_test_suite(int argc, char* args[]) {
	cerr << "My boost tests are just starting..." << endl;
	boost::unit_test::framework::master_test_suite().p_name.value = "my master test suite name";
	return 0;
}


void* operator new[](size_t size, const char* pName, int flags, unsigned     debugFlags, const char* file, int line)
{
    return malloc(size);
}

void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
    return malloc(size);
}

#define HEAP_TRACE

#include <new>

static size_t                             heap_trace_allocated_bytes            = 0;
static size_t                             heap_trace_deallocated_bytes          = 0;
static size_t                             heap_trace_allocated_bytes_baseline   = 0;
static size_t                             heap_trace_deallocated_bytes_baseline = 0;

void* operator new(std::size_t size) {
    void* alloc_entry = std::malloc(size);
    if (!alloc_entry) {
        throw std::bad_alloc();
    }
    heap_trace_allocated_bytes += *(size_t*)(((size_t)alloc_entry)-sizeof(size_t));
    //std::cout << "(-1) equals " << size << " : " << *(size_t*)(((size_t)alloc_entry)-sizeof(size_t)) << std::endl << std::flush;
    return alloc_entry;
}

void operator delete(void* alloc_entry) noexcept {
    heap_trace_deallocated_bytes += *(size_t*)(((size_t)alloc_entry)-sizeof(size_t));
    //std::cout << "(-1) : " << *(size_t*)(((size_t)alloc_entry)-sizeof(size_t)) << std::endl << std::flush;
    std::free(alloc_entry);
}

void setHeapTraceBaseline() {
    heap_trace_allocated_bytes_baseline   = heap_trace_allocated_bytes;
    heap_trace_deallocated_bytes_baseline = heap_trace_deallocated_bytes;
}

void getHeapTraceInfo(string title) {
    std::cout << "\t" << title << ":" << std::endl;
    std::cout << "\t\tAllocations:   " << (heap_trace_allocated_bytes   - heap_trace_allocated_bytes_baseline)   << " bytes" << std::endl;
    std::cout << "\t\tDeallocations: " << (heap_trace_deallocated_bytes - heap_trace_deallocated_bytes_baseline) << " bytes" << std::endl;
    std::cout << std::endl << std::flush;
}


// test & exploration functions
///////////////////////////////

void hashTableExperiments() {

    cout << endl << endl;
    cout << "Hash Table Experiments:" << endl;
    cout << "====================== " << endl << endl;

    #define _numberOfElements 2'048'000
    #define _threads          4

    setHeapTraceBaseline();

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

    getHeapTraceInfo("Random keys allocation costs");

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
    setHeapTraceBaseline();
    {
        StandardMapStringIndexExperiments standardMapStringIndexExperiments = StandardMapStringIndexExperiments(keys);
        // reentrancy tests
        for (int i=11; i<=10; i++) {
            if (i%10 == 0) {
                standardMapStringIndexExperiments.testReentrancy(_numberOfElements, true);
                getHeapTraceInfo("StandardMapStringIndexExperiments first pass allocation costs");
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
    getHeapTraceInfo("StandardMapStringIndexExperiments total allocation costs");

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
    setHeapTraceBaseline();
    {
        UnorderedMapStringIndexExperiments unorderedMapStringIndexExperiments = UnorderedMapStringIndexExperiments(keys);
        // reentrancy tests
        for (int i=11; i<=10; i++) {
            if (i%10 == 0) {
                unorderedMapStringIndexExperiments.testReentrancy(_numberOfElements, true);
                getHeapTraceInfo("UnorderedMapStringIndexExperiments first pass allocation costs");
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
    getHeapTraceInfo("UnorderedMapStringIndexExperiments total allocation costs");

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
    setHeapTraceBaseline();
    {
        SkaUnorderedMapStringIndexExperiments skaUnorderedMapStringIndexExperiments = SkaUnorderedMapStringIndexExperiments(keys);
        // reentrancy tests
        for (int i=11; i<=10; i++) {
            if (i%10 == 0) {
                skaUnorderedMapStringIndexExperiments.testReentrancy(_numberOfElements, true);
                getHeapTraceInfo("SkaUnorderedMapStringIndexExperiments total allocation costs");
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
    getHeapTraceInfo("SkaUnorderedMapStringIndexExperiments total allocation costs");

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
    setHeapTraceBaseline();
    {
        SkaByteLLMapStringIndexExperiments skaByteLLMapStringIndexExperiments = SkaByteLLMapStringIndexExperiments(keys);
        // reentrancy tests
        for (int i=11; i<=10; i++) {
            if (i%10 == 0) {
                skaByteLLMapStringIndexExperiments.testReentrancy(_numberOfElements, true);
                getHeapTraceInfo("SkaByteLLMapStringIndexExperiments total allocation costs");
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
    getHeapTraceInfo("SkaByteLLMapStringIndexExperiments total allocation costs");

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
    setHeapTraceBaseline();
    {
    	EastlUnorderedMapStringIndexExperiments eastlUnorderedMapStringIndexExperiments = EastlUnorderedMapStringIndexExperiments(keys);
        // reentrancy tests
        for (int i=11; i<=10; i++) {
            if (i%10 == 0) {
            	eastlUnorderedMapStringIndexExperiments.testReentrancy(_numberOfElements, true);
                getHeapTraceInfo("EastlUnorderedMapStringIndexExperiments total allocation costs");
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
    getHeapTraceInfo("EastlUnorderedMapStringIndexExperiments total allocation costs");

#undef _numberOfElements
#undef _threads
}


// Memory footprint test cases
//////////////////////////////

#define HEAP_MARK()                                                            \
    size_t marked_heap_trace_allocated_bytes   = heap_trace_allocated_bytes;   \
    size_t marked_heap_trace_deallocated_bytes = heap_trace_deallocated_bytes; \

#define HEAP_DEBUG(_taskName) {                                                                     \
    size_t allocations   = heap_trace_allocated_bytes   - marked_marked_heap_trace_allocated_bytes; \
    size_t deallocations = heap_trace_deallocated_bytes - marked_heap_trace_deallocated_bytes;      \
    string msg = _taskName + " allocation costs:\n" +                                               \
                 "      allocations: " + to_string(allocations)                 + " bytes\n" +      \
                 "    deallocations: " + to_string(deallocations)               + " bytes\n" +      \
                 "         retained: " + to_string(allocations - deallocations) + " bytes";         \
    cerr << msg;                                                                                    \
    BOOST_TEST_MESSAGE(msg);                                                                        \
}                                                                                                   \

#define DECLARE_ALGORITHM_ANALYSIS_AND_REENTRANCY_TEST_CLASS(_className, _ctor) \
    class _className: public AlgorithmComplexityAndReentrancyAnalysis {\
    public:\
        std::vector<string>&              keys;\
        ska::bytell_hash_map<string, int> map;\
        std::mutex                        writeGuard;\
        std::mutex*                       readGuard;\
\
        _className(std::vector<string>& keys)\
                : AlgorithmComplexityAndReentrancyAnalysis(_className, _numberOfElements, _numberOfElements, _numberOfElements)\
        		, readGuard(nullptr)\
                , keys(keys) {\
            map  = _ctor<string, int>(_numberOfElements);\
        }\
\
        void resetTables(EResetOccasion occasion) override {\
            map.clear();\
        }\
\
        /* algorithms under analysis & test */\
        /* //////////////////////////////// */\
\
        void insertAlgorithm(unsigned int i) override {\
            std::lock_guard<std::mutex> lock(writeGuard);\
        	readGuard = &writeGuard;\
            map[keys[i]] = ((int)i);\
            readGuard = nullptr;\
        }\
\
        void selectAlgorithm(unsigned int i) override {\
        	if (readGuard != nullptr) std::lock_guard<std::mutex> lock(*readGuard);\
            if (map[keys[i]] != ((int)i)) {\
                cerr << "Select: item #" << i << ", on the insert phase, should be " << ((int)i) << " but is " << map[keys[i]] << endl << flush;\
            }\
        }\
\
        void updateAlgorithm(unsigned int i) override {\
            std::lock_guard<std::mutex> lock(writeGuard);\
        	readGuard = &writeGuard;\
            map[keys[i]] = -((int)i);\
            readGuard = nullptr;\
        }\
\
        void deleteAlgorithm(unsigned int i) override {\
            std::lock_guard<std::mutex> lock(writeGuard);\
        	readGuard = &writeGuard;\
            int value = map[keys[i]];\
            map.erase(keys[i]);\
            if (value != -((int)i)) {\
                cerr << "Delete: item #" << i << ", on the update phase, should be " << -((int)i) << " but was " << value << endl << flush;\
            }\
            readGuard = nullptr;\
        }\
    }\

struct MemoryFootprintExperimentsObjects {

    // test case constants
    static constexpr int _numberOfElements = 4'096'000;
    static constexpr int _threads          = 4;

    // test case data
    unique_ptr<vector<string>> stdStringKeys; 

    // test case instances
    DECLARE_ALGORITHM_ANALYSIS_AND_REENTRANCY_TEST_CLASS(SkaByteLLMapStringIndexExperiments, ska::bytell_hash_map);
    unique_ptr<SkaByteLLMapStringIndexExperiments> skaByteLLMapStringIndexExperiments;


    MemoryFootprintExperimentsObjects() {
        cerr << endl << endl;
        cerr << "Memory Footprint Experiments:" << endl;
        cerr << "============================ " << endl << endl;
    }
    ~MemoryFootprintExperimentsObjects() {}


    void assureSkaByteLLMapStringIndexExperiments() {

        if (skaByteLLMapStringIndexExperiments) return;

        assureStdStringKeys();
        skaByteLLMapStringIndexExperiments = std::make_unique<SkaByteLLMapStringIndexExperiments>(keys);
    }

    void assureStdStringKeys() {

        if (stdStringKeys) return;

        HEAP_MARK();

        stdStringKeys = std::make_unique<std::vector<string>>(_numberOfElements);
        
        cerr << "Generating std::string keys... " << flush;
        std::string         str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
        std::random_device  rd;
        std::mt19937        generator(rd());
        for (int i=0; i<_numberOfElements; i++) {
            std::shuffle(str.begin(), str.end(), generator);
            string key       = str.substr(0, 16);
            stdStringKeys[i] = key;
            if (i%102400 == 0) {
                cout << "." << flush;
            }
        }

        cout << endl << endl << flush;
        HEAP_DEBUG("std::string " + to_string(_numberOfElements) + " Random keys");
    }
};

BOOST_FIXTURE_TEST_SUITE(MemoryFootprintExperiments, MemoryFootprintExperimentsObjects);

BOOST_AUTO_TEST_CASE(SkaByteLLMapStringIndexReentrancyTests) {
    
    assureSkaByteLLMapStringIndexExperiments();

    for (int i=11; i<=10; i++) {
        if (i%10 == 0) {
            HEAP_MARK()
            skaByteLLMapStringIndexExperiments.testReentrancy(_numberOfElements, true);
            HEAP_DEBUG("SkaByteLLMapStringIndexReentrancyTests");
        } else {
            skaByteLLMapStringIndexExperiments.testReentrancy(_numberOfElements, false);
            cout << "." << flush;
        }
    }
}

BOOST_AUTO_TEST_CASE(SkaByteLLMapStringIndexComplexityAnalysis) {
    
    assureSkaByteLLMapStringIndexExperiments();

    for (int i=10; i<=10; i++) {
        if (i%10 == 0) {
            HEAP_MARK()
            skaByteLLMapStringIndexExperiments.analyseComplexity(false, _threads, _threads, _threads, _threads, true);
            HEAP_DEBUG("SkaByteLLMapStringIndexComplexityAnalysis");
        } else {
            skaByteLLMapStringIndexExperiments.analyseComplexity(false, _threads, _threads, _threads, _threads, false);
            cout << "." << flush;
        }
    }

}

BOOST_AUTO_TEST_SUITE_END();
#undef DECLARE_ALGORITHM_ANALYSIS_AND_REENTRANCY_TEST_CLASS

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
        setHeapTraceBaseline();
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
        getHeapTraceInfo("std::string Random keys allocation costs");

        setHeapTraceBaseline();
        SkaByteLLMapStringIndexExperiments skaByteLLMapStringIndexExperiments = SkaByteLLMapStringIndexExperiments(keys);
        // reentrancy tests
        for (int i=11; i<=10; i++) {
            if (i%10 == 0) {
                skaByteLLMapStringIndexExperiments.testReentrancy(_numberOfElements, true);
                getHeapTraceInfo("SkaByteLLMapStringIndexExperiments total allocation costs");
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
    getHeapTraceInfo("SkaByteLLMapStringIndexExperiments total allocation costs");

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
        setHeapTraceBaseline();
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
        getHeapTraceInfo("eastl::string Random keys allocation costs");

        setHeapTraceBaseline();
    	EastlUnorderedMapStringIndexExperiments eastlUnorderedMapStringIndexExperiments = EastlUnorderedMapStringIndexExperiments(keys);
        // reentrancy tests
        for (int i=11; i<=10; i++) {
            if (i%10 == 0) {
            	eastlUnorderedMapStringIndexExperiments.testReentrancy(_numberOfElements, true);
                getHeapTraceInfo("EastlUnorderedMapStringIndexExperiments total allocation costs");
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
    getHeapTraceInfo("EastlUnorderedMapStringIndexExperiments total allocation costs");

#undef _numberOfElements
#undef _threads
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
    heap_trace_allocated_bytes_baseline   = 0;
    heap_trace_deallocated_bytes_baseline = 0;
    getHeapTraceInfo("memoryFootprintExperiments allocation totals");

    heap_trace_allocated_bytes_baseline   = 0;
    heap_trace_deallocated_bytes_baseline = 0;
    getHeapTraceInfo("Program allocation totals");
    return EXIT_SUCCESS;
}
