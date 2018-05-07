//
//  Syntax.h
//  Compiler
//

#ifndef Syntax_h
#define Syntax_h

#include <iostream>
#include <iomanip>
#include <string>
#include "lexer.h"
#include "assembly.h"

class Syntax {
public:
    void Rat18S();
    void OptFuncDef();
    void FuncDef();
    void Func();
    void OptParamList();
    void ParamList();
    void Parameter();
    void Qualifier();
    void Body();
    void OptDecList();
    void DecList();
    void Declaration();
    void IDs();
    void StatementList();
    void Statement();
    void Compound();
    void Assign();
    void If();
    void Return();
    void Print();
    void Scan();
    void While();
    void Condition();
    void Relop();
    void Expression();
    void ExpressionPrime();
    void Term();
    void TermPrime();
    void Factor();
    void Primary();
    void Empty();
    void lexAdv();
    
};

int tokenCounter = 0;
vector<TokenType> tokenList;                  
TokenType currentToken;
bool printSwitch = true;
string symboltype;


void Syntax::lexAdv() {
   
    if (tokenCounter < tokenList.size()) {
        
        currentToken = tokenList[tokenCounter];
        if (printSwitch) {
            
            fout << "\nToken: " << left << setw(20) << currentToken.token
            << left << setw(8) << "Lexeme: " << left << setw(20) << currentToken.lexeme << endl;
        }
        tokenCounter++;
    }
}

void Syntax::Rat18S() {
    
    lexAdv();
    
    if (printSwitch) {
        fout << "\t<Rat18S> ::= <Opt Function Definitions>\n\t\t\t\t %% <Opt Declaration List> <Statement List> \n";
    }
    
    OptFuncDef();
    
    if (currentToken.lexeme == "%%") {
        lexAdv();
        OptDecList();
        StatementList();
        fout << "Finished" << endl;
    }
    else {
        fout << "\nSyntax Error, expecting this %% before '" << currentToken.lexeme << "' on line " << currentToken.linenum;
        exit(1);
    }
}


void Syntax::OptFuncDef() {
    
    if (printSwitch) {
        fout << "\t<Opt Function Definition> ::= <Function Definitions> | <Empty>\n";
    }
    if (currentToken.lexeme == "function") {
        FuncDef();
    }
    else if (currentToken.lexeme == "%%") {
        Empty();
    }
    else {
        fout << "\nSyntax Error, expecting 'function' or '%%' before '" << currentToken.lexeme << "' on line " << currentToken.linenum;
        exit(1);
    }
    
}

void Syntax::FuncDef() {
    
    if (printSwitch) {
        fout << "\t<Function Definitions> ::= <Function> | <Function> <Function Definitions>\n";
    }
    while (currentToken.lexeme == "function") {
        Func();
    }
}

void Syntax::Func() {

    if (printSwitch) {
        fout << "\t<Function> ::= function <Identifier> [ <Opt Paramenter List> ] <Opt Declaration List> <fout>\n";
    }
    lexAdv();
    
    if (currentToken.token == "Identifier") {
        lexAdv();
        if (currentToken.lexeme == "[") {
            lexAdv();
            OptParamList();
            if (currentToken.lexeme == "]") {
                lexAdv();
                OptDecList();
                Body();
            }
            else {
                fout << "\nSyntax Error, expecting ']' before '" << currentToken.lexeme << "' on line " << currentToken.linenum;
                exit(1);
            }
        }
        else {
            fout << "\nSyntax Error, expecting '[' before '" << currentToken.lexeme << "' on line " << currentToken.linenum;
            exit(1);
        }
    }
    else {
        fout << "\nSyntax Error, expecting <Identifier> before '" << currentToken.lexeme << "' on line " << currentToken.linenum;
        exit(1);
    }
}

void Syntax::OptParamList() {
 
    if (printSwitch)
        fout << "\t<Opt Parameter List> ::= <Parameter List> | <Empty>\n";
    
    if (currentToken.token == "Identifier") {
        ParamList();
    }
    else if (currentToken.lexeme == "]") {
        Empty();
    }
    else {
        fout << "\nSyntax Error, expecting <Identifier> before '" << currentToken.lexeme << "' on line " << currentToken.linenum;
        exit(1);
    }
}

