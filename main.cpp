#include <string>
#include <cmath>
#include "dict.h"
#ifndef _WIN32_WINNT
#include "my_conio.h"

#else
#include <conio.h>
#include <windows.h>
void _clrscr()
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = {0, 0};
    DWORD count;

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hStdOut, &csbi);

    FillConsoleOutputCharacter(hStdOut, ' ', (DWORD)(csbi.dwSize.X * csbi.dwSize.Y), coord, &count);

    SetConsoleCursorPosition(hStdOut, coord);
}
#endif

using std::string;

void analyze_password(const string &password);

int word_diff(const string &rh_word, const string &lh_word);

int main() {
    bool ask_pass = true;

    _clrscr();

    _cputs("Password Checker v. 0.1 loading...\n");

    if (!loadDictionary()) {
        _cputs("Failed to load dictionary\n");
        return 1;
    }

    _cprintf("loaded dictionary: %lu words\n", wordlist.size());

    do {
        string passwd = "";
        int sym;
        char ch = 0;
        _cputs("\ndon`t enter an existing password - use a new one\nenter password>");
        do {
            sym = _getch();
            ch = (char) sym;
            if (ch=='\b' && passwd.length() != 0) {
                passwd.erase(passwd.begin() + ( passwd.length() - 1));
            } else if (ch == '\0') {
                ch = (char) _getch(); // ignore next char ?
                if (ch == 0x39) { // backspace?
                    passwd.erase(passwd.begin() + ( passwd.length() - 1));
                } else {
                    continue;
                }
            } else if (ch == '\r' || ch == '\n') {
                break;
            } else if (ch == '\x1b') {
                ask_pass = false;
                break;
            } else if (ch < ' ' || ch > '\x7f') { // ignore
                continue;
            } else {
                passwd += ch;
            }
            _cputs("\renter password>");
            for(int i=0; i<passwd.length(); ++i) {
                _cputs("*");
            }
            _cputs("  \b\b"); // erase stars
        }while (true);
        _cputs("\n");
        if (passwd.length() != 0) {
            analyze_password(passwd);
        } else {
            ask_pass = false;
        }
    } while (ask_pass);

    return 0;
}

char emptystr[1]={'\0'};

void print_estimate(int len, double variants, size_t nDigits = 0) {
    double pwvars = ( pow(variants, len) * pow(10, nDigits) ) / 2.0e6;

    _cputs("approximate hacking time on one CPU system: ");
    if (pwvars < 60) {
        _cprintf("%i seconds\n", (int)pwvars);
    } else if (pwvars < 3600) {
        _cprintf("%i minutes %i seconds\n", (int)(pwvars/60), (int(pwvars)%60));
    } else if (pwvars < 3600 * 24) {
        _cprintf("%i hours %i minutes\n", int(pwvars/3600), (int(pwvars)%3600)/60);
    } else if (pwvars < (3600ULL * 24 * 365)) {
        _cprintf("%u days\n", int (pwvars/(3600ULL*24)));
    } else if (pwvars < (3600ULL * 24 * 365 * 10)) {
        _cprintf("%u years %u days\n", (unsigned) (pwvars/(3600UL*24*365)), (unsigned)(pwvars/3600UL*24)%365);
    } else {
        _cprintf("%lf years\n", (pwvars/(3600UL*24*365)));
    }

    _cputs("approximate hacking time on a distributed 1000-CPU cluster: ");
    pwvars/=1000.0;
    if (pwvars < 60) {
        _cprintf("%i seconds\n", (int)pwvars);
    } else if (pwvars < 3600) {
        _cprintf("%i minutes %i seconds\n", (int)(pwvars/60), (int(pwvars)%60));
    } else if (pwvars < 3600 * 24) {
        _cprintf("%i hours %i minutes\n", int(pwvars/3600), (int(pwvars)%3600)/60);
    } else if (pwvars < (3600ULL * 24 * 365)) {
        _cprintf("%u days\n", int (pwvars/(3600ULL*24)));
    } else if (pwvars < (3600ULL * 24 * 365 * 10)) {
        _cprintf("%u years %u days\n", (unsigned) (pwvars/(3600UL*24*365)), (unsigned)(pwvars/3600UL*24)%365);
    } else {
        _cprintf("%lf years\n", (pwvars/(3600UL*24*365)));
    }
}

bool find_in_dict(const string& word, const string &orig_word, const char *neu = emptystr) {
    auto st = hasWord(word);

    if (st == wordStatus::Found) {
        _cprintf("%sDictionary word detected\n", neu);
        print_estimate(1, word_diff(word, orig_word) * wordlist.size());
        return true;
    }

    if (st == wordStatus::FoundWithNumbersBefore) {
        _cprintf("%sDictionary word with numbers detected\n", neu);
        size_t i=0;
        while(isdigit(word[i])) ++i;
        print_estimate(1, word_diff(word, orig_word) * wordlist.size(), i-1);
        return true;
    }

    if (st == wordStatus::FoundWithNumbersAfter) {
        _cprintf("%sDictionary word with numbers detected\n", neu);
        size_t i= word.length() - 1;
        while (isdigit(word[i])) --i;
        print_estimate(1, word_diff(word, orig_word) * wordlist.size(), word.length() - i - 1);
        return true;
    }

    if (st == wordStatus::FoundMultiWord) {
        _cprintf("2 %sDictionary words detected\n", neu);
        print_estimate(2, word_diff(word, orig_word) * wordlist.size());
        return true;
    }

    if (st == wordStatus::FoundSpaceMultiWord) {
        _cprintf("2 space-delimited %sDictionary words detected\n", neu);
        print_estimate(2, 2*word_diff(word, orig_word) * wordlist.size());
        return true;
    }

    return false;
}

int word_diff(const string &rh_word, const string &lh_word) {
    int diff = 1;
    for (int i=0; i<rh_word.size(); ++i) {
        if (rh_word[i] != lh_word[i]) ++ diff;
    }
    return diff;
}

void analyze_password(const string &password) {
    _cprintf("Password length: %i characters\n", password.length());

    if (find_in_dict(password, password)) {
        return;
    }

    auto passwds = generateWords(password);

    for (auto &passwd: passwds) {
        if (find_in_dict(passwd, password, "modified ")) {
            return;
        }
    }

    bool cat_num = false, cat_sym = false, cat_upper = false, cat_lower = false;
    for(char i : password) {
        if (isupper(i)) cat_upper = true;
        else if (islower(i)) cat_lower = true;
        else if (isdigit(i)) cat_num = true;
        else cat_sym = true;
    }

    _cprintf("Password has following character categories:\n\t%s%s%s%s\n",
            cat_num?"numbers ":"", cat_lower?"lowercase ":"",
            cat_upper?"uppercase ":"", cat_sym?"symbolic":"");

    double var;
    var = (cat_num?10:0) + (cat_upper?26:0)+(cat_lower?26:0) +(cat_sym?33:0);

    print_estimate((int) password.size(), var);
}

