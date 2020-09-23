#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "curl/curl.h"
#include <sys/stat.h>
#include "cstdio"
using namespace std;

int storeEasyDL();

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
//    getWesiteData();
    storeEasyDL();
}

struct url_data {
    size_t size;
    char* data;
};

static std::string strdetect_result;

//size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userdata)
//{
//  FILE *readhere = (FILE *)userdata;
//  curl_off_t nread;

//  /* copy as much data as possible into the 'ptr' buffer, but no more than
//     'size' * 'nmemb' bytes! */
//  size_t retcode = fread(ptr, size, nmemb, readhere);

//  nread = (curl_off_t)retcode;

//  fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T
//          " bytes from file\n", nread);
//  return retcode;
//}

size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
    string data((const char*) ptr, (size_t) size * nmemb);
//        *((std::stringstream*) stream) << data << endl;
        return size * nmemb;
}

size_t write_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
    string data((const char*) ptr, (size_t) size * nmemb);
//        *((std::stringstream*) stream) << data << endl;
        return size * nmemb;
}

int storeEasyDL() {
    const char *post_data_filename = "c:\\1.jpg";

    FILE *fp = NULL;
    struct stat stbuf = { 0, };

    fp = fopen(post_data_filename, "rb");

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
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

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

