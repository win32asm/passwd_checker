//
// Created by botanic on 3/23/17.
//
#include <map>
#include <iostream>
#include <fstream>
#include "dict.h"

using std::map;
using std::make_pair;
using std::ifstream;

vector<string> wordlist;
map<__uint16_t , unsigned long > index;

__uint16_t word_to_idx(const string &word) {
    if (word.empty()) return 0;
    return (((const __uint16_t)(word.c_str()[0]))<<8) + word.c_str()[1];
}

bool loadDictionary()
{
    ifstream word_file;
    __uint16_t firstch=0, nextch;
    word_file.open("cain.txt", std::ios_base::in);

    if (word_file.fail()) return false;

    while (! word_file.fail() && ! word_file.eof()) {
        wordlist.emplace_back();
        word_file >> wordlist.back();
        nextch = word_to_idx(wordlist.back());
        if (nextch != firstch) {
            firstch = nextch;
            index.insert(make_pair(firstch, wordlist.size() - 1));
        }
    }
    return true;
}

vector<string> generateWords(const string &passwd)
{
    return vector<string>();
}

wordStatus hasWord(const string &word, wordStatus prevStatus)
{
    __uint16_t beg = word_to_idx(word.c_str()), end;
    auto pt = index.lower_bound(beg);
    if (pt == index.end()) return wordStatus::NotFound;
    for (unsigned long idx = pt->second; idx < wordlist.size(); ++idx) {
        const string &dword = wordlist[idx];
        end = word_to_idx(dword);
        if (beg != end) break;
        if (word == dword) return prevStatus;
        auto find_idx = word.find(dword);
        if (prevStatus == wordStatus::Found && find_idx != string::npos) {
            auto t = hasWord(word.substr(dword.size()), wordStatus::FoundMultiWord);
            if (t != wordStatus::NotFound) return t;
            find_idx = dword.size();
            while (find_idx != word.size()) {
                if (!isdigit(word[find_idx])) break;
                ++find_idx;
            }
            if (find_idx == word.size()) {
                return wordStatus::FoundWithNumbersAfter;
            }
        }
    }
    if (prevStatus == wordStatus::Found) {
        size_t partIdx = 0;
        while (partIdx < word.size() && isdigit(word[partIdx])) ++partIdx;
        if (partIdx > 0 && partIdx < word.size()) {
            auto secondpart = word.substr(partIdx);
            beg = word_to_idx(secondpart);
            pt = index.lower_bound(beg);
            if (pt == index.end()) return wordStatus::NotFound;
            for (unsigned long idx = pt->second; idx < wordlist.size(); ++idx) {
                const string &dword = wordlist[idx];
                end = word_to_idx(dword);
                if (beg != end) break;
                if (secondpart == dword) return wordStatus::FoundWithNumbersBefore;
            }
        }
    }
    return wordStatus::NotFound;
}
