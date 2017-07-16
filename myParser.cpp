#include "myParser.h"


static vector<Production> production;
static vector<Closure> closure;
static map<string, vector<int> > follow;
static map<string, vector<int> > first;
static map<string, int> symbol;
static vector <vector<int> > analysis_table;
static const string START = "S";
static const int ACC = -100;
static const int MAX_ANALYSIS_TABLE_COL = 37;
static stack<int> analysis_stack;
static DUAL_LIST * parser_error_list;
static bool had_parser_error = false;
static bool parser_accept = false;


void parser(DUAL_LIST * source)
{
    initParser();
    makeFirst();
    makeFollow();
    makeItems();
    analysis(source);
}


void initParser()
{
    loadProduction();
    initSymbol();
}

void initSymbol()
{
    for (auto vt : VT::vt){
        symbol[vt.first] = vt.second;
    }
    for (auto vn : VN::vn){
        symbol[vn.first] = vn.second;
    }
}


void loadProduction()
{
    vector<string> temp;
    string *temp_string;
    Production *p;
    int i = 0;

    temp_string = new string(START);
    temp.clear();
    temp.push_back(VN::STATEMENT_TABLE);
    temp.push_back(VT::SHARP);
    p = new Production(*temp_string, temp, i, 0);
    production.push_back(*p);

    /*****
        语句表--语句
    */
    temp_string = new string(VN::STATEMENT_TABLE);
    temp.clear();
    temp.push_back(VN::STATEMENT);
    temp.push_back(VT::SHARP);
    p = new Production(*temp_string, temp, ++i, 0);
    production.push_back(*p);


    /*****
        语句表--语句
    */
    temp_string = new string(VN::STATEMENT_TABLE);
    temp.clear();
    temp.push_back(VN::STATEMENT);
    temp.push_back(VT::SEMI);
    temp.push_back(VN::STATEMENT_TABLE);
    temp.push_back(VT::SHARP);
    p = new Production(*temp_string, temp, ++i, 0);
    production.push_back(*p);


    /*****
        语句--赋值语句
    */
    temp_string = new string(VN::STATEMENT);
    temp.clear();
    temp.push_back(VN::ASSIGN_STATEMENT);
    temp.push_back(VT::SHARP);
    p = new Production(*temp_string, temp, ++i, 0);
    production.push_back(*p);


    /*****
        语句--条件语句
    */
    temp_string = new string(VN::STATEMENT);
    temp.clear();
    temp.push_back(VN::CONDITION_STATEMENT);
    temp.push_back(VT::SHARP);
    p = new Production(*temp_string, temp, ++i, 0);
    production.push_back(*p);


    /*****
        语句--while语句
    */
    temp_string = new string(VN::STATEMENT);
    temp.clear();
    temp.push_back(VN::WHILE_STATEMENT);
    temp.push_back(VT::SHARP);
    p = new Production(*temp_string, temp, ++i, 0);
    production.push_back(*p);


    /*****
        语句--复合语句
    */
    temp_string = new string(VN::STATEMENT);
    temp.clear();
    temp.push_back(VN::COMPOSITE_STATEMENT);
    temp.push_back(VT::SHARP);
    p = new Production(*temp_string, temp, ++i, 0);
    production.push_back(*p);


    /*****
        赋值语句 -- 变量:=算术表达式
    */
    temp_string = new string(VN::ASSIGN_STATEMENT);
    temp.clear();
    temp.push_back(VN::VARIABLE);
    temp.push_back(VT::ASSIGN);
    temp.push_back(VN::ARITHMETIC_EXPRESSION);
    temp.push_back(VT::SHARP);
    p = new Production(*temp_string, temp, ++i, 0);
    production.push_back(*p);


    /*****
        条件语句 -- if 关系表达式 then 语句 else 语句
    */
    temp_string = new string(VN::CONDITION_STATEMENT);
    temp.clear();
    temp.push_back(VT::IF);
    temp.push_back(VN::RELATION_EXPRESSION);
    temp.push_back(VT::THEN);
    temp.push_back(VN::STATEMENT);
    temp.push_back(VT::ELSE);
    temp.push_back(VN::STATEMENT);
    temp.push_back(VT::SHARP);
    p = new Production(*temp_string, temp, ++i, 0);
    production.push_back(*p);


    /*****
        While语句 -- while 关系表达式 do 语句
    */
    temp_string = new string(VN::WHILE_STATEMENT);
    temp.clear();
    temp.push_back(VT::WHILE);
    temp.push_back(VN::RELATION_EXPRESSION);
    temp.push_back(VT::DO);
    temp.push_back(VN::STATEMENT);
    temp.push_back(VT::SHARP);
    p = new Production(*temp_string, temp, ++i, 0);
    production.push_back(*p);


    /*****
        关系表达式 -- 算术表达式 关系符 算术表达式
    */
    temp_string = new string(VN::RELATION_EXPRESSION);
    temp.clear();
    temp.push_back(VN::ARITHMETIC_EXPRESSION);
    temp.push_back(VN::RELATION_OPERATOR);
    temp.push_back(VN::ARITHMETIC_EXPRESSION);
    temp.push_back(VT::SHARP);
    p = new Production(*temp_string, temp, ++i, 0);
    production.push_back(*p);


    /*****
        复合语句 -- begin 语句表 end
    */
    temp_string = new string(VN::COMPOSITE_STATEMENT);
    temp.clear();
    temp.push_back(VT::BEGIN);
    temp.push_back(VN::STATEMENT_TABLE);
    temp.push_back(VT::END);
    temp.push_back(VT::SHARP);
    p = new Production(*temp_string, temp, ++i, 0);
    production.push_back(*p);


    temp_string = new string(VN::ARITHMETIC_EXPRESSION);
    temp.clear();
    temp.push_back(VN::ITEM);
    temp.push_back(VT::SHARP);
    p = new Production(*temp_string, temp, ++i, 0);
    production.push_back(*p);


    temp_string = new string(VN::ARITHMETIC_EXPRESSION);
    temp.clear();
    temp.push_back(VN::ARITHMETIC_EXPRESSION);
    temp.push_back(VT::PLUS);
    temp.push_back(VN::ITEM);
    temp.push_back(VT::SHARP);
    p = new Production(*temp_string, temp, ++i, 0);
    production.push_back(*p);


    temp_string = new string(VN::ARITHMETIC_EXPRESSION);
    temp.clear();
    temp.push_back(VN::ARITHMETIC_EXPRESSION);
    temp.push_back(VT::SUB);
    temp.push_back(VN::ITEM);
    temp.push_back(VT::SHARP);
    p = new Production(*temp_string, temp, ++i, 0);
    production.push_back(*p);


    temp_string = new string(VN::ITEM);
    temp.clear();
    temp.push_back(VN::FACTOR);
    temp.push_back(VT::SHARP);
    p = new Production(*temp_string, temp, ++i, 0);
    production.push_back(*p);


    temp_string = new string(VN::ITEM);
    temp.clear();
    temp.push_back(VN::ITEM);
    temp.push_back(VT::MUL);
    temp.push_back(VN::FACTOR);
    temp.push_back(VT::SHARP);
    p = new Production(*temp_string, temp, ++i, 0);
    production.push_back(*p);


    temp_string = new string(VN::ITEM);
    temp.clear();
    temp.push_back(VN::ITEM);
    temp.push_back(VT::DIV);
    temp.push_back(VN::FACTOR);
    temp.push_back(VT::SHARP);
    p = new Production(*temp_string, temp, ++i, 0);
    production.push_back(*p);


    temp_string = new string(VN::FACTOR);
    temp.clear();
    temp.push_back(VN::VARIABLE);
    temp.push_back(VT::SHARP);
    p = new Production(*temp_string, temp, ++i, 0);
    production.push_back(*p);


    temp_string = new string(VN::FACTOR);
    temp.clear();
    temp.push_back(VN::CONSTANT);
    temp.push_back(VT::SHARP);
    p = new Production(*temp_string, temp, ++i, 0);
    production.push_back(*p);


    temp_string = new string(VN::FACTOR);
    temp.clear();
    temp.push_back(VT::LP);
    temp.push_back(VN::ARITHMETIC_EXPRESSION);
    temp.push_back(VT::RP);
    temp.push_back(VT::SHARP);
    p = new Production(*temp_string, temp, ++i, 0);
    production.push_back(*p);


    temp_string = new string(VN::VARIABLE);
    temp.clear();
    temp.push_back(VT::IDENTIFY);
    temp.push_back(VT::SHARP);
    p = new Production(*temp_string, temp, ++i, 0);
    production.push_back(*p);


    temp_string = new string(VN::CONSTANT);
    temp.clear();
    temp.push_back(VT::INTEGER);
    temp.push_back(VT::SHARP);
    p = new Production(*temp_string, temp, ++i, 0);
    production.push_back(*p);


    temp_string = new string(VN::RELATION_OPERATOR);
    temp.clear();
    temp.push_back(VT::LT);
    temp.push_back(VT::SHARP);
    p = new Production(*temp_string, temp, ++i, 0);
    production.push_back(*p);


    temp_string = new string(VN::RELATION_OPERATOR);
    temp.clear();
    temp.push_back(VT::LE);
    temp.push_back(VT::SHARP);
    p = new Production(*temp_string, temp, ++i, 0);
    production.push_back(*p);


    temp_string = new string(VN::RELATION_OPERATOR);
    temp.clear();
    temp.push_back(VT::EQUAL);
    temp.push_back(VT::SHARP);
    p = new Production(*temp_string, temp, ++i, 0);
    production.push_back(*p);


    temp_string = new string(VN::RELATION_OPERATOR);
    temp.clear();
    temp.push_back(VT::GT);
    temp.push_back(VT::SHARP);
    p = new Production(*temp_string, temp, ++i, 0);
    production.push_back(*p);


    temp_string = new string(VN::RELATION_OPERATOR);
    temp.clear();
    temp.push_back(VT::GE);
    temp.push_back(VT::SHARP);
    p = new Production(*temp_string, temp, ++i, 0);
    production.push_back(*p);


    temp_string = new string(VN::RELATION_OPERATOR);
    temp.clear();
    temp.push_back(VT::NEQUAL);
    temp.push_back(VT::SHARP);
    p = new Production(*temp_string, temp, ++i, 0);
    production.push_back(*p);
}



