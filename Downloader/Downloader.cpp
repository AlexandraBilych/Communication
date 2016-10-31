#include "Downloader.h"

#define HTTP_PORT 80
#define HTTPS_PORT 443
#define HTTP_STATUS_CODE_POS 9
#define HTTP_STATUS_CODE_LEN 3

Downloader::Downloader(const std::string& url) {
    size_t hostPos = url.find('/') + 2;

    fileName = url.substr(url.rfind('/') + 1, url.length());
    host = url.substr(hostPos, url.find('/', hostPos) - hostPos);
    address = url.substr(url.find('/', hostPos));
    isSecure = ( url.substr(0, 5) == "https" ) ? true : false;

    if ( isSecure ) {
        port = HTTPS_PORT;
    } else {
        port = HTTP_PORT;
    }

    sockfd = 0;
    contentStartPos = 0;
    contentLength = 0;
    ssl_ctx = NULL;
    conn = NULL;
}

bool Downloader::connectToServer() {
    struct hostent* hp;
    struct sockaddr_in client;

    hp = gethostbyname(&host[0]);
    if ( hp == NULL ) {
        throw new WrongHostException();
    }

    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if ( sockfd < 0 ) {
        throw new CreateSocketException();
    }

    memset((char *) &client, 0, sizeof(client));
    client.sin_family=AF_INET;
    memcpy((char *) &client.sin_addr, hp->h_addr, hp->h_length);
    client.sin_port=htons(port);

    return connect(sockfd, (struct sockaddr *) &client, sizeof(client));
}

void Downloader::getImageInfoSecure() {
    char buffer[LEN_BUFF];
    std::string httpHeader;

    conn = SSL_new(ssl_ctx);
    SSL_set_fd(conn, sockfd);
    SSL_connect(conn);

    memset(buffer, '\0', LEN_BUFF);
    sprintf(buffer,"HEAD %s HTTP/1.1\r\nHost: %s\r\n\r\n", address.c_str(), host.c_str());
    SSL_write(conn, buffer, strlen(buffer));
    SSL_read(conn, buffer, LEN_BUFF);

    httpHeader = buffer;
    if ( httpHeader.substr(HTTP_STATUS_CODE_POS, HTTP_STATUS_CODE_LEN) != httpCodeOk ) {
        throw new HttpErrorCodeException();
    }

    contentStartPos = httpHeader.find("Content-Length: ");
    if ( contentStartPos == std::string::npos ) {
        throw new ZeroContentLengthException();
    }

    contentStartPos += sizeof("Content-Length: ");
    contentLength = stoi(httpHeader.substr(contentStartPos, httpHeader.find("\n", contentStartPos)));
    contentStartPos = httpHeader.find("\r\n\r\n") - 1;
}

void Downloader::getImageInfo() {
    char buffer[LEN_BUFF];
    std::string httpHeader;

    memset(buffer, '\0', LEN_BUFF);
    sprintf(buffer,"HEAD %s HTTP/1.1\r\nHost: %s\r\n\r\n", address.c_str(), host.c_str());
    send(sockfd, buffer, strlen(buffer), 0);
    recv(sockfd, buffer, LEN_BUFF, 0);

    httpHeader = buffer;
    if ( httpHeader.substr(HTTP_STATUS_CODE_POS, HTTP_STATUS_CODE_LEN) != httpCodeOk ) {
        throw new HttpErrorCodeException();
    }

    contentStartPos = httpHeader.find("Content-Length: ");
    if ( contentStartPos == std::string::npos ) {
        throw new ZeroContentLengthException();
    }

    contentStartPos += sizeof("Content-Length: ");
    contentLength = stoi(httpHeader.substr(contentStartPos, httpHeader.find("\n", contentStartPos)));
    contentStartPos = httpHeader.find("\r\n\r\n") - 1;
}

void Downloader::writeImageInFile() {
    char buffer[LEN_BUFF];
    int byteAmount;
    std::ofstream archivo;

    memset(buffer, '\0', LEN_BUFF);
    sprintf(buffer,"GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", address.c_str(), host.c_str());
    write(sockfd, buffer, strlen(buffer));

    archivo.open(fileName, std::ios::out | std::ios::binary);

    memset(buffer, '\0', LEN_BUFF);
    byteAmount = recv(sockfd, buffer, LEN_BUFF, 0);
    archivo.write(buffer + contentStartPos, byteAmount - contentStartPos);

    memset(buffer, '\0', LEN_BUFF);
    for ( int i = contentStartPos; (byteAmount = recv(sockfd, buffer, LEN_BUFF, 0)) > 0; i += byteAmount ) {
        archivo.write(buffer, byteAmount);
        memset(buffer, '\0', byteAmount);
    }

    archivo.close();
    close(sockfd);
}

void Downloader::writeImageInFileSecure() {
    char buffer[LEN_BUFF];
    int byteAmount;
    std::ofstream archivo;

    memset(buffer, '\0', LEN_BUFF);
    sprintf(buffer,"GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", address.c_str(), host.c_str());
    SSL_write(conn, buffer, strlen(buffer));

    archivo.open(fileName, std::ios::out | std::ios::binary);

    memset(buffer, '\0', LEN_BUFF);
    byteAmount = SSL_read(conn, buffer, LEN_BUFF);

    archivo.write(buffer + contentStartPos, byteAmount - contentStartPos);

    memset(buffer, '\0', LEN_BUFF);
    for ( int i = contentStartPos;
        (byteAmount = SSL_read(conn, buffer, LEN_BUFF)) > 0;
        i += byteAmount ) {
        archivo.write(buffer, byteAmount);
        memset(buffer, '\0', byteAmount);
    }

    archivo.close();
    SSL_shutdown(conn);
    SSL_free(conn);
    close(sockfd);
}

void Downloader::downloadImage() {
    if ( isSecure ) {
        SSL_load_error_strings ();
        SSL_library_init ();
        ssl_ctx = SSL_CTX_new (SSLv23_client_method ());
    }

    if ( connectToServer() ) {
        throw new ConnectionException();
    }

    if ( isSecure ) {
        getImageInfoSecure();
        writeImageInFileSecure();
    } else {
        getImageInfo();
        writeImageInFile();
    }
}
