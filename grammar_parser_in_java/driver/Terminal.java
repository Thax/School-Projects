import java.util.HashMap;
import java.util.Map;

public class Terminal extends Symbol {
	private Token token;
	public Terminal(String input) {
		this.token = translate(input);
	}
	public Terminal(Token input) {
		this.token = input;
	}
	private static final Map<Integer,Token> tokenMap = new HashMap<Integer,Token>();
	static {
		for(Token type : Token.values()) {
			tokenMap.put(type.ordinal(), type);
		}
	}
	public Terminal(int input) {
		this.token = tokenMap.get(new Integer(input));
	}
	public boolean isTerminal() {
		return true;
	}
	public Token getToken() {
		return token;
	}
	public static Token translate(String input) {
		if(input.equals("BEGIN"))
			return Token.BEGIN;
		else if(input.equals("END"))
			return Token.END;
		else if(input.equals("PRINT"))
			return Token.PRINT;
		else if(input.equals("LEFTPAR"))
			return Token.LEFTPAR;
		else if(input.equals("RIGHTPAR"))
			return Token.RIGHTPAR;
		else if(input.equals("SEMICOLON"))
			return Token.SEMICOLON;
		else if(input.equals("ID"))
			return Token.ID;
		else if(input.equals("ASSIGN"))
			return Token.ASSIGN;
		else if(input.equals("READ"))
			return Token.READ;
		else if(input.equals("COMMA"))
			return Token.COMMA;
		else if(input.equals("INTNUM"))
			return Token.INTNUM;
		else if(input.equals("PLUS"))
			return Token.PLUS;
		else if(input.equals("MINUS"))
			return Token.MINUS;
		else if(input.equals("MULTIPLY"))
			return Token.MULTIPLY;
		else if(input.equals("MODULO"))
			return Token.MODULO;
                else if(input.equals("$"))
                    return Token.DOLLAR;
		else
			return null;
	}
	public boolean equals(Symbol other) {
		if(false == other.isTerminal())
			return false;
		else
			return ((Terminal)other).getToken().equals(this.getToken());
	}
	public String tokenString() {
		switch(this.getToken()) {
		case BEGIN:
			return "BEGIN";
		case END:
			return "END";
		case PRINT:
			return "PRINT";
		case LEFTPAR:
			return "LEFTPAR";
		case RIGHTPAR:
			return "RIGHTPAR";
		case SEMICOLON:
			return "SEMICOLON";
		case ID:
			return "ID";
		case ASSIGN:
			return "ASSIGN";
		case READ:
			return "READ";
		case COMMA:
			return "COMMA";
		case INTNUM:
			return "INTNUM";
		case PLUS:
			return "PLUS";
		case MINUS:
			return "MINUS";
		case MULTIPLY:
			return "MULTIPLY";
		case MODULO:
			return "MODULO";
                case DOLLAR:
                    return "$";
		default:
			throw new RuntimeException("Coding error");
		}
	}
	public String toString() {
		return tokenString();
	}
}