void Syntax::ParamList() {
  
    if (printSwitch)
        fout << "\t<Parameter List> ::= <Parameter> | <Parameter>, <Parameter List>\n";
    
    
    if (currentToken.token == "Identifier") {
        Parameter();
        if (currentToken.lexeme == ",") {
            lexAdv();
            ParamList();
        }
    }
}

void Syntax::Parameter() {
    
    if (printSwitch)
        fout << "\t<Parameter> ::= <IDs> : <Qualifier>\n";
    
    IDs();
    if (currentToken.lexeme == ":") {
        lexAdv();
        Qualifier();
    }
    else {
        fout << "\nSyntax Error, expecting ':' before '" << currentToken.lexeme << "' on line " << currentToken.linenum;
        exit(1);
    }
}

void Syntax::Qualifier() {
    
    if (printSwitch)
        fout << "\t<Qualifier> ::= int | boolean | real\n";
    
	if (currentToken.lexeme == "int" || currentToken.lexeme == "true"
		|| currentToken.lexeme == "false" || currentToken.lexeme == "real" || currentToken.lexeme == "boolean")
	{
		symboltype = currentToken.lexeme;
		lexAdv();
	}
    else {
        fout << "\nSyntax Error, expecting 'int', 'boolean', or 'real' before '" << currentToken.lexeme << "' on line " << currentToken.linenum;
        exit(1);
    }
}

void Syntax::Body() {
    
    if (printSwitch)
        fout << "\t<fout> ::= { <Statement List> }\n";
    
    if (currentToken.lexeme == "{") {
        lexAdv();
        StatementList();
        if (currentToken.lexeme == "}")
            lexAdv();
        else {
            fout << "\nSyntax Error, expecting '}' before '" << currentToken.lexeme << "' on line " << currentToken.linenum;
            exit(1);
        }
    }
    else {
        fout << "\nSyntax Error, expecting '{' before '" << currentToken.lexeme << "' on line " << currentToken.linenum;
        exit(1);
    }
}

void Syntax::OptDecList() {
    
    if (printSwitch) {
        fout << "\t<Opt Declaration List> ::= <Declaration List> | <Empty>\n";
    }
    
    if (currentToken.lexeme == "{") {
        Empty();
    }
    else if (currentToken.lexeme == "int" || currentToken.lexeme == "boolean" || currentToken.lexeme == "real") {
        DecList();
    }
    else {
        fout << "\nSyntax Error, expecting 'int', 'boolean', or 'real' before '" << currentToken.lexeme << "' on line: " << currentToken.linenum;
        exit(1);
    }
}

void Syntax::DecList() {
    
    if (printSwitch) {
        fout << "\t<Declaration List> ::= <Declaration>; | <Declaration> ; <Declaration List>\n";
    }
    
    Declaration();
    if (currentToken.lexeme == ";") {
        lexAdv();
        if (currentToken.lexeme == "int" || currentToken.lexeme == "boolean" || currentToken.lexeme == "real") {
            DecList();
        }
    }
    else {
        fout << "\nSyntax Error, expecting ';' on line " << currentToken.linenum;
        exit(1);
    }
}

void Syntax::Declaration() {
    
    if (printSwitch)
        fout << "\t<Declaration> ::= <Qualifier> <IDs>\n";
    
    Qualifier();
    IDs();
}

void Syntax::IDs() {
    
    if (printSwitch) {
        fout << "\t<IDs> ::= <Identifier> | <Identifier>, <IDs>\n";
    }
    if (currentToken.token == "Identifier") {
		add_symbol(currentToken.lexeme, memory_address, symboltype);
		if (stdinflag) { 
			add_instr("POPM", get_address(currentToken.lexeme)); 
			stdinflag = false; 
		} //Assembly_INSTR
		else { 
			add_instr("PUSHM", get_address(currentToken.lexeme)); 
		} //Assembly_INSTR
        lexAdv();
        if (currentToken.lexeme == ",") {
            lexAdv();
            IDs();
        }
        else if (currentToken.token == "Identifier") {
            fout << "\nSyntax Error, expecting ',' between multiple identifiers on line " << currentToken.linenum;
            exit(1);
        }
    }
    else {
        fout << "\nSyntax Error, expecting <Identifier> before '" << currentToken.lexeme << "' on line " << currentToken.linenum;
        exit(1);
    }
}

