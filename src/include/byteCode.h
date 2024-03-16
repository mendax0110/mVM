#ifndef BYTECODE_H
#define BYTECODE_H

#include <array>
#include <fstream>

/// @brief Namespace for ByteCode  \namespace ByteCodeInternals
namespace ByteCodeInternals
{
    constexpr int MAX_TOKENS_PER_FILE = 1024;
    constexpr int DEFAULT_STACK_SIZE = 1000;

    /// @brief Class for ByteCode \class ByteCode
    class ByteCode
    {
    public:
        ByteCode() = default;
        //~ByteCode() = default;

        static constexpr int NUM_OPCODES = 20;
        static std::array<const char*, NUM_OPCODES> opName;
        static std::array<int, NUM_OPCODES> operands;

        enum OpCode : unsigned short
        {
            IADD = 1,
            ISUB,
            IMUL,
            ILT,
            IEQ,
            BR,
            BRT,
            BRF,
            ICONST,
            LOAD,
            GLOAD,
            STORE,
            GSTORE,
            PRINT,
            POP,
            HALT,
            CALL,
            RET,
            INIT
        };
    };
}


#endif // BYTECODE_H