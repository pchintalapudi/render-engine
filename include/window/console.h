//
// Created by prem on 12/9/2018.
//

#ifndef FEATHER_CONSOLE_H
#define FEATHER_CONSOLE_H

#include "typedefs.h"
#include <functional>
#include <map>
#include <ostream>

namespace window {
    class Console;

    enum class LogLevel {
        DEBUG, INFO, LOG, WARN, ERROR
    };
}

//In theory this will be replaced by the Javascript engine's console object
class window::Console {
public:

    inline void log(LogLevel logLevel, DOMString data) { out << data; }

    inline void assert(bool success, std::function<DOMString()> messageProducer) {
        if (!success) error(messageProducer());
    }

    inline void clear() {/*There's no way to clear the output stream as of right now*/ out.flush(); }

    inline void debug(DOMString data) { log(LogLevel::DEBUG, data); }

    inline void error(DOMString data) { log(LogLevel::ERROR, data); }

    inline void log(DOMString data) { log(LogLevel::LOG, data); }

    inline void table(DOMString data) { log(LogLevel::LOG, data); }

    inline void trace(DOMString data) { log(LogLevel::DEBUG, data); }

    inline void warn(DOMString data) { log(LogLevel::WARN, data); }

    void count(DOMString data) {
        if (countMap.find(data) == countMap.end())
            log(LogLevel::LOG, data + " count: " + std::to_string(countMap[data] = 1));
        else log(LogLevel::LOG, data + " count: " + std::to_string(countMap[data]++));
    }

    void countReset(DOMString data) {
        if (countMap.find(data) == countMap.end()) log(LogLevel::LOG, data + " count: 0");
        else log(LogLevel::LOG, data + " count: " + std::to_string(countMap[data] = 0));
    }

private:
    std::ostream &out;
    std::map<DOMString, unsigned long> countMap;
};

#endif //FEATHER_CONSOLE_H
