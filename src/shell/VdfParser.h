#pragma once

#include <string>
#include <map>
#include <vector>

struct VdfNode
{
    std::string value;
    std::map<std::string, VdfNode> children;

    bool IsLeaf() const { return children.empty(); }

    // Convenience accessor — returns empty node if key not found
    const VdfNode& operator[](const std::string& key) const
    {
        static VdfNode empty;
        auto it = children.find(key);
        if (it == children.end()) { return empty; }
        return it->second;
    }
};

class VdfParser
{
public:
    static VdfNode ParseFile(const std::string& path);
    static VdfNode ParseString(const std::string& content);

private:
    enum class TokenType { String, Open, Close };

    struct Token
    {
        TokenType   type;
        std::string value;
    };

    static std::vector<Token> Tokenize(const std::string& content);
    static VdfNode            BuildTree(const std::vector<Token>& tokens, size_t& pos);
};