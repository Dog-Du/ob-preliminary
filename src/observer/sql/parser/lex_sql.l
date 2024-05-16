%top{
/*
这里的代码会被复制到lex_sql.cpp的最开始位置
定义yy_size_t的原因是因为flex生成的代码，会使用yy_size_t与其他类型的数字
做比较，导致编译报警
*/
#define YY_TYPEDEF_YY_SIZE_T
typedef int yy_size_t;

/* 参考生成的lex_sql.cpp代码，这个宏定义会放在每次运行yylex()最开始的地方 */
#define YY_USER_INIT                                         \
  yycolumn = 0;

/* 参考生成的lex_sql.cpp代码，这个宏定义会放在解析一个token之后，也可以在网上找到大量的参考资料 */
/* 我们在这里设置当前解析的token的位置信息，这样在yacc中就可以使用这些信息了 */
#define YY_USER_ACTION                                       \
do {                                                         \
  yylloc->first_line   = yylloc->last_line = yylineno;       \
  yylloc->first_column = yycolumn;                           \
  yylloc->last_column  = yylloc->first_column + yyleng - 1;  \
  yycolumn += yyleng;                                        \
}                                                            \
while (0);

}

%{
#include<string.h>
#include<stdio.h>

/**
 * flex 代码包含三个部分，使用 %% 分隔
 * 第一个部分的代码是C代码，flex会原样复制到目标文件中
 * 第二个部分是规则部分，使用正则表达式定义一系列规则
 * 第三个部分还是C代码，flex 会复制此段代码
 */

#include "sql/parser/parse_defs.h"
#include "common/log/log.h"
#include "yacc_sql.hpp"

#ifndef register
#define register
#endif // register

extern int atoi();
extern double atof();

#define RETURN_TOKEN(token) LOG_DEBUG("%s", #token);return token
%}

/* Prevent the need for linking with -lfl */
%option noyywrap
%option nounput
%option noinput
%option bison-bridge
%option reentrant
/* 不区分大小写 */
%option case-insensitive
%option bison-locations

WHITE_SAPCE      [\ \t\b\f]
DIGIT            [0-9]+

