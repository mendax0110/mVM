#ifndef MVM_H
#define MVM_H

#include <array>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <functional>
 #include <iomanip>

/// @brief Namespace for minimalistic Virtual Machine  \namespace mVM
namespace mVM
{
    /// @brief Class for VM \class VM
    class VM
    {
    public:
        VM(int *_code, int codeLength, int main, int dataSize, const std::string& oFileName);
        ~VM();

        void cpu();
        void execute();
        void dumpStack();
        void dumpDataMem();
        void dumpCodeMem();
        void disassemble(int ip, int opcode);
        void handleBinaryOp(std::function<int(int, int)> op);
        void handleBrtBrf(int addr, bool cond, int& ip, std::vector<int>& stack, int& sp);
        void handlePrint(int rvalue, std::ofstream& fout, std::vector<int>& stack, int& sp);
        void handleCall(int addr, int nargs, std::vector<int>& stack, int& sp, int& fp, int& ip, int* code);
        void handleRet(int& sp, int& fp, int& ip, std::vector<int>& stack, int nargs);
        void handleInit(int addr, int nargs, std::vector<int>& stack, int& sp, int& fp, int& ip);

        int *code;
        std::vector<int> stack;
        std::vector<int> globals;
        int ip;
        int sp;
        int fp;

        int trace;

        int arraySize;
        int numberOfGlobals;
    private:
        std::string outFileName;
        std::ofstream fout;
    };
}

#endif // MVM_H