void Syntax::StatementList() {
    
    if (printSwitch) {
        fout << "\t<Statement List> ::= <Statement> | <Statement> <Statement List>\n";
    }
    while (currentToken.lexeme == "if" || currentToken.lexeme == "return" || currentToken.lexeme == "put"
           || currentToken.lexeme == "get" || currentToken.lexeme == "while" || currentToken.token == "Identifier"
           || currentToken.lexeme == "function") {
        Statement();
    }
}

void Syntax::Statement() {
    
    if (printSwitch)
        fout << "\t<Statement> ::= <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>\n";
    
    if (currentToken.lexeme == "{")
        Compound();
    else if (currentToken.token == "Identifier")
        Assign();
    else if (currentToken.lexeme == "if")
        If();
    else if (currentToken.lexeme == "return")
        Return();
    else if (currentToken.lexeme == "put")
        Print();
    else if (currentToken.lexeme == "get")
        Scan();
    else if (currentToken.lexeme =="function")
        Func();
    else if (currentToken.lexeme == "while")
        While();
    else {
        fout << "\nSyntax Error, expecting proper '<Statement>' before '" << currentToken.lexeme << "' on line " << currentToken.linenum;
        exit(1);
    }
}

void Syntax::Compound() {
    
    if (printSwitch)
        fout << "\t<Compound> ::= {<Statement List>}\n";
    
    if (currentToken.lexeme == "{") {
        lexAdv();
        StatementList();
        if (currentToken.lexeme == "}") {
            lexAdv();
        }
    }
}

void Syntax::Assign() {
    
    if (printSwitch)
        fout << "\t<Assign> ::= <Identifier> = <Expression>;\n";
    
    if (currentToken.token == "Identifier") {
		save.lexeme = currentToken.lexeme;
        lexAdv();
        if (currentToken.lexeme == "=") {
            lexAdv();
            Expression();
			add_instr("POPM", get_address(save.lexeme)); //Assembly_INSTR
			if (currentToken.lexeme == ";")
			{
				lexAdv();
			}
            else {
                fout << "\n<Assign> Syntax Error";
                exit(1);
            }
        }
        else {
            fout << "\nSyntax Error, expecting '=' before '" << currentToken.lexeme << "' on line " << currentToken.linenum;
            exit(1);
        }
    }
}

void Syntax::If() {

	if (printSwitch)
		fout << "\t<If> ::= if (<Condition>) <Statement> endif | if (<Condition>) <Statement> else <Statement> endif\n";
	if (currentToken.lexeme == "if") {
		lexAdv();
		if (currentToken.lexeme == "(") {
			lexAdv();
			Condition();
			if (currentToken.lexeme == ")") {
				lexAdv();
				Statement();
				back_patch();
				if (currentToken.lexeme == "endif") {
					lexAdv();
				}
				else if (currentToken.lexeme == "else") {
					lexAdv();
					Statement();
					if (currentToken.lexeme == "endif") {
						lexAdv();
					}
					else {
						fout << "\nSyntax Error, expecting 'endif' on line " << currentToken.linenum;
						exit(1);
					}
				}
				else {
					fout << "\nSyntax Error, expecting 'endif' or 'else' on line " << currentToken.linenum;
					exit(1);
				}
			}
			else {
				fout << "\nSyntax Error, expecting ) after <Condition> on line " << currentToken.linenum;
				exit(1);
			}
		}
		else {
			fout << "\nSyntax Error, expecting ( on line " << currentToken.linenum;
			exit(1);
		}
	}
}

void Syntax::Return() {
    
    if (printSwitch)
        fout << "\t<Return> ::= return; | return <Expression>;\n";
    
    lexAdv();
    if (currentToken.lexeme == ";") {
        lexAdv();
    }
    else
    {
        Expression();
        if (currentToken.lexeme == ";")
            lexAdv();
        else {
            fout << "\nSyntax Error, expecting ';' before '" << currentToken.lexeme << "' on line " << currentToken.linenum;
            exit(1);
        }
    }
}

