/* genz.l - FINAL WORKING VERSION */
%{
#include "ast.h"
#include "genz.tab.h"
void yyerror(const char *s);
%}

%%
"the tea on"        { return T_DECLARE; }
"is"                { return T_ASSIGN; }
"spill"             { return T_SPILL; }
"~"                 { return T_TILDE; }
"exit"              { return T_EXIT; }
"vibe check"        { return T_VIBECHECK; }
"if"                { return T_IF; }
"then"              { return T_THEN; }
"no cap"            { return T_NOCAP; }
"else"              { return T_ELSE; }
"cap"               { return T_CAP; }
"on repeat"         { return T_ON_REPEAT; }
"while"             { return T_WHILE; }
"finna stop"        { return T_FINNA_STOP; }
"iykyk"[^\n]* ;
"=="                { return T_EQ; }
"!="                { return T_NEQ; }
">="                { return T_GTE; }
"<="                { return T_LTE; }
">"                 { return T_GT; }
"<"                 { return T_LT; }

"+"                 { return '+'; }
"-"                 { return '-'; }
"*"                 { return '*'; }
"/"                 { return '/'; }
"("                 { return '('; }
")"                 { return ')'; }

[a-zA-Z_][a-zA-Z0-9_]* { yylval.sval = strdup(yytext); return T_IDENTIFIER; }
[0-9]+                { yylval.ival = atoi(yytext); return T_NUMBER; }
\"[^\"]*\" {
    yytext[strlen(yytext)-1] = '\0';
    yylval.sval = strdup(yytext+1);
    return T_STRING;
}

[ \t\n]+            { /* ignore whitespace */ }
.                   { fprintf(stderr, "Unknown token: %s\n", yytext); }
%%
int yywrap(void) { return 1; }