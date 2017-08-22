#include "myLEX.h"
using namespace std;

static int file_row = 1;
static int file_col = 0;

static bool hadError = false;

static DUAL_LIST * my_dual_list;
static DUAL_LIST * error_list;

void loadSourceFile(char *file_name)
{
    FILE *file;
    if ( !(file = fopen(file_name, "r")) ){
        printf("Error : can't open file %s\n", file_name);
        return;
    }

    initLex();
    scanner(file);

    fclose(file);
}


void initLex()
{
    my_dual_list = (DUAL_LIST *)malloc(sizeof(DUAL_LIST));
    if (my_dual_list == nullptr){
        printf("\nError : can't create list \n");
        exit(1);
    }

    error_list = (DUAL_LIST *)malloc(sizeof(DUAL_LIST));
    if (error_list == nullptr){
        printf("\nError : can't create list \n");
        exit(1);
    }

    my_dual_list->item = nullptr;
    my_dual_list->next = nullptr;
    error_list->item = nullptr;
    error_list->next = nullptr;
}


void scanner(FILE *file)
{
    char ch;
    while( (ch = fgetc(file)) != EOF ){
        ++file_col;
        skipWhiteSpace(file, &ch);

        if (ch == '/'){
            char next_char = lookAhead(file);
            if (next_char == '/' || next_char == '*'){
                giveupCommentContent(file);
                continue;
            }
        }

        if ( isdigit(ch) ){
            recognizeDigit(file, &ch);
            continue;
        }

        if ( isalpha(ch) ){
            recognizeString(file, &ch);
            continue;
        }

        recognizeOperator(file, ch);
    }
}


void giveupCommentContent(FILE *file)
{
    char ch;
    char next_char = fgetc(file);
    ++file_col;
            /** single comment **/
            if ( next_char == '/' ){
                do{
                    ch = fgetc(file);
                } while (ch != '\n' && ch != EOF);
                skipWhiteSpace(file, &ch);
            }
            /** multiple comment **/
            else if (next_char == '*'){
                do{
                    skipWhiteSpace(file, &ch);
                    ch = fgetc(file);
                    skipWhiteSpace(file, &ch);
                } while (ch != '*' || (ch =fgetc(file)) != '/');
            }
            else {

            }
}



char lookAhead(FILE * file)
{
    char next_char = fgetc(file);
    ungetc(next_char, file);
    return next_char;
}



void lookup(const char *token)
{
    int keywords_num = sizeof(Ktab) / sizeof(KTAB);

    DUAL * new_dual = (DUAL *)malloc( sizeof(DUAL) );
    strncpy(new_dual->lexeme.lexeme_text, token, strlen(token)+sizeof(char));
    new_dual->row = file_row;
    new_dual->col = file_col;

    for (int i = 0; i < keywords_num; ++i){
        if (strcmp(token, Ktab[i].keyword_name.c_str()) == 0) {
            new_dual->dual_type = 300 + i + 1;
            addToDualList(new_dual);
            return;
        }
    }
    new_dual->dual_type = ID;

    addToDualList(new_dual);
}

void skipWhiteSpace(FILE * file, char * ch)
{
    char next_char = *ch;
    while (next_char == ' ' || next_char == '\n' || next_char == '\t'){
        if (next_char == '\n'){
            ++file_row;
            file_col = 0;
        }
        else if (next_char == '\t'){
            file_col += 3;
        }
        next_char = lookAhead(file);
        if (next_char == ' ' || next_char == '\n' || next_char == '\t'){
            *ch = fgetc(file);
            ++file_col;
        }
    }
}

void recognizeDigit(FILE * file, char * ch)
{
    char token[MAX_IDENTIFIER_LENTH];
    int token_index = 0;

    do {
        token[token_index++] = *ch;
        char next_char = lookAhead(file);

        if ( !isdigit(next_char) ){
            switch (next_char) {
                case ' ':
                case '\t':
                case '\n':
                case ';':
                case '+':
                case '-':
                case '*':
                case '/':
                case '<':
                case '>':
                case '=':
                case '(':
                case ')':
                case EOF:
                {
                    DUAL * new_dual = (DUAL *)malloc( sizeof(DUAL) );
                    new_dual->row = file_row;
                    new_dual->col = file_col;
                    new_dual->dual_type = INTEGER;
                    token[token_index] = '\0';
                    new_dual->lexeme.lexeme_num = atoi(token);

                    addToDualList(new_dual);
                }
                    break;
                default:
                    setErrorDual();
                    break;
            }
            break;
        }
        *ch = fgetc(file);
        ++file_col;
    } while ( isdigit(*ch) );
}


void recognizeString(FILE * file, char * ch)
{
    char token[MAX_IDENTIFIER_LENTH];
    int token_index = 0;
    char next_char = '\0';
    bool isTooLong = false;

    while ( isalnum(*ch) ){
        token[token_index++] = *ch;
        if (token_index >= MAX_IDENTIFIER_LENTH){
            isTooLong = true;
            printf("Error : id length too long \n");
        }
        next_char = lookAhead(file);
        if ( !isalnum(next_char)) {
            break;
        }
        *ch = fgetc(file);
        ++file_col;
    }
    if (isTooLong){
        setErrorDual();
        return;
    }
    token[token_index] = '\0';
    lookup(token);
}


