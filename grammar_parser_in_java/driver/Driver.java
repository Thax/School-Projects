import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;
import java.util.Scanner;
import java.util.Stack;

public class Driver {
	public static void main(String[] args) {
		FileReader tbl = null;
		FileReader in = null;
		Scanner tbls;
		Scanner ins;
		StringBuilder tblb;
		Queue<Terminal> inq;
		
		for(int i = 0; i < args.length; i+=2) {
			if(i+1 >= args.length) {
				System.out.println("Files must come in parsetable / input pairs");
				break;
			}
			try {
				tbl = new java.io.FileReader(args[i]);
				in = new java.io.FileReader(args[i+1]);
			}
			catch (java.io.IOException e) {
				System.out.println("IO error reading file \""+args[i]+"\" or + \""+args[i+1]+"\"");
				System.out.println(e);
			}
			tbls = new Scanner(tbl);
			ins = new Scanner(in);
			tblb = new StringBuilder();
			
			while(tbls.hasNextLine()) {
				tblb.append(tbls.nextLine());
				tblb.append("\n");
			}
			tbls.close();
			inq = new LinkedList<Terminal>();
			while(ins.hasNext()) {
				inq.add(new Terminal(ins.next()));
			}
			ins.close();
			ParseTable table = new ParseTable(tblb.toString());
			Driver driver = new Driver(table, inq);
			driver.process();
		}
	}
	ParseTable table;
	Queue<Terminal> in;
	Stack<Symbol> stack;
	List<Rule> out;
	public Driver(ParseTable tbl, Queue<Terminal> input) {
		this.table = tbl;
		this.in = input;
		stack = new Stack<Symbol>();
                stack.add(new Terminal("$"));
		stack.add(tbl.startSymbol());
                input.add(new Terminal("$"));
		out = new ArrayList<Rule>();
	}
	private String describeState(String top, String head) {
		StringBuilder output = new StringBuilder();
		output.append("Top of stack: ");
		output.append(top);
		output.append(" Next input: ");
		output.append(head);
		output.append("\nStack: ");
		output.append(stack.toString());
		output.append("\nInput Queue: ");
		output.append(in.toString());
		output.append("\nOutput so far: ");
		output.append(out.toString());
                return output.toString();
	}
	public void process() {
		while(false == stack.isEmpty()) {
//                  System.out.println("Starting loop");
//                    System.out.println(stack);
//                    System.out.println(in);
			Symbol top = stack.pop();
			if(in.isEmpty())
				throw new ParseFailedException("End of input while there is still items on the stack\n" + describeState(top.toString(), "[empty]"));
			Terminal head = in.peek();
			if(top.isTerminal()) {
				in.poll();
  //                              System.out.println("Matched: " + head);
				if(false == top.equals(head))
					throw new ParseFailedException("Stack has terminal which does not match input queue.\n" + describeState(top.toString(), head.toString()));
			}
			else {
				Rule rule = table.getRule(top, head);
//                                System.out.println("HEHE");
//                                System.out.println(rule);
				for(Symbol i : rule.getRule())
					stack.add(i);
//                                System.out.println(stack);
//                                System.out.println("HOHO");
                        }
		}
		if(false == in.isEmpty())
			throw new ParseFailedException("End of stack reached while there is still input left\n" + describeState("", ""));
		else
			System.out.println("Successfully parsed file");
	}
}