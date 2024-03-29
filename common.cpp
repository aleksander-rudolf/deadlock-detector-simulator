#include "common.h"
#include <cctype>
#include <chrono>
#include <iostream>

using VS = std::vector<std::string>;

// split string p_line into vector of strings (words)
// the delimiters are 1 or more whitespaces
VS split(const std::string& p_line)
{
    auto line = p_line + " ";
    VS res;
    bool in_str = false;
    std::string curr_word = "";
    for (auto c : line) {
        if (isspace(c)) {
            if (in_str)
                res.push_back(curr_word);
            in_str = false;
            curr_word = "";
        } else {
            curr_word.push_back(c);
            in_str = true;
        }
    }
    return res;
}

std::string stdin_readline()
{
    std::string result;
    while (1) {
        int c = fgetc(stdin);
        if (c == -1)
            break;
        result.push_back(c);
        if (c == '\n')
            break;
    }
    return result;
}

std::string join(const VS& toks, const std::string& sep)
{
    std::string res;
    bool first = true;
    for (auto& t : toks) {
        res += (first ? "" : sep) + t;
        first = false;
    }
    return res;
}

std::string simplify(const std::string& str) { return join(split(str)); }

bool is_alnum(const std::string& str)
{
    for (int c : str)
        if (!isalnum(c))
            return false;
    return true;
}

int Word2Int::get(const std::string& w)
{
    auto f = myset.find(w);
    if (f == myset.end()) {
        myset[w] = counter;
        counter++;
        return counter - 1;
    }
    return f->second;
}

struct Timer::Pimpl {
    std::chrono::time_point<std::chrono::steady_clock> start;
};
Timer::Timer() { pimpl_ = new Pimpl; reset(); }
Timer::~Timer() { delete pimpl_; }
double Timer::elapsed(bool reset_p)
{
    double result =  1e-6
        * std::chrono::duration_cast<std::chrono::microseconds>(
              std::chrono::steady_clock::now() - pimpl_->start)
              .count();
    if( reset_p) reset();
    return result;
}
void Timer::reset() {
    pimpl_-> start= std::chrono::steady_clock::now();
}
