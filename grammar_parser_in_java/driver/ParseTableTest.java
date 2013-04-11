import org.junit.Test;
import org.junit.Assert;

public class ParseTableTest {
	@Test public void parseOutTest1() {
		ParseTable table = testString("LEFTPAR,RIGHTPAR,READ\nasdf,LEFTPAR:fdsa:RIGHTPAR,?,?\nfdsa,?,SIGMA,READ\njsidjsid,READ,RIGHPAR:LEFTPAR,fdsa:asdf:jsidjsid");
		Assert.assertEquals("Incorrect start symbol", "asdf", table.startSymbol().toString());
	}
	@Test public void parseOutTest2() {
		ParseTable table = testString("LEFTPAR,RIGHTPAR,READ\nasdf,LEFTPAR:fdsa:RIGHTPAR,?,?\nfdsa,?,SIGMA,READ\njsidjsid,SIGMA,RIGHPAR:LEFTPAR,fdsa:asdf:jsidjsid");
		Assert.assertEquals("Incorrect start symbol", "asdf", table.startSymbol().toString());
	}
	@Test public void parseFailTest1() {
		boolean parseFail = false;
		ParseTable table = new ParseTable("LEFTPAR,RIGHTPAR,READ\nasdf,LEFTPAR:fdsa:RIGHTPAR,?,?\nfdsa,?,SIGMA,READ\njsidjsid,SIGMA,RIGHPAR:LEFTPAR,fdsa:asdfs:jsidjsid");
		try {
			table.validate();
		}
		catch (ParseFailedException e) {
			parseFail = true;
		}
		Assert.assertTrue("Accepted invalid parsetable", parseFail);
	}

	private ParseTable testString(String string) {
		ParseTable table = new ParseTable(string);
		Assert.assertEquals("Parse table did not input and output the same", string, table.toString());
		return table;
	}
}