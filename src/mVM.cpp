/**
 * @file mVM.cpp
 * @author Adrian Goessl
 * @brief This is the micro virtual machine implementation file
 * @version 0.1
 * @date 2024-03-16
 * 
 * @copyright MIT 2024
 * 
 */
#include "../src/include/mVM.h"
#include "../src/include/parser.h"
#include "../src/include/byteCode.h"
#include "../src/include/macroBase.h"

using namespace mVM;
using namespace ParserInternals;
using namespace ByteCodeInternals;
using namespace std;


/// @brief This is the constructor for the VM class
/// @param _code This is the code array
/// @param codeLength This is the length of the code array
/// @param main This is the main function
/// @param dataSize This is the size of the data
/// @param oFileName This is the output file name
VM::VM(int* _code, int codeLength, int main, int dataSize, const string& oFileName)
    : code(_code), arraySize(codeLength), numberOfGlobals(dataSize), ip(main),
    globals(dataSize), stack(DEFAULT_STACK_SIZE), sp(-1), fp(-1), trace(false), outFileName(oFileName),
    fout(oFileName.empty() ? ofstream() : ofstream(oFileName))
{

}


/// @brief This is the destructor for the VM class, which deletes the code array
VM::~VM()
{
    delete[] code;
}

/// @brief This function handles binary operations
/// @param op This is the operation to be performed
void VM::handleBinaryOp(function<int(int, int)> op) 
{
    int b = stack[sp--];
    int a = stack[sp--];
    stack[++sp] = op(a, b);
}

/// @brief The function handles the BRT and BRF instructions
/// @param addr This is the address
/// @param cond This is the condition
/// @param ip Reference to the instruction pointerVM::handlePrint
/// @param stack Reference to the stack
/// @param sp Reference to the stack pointer
void VM::handleBrtBrf(int addr, bool cond, int& ip, vector<int>& stack, int& sp)
{
    if (stack[sp--] == cond)
    {
        ip = addr;
    }
}

/// @brief The function handles the PRINT instruction
/// @param rvalue This is the right value
/// @param fout Reference to the output file stream
/// @param stack Reference to the stack
/// @param sp Reference to the stack pointer
void VM::handlePrint(int rvalue, ofstream& fout, vector<int>& stack, int& sp)
{
    rvalue = stack[sp--];
    
    if (fout.is_open()) 
    {
        fout << rvalue << endl;
    }
    else 
    {
        cout << rvalue << endl;
    }
}

/// @brief The function handles the CALL instruction
/// @param addr This is the address
/// @param nargs This is the number of arguments
/// @param stack Reference to the stack
/// @param sp Reference to the stack pointer
/// @param fp Reference to the frame pointer
/// @param ip Reference to the instruction pointer
/// @param code This is the code array pointer
void VM::handleCall(int addr, int nargs, vector<int>& stack, int& sp, int& fp, int& ip, int* code)
{
    addr = code[ip++];
    nargs = code[ip++];
    stack[++sp] = nargs;
    stack[++sp] = fp;
    stack[++sp] = ip;
    fp = sp;
    ip = addr;
}

/// @brief The function handles the RET instruction
/// @param sp Reference to the stack pointer
/// @param fp Reference to the frame pointer
/// @param ip Reference to the instruction pointer
/// @param stack Reference to the stack
/// @param nargs This is the number of arguments
void VM::handleRet(int& sp, int& fp, int& ip, vector<int>& stack, int nargs)
{
    int res = stack[sp--];
    sp = fp;
    ip = stack[sp--];
    fp = stack[sp--];
    nargs = stack[sp--];
    sp -= nargs;
    stack[++sp] = res;
}

/// @brief The function handles the INIT instruction
/// @param addr This is the address
/// @param nargs This is the number of arguments
/// @param stack Reference to the stack
/// @param sp Reference to the stack pointer
/// @param fp Reference to the frame pointer
/// @param ip Reference to the instruction pointer
void VM::handleInit(int addr, int nargs, vector<int>& stack, int& sp, int& fp, int& ip)
{
    nargs = stack[sp--];
    addr = stack[sp--];
    
    for (int i = 0; i < nargs; i++) 
    {
        stack[fp + static_cast<vector<int, allocator<int>>::size_type>(i)] 
            = stack[sp - static_cast<vector<int, allocator<int>>::size_type>(nargs) + i + 1];
    }

    sp -= nargs;
    ip = addr;
}

