

/**
		Tiny tokenizer
*/

import java.util.ArrayList;

%%

%public
%class TTokenize
%standalone

%unicode

%{
	ArrayList<TToken> tokens = new ArrayList<TToken>();
	private int token(String string) {
		return 1;
	}
	private int tokVal = -1;
	private int setToken(int val) {
	  tokVal = val;
	  if(token().length() <= 0)
		tokVal = 0;
	  return tokVal;
	}
	public String token() {
	  return yytext().trim();
	}
	public int getVal() {
	  return tokVal;
	}
	
%}

%%

[_A-Za-z][_A-Za-z0-9]{10}[_A-Za-z0-9]* { }
")" { return Token.RIGHTPAR.ordinal(); }
"(" { return Token.LEFTPAR.ordinal(); }
";" { return Token.SEMICOLON.ordinal(); }
"," { return Token.COMMA.ordinal(); }
":=" { return Token.ASSIGN.ordinal(); }
"*" { return Token.MULTIPLY.ordinal(); }
"%" { return Token.MODULO.ordinal(); }
"+" { return Token.PLUS.ordinal(); }
"-" { return Token.MINUS.ordinal(); }
"begin" { return Token.BEGIN.ordinal(); }
"end" { return Token.END.ordinal(); }
"print" { return Token.PRINT.ordinal(); }
"read" { return Token.READ.ordinal(); }
!(!([_A-Za-z]([_A-Za-z0-9]{0,9}))|(([_A-Za-z0-9]*__[_A-Za-z0-9]*)|([_A-Za-z0-9]*_))|([_A-Za-z][_A-Za-z0-9]{10}[_A-Za-z0-9]*)) { return Token.ID.ordinal(); }
([1-9][0-9]*)|[0] { return Token.INTNUM.ordinal(); }
[ \n\t\r\f] { }