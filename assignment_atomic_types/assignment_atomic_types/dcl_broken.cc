#include <mutex>
#include <atomic>

std::atomic<bool> initialized{ false };

class some_type {
	// ...
public:
	void do_it() { /*...*/ }
};

some_type *ptr{nullptr};            // Variable to be lazily initialized
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
