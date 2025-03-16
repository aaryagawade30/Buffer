#include <iostream>
#include <curl/curl.h>
#include <string>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    output->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void queryGeminiAPI(const std::string& prompt) {
    std::string api_key = "AIzaSyAtJlXxpcXr-2O7IpcCiTEBCvGj0ehJefM";  // Replace with your actual API key
    std::string url = "https://generativelanguage.googleapis.com/v1/models/gemini-pro:generateContent?key=" + api_key;

    std::string postData = R"({"contents":[{"parts":[{"text":")" + prompt + R"("}]}]})";

    CURL* curl = curl_easy_init();
    if (curl) {
        std::string response;
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            std::cout << "Response: " << response << std::endl;
        } else {
            std::cerr << "cURL Error: " << curl_easy_strerror(res) << std::endl;
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
}

int main() {
    curl_global_init(CURL_GLOBAL_ALL);
    queryGeminiAPI("Hello, how are you?");
    curl_global_cleanup();
    return 0;
}