Production::Production(const string & left, const vector<string> & right, int order, int dot_pos)
{
    this->left = left;
    this->right = right;
    this->order = order;
    this->dot_pos = dot_pos;
}

void Production::print()
{
    printf("%-2d %s ->\n\t", this->order, this->left.c_str());
    int i;
    for (i = 0; i < (int)this->right.size(); ++i){
        if (this->dot_pos == i){
            putchar('?');
        }
        printf(" %s ", this->right[i].c_str());
    }
    if (this->dot_pos == i){
        putchar('?');
    }
    putchar('\n');
}

bool Production::operator==(const Production& p) const
{
    if (this->left == p.left
        && this->right == p.right
        && this->order == p.order
        && this->dot_pos == p.dot_pos){
            return true;
    }
    return false;
}


bool Closure::operator==(const Closure& c) const
{
    if (this->productions.size() != c.productions.size()){
        return false;;
    }
    for (int i = 0; i < (int)this->productions.size(); ++i){
        if (this->productions[i] == c.productions[i]){
            continue;
        } else {
            return false;
        }
    }
    return true;
}


void makeItems()
{
    int id = 0;
    if (closure.empty()){
        Closure * new_closure = new Closure();
        new_closure->productions.push_back(production[0]);
        new_closure->id = id++;
        closure.push_back(*makeClosure(new_closure));
    }

    for (int i = 0; i < (int)closure.size(); ++i){
        Closure c = closure[i];
        for (auto p : c.productions){
            if (p.dot_pos > (int)p.right.size() - 1){
                continue;
            } else if (p.dot_pos == (int)p.right.size() - 1){
                for (auto s : follow.find(p.left)->second){
                    if (p.order == 0){
                        p.order = -ACC;
                    }
                    makeAnalysisTable(c.id, s, -p.order);
                }
                continue;
            }
            Closure * new_closure = shiftIn(c.productions, p.right[p.dot_pos]);

            if (hadSameClosure(*new_closure)){
                makeAnalysisTableWhenHadSameClosure(c.id, symbol.find(p.right[p.dot_pos])->second, *new_closure);
                delete new_closure;
                continue;
            }

            if (new_closure){
                new_closure->id = id++;
                makeAnalysisTable(c.id, symbol.find(p.right[p.dot_pos])->second, new_closure->id);
                closure.push_back(*new_closure);
            }
        }
    }
}


