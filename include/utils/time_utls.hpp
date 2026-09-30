#pragma once

#include <ctime>
#include <sstream>
#include <string>
#include <chrono>
#include <iomanip>

using namespace std;

class TimeConverter {
    public:
        inline static std::chrono::system_clock::time_point
        sqliteToTimePoint(const std::string &s) {
            std::tm tm{};
    
            int Y, M, D, h, m, sec;
    
            if (std::sscanf(s.c_str(), "%d-%d-%d %d:%d:%d", &Y, &M, &D, &h, &m,
                            &sec) != 6) {
                throw std::runtime_error("Bad datetime: " + s);
            }
    
            tm.tm_year = Y - 1900;
            tm.tm_mon = M - 1;
            tm.tm_mday = D;
            tm.tm_hour = h;
            tm.tm_min = m;
            tm.tm_sec = sec;
            tm.tm_isdst = 0;
    
            std::time_t t = timegm(&tm);
    
            if(t == static_cast<std::time_t>(-1)) throw std::runtime_error("timegm failed: " + s);
    
            return std::chrono::system_clock::from_time_t(t);
        }
    
        inline static string timePointToString(std::chrono::system_clock::time_point& time) {
            std::time_t t = std::chrono::system_clock::to_time_t(time);

            stringstream ss;
            ss << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S");

            string str = ss.str();
            return str;
        }
};