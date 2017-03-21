#include <iostream>
#include <string>
#include <zconf.h>

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
            } else {
                passwd += ch;
            }
            cputs("\renter password>");
            for(char i:passwd) {
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

}