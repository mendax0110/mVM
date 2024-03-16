/**
 * @file parser.h
 * @author Adrian Goessl
 * @brief This is the header file for the parser
 * @version 0.1
 * @date 2024-03-16
 * 
 * @copyright MIT 2024
 * 
 */
#ifndef PARSER_H
#define PARSER_H

#include <array>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

/// @brief Namespace for Parser  \namespace ParserInternals
namespace ParserInternals
{
    constexpr int MAX_TOKENS_PER_FILE = 1024;

    /// @brief Class for Parser \class Parser
    class Parser
    {
    private:
        std::array<int, MAX_TOKENS_PER_FILE> token;
        std::string infilename;
        std::ifstream fin;
        std::string lowercase(const std::string& opstring);

        int find(const std::string& opstr);
        void setiaddr(int i){iaddr = i;}
        void setszToken(int s){szToken = s;}

    public:
        Parser(const std::string& ifilename);
        int getiaddr() const {return iaddr;}							
        int getszToken() const {return szToken;}
        void parse(std::array<int, MAX_TOKENS_PER_FILE>& token);
        int iaddr;								
        int szToken;	
    };
}

#endif // PARSER_H