Closure * shiftIn(const vector<Production> & productions, const string shift_in_symbol)
{
    Closure * new_closure = new Closure();
    for (auto p : productions){
        if (p.dot_pos >= (int)p.right.size() - 1){
            continue;
        }
        if (p.right[p.dot_pos] == shift_in_symbol){
            Production * new_production = new Production(p.left, p.right, p.order, p.dot_pos+1);
            new_closure->productions.push_back(*new_production);
        }
    }
    if (new_closure->productions.empty()){
        delete new_closure;
        return nullptr;
    }
    new_closure = makeClosure(new_closure);
    return new_closure;
}


Closure * makeClosure(Closure * c)
{
    for (int i = 0; i < (int)(*c).productions.size(); ++i){
        Production temp_production = (*c).productions[i];
        if (temp_production.dot_pos >= (int)temp_production.right.size() - 1){
            continue;
        }
        string next_right = temp_production.right[temp_production.dot_pos];
        if (isInVN(next_right)){
            for (auto p : production){
                if (p.left == next_right){
                    bool is_exist = false;
                    for (auto exist_p : (*c).productions){
                        if (p == exist_p){
                            is_exist = true;
                            break;
                        }
                    }
                    if (!is_exist){
                        (*c).productions.push_back(p);
                    }
                }
            }
        }
    }

    return c;
}