ID               [A-Za-z_]+[A-Za-z0-9_]*
DOT              \.
QUOTE            [\'\"]
%x STR

/* 规则匹配的优先级：*/
/* 1. 匹配的规则长的优先 */
/* 2. 写在最前面的优先 */

/* yylval 就可以认为是 yacc 中 %union 定义的结构体(union 结构) */
%%

{WHITE_SAPCE}                                        // ignore whitespace
\n                                                   ;

[\-]?{DIGIT}+                           yylval->number=atoi(yytext); RETURN_TOKEN(NUMBER);
[\-]?{DIGIT}+{DOT}{DIGIT}+              yylval->floats=(float)(atof(yytext)); RETURN_TOKEN(FLOAT);

";"                                     RETURN_TOKEN(SEMICOLON);
{DOT}                                   RETURN_TOKEN(DOT);
EXIT                                    RETURN_TOKEN(EXIT);
HELP                                    RETURN_TOKEN(HELP);
DESC                                    RETURN_TOKEN(DESC);
CREATE                                  RETURN_TOKEN(CREATE);
DROP                                    RETURN_TOKEN(DROP);
TABLE                                   RETURN_TOKEN(TABLE);
TABLES                                  RETURN_TOKEN(TABLES);
INDEX                                   RETURN_TOKEN(INDEX);
ON                                      RETURN_TOKEN(ON);
SHOW                                    RETURN_TOKEN(SHOW);
SYNC                                    RETURN_TOKEN(SYNC);
SELECT                                  RETURN_TOKEN(SELECT);
CALC                                    RETURN_TOKEN(CALC);
FROM                                    RETURN_TOKEN(FROM);

MAX                                     RETURN_TOKEN(MAX);
SUM                                     RETURN_TOKEN(SUM);
MIN                                     RETURN_TOKEN(MIN);
COUNT                                   RETURN_TOKEN(COUNT);
AVG                                     RETURN_TOKEN(AVG);

IS                                      RETURN_TOKEN(IS_T);
NULL                                    RETURN_TOKEN(NULL_T);
NULLABLE                                RETURN_TOKEN(NULL_T);
NOT                                     RETURN_TOKEN(NOT);

LIKE                                    RETURN_TOKEN(LIKE_T);

JOIN                                    RETURN_TOKEN(JOIN_T);
INNER                                   RETURN_TOKEN(INNER_T);

IN                                      RETURN_TOKEN(IN_T);
EXISTS                                  RETURN_TOKEN(EXIST_T);

UNIQUE                                  RETURN_TOKEN(UNIQUE_T);

ORDER                                   RETURN_TOKEN(ORDER_T);
BY                                      RETURN_TOKEN(BY_T);
ASC                                     RETURN_TOKEN(ASC_T);

TEXT                                    RETURN_TOKEN(TEXT_T);

WHERE                                   RETURN_TOKEN(WHERE);
AND                                     RETURN_TOKEN(AND);
INSERT                                  RETURN_TOKEN(INSERT);
INTO                                    RETURN_TOKEN(INTO);
VALUES                                  RETURN_TOKEN(VALUES);
DELETE                                  RETURN_TOKEN(DELETE);
UPDATE                                  RETURN_TOKEN(UPDATE);
SET                                     RETURN_TOKEN(SET);
BEGIN                                   RETURN_TOKEN(TRX_BEGIN);
COMMIT                                  RETURN_TOKEN(TRX_COMMIT);
ROLLBACK                                RETURN_TOKEN(TRX_ROLLBACK);
DATE                                    RETURN_TOKEN(DATE_T);
INT                                     RETURN_TOKEN(INT_T);
CHAR                                    RETURN_TOKEN(STRING_T);
FLOAT                                   RETURN_TOKEN(FLOAT_T);
LOAD                                    RETURN_TOKEN(LOAD);
DATA                                    RETURN_TOKEN(DATA);
INFILE                                  RETURN_TOKEN(INFILE);
EXPLAIN                                 RETURN_TOKEN(EXPLAIN);
{ID}                                    yylval->string=strdup(yytext); RETURN_TOKEN(ID);
"("                                     RETURN_TOKEN(LBRACE);
")"                                     RETURN_TOKEN(RBRACE);

","                                     RETURN_TOKEN(COMMA);
"="                                     RETURN_TOKEN(EQ);
"<="                                    RETURN_TOKEN(LE);
"<>"                                    RETURN_TOKEN(NE);
"!="                                    RETURN_TOKEN(NE);
"<"                                     RETURN_TOKEN(LT);
">="                                    RETURN_TOKEN(GE);
">"                                     RETURN_TOKEN(GT);

"+" |
"-" |
"*" |
"/"                                     { return yytext[0]; }


 /*{QUOTE}[0-9]{4}\-([0-9]{1}|[0-9]{2})\-([0-9]{1}|[0-9]{2}){QUOTE} yylval->string=strdup(yytext); RETURN_TOKEN(DATE_STR); */
 /* 使用正则表达式过滤DATE。需要在yacc文件中增加 %token <string> DATE_STR.相比参考，这里把DATE的限制给放宽了，只要是 **** - *|** - *|** 的全部可以过滤成DATE。至于日期的合理性判断，则由之后的函数判断。*/
 /* 但是，其实最好的是，在这里写一个完备的正则表达式，可以完全找到所有合法的日期。虽然会出现另一个问题：非法日期会被当成chars，需要进一步判断。*/
 /* 所以两害取其轻，选择放宽 -> 进一步发现问题：因为chars和date有交集，所以chars类型可能会被识别成date，这是不合理的，所以完全取消date_str，全靠之后的转化。*/

\"[^"]*\"                               yylval->string = strdup(yytext); RETURN_TOKEN(SSS);
'[^']*\'                                yylval->string = strdup(yytext); RETURN_TOKEN(SSS);

.                                       LOG_DEBUG("Unknown character [%c]",yytext[0]); return yytext[0];
%%

void scan_string(const char *str, yyscan_t scanner) {
  yy_switch_to_buffer(yy_scan_string(str, scanner), scanner);
}
