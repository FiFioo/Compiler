#ifndef __MYPARSER_H__
#define __MYPARSER_H__

    #include <cstdio>
    #include <cstring>
    #include <iostream>
    #include <string>
    #include <map>
    #include <stack>
    #include <vector>
    #include <iterator>
    #include <typeinfo>
    #include <algorithm>
    #include "myLEX.h"
    #include "class.h"
    using namespace std;


    class Production
    {
    public:
        string left;
        vector<string> right;
        int order;
        int dot_pos;

        Production(const string & left, const vector<string> & right, int order, int dot_pos);
        void print();
        bool operator == (const Production& p) const;
    };

    class Closure
    {
    public:
        vector<Production> productions;
        int id;
        bool operator == (const Closure& c) const;
    };


/********************************************
    function
*/
    void parser(DUAL_LIST * source);
    void initSymbol();
    void initParser();
    void loadProduction();
    void makeItems();
    Closure * makeClosure(Closure * c);
    Closure * shiftIn(const vector<Production> & productions, const string shift_in_symbol);
    bool hadSameClosure(const Closure c);
    bool isInVN(string source);
    void makeFirst();
    void makeFollow();
    void makeAnalysisTable(int row_id, int col_id, int state_id);
    void makeAnalysisTableWhenHadSameClosure(int row_id, int col_id, const Closure c);
    void analysis(DUAL_LIST * source);
    void reduction(int production_id);
    int getInputSymbolId(int input);
    void setFinalSymbolToInput(DUAL_LIST * source);
    void addToParserErrorList(DUAL_LIST * new_error_dual);
    void printFirst();
    void printFollow();
    void printClosure();
    void printAnalysisTable();
    void printAllProduction();
    DUAL_LIST * getParserErrorList();
    bool hadParserError();
    bool isParserAccept();
    void deleteParserErrorList();
    void freeParserResource();

/********************************************
*/

    class VN
    {
    private:
        VN(){};
    public:
        static const map<string, int> vn;
        static void printVN();
        static int getVNId(string vn_name);

        static const string STATEMENT_TABLE;
        static const string STATEMENT;
        static const string ASSIGN_STATEMENT;
        static const string CONDITION_STATEMENT;
        static const string WHILE_STATEMENT;
        static const string COMPOSITE_STATEMENT;
        static const string VARIABLE;
        static const string ARITHMETIC_EXPRESSION;
        static const string RELATION_EXPRESSION;
        static const string ITEM;
        static const string FACTOR;
        static const string CONSTANT;
        static const string RELATION_OPERATOR;
    };

    class VT
    {
    private:
        VT(){};
    public:
        static const map<string, int> vt;
        static void printVT();
        static string getVTName(int vt_id);
        static int getVTId(string vt_name);

        static const string IF;
        static const string THEN;
        static const string ELSE;
        static const string WHILE;
        static const string DO;
        static const string BEGIN;
        static const string END;
        static const string SEMI;   //;
        static const string ASSIGN; //:=
        static const string PLUS;   //+
        static const string SUB;    //-
        static const string MUL;    //*
        static const string DIV;    // /
        static const string LP;     //(
        static const string RP;     //)
        static const string LT;     //<
        static const string LE;     //<=
        static const string EQUAL;  //==
        static const string GT;     //>
        static const string GE;     //>=
        static const string NEQUAL; //<>
        static const string IDENTIFY;
        static const string INTEGER;
        static const string SHARP;
    };

#endif // __MYPARSER_H__
