import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Scanner;

public class Rule {
	private boolean isValid;
	private List<Symbol> rule;

	public Rule() {
		isValid = false;
		rule = new ArrayList<Symbol>();
	}
	public Rule(String input) {
		if(input.equals("?")) {
			isValid = false;
			this.rule = new ArrayList<Symbol>();
		}
		else {
			isValid = true;
			this.rule = takeInput(input);
		}
	}
	private Symbol makeSymbol(String input) {
		Token token = Terminal.translate(input);
		if(null != token)
			return new Terminal(token);
		else
			return new NonTerminal(input);
	}
	private List<Symbol> takeInput(String input) {
		List<Symbol> output = new ArrayList<Symbol>();
		if(input.equals("SIGMA"))
			return output;
		String[] split = input.split(":");
		for(int i = split.length-1; i >= 0; i--) {
			output.add(makeSymbol(split[i]));
		}
		return output;
	}
	public void validate(ArrayList<NonTerminal> nonTerminals) {
		for(Symbol i : rule) {
			if(false == i.isTerminal() && false == nonTerminals.contains(i))
				throw new ParseFailedException("Invalid parsing table, contains invalid non-terminal: [" + i.toString() + "]");
		}
	}
	public boolean isValid() {
		return isValid;
	}
	public List<Symbol> getRule() {
		return rule;
	}
	public String toString() {
		StringBuilder out = new StringBuilder();
		if(isValid()) {
			if(rule.isEmpty()) {
				out.append("SIGMA");
			}
			else {
				Iterator iter = rule.iterator();
				while (iter.hasNext()) {
					out.append(iter.next().toString());
					if(iter.hasNext())
						out.append(":");
				}
			}
		}
		else {
			out.append("?");
		}
		return out.toString();
	}
}