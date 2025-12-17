#include <iostream>
#include <thread>
#include <condition_variable>
#include <string>

using namespace std;
using namespace std::chrono;

// Global variables
mutex mut;
bool ready{false};
string sdata{"Empty"};

// Waiting thread
void reader() {
	unique_lock<std::mutex> guard(mut);        // Acquire lock
	// cv.wait(guard;                          // Wait for condition variable to be notified
	while(!ready) {                            // Simulate condition variable unlocking the mutex
		guard.unlock();                        // Allow writer thread to access cvReplacement
		std::this_thread::sleep_for(10ms);
		guard.lock();                          // Lock mutex before we access cvReplacement
	}
	cout << "Data is " << sdata << endl;       // Wake up and use the new value
}

// Modyifing thread
void writer() {
	cout << "Writing data..." << endl;
	std::this_thread::sleep_for(2s);           // Pretend to be busy...
	lock_guard<std::mutex> sm(mut);            // Protect data
	sdata = "Populated";                       // Modify the data
	// cv.notify_one();                        // Notify the condition variable
	ready = true;                              // Set the flag
}

int main() {
	cout << "Data is " << sdata << endl;
	thread read{reader};
    thread write{writer};
	/*
	// If the writer thread finishes before the reader thread starts, the notification is lost
	thread write{writer};
	std::this_thread::sleep_for(500ms);
	thread read{reader};
	*/
	write.join();
	read.join();
}