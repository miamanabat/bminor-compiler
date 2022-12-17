%{
#include "token.h"
%}
DIGIT [0-9]
LETTER [a-zA-Z]
MULTI_COMMENT \/\*[^\*]*(\*)+([^\/\*][^\*]*(\*)+)*\/
STRING \"([^\"\0\n\t]|(\\.))*\"
IDENT ({LETTER}|_)+({LETTER}|{DIGIT}|_)* 
%%

[[:space:]] /* skip whitespace */


{MULTI_COMMENT}  /* strings, comments, chars */

\/\/[^\n]*\n    
{STRING}        { return TOKEN_STRING; }

\'\'            { return TOKEN_ERROR_CHAR; }  
\'(((\\)?[^\\\'])|(\\\')|(\\\\))\'        { return TOKEN_CHAR; }

array           { return TOKEN_ARRAY; }  /* keywords */
auto            { return TOKEN_AUTO; }
boolean         { return TOKEN_BOOLEAN; }
char            { return TOKEN_KEYWORD_CHAR; }
else            { return TOKEN_ELSE; }
for             { return TOKEN_FOR; }
function        { return TOKEN_FUNCTION; }
if              { return TOKEN_IF; }
integer         { return TOKEN_INTEGER; }
print           { return TOKEN_PRINT; }
return          { return TOKEN_RETURN; }
string          { return TOKEN_KEYWORD_STRING; }
void            { return TOKEN_VOID; }
while           { return TOKEN_WHILE; }
true            { return TOKEN_TRUE; }
false           { return TOKEN_FALSE; }


&&              { return TOKEN_AND; } /* valid symbols */
\|\|            { return TOKEN_OR; }
\?              { return TOKEN_TERNARY; }
:               { return TOKEN_COLON; }
;               { return TOKEN_SEMICOLON; }
,               { return TOKEN_COMMA; }
\[              { return TOKEN_LBRACKET; }
\]              { return TOKEN_RBRACKET; }
\{              { return TOKEN_LBRACE; }
\}              { return TOKEN_RBRACE; }
\(              { return TOKEN_LPAREN; }
\)              { return TOKEN_RPAREN; }
\=              { return TOKEN_ASSIGNMENT; }
\^              { return TOKEN_EXPONENT; }
\+\+            { return TOKEN_INCREMENT; }
--              { return TOKEN_DECREMENT; }
\+              { return TOKEN_ADD; }
\-              { return TOKEN_HYPHEN; }
\*              { return TOKEN_ASTERISK; }
\/              { return TOKEN_FORWARD_SLASH; }
%               { return TOKEN_PERCENT; }
==              { return TOKEN_EQ; }
\<\=            { return TOKEN_LE; }
\>\=            { return TOKEN_GE; }
\<              { return TOKEN_LT; }
\>              { return TOKEN_GT; }
\!\=            { return TOKEN_NE; }
\!              { return TOKEN_NOT; }
{DIGIT}+        { return TOKEN_NUMBER; }
{IDENT}         { return TOKEN_IDENT; }

.               { return TOKEN_ERROR_INVALID; } /* anything else is invalid */

%%
int yywrap() { return 1; }
