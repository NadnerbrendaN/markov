/*
 * Copyright (C) 2026 NadnerbrendaN <albertdock@duck.com>
 *
 * A markov chain system for the creation of markov babblers
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 *  If you did not recieve a copy of the MPL-2.0 with this source code, please contact the author
 *  to report the issue, and visit https://www.mozilla.org/media/MPL/2.0/index.f75d2927d3c1.txt
 *  to obtain a copy of the license text.
 */

#include <deque>
#include <vector>
#include <iostream>
#include <map>
#include <ctime>
#include <string>
#include <fstream>
#include <sstream>

const int context_length = 2;
const int max_tokens = 500;
typedef std::deque<std::string> Context;
std::map<Context, std::vector<std::string>> chain;

void add(Context& context, const std::string& word) {
    if (context.size() == context_length) {
        chain[context].push_back(word);
        context.pop_front();
    }
    context.push_back(word);
}

std::string gen(Context context) {
    std::vector<std::string> nexts = chain[context];
    // std::cout << "\n" << context[0] << "|" << context[1] << ": " << nexts.size() << '\n'; 
    if (nexts.size() == 0) {
        return "\n";
    }
    return nexts[std::rand() % nexts.size()];
}

int main(int argc, char** argv) {
    std::string nl = "\n";
    if (argc < 3) {
        throw std::invalid_argument("\nWrong number or format of arguments -- expected:\n-[df] (filename|data0)...(dataN)");
    }
    
    Context context;
    for (int i = 0; i < context_length; ++i) {
        add(context, nl);
    }
    if (argv[1][1] == 'd') {
        for (int i = 2; i < argc; ++i) {
            add(context, argv[i]);
        }
    } else if (argv[1][1] == 'f') {
        std::ifstream file(argv[2]);
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream linestream(line);
            std::string word;
            while (std::getline(linestream, word, ' ')) {
                add(context, word);
            }
        }
    }

    if (argv[1][1] == 'f' && argc == 4 + context_length && argv[3][1] == 'p') {
        for (int i = 4; i < 4 + context_length; ++i) {
            add(context, argv[i]);
            std::cout << argv[i] << " ";
        }
    } else {
        for (int i = 0; i < context_length; ++i) {
            add(context, nl);
        }
    }

    std::srand(time(0));
    std::string next = gen(context);
    // std::cout << context.size() << nl;
    int counter = 0;
    while(next != "\n" && counter < max_tokens) {
        std::cout << next << " ";
        context.pop_front();
        context.push_back(next);

        next = gen(context);
        ++counter;
    }
    std::cout << '\n';
    return 0;
}
