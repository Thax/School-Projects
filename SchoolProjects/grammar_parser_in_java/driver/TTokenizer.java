import java.io.FileWriter;
import java.io.FileReader;
import java.io.File;
import java.io.IOException;
import java.io.Reader;
import java.io.Writer;

public class TTokenizer {
	public static void tokenizeStream(Reader input, Writer out) {
		int type;
		TTokenize scanner = null;
		try {
			scanner = new TTokenize(input);
		}
		catch (Exception e) {
			System.out.println("Unexpected exception:");
			e.printStackTrace();
		}
		try {
			while((type = scanner.yylex()) != -1) {
				//				System.out.println(type);
				out.write((new Terminal(type)) + " ");
				//				System.out.println(new Terminal(type));
				//out.write("" + type);
				//out.write("&");
				//out.write(scanner.yytext());
				//out.write("\n");
			}
		}
		catch (IOException e) {
			System.out.println("Unexpected exception while lexing file");
			System.out.println(e);
		}
		try { scanner.yyclose(); }
		catch (IOException e) {
			System.out.println("Unexpected exception when closing input file");
			System.out.println(e);
		}
	}
	public static void main(String[] args) {
		if(args.length <= 0)
			System.out.println("Usage: java TTokenizer <input-file>");
		String file;
		String name;
		String outfile;
		FileWriter out = null;
		TTokenize scanner = null;
		FileReader in = null;
		for(int i=0; i<args.length; i++) {
			file = args[i];
			name = file.substring(0, file.lastIndexOf("."));
			outfile = name + ".tok";
			
			try {
				in = new java.io.FileReader(args[i]);
				//scanner = new TTokenize( new java.io.FileReader(args[i]) );
			}
			catch (java.io.FileNotFoundException e) {
				System.out.println("File not found : \""+args[i]+"\"");
			}
			catch (java.io.IOException e) {
				System.out.println("IO error scanning file \""+args[i]+"\"");
				System.out.println(e);
			}
			catch (Exception e) {
				System.out.println("Unexpected exception:");
				e.printStackTrace();
			}

			try { out = new FileWriter(outfile); }
			catch (IOException e) {
				System.out.println("Unexpected exception while opening output file");
				System.out.println(e);
				continue;
			}

			tokenizeStream(in, out);

			try { out.close(); }
			catch (IOException e) {
				System.out.println("Unexpected exception when closing output file");
				System.out.println(e);
			}
			try { in.close(); }
			catch (IOException e) {
				System.out.println("Unexpected exception when closing input stream");
				System.out.println(e);
			}
		}
	}
}