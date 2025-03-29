#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <chrono>
#include <curl/curl.h>
#include "gumbo.h"

int main() {
    curl_global_init(CURL_GLOBAL_ALL);

    CrawlerState state;
    string startUrl = "http://example.com"; // Change this URL

    cout << "=== Serial Crawler ===" << endl;
    SerialCrawler(startUrl, state);

    state.visited.clear(); // Reset for next run

    cout << "\n=== Concurrent Crawler ===" << endl;
    ConcurrentCrawler(startUrl, state);

    curl_global_cleanup();
    return 0;
}