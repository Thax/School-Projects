public class NonTerminal extends Symbol {
	private String value;
	public NonTerminal(String input) {
		value = input;
	}
	public boolean isTerminal() {
		return false;
	}
	public String getValue() {
		return value;
	}
	public boolean equals(Symbol other) {
		if(other.isTerminal())
			return false;
		else
			return ((NonTerminal)other).getValue().equals(this.getValue());
	}
	public String toString() {
		return value;
	}
}