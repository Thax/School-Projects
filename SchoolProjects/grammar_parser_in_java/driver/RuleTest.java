import org.junit.Test;
import org.junit.Assert;

public class RuleTest {
	@Test public void validInTest() {
		Rule rule = new Rule("?");
		Assert.assertTrue("Rule is not invalid when it should be", false == rule.isValid());
	}
	@Test public void validOutTest() {
		Rule rule = new Rule("?");
		Assert.assertEquals("Invalid rule does not convert to string properly", "?", rule.toString());
	}
	@Test public void emptyInTest() {
		Rule rule = new Rule("SIGMA");
		Assert.assertTrue("Rule did not construct properly from the SIGMA string", rule.getRule().isEmpty());
	}
	@Test public void emptyOutTest() {
		Rule rule = new Rule("SIGMA");
		Assert.assertEquals("Empty rule does not convert to string properly", "SIGMA", rule.toString());
	}
	@Test public void listTest1() {
		testString("d:BEGIN:j");
	}
	@Test public void listTest2() {
		testString("LEFTPAR:dfasdf:RIGHTPAR");
	}
	@Test public void listTest3() {
		testString("j");
	}
	@Test public void listTest4() {
		testString("READ");
	}
	@Test public void listInTest1() {
		Rule rule = new Rule("LEFTPAR");
		Assert.assertEquals("Rule did not get the proper number of elements", 1, rule.getRule().size());
		Assert.assertTrue("Rule did not have a terminal symbol", rule.getRule().get(0).isTerminal());
	}
	@Test public void listInTest2() {
		Rule rule = new Rule("LEFTPAR:sdfsdf:RIGHTPAR:COMMA");
		Assert.assertEquals("Rule did not get the proper number of elements", 4, rule.getRule().size());
		Assert.assertTrue("Rule did not have a terminal symbol", rule.getRule().get(0).isTerminal());
		Assert.assertTrue("Rule did not have a nonterminal symbol", false == rule.getRule().get(1).isTerminal());
		Assert.assertTrue("Rule did not have a terminal symbol", rule.getRule().get(2).isTerminal());
		Assert.assertTrue("Rule did not have a terminal symbol", rule.getRule().get(3).isTerminal());
	}
	@Test public void listInTest3() {
		Rule rule = new Rule("asdfads");
		Assert.assertEquals("Rule did not get the proper number of elements", 1, rule.getRule().size());
		Assert.assertTrue("Rule did not have a nonterminal symbol", false == rule.getRule().get(0).isTerminal());
	}
	private void testString(String string) {
		Rule rule = new Rule(string);
		Assert.assertEquals("Rule did not convert to and from string properly", string, rule.toString());
	}
}