bool hadSameClosure(const Closure c)
{
    for (auto source : closure){
        if (source == c){
            return true;
        }
    }
    return false;
}

void makeAnalysisTable(int row_id, int col_id, int state_id)
{
    while ((int)analysis_table.size() <= row_id){
        vector<int> * new_row = new vector<int>(MAX_ANALYSIS_TABLE_COL);
        analysis_table.push_back(*new_row);
    }
    analysis_table[row_id][col_id] = (state_id);
}

void makeAnalysisTableWhenHadSameClosure(int row_id, int col_id, const Closure c)
{
    int state_id = 0;
    for (auto source : closure){
        if (source == c){
            state_id = source.id;
            break;
        }
    }


    while ((int)analysis_table.size() <= row_id){
        vector<int> * new_row = new vector<int>(37);
        analysis_table.push_back(*new_row);
    }

    analysis_table[row_id][col_id] = (state_id);
}


void analysis(DUAL_LIST *source)
{
    if (source == nullptr){
        return;
    }
    const int start_state = 0;
    const int error_state = 0;
    analysis_stack.push(start_state);


    DUAL_LIST * input_dual = source;
    setFinalSymbolToInput(input_dual);


    while (input_dual != nullptr){
        int input_symbol_id = getInputSymbolId(input_dual->item->dual_type);

        int now_state = analysis_stack.top();
        int next_state = analysis_table[now_state][input_symbol_id];

        if (next_state == error_state){
            had_parser_error = true;
            addToParserErrorList(input_dual);
            input_dual = input_dual->next;
            continue;
        } else if (next_state > 0){
            analysis_stack.push(next_state);
            input_dual = input_dual->next;
        } else if (next_state == -100){
            if (!had_parser_error){
                parser_accept = true;
            }
            return;
        } else {
            int production_id = -next_state;
            reduction(production_id);
            now_state = analysis_stack.top();
            int reduced_col = VN::getVNId(production[production_id].left);
            int reduced_next_state = analysis_table[now_state][reduced_col];
            analysis_stack.push(reduced_next_state);
        }
    }
}

