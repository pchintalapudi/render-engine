#include <execinfo.h>
#include <csignal>
#include <unistd.h>
#include <iostream>

#include "nodes/elements/selectors/css_selector.h"

using namespace std;

//Shamelessly copied from StackOverflow
void handler(int sig) {
    void *array[10];
    int size;

    // get void*'s for all entries on the stack
    size = backtrace(array, 10);

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}

void begin() {
    string selector;
    getline(cin, selector);
    auto sel = feather::dom::selector::CSSSelector::parse(selector, nullptr);
    cout << sel.toString() << endl;
}

int main() {
    //Also shamelessly copied from stackoverflow
    signal(SIGSEGV, handler);   // install our handler
    signal(SIGABRT, handler);   // install our handler
    begin();
    return 0;
}