/// @brief This function is the main CPU loop
void VM::cpu() 
{
    int addr = 0;
    int offset;
    int rvalue = 0;
    int nargs = 0;

    if (!outFileName.empty()) 
    {
        fout.open(outFileName, ios::app);
        if (!fout.is_open()) 
        {
            cerr << "Failed to open '" << outFileName << "' file\n";
        }
    }

    int opcode = code[ip];

    while (opcode != ByteCode::HALT && ip < arraySize) 
    {
        if (trace == 1) 
        {
            disassemble(ip, opcode);
        }

        ip++;
        switch (opcode) 
        {
            case ByteCode::IADD:
                handleBinaryOp([](int a, int b) { return a + b; });
                break;
            case ByteCode::ISUB:
                handleBinaryOp([](int a, int b) { return a - b; });
                break;
            case ByteCode::IMUL:
                handleBinaryOp([](int a, int b) { return a * b; });
                break;
            case ByteCode::ILT:
                handleBinaryOp([](int a, int b) { return a < b; });
                break;
            case ByteCode::IEQ:
                handleBinaryOp([](int a, int b) { return a == b; });
                break;
            case ByteCode::BR:
                ip = code[ip];
                break;
            case ByteCode::BRT: 
                handleBrtBrf(addr, true, ip, stack, sp);
                break;
            case ByteCode::BRF: 
                handleBrtBrf(addr, false, ip, stack, sp);
                break;
            case ByteCode::ICONST: 
                stack[++sp] = code[ip++];
                break;
            case ByteCode::LOAD: 
                offset = code[ip++];
                stack[++sp] = stack[fp + static_cast<vector<int, allocator<int>>::size_type>(offset)];
                break;
            case ByteCode::GLOAD:
                offset = code[ip++];
                stack[++sp] = globals[offset];
                break;
            case ByteCode::STORE:
                offset = code[ip++];
                stack[fp + static_cast<vector<int, allocator<int>>::size_type>(offset)] = stack[sp--];
                break;
            case ByteCode::GSTORE:
                offset = code[ip++];
                globals[offset] = stack[sp--];
                break;
            case ByteCode::PRINT:
                handlePrint(rvalue, fout, stack, sp);
                break;
            case ByteCode::POP:
                --sp;
                break;
            case ByteCode::CALL: 
                handleCall(addr, nargs, stack, sp, fp, ip, code);
                break;
            case ByteCode::RET: 
                handleRet(sp, fp, ip, stack, nargs);
                break;
            case ByteCode::INIT:
                handleInit(addr, nargs, stack, sp, fp, ip);
                break;
            case ByteCode::HALT: 
                break;
            default: 
                cerr << "Unknown opcode: " << opcode << endl;
                throw runtime_error("Unknown opcode");
        }

        if (trace == 1) 
        {
            dumpStack();
        }

        opcode = code[ip];
    }

    if (trace == 1) 
    {
        disassemble(ip, code[ip]);
        dumpStack();
        dumpDataMem();
    }

    if (fout.is_open()) 
    {
        fout.close();
    }
}

/// @brief This function executes the VM
void VM::execute() 
{
    try
    {
        cpu();
    }
    LOG_EXCEPTION_AND_CONTINUE("An error occurred while running the VM.");
}

/// @brief This function dumps the stack
void VM::dumpStack() 
{
    ostream& out = fout.is_open() ? fout : cout;
    out << "      stack=[";
    
    for (int i = 0; i <= sp; i++) 
    {
        if (i > 0) 
        {
            out << ",";
        }
        out << i << "=" << stack[i];
    }

    out << "]\n";
}

/// @brief This function disassembles the code
/// @param ip This is the instruction pointer
/// @param opcode This is the opcode
void VM::disassemble(int ip, int opcode) 
{
    cout << "Opcode: " << opcode << endl;

    const char *instr = ByteCode::opName[opcode];
    int nops = ByteCode::operands[opcode];
    ostream& out = fout.is_open() ? fout : cout;

    out << setfill('0') << setw(4) << ip << ": " << setw(6) << instr;

    if (nops == 1) 
    {
        out << setw(4) << code[ip + 1];
    } 
    else if (nops == 2)
    {
        out << " " << code[ip + 1] << " " << code[ip + 2];
    }
    else if (opcode != 14)
    {
        out << "    ";
    }

    out << "\n";
}

/// @brief This function dumps the data memory
void VM::dumpDataMem()
{
    ostream& out = fout.is_open() ? fout : cout;
    out << "\n\tData memory\n\t---------\n";

    for (int i = 0; i < numberOfGlobals; i++)
    {
        if (globals[i] >= 0)
        {
            out << "\t" << setfill('0') << setw(4) << i << ": " << globals[i] << "\n";
        }
    }

    out << "\n";
}

/// @brief This function dumps the code memory
void VM::dumpCodeMem()
{
    ostream& out = fout.is_open() ? fout : cout;
    out << "\n\tCode memory:\n\t---------\n";

    for (int i = 0; i < arraySize; i++)
    {
        if (code[i] >= 0)
        {
            out << "\t" << setfill('0') << setw(4) << i << ": " << code[i] << "\n";
        }
    }

    out << "\n";
}