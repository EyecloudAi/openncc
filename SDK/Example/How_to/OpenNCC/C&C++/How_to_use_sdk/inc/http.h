#ifndef HTTP_H_
#define HTTP_H_

#include <curl/curl.h>
#include <string>
#include <string.h>
#include <time.h>
#include <iostream>
#include <cJSON.h>
#include <cmath>

#define POST_URL "https://ircam.eyecloudtech.com/"

extern int ncc_bas64(unsigned char* pt_buf,int in_size,unsigned char* OutBuf,int outBufLen);

class Http
{
public:
    Http();
    ~Http();

    bool login(char *device_id, char *version);
    bool post(char *device_id, char *version, char *image);
    int timer();

private:
    CURL *curl;
    CURLcode res;
    struct tm time_;
};

#endif // HTTP_H_