
#include <iostream>
#include "myLEX.h"
#include "myParser.h"

using namespace std;

int main()
{

    loadSourceFile("PascalSourceFile\\or\\passed3.pas");

    if (getHadError()){
        printf("           lex had Error : \n");
        printList(getErrorList());
        deleteErrorList();
        deleteDualList();
        exit(1);
    }

    //printList(getDualList());

    parser(getDualList());

    //printFirst();
    //printFollow();
    //printClosure();
    //printAnalysisTable();

    deleteDualList();
    if (hadParserError()){
        printf("         parser had Error\n");
        printList(getParserErrorList());
        freeParserResource();
        exit(1);
    }
    if (isParserAccept()){
        freeParserResource();
        cout << "parser succeed!" << endl;
    }

    return 0;
}
