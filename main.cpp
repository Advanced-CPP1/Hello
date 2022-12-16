#include <iostream>
#include <string>
#include <curl/curl.h>

int main() {
  // Initialize the curl library
  curl_global_init(CURL_GLOBAL_ALL);
  // Create a curl handle
  CURL *curl = curl_easy_init();

  // Set the URL of the web page you want to retrieve
  curl_easy_setopt(curl, CURLOPT_URL, "https://www.example.com");
  // Set the callback function that will be called to handle the response data
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, [](char *ptr, size_t size, size_t nmemb, void *userdata) {
    // Append the received data to the string passed as userdata
    static_cast<std::string *>(userdata)->append(ptr, size * nmemb);
    return size * nmemb;
  });
  // Set the userdata argument that will be passed to the callback function
  std::string html;
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &html);

  // Perform the request
  CURLcode res = curl_easy_perform(curl);
  // Check if the request was successful
  if (res != CURLE_OK) {
    std::cerr << "Error: " << curl_easy_strerror(res) << std::endl;
  }

  // Clean up
  curl_easy_cleanup(curl);
  curl_global_cleanup();

  // Extract the desired HTML element(s) from the retrieved HTML content
  // You can use an HTML parser library, such as libxml or htmlcxx, to parse the HTML and extract the elements you want.
  // For example, with htmlcxx:
  HTML::ParserDom parser;
  tree<HTML::Node> dom = parser.parseTree(html);
  tree<HTML::Node>::iterator it = dom.begin();
  tree<HTML::Node>::iterator end = dom.end();
  for (; it != end; ++it) {
    if (it->tagName() == "div" && it->attribute("class").second == "my-class") {
      std::cout << it->text() << std::endl;
    }
  }

  return 0;
}
