#ifndef __CLASS_H__
#define __CLASS_H__

    #include <string>

/*********************************
    class
*/
    /************
            special class
    */
        static const int  UNKNOW = 0;
        static const int  ID = 1;
        static const int  INTEGER = 2;
        static const int  SHARP = 3; //#
    /************
    */

    /************
            operate class
    */
        static const int  PLUS = 101;  //+
        static const int  SUB = 102;   //-
        static const int  MUL = 103;   //*
        static const int  DIV = 104;   // /
        static const int  LT = 105;    //<
        static const int  GT = 106;    //>
        static const int  LE = 107;    //<=
        static const int  GE = 108;    //>=
        static const int  EQUAL = 109; //==
        static const int  NEQUAL = 110;    //<>
        static const int  ASSIGN = 111;    //:=
        static const int  LP = 112;    //(
        static const int  RP = 113;    //)
        static const int  SEMI = 114;  //;
    /************
    */

    /************
            keyword class
    */
        static const int  IF = 301;
        static const int  THEN = 302;
        static const int  ELSE = 303;
        static const int  WHILE = 304;
        static const int  DO = 305;
        static const int  BEGIN = 306;
        static const int  END = 307;

        typedef struct _ktab{
            std::string keyword_name;
            int keyword_num;
        }KTAB;

        static const KTAB Ktab[] = {
            {"IF",   IF},
            {"THEN",   THEN},
            {"ELSE",   ELSE},
            {"WHILE",   WHILE},
            {"DO",   DO},
            {"BEGIN",   BEGIN},
            {"END",   END}
        };

    /************
    */



/*********************************
*/

#endif
