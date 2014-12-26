#include "parser.h"



Parser::Parser(QString source, QList<VarNode *> *registry)
{
    this->source = source;
    this->lexer = new Lexer(source);
    this->registry = registry;
}


Parser::~Parser() {}


Calculable* Parser::run()
{
    QList<Token> tokens = this->lexer->run();

    if (tokens.length() == 0)
    {
        return NULL;
    }

    int assignationNumber = 0;

    foreach (Token token, tokens)
    {
        if (token.getKind() == T_ASSIGNMENT)
        {
            assignationNumber++;
        }
    }

    if (assignationNumber > 1)
    {
        throw std::runtime_error("Multiple assignation");
        return NULL;
    }
    else
    {
        Node *tree = this->generateTree(tokens);
        return tree->execute();
    }
}


Node* Parser::generateTree(QList<Token> tokens)
{
    if (tokens.length() == 1)
    {
        Token token = tokens[0];
        TokenKind kind = token.getKind();


        if (kind == T_SCALAR || kind == T_STRING)
        {
            ExpressionNode* node = new ExpressionNode(tokens, this->registry);
            return node;
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        int assignementPos = 0;

        for (int i = 0; i < tokens.length(); i++)
        {
            //Search for an assignement
            if (!assignementPos && tokens[i].getKind() == T_ASSIGNMENT)
            {
                assignementPos = i;
            }
            else if (tokens[i].getKind() == T_SUB)
            {
                //Beginning of expression
                if (i == 0 || tokens[i-1].getKind() == T_PARENTHESIS_LEFT)
                {
                    if (tokens[i+1].getKind() == T_SCALAR)
                    {
                        tokens[i+1].setValue("-" + tokens[i+1].getValue());
                        tokens.removeAt(i);
                    }
                    else if (tokens[i+1].getKind() == T_STRING)
                    {
                        if (!isFunction(tokens[i+1])) {
                            //Var
                            VarNode *var = VarNode::getVar(tokens[i+1].getValue(), this->registry);
                            if (!var->getValue() == NULL)
                            {
                                tokens[i+1].setValue("-" + var->getValue()->getValue());
                                tokens[i+1].setKind(T_SCALAR);
                                tokens.removeAt(i);
                            }
                            else
                            {
                                throw std::runtime_error("Var " + tokens[i+1].getValue().toStdString() + " is not defined.");
                            }
                        }
                        else
                        {
                            QList<Token> newTokens;
                            newTokens.append(tokens.mid(0, i));
                            newTokens.append(Token(T_SCALAR, "-1"));
                            newTokens.append(Token(T_MULTIPLY, "*"));
                            newTokens.append(tokens.mid(i+1, tokens.length()));
                            tokens = newTokens;
                        }
                    }
                }
            }
            else if (tokens[i].getKind() == T_STRING && !isFunction(tokens[i]))
            {
                VarNode *var = VarNode::getVar(tokens[i].getValue(), this->registry);
                if (!var->getValue() == NULL)
                {
                    tokens[i+1].setValue("-" + var->getValue()->toString());
                    tokens[i+1].setKind(T_SCALAR);
                }
            }
        }

        if (!assignementPos)
        {
            return new ExpressionNode(tokens, this->registry);
        }
        else
        {
            QList<Token> varName = tokens.mid(0, assignementPos);

            if (varName.length() > 1 || varName[0].getKind() != T_STRING)
            {
                throw std::runtime_error("Invalid syntax for assignment");
                return NULL;
            }

            ExpressionNode* right = (ExpressionNode*) generateTree(tokens.mid(assignementPos+1, tokens.length()-1));
            VarNode *varNode = VarNode::getVar(varName[0].getValue(), this->registry);


            AssignationNode *assignationNode = new AssignationNode(varNode, right);
            return assignationNode;

        }
    }



}


bool Parser::isFunction(Token token)
{
    QString value = token.getValue();

    return (value == "NORME" || value == "DET" || value == "SOLVE" || value == "TEST");
}
