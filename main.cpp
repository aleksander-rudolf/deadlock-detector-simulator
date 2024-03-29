#include "common.h"
#include "deadlock_detector.h"
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <memory>
#include <numeric>
#include <set>
#include <vector>

using VS = std::vector<std::string>;

static void run_graph()
{
    std::cout << "Reading in lines from stdin...\n";
    VS all_lines;
    int line_no = 0;
    while (1) {
        // read in the next line and quit loop on EOF
        auto line = stdin_readline();
        if (line.size() == 0)
            break;
        line_no++;

        // get rid of trailing \n
        if (line.size() && line.back() == '\n')
            line.pop_back();

        // parse input line, skip empty lines
        auto toks = split(line);
        if (toks.size() == 0)
            continue;

        // validate line
        if (toks.size() != 3 || (toks[1] != "->" && toks[1] != "<-") || !is_alnum(toks[0])
            || !is_alnum(toks[2])) {
            std::cout << "Syntax error on line " << line_no << ": " << line << "\n";
            exit(-1);
        }

        all_lines.push_back(line);
    }

    std::cout << "Running detect_deadlock()...\n";
    Timer timer;
    Result res = detect_deadlock(all_lines);
    std::cout << "\n"
              << "edge_index : " << res.edge_index << "\n"
              << "dl_procs   : [" << join(res.dl_procs, ",") << "]\n"
              << "real time  : " << std::fixed << std::setprecision(4) << timer.elapsed()
              << "s\n\n";
}
static int usage(const std::string & pname)
{
    std::cout << "Usage:\n"
              << "    " << pname << " < input\n"
              << "        - to process input from stdin\n";
    exit(-1);
}

static int cppmain(const VS & args)
{
    if (args.size() != 1)
        usage(args[0]);
    run_graph();
    return 0;
}

int main(int argc, char ** argv) 
{ 
  return cppmain({ argv + 0, argv + argc }); }