void reduction(int production_id)
{
    int pop_times = production[production_id].right.size() -1;

    printf("归约的产生式是 ： -> ");
    production[production_id].print();

    for (int i = 0; i < pop_times; ++i){
        analysis_stack.pop();
    }
}

int getInputSymbolId(int input)
{
    switch (input){
        case IF:
            return VT::getVTId(VT::IF);
            break;
        case THEN:
            return VT::getVTId(VT::THEN);
            break;
        case ELSE:
            return VT::getVTId(VT::ELSE);
            break;
        case WHILE:
            return VT::getVTId(VT::WHILE);
            break;
        case DO:
            return VT::getVTId(VT::DO);
            break;
        case BEGIN:
            return VT::getVTId(VT::BEGIN);
            break;
        case END:
            return VT::getVTId(VT::END);
            break;
        case SEMI:
            return VT::getVTId(VT::SEMI);
            break;
        case ASSIGN:
            return VT::getVTId(VT::ASSIGN);
            break;
        case PLUS:
            return VT::getVTId(VT::PLUS);
            break;
        case SUB:
            return VT::getVTId(VT::SUB);
            break;
        case MUL:
            return VT::getVTId(VT::MUL);
            break;
        case DIV:
            return VT::getVTId(VT::DIV);
            break;
        case LP:
            return VT::getVTId(VT::LP);
            break;
        case RP:
            return VT::getVTId(VT::RP);
            break;
        case LT:
            return VT::getVTId(VT::LT);
            break;
        case LE:
            return VT::getVTId(VT::LE);
            break;
        case EQUAL:
            return VT::getVTId(VT::EQUAL);
            break;
        case GT:
            return VT::getVTId(VT::GT);
            break;
        case GE:
            return VT::getVTId(VT::GE);
            break;
        case NEQUAL:
            return VT::getVTId(VT::NEQUAL);
            break;
        case ID:
            return VT::getVTId(VT::IDENTIFY);
            break;
        case INTEGER:
            return VT::getVTId(VT::INTEGER);
            break;
        case SHARP:
            return VT::getVTId(VT::SHARP);
        default:
            return -1;
            break;
    }
}


void setFinalSymbolToInput(DUAL_LIST * source)
{
    DUAL_LIST * temp_dual = source;
    while (temp_dual->next){
        temp_dual = temp_dual->next;
    }
    DUAL_LIST * final_dual = (DUAL_LIST *)malloc(sizeof(DUAL_LIST));
    final_dual->next = nullptr;
    final_dual->item = (DUAL *)malloc(sizeof(DUAL));
    final_dual->item->row = temp_dual->item->row + 1;
    final_dual->item->col = 0;
    final_dual->item->dual_type = SHARP;
    strcpy(final_dual->item->lexeme.lexeme_text, "#");
    temp_dual->next = final_dual;
}

void addToParserErrorList(DUAL_LIST * new_error_dual)
{
    if (parser_error_list == nullptr){
        parser_error_list = new DUAL_LIST();
        parser_error_list->item = new_error_dual->item;
        parser_error_list->next = nullptr;
        return;
    } else {
        DUAL_LIST * temp_dual = parser_error_list;
        while (temp_dual->next){
            temp_dual = temp_dual->next;
        }
        temp_dual->next = new DUAL_LIST();
        temp_dual->next->item = new_error_dual->item;
        temp_dual->next->next = nullptr;
    }
}

void makeFirst()
{
    for (auto p : production){
        if (first.find(p.left) == first.end()){
            vector<int> first_of_left;
            first[p.left] = first_of_left;
            for (auto pp : production){
                if (pp.left == p.left){
                    if (isInVN(pp.right[0])){
                        makeFirst();
                        for (auto f : first.find(pp.right[0])->second){
                            first_of_left.push_back(f);
                        }
                    } else {
                        first_of_left.push_back(VT::vt.find(pp.right[0])->second);
                    }
                }
                sort(first_of_left.begin(), first_of_left.end());
                first_of_left.erase(unique(first_of_left.begin(), first_of_left.end()), first_of_left.end());
                first[p.left] = first_of_left;
            }
        }
    }
}

