#include "mainwindow.h"
#include <sys/stat.h>
#include "curl/curl.h"
#include <QApplication>
#include <QFile>
#include <QFileDialog>

int storeEasyDL();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    storeEasyDL();
    return a.exec();

}

int storeEasyDL() {
    const char *post_data_filename = "C:\\Users\\Administrator\\Desktop\\1.png";

    FILE *fp = NULL;
    struct stat stbuf = { 0 };

    fp = fopen(post_data_filename, "r");

    if (!fp) {
        fprintf(stderr, "Error: failed to open file \"%s\"\n", post_data_filename);
        return -1;
    }

    if (fstat(fileno(fp), &stbuf) || !S_ISREG(stbuf.st_mode)) {
        fprintf(stderr, "Error: unknown file size \"%s\"\n", post_data_filename);
        return -1;
    }

    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();
    if (curl != NULL) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:24401?threshold=0.1");
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE,(curl_off_t)stbuf.st_size);
        curl_easy_setopt(curl, CURLOPT_READDATA, (void *)fp);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    fclose(fp);
    return 0;
}
