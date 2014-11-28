#ifndef TOKEN_H
#define TOKEN_H

#include <QDebug>
#include <QString>


enum TokenKind
{
    T_NULL,
    T_ASSIGNMENT,
    T_WHITESPACE,
    T_SOLVE,
    T_MULTIPLY,
    T_DIVIDE,
    T_SUM,
    T_SUB,
    T_DOUBLE,
    T_PARENTHESIS_LEFT,
    T_PARENTHESIS_RIGHT,
    T_STRING
};


class Token
{
    public:
        Token(TokenKind kind, QString value="");
        
        TokenKind getKind() const;
        QString getValue() const;

    protected:
        TokenKind kind;
        QString value;
};

#endif // TOKEN_H