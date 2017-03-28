//
// Created by botanic on 3/23/17.
//
#include <map>
#include <iostream>
#include <fstream>
#include "dict.h"

#ifdef _WIN32_WINNT
#include <cstdint>
#endif

using std::map;
using std::make_pair;
using std::ifstream;

vector<string> wordlist;
map<uint16_t , unsigned long > index;

uint16_t word_to_idx(const string &word) {
    if (word.empty()) return 0;
    return (((const uint16_t)(word.c_str()[0]))<<8) + word.c_str()[1];
}

bool loadDictionary()
{
    ifstream word_file;
    uint16_t firstch=0, nextch;
    word_file.open("cain.txt", std::ios_base::in);

    if (word_file.fail()) return false;

    while (! word_file.fail() && ! word_file.eof()) {
        char buffer[128];
        word_file.getline(buffer, 128);
        wordlist.emplace_back(buffer);
        nextch = word_to_idx(wordlist.back());
        if (nextch != firstch) {
            firstch = nextch;
            index.insert(make_pair(firstch, wordlist.size() - 1));
        }
    }
    return true;
}

vector<char> getReplaces(char ch) {
    switch (ch) {
        case '@':
        case '4': return vector<char>({'a'});
        case '!':
        case '1':
        case '|': return vector<char>({'i','l'});
        case '#': return vector<char>({'h'});
        case '$':
        case '5': return vector<char>({'s'});
        case '(': return vector<char>({'c'});
        case '6':
        case '8': return vector<char>({'b'});
        case '3': return vector<char>({'e'});
        case '7': return vector<char>({'t','z'});
        case '9': return vector<char>({'g','q'});
        case '0':
        case '*': return vector<char>({'o'});
        default:break;
    }
    return vector<char>();
}

vector<string> generateWords(const string &passwd)
{
    vector<string> variants;
    string lowcase = passwd;
    for (auto &ch:lowcase) {
        if (isalpha(ch)) ch = (char) tolower(ch);
    }
    variants.push_back(lowcase);

    string modchars=lowcase;
    for (size_t i=0; i<modchars.length(); ++i) {
        auto x = getReplaces(modchars[i]);
        if (x.size() != 0) {
            for (long y= (long) (variants.size() - 1); y >= 0; --y) {
                for (char ch:x) {
                    variants.emplace_back(variants[y]);
                    variants.back()[i] = ch;
                }
            }
        }
    }

    if (variants.front() == passwd) {
        variants.front() = variants.back();
        variants.erase(variants.end() - 1);
    }
    return variants;
}

wordStatus hasWord(const string &word, wordStatus best_case_status)
{
    uint16_t beg = word_to_idx(word.c_str()), end;
    auto pt = index.lower_bound(beg);
    auto best_guess = wordStatus ::NotFound;
    if (pt == index.end()) return best_guess;
    for (unsigned long idx = pt->second; idx < wordlist.size(); ++idx) {
        const string &dword = wordlist[idx];
        end = word_to_idx(dword);
        if (beg != end) break;
        if (word == dword) return best_case_status;
        auto find_idx = word.find(dword);
        if (best_case_status == wordStatus::Found && find_idx != string::npos) {
            if (best_guess == wordStatus::NotFound) {
                auto t = hasWord(word.substr(dword.size()), wordStatus::FoundMultiWord);
                if (t != wordStatus::NotFound) {
                    best_guess = wordStatus::FoundMultiWord;
                    continue;
                }
            }
            find_idx = dword.size();
            while (find_idx != word.size()) {
                if (!isdigit(word[find_idx])) break;
                ++find_idx;
            }
            if (find_idx == word.size()) {
                best_guess = wordStatus::FoundWithNumbersAfter;
            }
        }
    }
    if (best_case_status == wordStatus::Found) {
        size_t partIdx = 0;
        while (partIdx < word.size() && isdigit(word[partIdx])) ++partIdx;
        if (partIdx > 0 && partIdx < word.size()) {
            auto secondpart = word.substr(partIdx);
            beg = word_to_idx(secondpart);
            pt = index.lower_bound(beg);
            if (pt == index.end()) return best_guess;
            for (unsigned long idx = pt->second; idx < wordlist.size(); ++idx) {
                const string &dword = wordlist[idx];
                end = word_to_idx(dword);
                if (beg != end) break;
                if (secondpart == dword) {
                    best_guess = wordStatus::FoundWithNumbersBefore;
                    break;
                }
            }
        }
    }
    return best_guess;
}
