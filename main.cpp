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

// Function to fetch HTML content of a URL using libcurl
string fetchPageContent(const string& url) {
    CURL* curl = curl_easy_init();
    if (!curl) return "";

    string pageContent;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &pageContent);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    return (res == CURLE_OK) ? pageContent : "";
}

// Function to parse HTML and extract links
vector<string> getLinksFromHTML(const string& html) {
    vector<string> links;
    GumboOutput* output = gumbo_parse(html.c_str());

    if (output) {
        extractLinks(output->root, links);
        gumbo_destroy_output(&kGumboDefaultOptions, output);
    }
    return links;
}

// serial crawler
void SerialCrawler(string url, CrawlerState& state) {
    if (state.visited[url]) return;

    state.visited[url] = true;
    cout << "Fetching: " << url << endl;

    string html = fetchPageContent(url);
    if (html.empty()) return;

    vector<string> links = getLinksFromHTML(html);
    for (const auto& link : links) {
        SerialCrawler(link, state);
    }
}

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