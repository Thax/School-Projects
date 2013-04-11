

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
")" { return setToken(TToken.RIGHTPAR); }
"(" { return setToken(TToken.LEFTPAR); }
";" { return setToken(TToken.SEMICOLON); }
"," { return setToken(TToken.COMMA); }
":=" { return setToken(TToken.ASSIGN); }
"*" { return setToken(TToken.MULT); }
"%" { return setToken(TToken.MOD); }
"+" { return setToken(TToken.PLUS); }
"-" { return setToken(TToken.MINUS); }
"begin" { return setToken(TToken.BEGIN); }
"end" { return setToken(TToken.END); }
"print" { return setToken(TToken.PRINT); }
"read" { return setToken(TToken.READ); }
!(!([_A-Za-z]([_A-Za-z0-9]{0,9}))|(([_A-Za-z0-9]*__[_A-Za-z0-9]*)|([_A-Za-z0-9]*_))|([_A-Za-z][_A-Za-z0-9]{10}[_A-Za-z0-9]*)) { return setToken(TToken.ID); }
([1-9][0-9]*)|[0] { return setToken(TToken.INTNUM); }
[ \n\t\r\f] { }