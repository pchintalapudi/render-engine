#include <csignal>
#include <unistd.h>
#include <iostream>

#include "nodes/elements/selectors/css_selector.h"
#include "parser/html_tokenizer.h"

using namespace std;

#ifdef __linux__
#include <execinfo.h>

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
#endif

void begin() {
}

int main() {
#ifdef __linux__
    //Also shamelessly copied from stackoverflow
    signal(SIGSEGV, handler);   // install our handler
    signal(SIGABRT, handler);   // install our handler
#endif
#ifdef DEBUG_MODE
    std::cout << "hello" << std::endl;
#endif
    feather::init();
#ifdef DEBUG_MODE
    std::cout << std::endl << "initialized" << std::endl;
#endif
    begin();
    return 0;
}