void recognizeOperator(FILE * file, char ch)
{
    char token[MAX_IDENTIFIER_LENTH];
    DUAL * new_dual = nullptr;
    switch (ch){
        case ' ':
        case '\t':
        case '\n':
            break;
        case ';':
            strcpy(token, ";");
            new_dual = setOperatorForREAL_DUAL(token, SEMI);
            break;
        case ':':
            if (lookAhead(file) == '='){
                fgetc(file);
                strcpy(token, ":=");
                new_dual = setOperatorForREAL_DUAL(token, ASSIGN);
            }
            else {
                setErrorDual();
            }
            break;
        case '+':
            strcpy(token, "+");
            new_dual = setOperatorForREAL_DUAL(token, PLUS);
            break;
        case '-':
            strcpy(token, "-");
            new_dual = setOperatorForREAL_DUAL(token, SUB);
            break;
        case '*':
            strcpy(token, "*");
            new_dual = setOperatorForREAL_DUAL(token, MUL);
            break;
        case '/':
            strcpy(token, "/");
            new_dual = setOperatorForREAL_DUAL(token, DIV);
            break;
        case '<':
            if (lookAhead(file) == '='){
                fgetc(file);
                strcpy(token, "<=");
                new_dual = setOperatorForREAL_DUAL(token, LE);
            }
            else if (lookAhead(file) == '>'){
                fgetc(file);
                strcpy(token, "<>");
                new_dual = setOperatorForREAL_DUAL(token, NEQUAL);
            }
            else {
                strcpy(token, "<");
                new_dual = setOperatorForREAL_DUAL(token, LT);
            }
            break;
        case '>':
            if (lookAhead(file) == '='){
                fgetc(file);
                strcpy(token, ">=");
                new_dual = setOperatorForREAL_DUAL(token, GE);
            }
            else {
                strcpy(token, ">");
                new_dual = setOperatorForREAL_DUAL(token, GT);
            }
            break;
        case '=':
            if (lookAhead(file) == '='){
                fgetc(file);
                strcpy(token, "==");
                new_dual = setOperatorForREAL_DUAL(token, EQUAL);
            }
            else {
                setErrorDual();
            }
            break;
        case '(':
            strcpy(token, "(");
            new_dual = setOperatorForREAL_DUAL(token, LP);
            break;
        case ')':
            strcpy(token, ")");
            new_dual = setOperatorForREAL_DUAL(token, RP);
            break;
        default:
            setErrorDual();
            break;
        }
    if (new_dual != nullptr){
        addToDualList(new_dual);
    }
}

DUAL *  setOperatorForREAL_DUAL(const char * token, const int type )
{
    DUAL * new_dual = (DUAL *)malloc( sizeof(DUAL) );
    new_dual->row = file_row;
    new_dual->col = file_col;
    strncpy(new_dual->lexeme.lexeme_text, token, (int)sizeof(token));
    new_dual->dual_type = type;
    return new_dual;
}

void setErrorDual()
{
    hadError = true;
    char error_message[] = "Error";
    DUAL * new_dual = (DUAL *)malloc( sizeof(DUAL) );
    if (new_dual == nullptr){
        printf("error : no buffer \n");
        exit(1);
    }
    new_dual->row = file_row;
    new_dual->col = file_col;
    new_dual->dual_type = UNKNOW;
    strcpy(new_dual->lexeme.lexeme_text, error_message);
    addToErrorList(new_dual);
}

void addToDualList(DUAL * item)
{

    if (item == nullptr){
        printf("item is null \n");
        return;
    }


    if (my_dual_list->item == nullptr){
        my_dual_list->item = item;
        my_dual_list->next = nullptr;
    }
    else {
        DUAL_LIST * temp_node = (DUAL_LIST *)malloc(sizeof(DUAL_LIST));
        if (temp_node == nullptr){
            printf("\nError : can't create node \n");
            exit(1);
        }
        temp_node->item = item;
        temp_node->next = nullptr;

        DUAL_LIST * temp = my_dual_list;
        while (temp->next){
            temp = temp->next;
        }
        temp->next = temp_node;
    }
}

void addToErrorList(DUAL * item)
{
    if (item == nullptr){
        printf("item is null \n");
        return;
    }

    if (error_list->item == nullptr){
        error_list->item = item;
        error_list->next = nullptr;
    }
    else {
        DUAL_LIST * temp_node = (DUAL_LIST *)malloc(sizeof(DUAL_LIST));
        if (temp_node == nullptr){
            printf("\nError : can't create node \n");
            exit(1);
        }
        temp_node->item = item;
        temp_node->next = nullptr;

        DUAL_LIST * temp = error_list;
        while (temp->next){
            temp = temp->next;
        }
        temp->next = temp_node;
    }
}

void printList(DUAL_LIST * list1)
{
    DUAL_LIST * temp = list1;
    if (temp == nullptr) {
        printf("null list \n");
    }
    while (temp && temp->item){
        DUAL * dual = temp->item;
        if (dual->dual_type == INTEGER){
            printf("row : %2d , type : %3d , value : %d\n", dual->row, dual->dual_type, dual->lexeme.lexeme_num);
        } else {
            printf("row : %2d , type : %3d , value : %s\n", dual->row, dual->dual_type, dual->lexeme.lexeme_text);
        }

        temp = temp->next;
    }
}


void deleteDualList()
{
    DUAL_LIST * temp = my_dual_list;
    while (temp){
        temp = temp->next;
        free(my_dual_list);
        my_dual_list = temp;
    }
}

void deleteErrorList()
{
    DUAL_LIST * temp = error_list;
    while (temp){
        temp = temp->next;
        free(error_list);
        error_list = temp;
    }
}

bool getHadError()
{
    return hadError;
}

DUAL_LIST * getDualList()
{
    return my_dual_list;
}

DUAL_LIST * getErrorList()
{
    return error_list;
}

