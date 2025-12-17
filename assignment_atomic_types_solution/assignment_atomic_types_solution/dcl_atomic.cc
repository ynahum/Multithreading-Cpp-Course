#include <mutex>
#include <thread>
#include <vector>
#include <atomic>

using namespace std;

class some_type {
	// ...
public:
	void do_it() { /*...*/ }
};

atomic<some_type *> ptr{nullptr};            // Variable to be lazily initialized
std::mutex process_mutex;

void process() {
    if (!ptr) {                                                                                  // First check of ptr
        std::lock_guard<std::mutex> lk(process_mutex);
        
        if (!ptr)                  // Second check of ptr
            ptr = new some_type;   // Initialize ptr
    }
     some_type *sp = ptr;
	 sp->do_it();
}

int main() {
	vector<thread> threads;
	
	for (int i = 0; i < 10; ++i)
		threads.push_back(thread{process});
	for (auto&t : threads)
		t.join();
}

// My solution
#if 0

#include <mutex>
#include <atomic>
#include <thread>

std::atomic<bool> initialized{ false };

class some_type {
    // ...
public:
    void do_it() { /*...*/ }
};

some_type* ptr{ nullptr };            // Variable to be lazily initialized
std::mutex process_mutex;

void process() {
    if (!initialized) {                     // First check of ptr
        std::lock_guard<std::mutex> lk(process_mutex);

        if (!initialized) {
            // Second check of ptr
            ptr = new some_type;   // Initialize ptr
            initialized = true;
        }
    }
    ptr->do_it();
}

int main() {
    std::thread t1(process);
    std::thread t2(process);
    std::thread t3(process);
    t1.join();
    t2.join();
    t3.join();
    return 0;
}

#endif