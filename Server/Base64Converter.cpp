﻿#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#pragma warning(disable: 26451)
#pragma warning(disable: 6297)

namespace base64 {
    const std::string table("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
    inline bool encode(const std::vector<unsigned char>& src, std::string& dst) {
        std::string cdst;
        for (std::size_t i = 0; i < src.size(); ++i) {
            switch (i % 3) {
            case 0:
                cdst.push_back(table[(src[i] & 0xFC) >> 2]);
                if (i + 1 == src.size()) {
                    cdst.push_back(table[(src[i] & 0x03) << 4]);
                    cdst.push_back('=');
                    cdst.push_back('=');
                }
                break;
            case 1:
                cdst.push_back(table[((src[i - 1] & 0x03) << 4) | ((src[i + 0] & 0xF0) >> 4)]);
                if (i + 1 == src.size()) {
                    cdst.push_back(table[(src[i] & 0x0F) << 2]);
                    cdst.push_back('=');
                }
                break;
            case 2:
                cdst.push_back(table[((src[i - 1] & 0x0F) << 2) | ((src[i + 0] & 0xC0) >> 6)]);
                cdst.push_back(table[src[i] & 0x3F]);
                break;
            }
        }
        dst.swap(cdst);
        return true;
    }

    inline bool decode(const std::string& src, std::vector<unsigned char>& dst) {
        if (src.size() & 0x00000003) return false;
        std::vector<unsigned char> cdst;
        for (std::size_t i = 0; i < src.size(); i += 4) {
            if (src[i + 0] == '=') return false;
            else if (src[i + 1] == '=')  false;
            else if (src[i + 2] == '=') {
                const std::string::size_type s1 = table.find(src[i + 0]);
                const std::string::size_type s2 = table.find(src[i + 1]);
                if (s1 == std::string::npos || s2 == std::string::npos)  return false;
                cdst.push_back(static_cast<unsigned char>(((s1 & 0x3F) << 2) | ((s2 & 0x30) >> 4)));
                break;
            }
            else if (src[i + 3] == '=') {
                const std::string::size_type s1 = table.find(src[i + 0]);
                const std::string::size_type s2 = table.find(src[i + 1]);
                const std::string::size_type s3 = table.find(src[i + 2]);
                if (s1 == std::string::npos || s2 == std::string::npos || s3 == std::string::npos) return false;
                cdst.push_back(static_cast<unsigned char>(((s1 & 0x3F) << 2) | ((s2 & 0x30) >> 4)));
                cdst.push_back(static_cast<unsigned char>(((s2 & 0x0F) << 4) | ((s3 & 0x3C) >> 2)));        
                break;
            }
            else {
                const std::string::size_type s1 = table.find(src[i + 0]);
                const std::string::size_type s2 = table.find(src[i + 1]);
                const std::string::size_type s3 = table.find(src[i + 2]);
                const std::string::size_type s4 = table.find(src[i + 3]);
                if (s1 == std::string::npos || s2 == std::string::npos || s3 == std::string::npos || s4 == std::string::npos) return false;
                cdst.push_back(static_cast<unsigned char>(((s1 & 0x3F) << 2) | ((s2 & 0x30) >> 4)));
                cdst.push_back(static_cast<unsigned char>(((s2 & 0x0F) << 4) | ((s3 & 0x3C) >> 2)));
                cdst.push_back(static_cast<unsigned char>(((s3 & 0x03) << 6) | ((s4 & 0x3F) >> 0)));
            }
        }
        dst.swap(cdst);
        return true;
    }
    std::string encode(const std::string& str) {
        auto ucharbufstr = reinterpret_cast<const unsigned char*>(str.c_str());
        std::vector<unsigned char> arr(ucharbufstr, ucharbufstr + str.size());
        std::string buf{};
        encode(arr, buf);
        buf.resize(std::strlen(buf.c_str()));
        return buf;
    }

    std::string decode(const std::string& str) {
        std::vector<unsigned char> buf{};
        decode(str, buf);
        std::stringstream ss{};
        for (const auto& i : buf) ss << std::hex << i;
        return ss.str();
    }
}
