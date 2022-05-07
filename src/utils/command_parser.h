#ifndef COMMAND_PARSER_HPP
#define COMMAND_PARSER_HPP

#include "command.h"
#include "iostream"
#include "istream"
#include "optional.h"
#include "sstream"
#include "vector"


class CommandParser {

    protected:
    CommandParser() = default;
    public:
    virtual ~CommandParser() = default;

    virtual Optional<CommandData> processCommand() noexcept(true) = 0;
};

CommandParser* newCmdParser();

#endif//COMMAND_PARSER_HPP