#ifndef __MYLEX_H__
#define __MYLEX_H__

    #include <cstdio>
    #include <cstdlib>
    #include <cstring>
    #include <cctype>

    #include "class.h"

    /************
    global constant size
    */
        const int MAX_IDENTIFIER_LENTH = 9;
        const int MAX_INTEGER_VALUE = 65535;


    /************
        dual
    */
        typedef struct _dual{
            int dual_type;
            union {
                char lexeme_text[MAX_IDENTIFIER_LENTH];
                int lexeme_num;
            }lexeme;
            int row, col;
        }DUAL;



    /************
        dual list
    */

        typedef struct _dual_list{
            DUAL * item;
            struct _dual_list * next;
        }DUAL_LIST;



    void loadSourceFile(char *file_name);
    void scanner(FILE *file);
    void initLex();
    void giveupCommentContent(FILE *file);
    char lookAhead(FILE * file);
    void lookup(const char * token);
    void recognizeDigit(FILE * file, char * ch);
    void recognizeString(FILE * file, char * ch);
    void skipWhiteSpace(FILE * file, char * ch);
    void recognizeOperator(FILE * file, char ch);
    void setErrorDual();
    void addToDualList(DUAL * item);
    void addToErrorList(DUAL * item);
    DUAL *  setOperatorForREAL_DUAL(const char * token, const int type );
    void printList(DUAL_LIST * list1);
    void addToDualList(DUAL * item);
    void deleteDualList();
    void deleteErrorList();
    bool getHadError();
    DUAL_LIST * getDualList();
    DUAL_LIST * getErrorList();



#endif // __MYLEX_H__
