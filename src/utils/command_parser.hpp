#ifndef COMMAND_PARSER_HPP
#define COMMAND_PARSER_HPP

#include "command.hpp"
#include "iostream"
#include "istream"
#include "optional.hpp"
#include "sstream"
#include "vector"


class CommandParser {

    public:
    virtual optional<CommandData> processCommand() noexcept(true) = 0;
};

CommandParser* newCmdParser();

#endif//COMMAND_PARSER_HPP