/**
 * @file parser.cpp
 * @author Adrian Goessl
 * @brief This is the implementation of the parser
 * @version 0.1
 * @date 2024-03-16
 * 
 * @copyright MIT 2024
 * 
 */
#include "../src/include/parser.h"
#include "../src/include/mVM.h"
#include "../src/include/byteCode.h"
#include "../src/include/macroBase.h"

using namespace std;
using namespace ParserInternals;
using namespace ByteCodeInternals;


array<const char*, ByteCode::NUM_OPCODES> ByteCode::opName = {
    nullptr, "iadd", "isub", "imul", "ilt", "ieq", "br", "brt",
    "brf", "iconst", "load", "gload", "store", "gstore",
    "print", "pop", "halt", "call", "ret", "init"
};

array<int, ByteCode::NUM_OPCODES> ByteCode::operands = {
    0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 2, 0, 0
};

/// @brief This is the constructor for the Parser class
/// @param ifilename Reference to the input file name
Parser::Parser(const string& ifilename)
     : infilename(ifilename), iaddr(0), szToken(0), token()
{
    fin.open(infilename);

    if (!fin.is_open())
    {
        cerr << "Failed to open '" << infilename << "' file\n";
        throw runtime_error("Failed to open the input file.");
    }

    cout << "File opened successfully: " << infilename << endl;
}


/// @brief This function finds the opcode
/// @param opstr Reference to the opcode string
/// @return Will return the opcode
int Parser::find(const string& opstr)
{
    for (auto i = 0; i < ByteCodeInternals::ByteCode::opName.size(); i++)
    {
        if (i == 0)
        {
			continue;
		}

        cout << "opstr: " << opstr << " ByteCode::opName[i]: " << ByteCodeInternals::ByteCode::opName[i] << endl;

        if (lowercase(opstr) == lowercase(ByteCodeInternals::ByteCode::opName[i]))
        {
            return i;
        }
    }

    return -1;
}

/// @brief This function converts the opcode string to lowercase
/// @param opstring Reference to the opcode string
/// @return Will return the lowercase string
string Parser::lowercase(const string& opstring)
{
    string lower;

    for (auto c : opstring) 
    {
        cout << "c: " << c << endl;
        lower += tolower(c);
        cout << "lower: " << lower.back() << endl;
    }

    while (!lower.empty() && ispunct(lower.back()))
    {
        lower.pop_back();
    }

    return lower;
}

/// @brief This function parses the input file
/// @return Will return the token array
void Parser::parse(array<int, MAX_TOKENS_PER_FILE>& token)
{
    string line;
    int iaddr = 0;

    while (getline(fin, line))
    {
        istringstream iss(line);
        string tok;

        while (iss >> tok)
        {
            if (line.substr(0, 2) == "//")
            {
                continue;
            }

            int opcode = find(tok);

            if (opcode != -1)
            {
                token[iaddr] = opcode;
                iaddr++;

                if (ByteCodeInternals::ByteCode::operands[opcode] > 0)
                {
                    iss >> tok;
                    token[iaddr] = stoi(tok);
                    iaddr++;
                }
            }
        }
    }

    this->iaddr = iaddr;
    this->szToken = iaddr;
}
