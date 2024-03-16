#include <memory>
#include <chrono>

#include "../src/include/mVM.h"
#include "../src/include/parser.h"

using namespace std;
using namespace mVM;


/// @brief Show usage menu
void showMenu()
{
    cout << "Usage: mVM <filename> [-d] [-s <datasize>] [-o <outputfile>]\n";
    cout << "Options:\n";
    cout << "\t-d\t\t\ttrace execution\n";
    cout << "\t-s <datasize>\t\tset data memory size\n";
    cout << "\t-o <outputfile>\t\toutput disassembly to file\n";
}

/// @brief The main function, which executes the minimalistic Virtual Machine
/// @param argc Number of arguments
/// @param argv Array of arguments
/// @return Will return 0 if successful, -1 if failed
int main(int argc, char* argv[])
{
    int datasize = 0;
    string infile, outfile;
    bool boolTrace = false;
    bool infileSet = false;

    if (argc < 2)
    {
        showMenu();
        return 0;
    }

    infile = argv[1];

    for (int i = 1; i < argc; ++i)
    {
        string arg = argv[i];

        if (!infileSet)
        {
            infile = arg;
            infileSet = true;
            continue;
        }

        if (arg == "-d")
        {
            boolTrace = true;
        }
        else if (arg == "-s" && i < argc - 1)
        {
            datasize = stoi(argv[i + 1]);
            ++i;
        }
        else if (arg == "-o" && i < argc - 1)
        {
            outfile = argv[i + 1];
            ++i;
        }
        else
        {
            showMenu();
            return 0;
        }
    }

    array<int, ParserInternals::MAX_TOKENS_PER_FILE> bytecode;

    ParserInternals::Parser parser(infile);
    parser.parse(bytecode);

    if (bytecode[0] == -1)
    {
        return -1;
    }

    auto start = chrono::high_resolution_clock::now();

    auto vm = make_unique<mVM::VM>(bytecode.data(), parser.getszToken(), parser.getiaddr(), datasize, outfile);
    vm->trace = boolTrace;
    vm->execute();

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration = end - start;

    if (boolTrace)
    {
        vm->dumpCodeMem();
    }

    cout << "\n\tduration = " << duration.count() << " ms\n";

    return 0;
}