void Syntax::Print() {
    
    if (printSwitch)
        fout << "\t<Print> ::= print (<Expressions>);\n";
    
    lexAdv();
    if (currentToken.lexeme == "(") {
        lexAdv();
        Expression();
		add_instr("STDOUT", -1); //Assembly_INSTR;
        if (currentToken.lexeme == ")") {
            lexAdv();
            if (currentToken.lexeme == ";")
                lexAdv();
            else {
                fout << "\nSyntax Error, expecting ';' before '" << currentToken.lexeme << "' on line " << currentToken.linenum;
                exit(1);
            }
        }
        else {
            fout << "\nSyntax Error, expecting ')' before '" << currentToken.lexeme << "' on line " << currentToken.linenum;
            exit(1);
        }
    }
    else {
        fout << "\nSyntax Error, expecting '(' before '" << currentToken.lexeme << "' on line " << currentToken.linenum;
        exit(1);
    }
}

void Syntax::Scan() {
    
    if (printSwitch)
        fout << "\t<Scan> ::= get (<IDs>);\n";
    
    lexAdv();
    if (currentToken.lexeme == "(") {
        lexAdv();
		add_instr("STDIN", -1); stdinflag = true; //Assembly_INSTR
        IDs();
        if (currentToken.lexeme == ")") {
            lexAdv();
            if (currentToken.lexeme == ";")
                lexAdv();
            else {
                fout << "\nSyntax Error. Expecting ';' before '" << currentToken.lexeme << "' on line " << currentToken.linenum;
                exit(1);
            }
        }
        else {
            fout << "\nSyntax Error, expecting ')' before '" << currentToken.lexeme << "' on line " << currentToken.linenum;
            exit(1);
        }
    }
    else {
        fout << "\nSyntax Error, expecting '(' before '" << currentToken.lexeme << "' on line " << currentToken.linenum;
        exit(1);
    }
}

void Syntax::While() {
    
    if (printSwitch)
        fout << "\t<While> ::= while (<Condition>) <Statement>\n";
	int addr = InstrTable.size() + 1;
	add_instr("LABEL", -1); /* INSTR */
    lexAdv();
    if (currentToken.lexeme == "(")
    {
        lexAdv();
        Condition();
        if (currentToken.lexeme == ")")
        {
            lexAdv();
            Statement();
			add_instr("JUMP", addr); //Assembly_INSTR
			back_patch(); //Assembly_INSTR
        }
    }
}

void Syntax::Condition() {
    
    if (printSwitch)
        fout << "\t<Condition> ::= <Expression> <Relop> <Expression>\n";
    
    Expression();
    Relop();
    Expression();
    
	if (symboltype == "==") { add_instr("EQU", -1); jumpstack.push(InstrTable.size()); add_instr("JUMPZ", -1); }
	else if (symboltype == "^=") { add_instr("NEQ", -1); jumpstack.push(InstrTable.size()); add_instr("JUMPZ", -1); }
	else if (symboltype == "<") { add_instr("LES", -1); jumpstack.push(InstrTable.size()); add_instr("JUMPZ", -1); }
	else if (symboltype == ">") { add_instr("GRT", -1); jumpstack.push(InstrTable.size()); add_instr("JUMPZ", -1); }
	else if (symboltype == "=>") { add_instr("GEQ", -1); jumpstack.push(InstrTable.size()); add_instr("JUMPZ", -1); }
	else if (symboltype == "=<") { add_instr("LEQ", -1); jumpstack.push(InstrTable.size()); add_instr("JUMPZ", -1); }
}

void Syntax::Relop() {
    
    if (printSwitch)
        fout << "\t<Relop> ::= == | ^= | > | < | => | =<\n";
    
    if (currentToken.lexeme == "==" || currentToken.lexeme == "^=" || currentToken.lexeme == ">"
        || currentToken.lexeme == "<" || currentToken.lexeme == "=>" || currentToken.lexeme == "=<")
    {
		symboltype = currentToken.lexeme;
        lexAdv();
    }
    else
    {
        fout << "\nSyntax error, expecting valid comparison operator before " << currentToken.lexeme << " on line " << currentToken.linenum;
        exit(1);
    }
}

void Syntax::Expression() {
    
    if (printSwitch)
        fout << "\t<Expression> ::= <Term> <Expression Prime>\n";
    
    Term();
    ExpressionPrime();
}