void makeFollow()
{
    vector<string> production_left;
    for (auto p : production){
        production_left.push_back(p.left);
    }
    production_left.erase(unique(production_left.begin(), production_left.end()), production_left.end());

    for (auto p_l : production_left){
        if (follow.find(p_l) != follow.end()){
            return;
        }
        vector<int> follow_of_left;
        follow[p_l] = follow_of_left;
        if (p_l == START){
            follow_of_left.push_back(VT::vt.find(VT::SHARP)->second);
        }
        for (auto p : production){
            int position = 0;
            for (auto r : p.right){
                ++position;
                if (r == p_l){
                    if (position >= (int)p.right.size()){
                        break;
                    } else if (position == (int)p.right.size() -1){
                        makeFollow();
                        auto f_l = follow.find(p.left);
                        for (auto f_l_r : f_l->second){
                            follow_of_left.push_back(f_l_r);
                        }
                    } else if (isInVN(p.right[position])){
                        for (auto f_r : first.find(p.right[position])->second){
                            follow_of_left.push_back(f_r);
                        }
                    } else {
                        follow_of_left.push_back(VT::vt.find(p.right[position])->second);
                    }
                }
            }
        }
        sort(follow_of_left.begin(), follow_of_left.end());
        follow_of_left.erase(unique(follow_of_left.begin(), follow_of_left.end()), follow_of_left.end());
        follow[p_l] = follow_of_left;
    }
}

void printFirst()
{
    vector<string> production_left;
    for (auto p : production){
        production_left.push_back(p.left);
    }
    production_left.erase(unique(production_left.begin(), production_left.end()), production_left.end());
    for (auto p : production_left){
        auto temp = first.find(p);
        if (temp == first.end()){
            continue;
        }
        printf("%s -> \n", temp->first.c_str());
        putchar('\t');
        bool first_time = false;
        for (auto i : temp->second){
            if (!first_time){
                first_time = true;
            } else {
                printf(" | ");
            }
            printf(" %s ",VT::getVTName(i).c_str());
        }
        putchar('\n');
    }
}


void printFollow()
{
    vector<string> production_left;
    for (auto p : production){
        production_left.push_back(p.left);
    }
    production_left.erase(unique(production_left.begin(), production_left.end()), production_left.end());


    for (auto p : production_left){
        auto temp = follow.find(p);
        if (temp == follow.end()){
            continue;
        }
        printf("%s -> \n", temp->first.c_str());
        putchar('\t');
        for (auto i : temp->second){
            printf(" %s ",VT::getVTName(i).c_str());
        }
        putchar('\n');
    }
}


void printClosure()
{
    for (auto c : closure){
        cout << "----------------------Closure id : " << c.id << endl;
        for (auto p : c.productions){
            p.print();
        }
    }
}

void printAnalysisTable()
{
    int row = 0;
    printf("    ");
    for (int i = 0; i < (int)analysis_table[0].size(); ++i){
        printf("%3d ", i);
    }
    putchar('\n');

    for (auto a_col : analysis_table){
        printf("%2d ", row++);
        for (auto a_state : a_col){
            if (a_state == 0){
                printf("    ");
            } else {
                printf("%3d ", a_state);
            }
        }
        putchar('\n');
    }
}

void printAllProduction()
{
    for (auto p : production){
        p.print();
    }
}

DUAL_LIST * getParserErrorList()
{
    return parser_error_list;
}

bool hadParserError()
{
    return had_parser_error;
}

bool isParserAccept()
{
    return parser_accept;
}

void deleteParserErrorList()
{
    DUAL_LIST * temp = parser_error_list;
    while (temp){
        temp = temp->next;
        free(parser_error_list);
        parser_error_list = temp;
    }
}

