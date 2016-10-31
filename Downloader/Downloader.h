#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <iostream>
#include <fstream>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define LEN_BUFF 1025

class WrongUrlException {};
class ConnectionException {};
class WrongHostException {};
class CreateSocketException {};
class HttpErrorCodeException {};
class ZeroContentLengthException{};

class Downloader {
    private:
        std::string fileName, host, address;
        size_t contentLength, contentStartPos;
        int sockfd;
        u_short port;
        SSL_CTX *ssl_ctx;
        SSL *conn;
        bool isSecure;

        const std::string httpCodeOk = "200";

        bool connectToServer();
        void getImageInfo();
        void getImageInfoSecure();
        void writeImageInFile();
        void writeImageInFileSecure();

    public:
        Downloader(const std::string& url);
        ~Downloader() {};

        void operator()() { downloadImage(); };

        void downloadImage();
};

#endif