void Syntax::ExpressionPrime() {
   
    if (printSwitch)
        fout << "\t<Expression Prime> ::= + <Term> <Expression Prime> | - <Term> <Expression Prime> | <Empty>\n";
	string op;

    if (currentToken.lexeme == "+" || currentToken.lexeme == "-")
    {
		op = currentToken.lexeme;
        lexAdv();
        Term();
		if (op == "+")
			add_instr("ADD", -1); //Assembly_INSTR
		if (op == "-")
			add_instr("SUB", -1); //Assembly_INSTR
        ExpressionPrime();
    }
    else if (currentToken.token == "Unknown") {
        fout << "\nSyntax error, expecting '+', '-', or nothing before '" << currentToken.lexeme << "' on line " << currentToken.linenum;
        exit(1);
    }
    else {
        Empty();
    }
}

void Syntax::Term() {
    
    if (printSwitch)
        fout << "\t<Term> ::= <Factor> <Term Prime>\n";
    
    Factor();
    TermPrime();
}

void Syntax::TermPrime() {
    
    if (printSwitch)
        fout <<  "\t<Term Prime> ::= * <Factor> <Term Prime> | / <Factor> <Term Prime> | <Empty>\n";
    
    if (currentToken.lexeme == "*" || currentToken.lexeme == "/") {
        lexAdv();
        Factor();
		if(currentToken.lexeme == "*")
			add_instr("MUL", -1); //Assembly_INSTR
		else
			add_instr("DIV", -1); //Assembly_INSTR
        TermPrime();
    }
    else if (currentToken.token == "Unknown") {
        fout << "\nSyntax Error, expecting '*', '/', or 'Empty' before '" << currentToken.lexeme << "' on line " << currentToken.linenum;
        exit(1);
    }
    else {
        Empty();
    }
}

void Syntax::Factor() {
    
    if (printSwitch)
        fout << "\t<Factor> ::= - <Primary> | <Primary>\n";
    
    if (currentToken.lexeme == "-") {
        lexAdv();
        Primary();
    }
    
    else if (currentToken.token != "Unknown") {
        Primary();
    }
    
    else {
        fout << "\nSyntax Error, expecting something different before '" << currentToken.lexeme << "' on line " << currentToken.linenum;
        exit(1);
    }
}

void Syntax::Primary() {
    
    if (printSwitch)
        fout << "\t<Primary> ::= <Identifier> | <Integer> | <Identifier> [<IDs>] | (<Expression>) | <Real> | true | false\n";
    
    if (currentToken.token == "Identifier") {
		add_instr("PUSHM", get_address(currentToken.lexeme));
        lexAdv();
        if (currentToken.lexeme == "[") {
            lexAdv();
            IDs();
            if (currentToken.lexeme == "]") {
                lexAdv();
            }
            else {
                fout << "\nSyntax Error, expecting ']' before '" << currentToken.lexeme << "' on line " << currentToken.linenum;
                exit(1);
            }
        }
        else {
            // Do nothing
        }
        
    }
    
    else if (currentToken.token == "Integer" || currentToken.token == "Real") {
		int myint1 = stoi(currentToken.lexeme);
		add_instr("PUSHI", myint1); //Assembly_INSTR
        lexAdv();
    }
    
    else if (currentToken.lexeme == "(") {
        lexAdv();
        Expression();
        if (currentToken.lexeme == ")") {
            lexAdv();
        }
        else {
            fout << "\nSyntax Error, expecting ')' before '" << currentToken.lexeme << "' on line " << currentToken.linenum;
            exit(1);
        }
    }
    
    else if (currentToken.lexeme == "true" || currentToken.lexeme == "false") {
        if(currentToken.lexeme == "true")
			add_instr("PUSHI", 1); //Assembly_INSTR
		else
		{
			add_instr("PUSHI", 0); //Assembly_INSTR
		}
		lexAdv();
    }
    else {
        fout << "\nSyntax Error, expecting '<Identifer>', '<Qualifier>' or '<Expression>' before '" << currentToken.lexeme << "' on line " << currentToken.linenum;
        exit(1);
    }
}

void Syntax::Empty() {
    if (printSwitch)
        fout << "\t<Empty> ::= epsilon\n";
    
}



#endif /* Syntax_h */
