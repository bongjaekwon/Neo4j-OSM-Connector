#include <iostream>
#include <string>
#include <curl/curl.h>

//
std::string send_cypher_query(const std::string& query) {
    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        // 
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:7476/db/data/transaction/commit");

        // 
        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "Authorization: Basic bmVvNGo6dGVzdA==");  // 

        // 
        std::string data = "{\"statements\" : [{\"statement\" : \"" + query + "\"}]}";

        // 
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

        //
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            std::cerr << "CURL 요청 실패: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl);
            curl_slist_free_all(headers);
            curl_global_cleanup();
            return "Query failed";
        }

        // 
        std::cout << "Cypher Query Successfully Sent!" << std::endl;

        //
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    curl_global_cleanup();
    return "Query sent successfully";
}

int main() {
    // 
    std::string query = "CREATE (a:Place {name: 'Location1', lat: 40.7128, lon: -74.0060})";
    
    // 
    std::string result = send_cypher_query(query);
    std::cout << result << std::endl;

    return 0;
}