void freeParserResource()
{
    deleteParserErrorList();
    first.clear();
    follow.clear();
    production.clear();
    closure.clear();
    symbol.clear();
    analysis_table.clear();
    while (!analysis_stack.empty()){
        analysis_stack.pop();
    }
}

bool isInVN(string source)
{
    if (VN::vn.find(source) == VN::vn.end()){
        return false;
    }
    return true;
}


const map<string,int> VN::vn = {
    {"statement table",24},
    {"statement", 25},
    {"assign statement", 26},
    {"condition statement", 27},
    {"while statement", 28},
    {"composite statement", 29},
    {"variable", 30},
    {"arithmetic expression", 31},
    {"relation expression", 32},
    {"item", 33},
    {"factor", 34},
    {"constant", 35},
    {"relation operator", 36}
};

const std::string VN::STATEMENT_TABLE("statement table");
const std::string VN::STATEMENT("statement");
const std::string VN::ASSIGN_STATEMENT("assign statement");
const std::string VN::CONDITION_STATEMENT("condition statement");
const std::string VN::WHILE_STATEMENT("while statement");
const std::string VN::COMPOSITE_STATEMENT("composite statement");
const std::string VN::VARIABLE("variable");
const std::string VN::ARITHMETIC_EXPRESSION("arithmetic expression");
const std::string VN::RELATION_EXPRESSION("relation expression");
const std::string VN::ITEM("item");
const std::string VN::FACTOR("factor");
const std::string VN::CONSTANT("constant");
const std::string VN::RELATION_OPERATOR("relation operator");

void VN::printVN()
{
    for ( auto v : vn){
        cout << v.first << "->" << v.second << endl;
    }
}

int VN::getVNId(string vn_name)
{
    auto it = vn.find(vn_name);
    if (it != vn.end()){
        return it->second;
    }
    return -1;
}


const map<string,int> VT::vt{
    {"IF", 1},
    {"THEN", 2},
    {"ELSE", 3},
    {"WHILE", 4},
    {"DO", 5},
    {"BEGIN", 6},
    {"END", 7},
    {";", 8},
    {":=", 9},
    {"+", 10},
    {"-", 11},
    {"*", 12},
    {"/", 13},
    {"(", 14},
    {")", 15},
    {"<", 16},
    {"<=", 17},
    {"==", 18},
    {">", 19},
    {">=", 20},
    {"<>", 21},
    {"id", 22},
    {"integer", 23},
    {"#", 0}
};

string VT::getVTName(int vt_id)
{
    for (auto v : vt){
        if (v.second == vt_id){
            return v.first;
        }
    }
    string s;
    return s;
}

int VT::getVTId(string vt_name)
{
    auto it = vt.find(vt_name);
    if (it != vt.end()){
        return it->second;
    }
    return -1;
}

const std::string VT::IF("IF");
const std::string VT::THEN("THEN");
const std::string VT::ELSE("ELSE");
const std::string VT::WHILE("WHILE");
const std::string VT::DO("DO");
const std::string VT::BEGIN("BEGIN");
const std::string VT::END("END");
const std::string VT::SEMI(";");   //;
const std::string VT::ASSIGN(":="); //:=
const std::string VT::PLUS("+");   //+
const std::string VT::SUB("-");    //-
const std::string VT::MUL("*");    //*
const std::string VT::DIV("/");    // /
const std::string VT::LP("(");     //(
const std::string VT::RP(")");     //)
const std::string VT::LT("<");     //<
const std::string VT::LE("<=");     //<=
const std::string VT::EQUAL("==");  //==
const std::string VT::GT(">");     //>
const std::string VT::GE(">=");     //>=
const std::string VT::NEQUAL("<>"); //<>
const std::string VT::IDENTIFY("id");
const std::string VT::INTEGER("integer");
const std::string VT::SHARP("#");

void VT::printVT()
{
    for ( auto v : vt){
        cout << v.first << "->" << v.second << endl;
    }
}
