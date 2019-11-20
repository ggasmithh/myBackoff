//  Author: Garrett Smith
//  NetID: gas203

//  Sources Consulted
//
//	1). https://www.geeksforgeeks.org/rand-and-srand-in-ccpp/
//	2). http://www.cplusplus.com/reference/algorithm/count/
//	3). https://cpppatterns.com/patterns/choose-random-element.html (unused)
//	4). https://stackoverflow.com/questions/30973994/how-can-i-convert-an-int-to-a-string-in-c11-without-using-to-string-or-stoi

#include <fstream>
#include <stdlib.h>
#include <vector>
#include <cmath>
//#include <time.h>

#define MAX_DEVICES 6000
#define NUM_REPEAT 10

using namespace std;

int main() {

	int linear_sum = 0;
	int beb_sum = 0;
	int log_sum = 0;

	int trial_latency = 0;
	int last_successful_device = 0;
	int window_size = 0;
	int slot_to_try = 0;

	ofstream linear_latency("linearLatency.txt", ios_base::trunc);
	ofstream beb_latency("binaryLatency.txt", ios_base::trunc);
	ofstream log_latency("logLatency.txt", ios_base::trunc);

	srand(time(NULL));

	for (int devices = 100; devices < MAX_DEVICES; devices += 100) {
		linear_sum = 0;
		beb_sum = 0;
		log_sum = 0;

		for (int rep = 0; rep < NUM_REPEAT; rep++) {

			// LINEAR BACKOFF
			trial_latency = 0;

			last_successful_device = 0;
			window_size = 2;

			while (last_successful_device < devices - 1) {
				vector<bool> linear_slot_occupied(window_size, false);

				trial_latency += window_size;

				for (int i = last_successful_device; i < devices; i++) {
					slot_to_try = rand() % window_size;
					if (linear_slot_occupied[slot_to_try]) {
						window_size++;
						break;
					}
					else {
						linear_slot_occupied[slot_to_try] = true;
						last_successful_device = i;
					}
				}
			}

			trial_latency += slot_to_try;

			linear_sum += trial_latency;

			// BINARY EXPONENTIAL BACKOFF
			trial_latency = 0;

			last_successful_device = 0;
			window_size = 2;

			while (last_successful_device < devices - 1) {
				vector<bool> beb_slot_occupied(window_size, false);

				trial_latency += window_size;

				for (int i = last_successful_device; i < devices; i++) {
					slot_to_try = rand() % window_size;
					if (beb_slot_occupied[slot_to_try]) {
						window_size *= 2;
						break;
					}
					else {
						beb_slot_occupied[slot_to_try] = true;
						last_successful_device = i;
					}
				}
			}

			trial_latency += slot_to_try;

			beb_sum += trial_latency;

			// LOGARITHMIC BACKOFF
			trial_latency = 0;

			last_successful_device = 0;
			window_size = 2;
		
			while (last_successful_device < devices - 1) {
				vector<bool> log_slot_occupied(window_size, false);
				trial_latency += window_size;

				for (int i = last_successful_device; i < devices; i++) {
					slot_to_try = rand() % window_size;
					if (log_slot_occupied[slot_to_try]) {
						window_size  = (1 + int((1.0 / log2(window_size)))) * window_size;
						break;
					}
					else {
						log_slot_occupied[slot_to_try] = true;
						last_successful_device = i;
					}
				}
			}

			trial_latency += slot_to_try;

			log_sum += trial_latency;

		}

		linear_latency << linear_sum / NUM_REPEAT << "\n";
		beb_latency << beb_sum / NUM_REPEAT << "\n";
		log_latency << log_sum / NUM_REPEAT << "\n";
	}

	linear_latency.close();
	beb_latency.close();
	log_latency.close();

	return 0;
}
