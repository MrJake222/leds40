#include "logger.hpp"

namespace mrwski {

void Logger::begin_serial(HardwareSerial& serial_) {
    sen = true;
    serial = &serial_;
}

void Logger::begin_fs(const char* filename_) {
    strcpy(filename, filename_);

    file = LittleFS.open(filename, "a+"); // append
    if (!file) {
        println("log open failed");
    }

    else {
        println("log file opened");
        fsen = true;
    }
}

void Logger::flush() {
    if (fsen)
        file.flush();
    characters_since_flush = 0;
}

size_t Logger::write(uint8_t c) {
    if (prepend_date_flag) {
        prepend_date();
        prepend_date_flag = false;
    }

    if (fsen)
        file.write(c);
    if (sen)
        serial->write(c);

    characters_since_flush += 1;

    if (c == '\n') {
        prepend_date_flag = true;

        if (characters_since_flush >= flush_trigger) {
            flush();
            if (fsen && (file.size() >= trunc_trigger))
                truncate();
        }
    }

    return 1;
}

void Logger::prepend_date() {
    if (fsen) {
        file.print(get_date_time());
        file.print(": ");
    }

    if (sen) {
        serial->print(get_date_time());
        serial->print(": ");
    }
}

void Logger::truncate() {
    if (!fsen)
        return;

    size_t size_pre = file.position();
    long time_pre = millis();

    file.seek(trunc_target_size, SeekEnd);
    while (file.read() != '\n'); // find first newline

    File file_tmp = LittleFS.open("/log.tmp", "w");
    size_t read;
    do {
        read = file.readBytes(trunc_buf, trunc_buf_size);
        file_tmp.write(trunc_buf, read);
    } while (read == trunc_buf_size);
    file_tmp.close();

    file.close();
    LittleFS.rename("/log.tmp", filename);
    file = LittleFS.open(filename, "a+");

    file.seek(0, SeekEnd);
    size_t size_post = file.position();
    long time_post = millis();

    printf("Truncated log file from %d to %d bytes (took %ld ms)\n", size_pre, size_post, time_post-time_pre);
}

String Logger::get_date_time() {
    time_t now;
    tm tm;
    time(&now);
    localtime_r(&now, &tm);

    const char MONTHS[12][4] = { "Sty", "Lut", "Mar", "Kwi", "Maj", "Cze", "Lip", "Sie", "Wrz", "Paz", "Lis", "Gru" };
    const char WDAYS[7][6] = { "Niedz", "Pon", "Wt", "Sr", "Czw", "Pt", "Sob" };

    char buf[32];

    if (1900 + tm.tm_year == 1970) {
        // not set
        return "";
    }

    // Wt, 20 Sty 2022, 12:40:44
    sprintf(buf, "%s, %02d %s %d, %02d:%02d:%02d", WDAYS[tm.tm_wday], tm.tm_mday, MONTHS[tm.tm_mon], 1900 + tm.tm_year, tm.tm_hour, tm.tm_min, tm.tm_sec);
    String date(buf);

    return date;
}

}; // end namespace