#pragma once

#include <Print.h>
#include <HardwareSerial.h>
#include <LittleFS.h>

namespace mrwski {

/**
 * Sends characters both to file and to serial
 */

class Logger : public Print {

    const size_t flush_trigger      = 1;
    const size_t trunc_trigger      = 128*1024;  // 128kB ~ 4 months
    const size_t trunc_target_size  =  64*1024;  //  64kB ~ 2 months

    static const size_t trunc_buf_size = 2*1024; // 2kB ~ 32 iterations to truncate
    char trunc_buf[trunc_buf_size];

    // serial enable
    bool sen = false;
    HardwareSerial* serial;

    // fs enable
    bool fsen = false;
    File file;

    char filename[32];
    bool prepend_date_flag = true;
    size_t characters_since_flush = 0;

    void prepend_date();
    void truncate();
    static String get_date_time();

public:
    void begin_serial(HardwareSerial& serial_);
    void begin_fs(const char* filename_);
    void flush();

    size_t write(uint8_t c) override;
};

}; // end namespace