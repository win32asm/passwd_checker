//
// Created by botanic on 3/23/17.
//

#ifndef PASSWD_CHECKER_DICT_H
#define PASSWD_CHECKER_DICT_H

#include <vector>
#include <string>

using std::vector;
using std::string;

enum class wordStatus {
    NotFound,
    Found,
    FoundMultiWord,
    FoundSpaceMultiWord,
    FoundWithNumbersBefore,
    FoundWithNumbersAfter,
};

extern vector<string> wordlist;

bool loadDictionary();
vector<string> generateWords(const string &passwd);
wordStatus hasWord(const string &passwds, wordStatus best_case_status = wordStatus::Found);

#endif //PASSWD_CHECKER_DICT_H
