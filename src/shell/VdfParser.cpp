#include "VdfParser.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>

VdfNode VdfParser::ParseFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "VdfParser: could not open " << path << "\n";
        
        return {};
    }

    std::ostringstream ss;
    ss << file.rdbuf();

    return ParseString(ss.str());
}

VdfNode VdfParser::ParseString(const std::string& content)
{
    try
    {
        auto tokens = Tokenize(content);
        size_t pos  = 0;

        return BuildTree(tokens, pos);
    }
    catch (const std::exception& e)
    {
        std::cerr << "VdfParser: parse error: " << e.what() << "\n";

        return {};
    }
}

std::vector<VdfParser::Token> VdfParser::Tokenize(const std::string& content)
{
    std::vector<Token> tokens;
    size_t i = 0;

    while (i < content.size())
    {
        char c = content[i];

        // Skip whitespace
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
        {
            i++;

            continue;
        }

        // Skip single line comments
        if (c == '/' && i + 1 < content.size() && content[i + 1] == '/')
        {
            while (i < content.size() && content[i] != '\n')
            {
                i++;
            }

            continue;
        }

        // Open brace
        if (c == '{')
        {
            tokens.push_back({ TokenType::Open, "{" });
            i++;

            continue;
        }

        // Close brace
        if (c == '}')
        {
            tokens.push_back({ TokenType::Close, "}" });
            i++;

            continue;
        }

        // Quoted string
        if (c == '"')
        {
            i++; // skip opening quote
            std::string value;

            while (i < content.size() && content[i] != '"')
            {
                // Handle escape sequences
                if (content[i] == '\\' && i + 1 < content.size())
                {
                    i++;
                    switch (content[i])
                    {
                        case 'n':  value += '\n';
                            break;
                        case 't':  value += '\t';
                            break;
                        case '"':  value += '"'; 
                            break;
                        case '\\': value += '\\';
                            break;
                        default:   value += content[i];
                            break;
                    }
                }
                else
                {
                    value += content[i];
                }
                i++;
            }

            i++; // skip closing quote
            tokens.push_back({ TokenType::String, value });

            continue;
        }

        // Unquoted string (some VDF files have these)
        if (std::isalnum(c) || c == '_' || c == '-' || c == '.')
        {
            std::string value;
            while (i < content.size() && (std::isalnum(content[i]) || content[i] == '_' || content[i] == '-' || content[i] == '.'))
            {
                value += content[i++];
            }
            tokens.push_back({ TokenType::String, value });

            continue;
        }

        // Unknown character — skip
        i++;
    }

    return tokens;
}

VdfNode VdfParser::BuildTree(const std::vector<Token>& tokens, size_t& pos)
{
    VdfNode node;

    while (pos < tokens.size())
    {
        const Token& tok = tokens[pos];

        // End of block
        if (tok.type == TokenType::Close)
        {
            pos++;

            return node;
        }

        // Expect a key string
        if (tok.type != TokenType::String)
        {
            pos++;

            continue;
        }

        std::string key = tok.value;
        std::transform(key.begin(), key.end(), key.begin(), ::tolower);
        pos++;

        if (pos >= tokens.size())
        {
            break;
        }

        const Token& next = tokens[pos];

        if (next.type == TokenType::String)
        {
            // Key-value pair
            VdfNode child;
            child.value = next.value;
            node.children[key] = child;
            pos++;
        }
        else if (next.type == TokenType::Open)
        {
            // Key with nested block
            pos++; // skip the '{'
            VdfNode child = BuildTree(tokens, pos);
            node.children[key] = child;
        }
    }

    return node;
}