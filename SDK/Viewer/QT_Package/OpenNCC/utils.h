#ifndef UTILS_H
#define UTILS_H
#include <QString>
#include <sdk.h>
#include <QDebug>

int loadFw(const QString& bootPath,const QString& fwPath);

void setStyleByCss();

extern int manualset_exp;
extern int streamrun;
extern int mexp_gain;
extern int mexp_time;

class BinaryWriter {
    public:
        BinaryWriter(){
            fp_ = nullptr;
        }
        bool open(std::string path) {
            fp_ = fopen(path.c_str(), "wb");
            if (fp_ == NULL) {
                return false;
            }
            return true;
        }

        bool append(unsigned char* data, int len) {
            if (fp_ == nullptr) {
                return false;
            }

            int freeSize = len;

            while (freeSize > 0) {

                int tmpSize = fwrite(data + len - freeSize, 1, freeSize, fp_);
                if (tmpSize <= 0) {
                    fflush(fp_);
                    return false;
                }
                freeSize -= tmpSize;
            }
            fflush(fp_);
            return true;
        }

        void close() {
            if(fp_ != nullptr){
                fclose(fp_);
            }else{
                qInfo()<<__FUNCTION__<<", login error";
            }
            fp_ = NULL;
        }
    private:

        FILE* fp_;
    };



#endif // UTILS_H
