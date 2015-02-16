// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// gameError.h v1.0
// Error class thrown by game engine.

#ifndef _GAMEERROR_H            // prevent multiple definitions if this 
#define _GAMEERROR_H            // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <string>
#include <exception>

namespace GameExceptionNS
{
    // Error codes
    // Negative numbers are fatal errors that may require the game to be shutdown.
    // Positive numbers are warnings that do not require the game to be shutdown.
    const int FATAL_ERROR = -1;
    const int WARNING = 1;
}

// Game Error class. Thrown when an error is detected by the game engine.
// Inherits from std::exception
class GameException : public std::exception
{
private:
    int     m_errorCode;
    std::string m_message;
public:
    // default constructor
	GameException() throw() :m_errorCode(GameExceptionNS::FATAL_ERROR), m_message("Undefined Error in game.") {}
    // copy constructor
    GameException(const GameException& e) throw(): std::exception(e), m_errorCode(e.m_errorCode), m_message(e.m_message) {}
    // constructor with args
    GameException(int code, const std::string &s) throw() :m_errorCode(code), m_message(s) {}
    // assignment operator
    GameException& operator= (const GameException& rhs) throw() 
    {
        std::exception::operator=(rhs);
        this->m_errorCode = rhs.m_errorCode;
        this->m_message = rhs.m_message;
    }
    // destructor
    virtual ~GameException() throw() {};

    // override what from base class
    virtual const char* what() const throw() { return this->getMessage(); }

    const char* getMessage() const throw() { return m_message.c_str(); }
    int getErrorCode() const throw() { return m_errorCode; }
};

#endif
