#include <iostream>
#include <cmath>

#ifndef WIN32
#include "conio.h"
#endif

using std::string;
using std::cin;
using std::cout;
using std::endl;

void analyze_password(string password);

int main() {
    bool ask_pass = true;

    clrscr();

    cputs("Password Checker v. 0.1 loading...\n");

    do {
        string passwd = "";
        int sym;
        char ch = 0;
        cputs("don`t enter an existing password - use a new one\nenter password>");
        do {
            sym = getch();
            ch = (char) sym;
            if (ch=='\b' && passwd.length() != 0) {
                passwd.erase(passwd.begin() + ( passwd.length() - 1));
            } else if (ch == '\0') {
                ch = (char) getch(); // ignore next char ?
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
            cputs("\renter password>");
            for(int i=0; i<passwd.length(); ++i) {
                cputs("*");
            }
            cputs("  \b\b"); // erase stars
        }while (true);
        cputs("\n");
        if (passwd.length() != 0) {
            analyze_password(passwd);
        } else {
            ask_pass = false;
        }
    } while (ask_pass);

    return 0;
}

void analyze_password(string password) {
    cprintf("Password length: %i characters\n", password.length());

    bool cat_num = false, cat_sym = false, cat_upper = false, cat_lower = false;
    for(char i : password) {
        if (isupper(i)) cat_upper = true;
        else if (islower(i)) cat_lower = true;
        else if (isdigit(i)) cat_num = true;
        else cat_sym = true;
    }

    cprintf("Password has following character categories:\n\t%s%s%s%s\n",
            cat_num?"numbers ":"", cat_lower?"lowercase ":"",
            cat_upper?"uppercase ":"", cat_sym?"symbolic":"");

    double var, pwvars;
    var = (cat_num?10:0) + (cat_upper?26:0)+(cat_lower?26:0) +(cat_sym?33:0);

    pwvars=pow(var, password.length()) / 2.0e6;

    cputs("approximate hacking time on one CPU system: ");
    if (pwvars < 60) {
        cprintf("%i seconds\n", (int)pwvars);
    } else if (pwvars < 3600) {
        cprintf("%i minutes %i seconds\n", (int)(pwvars/60), (int(pwvars)%60));
    } else if (pwvars < 3600 * 24) {
        cprintf("%i hours %i minutes\n", int(pwvars/3600), (int(pwvars)%3600)/60);
    } else if (pwvars < (3600ULL * 24 * 365)) {
        cprintf("%u days\n", int (pwvars/(3600ULL*24)));
    } else if (pwvars < (3600ULL * 24 * 365 * 10)) {
        cprintf("%u years %u days\n", (unsigned) (pwvars/(3600UL*24*365)), (unsigned)(pwvars/3600UL*24)%365);
    } else {
        cprintf("%lf years\n", (pwvars/(3600UL*24*365)));
    }

    cputs("approximate hacking time on a distributed 1000-CPU cluster: ");
    pwvars/=1000.0;
    if (pwvars < 60) {
        cprintf("%i seconds\n", (int)pwvars);
    } else if (pwvars < 3600) {
        cprintf("%i minutes %i seconds\n", (int)(pwvars/60), (int(pwvars)%60));
    } else if (pwvars < 3600 * 24) {
        cprintf("%i hours %i minutes\n", int(pwvars/3600), (int(pwvars)%3600)/60);
    } else if (pwvars < (3600ULL * 24 * 365)) {
        cprintf("%u days\n", int (pwvars/(3600ULL*24)));
    } else if (pwvars < (3600ULL * 24 * 365 * 10)) {
        cprintf("%u years %u days\n", (unsigned) (pwvars/(3600UL*24*365)), (unsigned)(pwvars/3600UL*24)%365);
    } else {
        cprintf("%lf years\n", (pwvars/(3600UL*24*365)));
    }
}