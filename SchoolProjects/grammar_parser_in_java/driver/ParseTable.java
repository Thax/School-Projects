import java.util.ArrayList;
import java.util.Arrays;
import java.util.EnumMap;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

public class ParseTable {
	Map<String,Map<Token,Rule>> table = new HashMap<String,Map<Token,Rule>>();
	ArrayList<NonTerminal> nonTerminals = new ArrayList<NonTerminal>();
	Terminal[] terminals;

	public ParseTable(String definition) {
//		System.out.println("AAAAAAAAAAAAAAA");
//		System.out.println(definition);
//		System.out.println("VVVVVVVVVVVVVVV");
		String[] byLine = definition.split("\n");
		if(byLine.length < 2)
			throw new RuntimeException("Invalid parse table, needs at least one rule");
		String[] terms = byLine[0].split(",");
		terminals = new Terminal[terms.length];
		for(int i = 0; i < terms.length; i++) {
			terminals[i] = new Terminal(terms[i]);
		}
		for(int i = 1; i < byLine.length; i++) {
			String[] line = byLine[i].split(",");
			if(line.length != terminals.length + 1)
				throw new RuntimeException("Invalid row length for line: " + i + " was : " + line.length + " but should be: " + (terminals.length + 1));
			NonTerminal nonTerminal = new NonTerminal(line[0]);
			nonTerminals.add(nonTerminal);
			for(int j = 1; j < line.length; j++) {
				Terminal terminal = terminals[j-1];
				Rule rule = new Rule(line[j]);
				setRule(nonTerminal, terminal, rule);
			}
		}
	}

	public NonTerminal startSymbol() {
		return nonTerminals.get(0);
	}

	public void validate() {
		for(Map<Token,Rule> i : table.values()) {
			for(Rule j : i.values()) {
				j.validate(nonTerminals);
			}
		}
	}

	public boolean containsRule(NonTerminal nonTerminal, Terminal terminal) {
		if(false == table.containsKey(nonTerminal.getValue()))
			return false;
		return table.get(nonTerminal.getValue()).containsKey(terminal.getToken());
	}

	public Rule getRule(Symbol nonTerminal, Terminal terminal) {
		if(nonTerminal.isTerminal())
			throw new RuntimeException("nonTerminal is a terminal symbol");
		NonTerminal nonTerm = (NonTerminal)nonTerminal;
		return table.get(nonTerm.getValue()).get(terminal.getToken());
	}

	public void setRule(Symbol nonTerminal, Terminal terminal, Rule value) {
//            System.out.println("Blarg");
		if(nonTerminal.isTerminal())
			throw new RuntimeException("nonTerminal is a terminal symbol");
		NonTerminal nonTerm = (NonTerminal)nonTerminal;
		if(false == table.containsKey(nonTerm.getValue()))
			table.put(nonTerm.getValue(), new EnumMap<Token,Rule>(Token.class));
		table.get(nonTerm.getValue()).put(terminal.getToken(), value);
	}

	private String combineArray(Object[] array) {
		StringBuilder out = new StringBuilder();
		Iterator iter = Arrays.asList(array).iterator();
		while(iter.hasNext()) {
			out.append(iter.next().toString());
			if(iter.hasNext())
				out.append(",");
		}
		return out.toString();
	}

	public String toString() {
		StringBuilder out = new StringBuilder();
		out.append(combineArray(terminals));
		for(NonTerminal i : nonTerminals) {
			out.append("\n");
			out.append(i.toString());
			for(Terminal j : terminals) {
				out.append(",");
				out.append(getRule(i,j));
			}
		}
		return out.toString();
	}
}