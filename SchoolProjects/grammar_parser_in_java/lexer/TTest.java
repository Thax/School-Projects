import java.io.StringReader;
import java.io.StringWriter;
import org.junit.Test;
import org.junit.Assert;

public class TTest extends org.hamcrest.CoreMatchers {
	@org.junit.Test public void idTest() {
		String[] reject = {"_a_d_43_i8d",
						   "__9d9_f_id",
						   "9d_df_dff8",
						   "f_98df_l0_",
						   "f_98df__fd",
						   "f_98d__l09"};
		String[] accept = {"asdfasdf",
						   "_a_d_43_i8",
						   "_9_d9_f_id",
						   "d9_df_dff8",
						   "f_98df_l_0",
						   "f_98df_l_d",
						   "fasdfasdff",
						   "a787dfa97s",
						   "a787dfa979",
						   "_sdf"};
		TTokenize scanner = null;
		for(int i = 0; i < accept.length; i++) {
			scanner = new TTokenize(new StringReader(accept[i]));
			try { Assert.assertEquals("Token is an ID", TToken.ID, scanner.yylex()); }
			catch (Exception e) { Assert.fail(e.toString()); }
			org.junit.Assert.assertEquals(accept[i], scanner.token());
		}
		for(int i=0; i < reject.length; i++) {
			scanner = new TTokenize(new StringReader(reject[i]));
			try { scanner.yylex(); }
			catch (RuntimeException e) { continue; }
			catch (Exception e) { Assert.fail(e.toString()); }
			Assert.assertThat("Token equals reject string", scanner.token(), is(not(reject[i])));
		}
	}
	@org.junit.Test public void intnumTest() {
		String[] reject = {"_a_d_43_i8d",
						   "__9d9_f_id",
						   "9d_df_dff8",
						   "f_98df_l0_",
						   "f_98df__fd",
						   "f_98d__l09",
						   "34524f54235",
						   "0334",
						   "01"};
		String[] accept = {"4564562456245",
						   "1",
						   "0",
						   "34000",
						   "100000"};
		TTokenize scanner = null;
		for(int i = 0; i < accept.length; i++) {
			scanner = new TTokenize(new StringReader(accept[i]));
			try { Assert.assertEquals("Token is not an INTNUM", TToken.INTNUM, scanner.yylex()); }
			catch (Exception e) { Assert.fail("Runtime Exception on <[" + accept[i] + "]>"); }
			org.junit.Assert.assertEquals(accept[i], scanner.token());
		}
		for(int i=0; i < reject.length; i++) {
			scanner = new TTokenize(new StringReader(reject[i]));
			try { scanner.yylex(); }
			catch (RuntimeException e) { continue; }
			catch (Exception e) { Assert.fail("Exception on <[" + reject[i] + "]>     " + e.toString()); }
			Assert.assertThat("Token equals reject string", scanner.token(), is(not(reject[i])));
		}
	}
	@org.junit.Test public void fileTest() {
		String input = "\n343\ndsf sdf\n4	 begin asdfas 234234 _sdf3 sdfsdf;\nsdf;\njj342dj;sdfsdf\n(jwkeihss:=3434;)\nsdfsdf(:=sdf;4423:=sdfsdf;sdf+343+sdf-234*)_lk_df9;\nsdfbel*llk3j%4334:=sdfagsdfs435342kdfse\n;\n+\n";
		String expected = "2&343\n1&dsf\n1&sdf\n2&4\n13&begin\n1&asdfas\n2&234234\n1&_sdf3\n1&sdfsdf\n15&;\n1&sdf\n15&;\n1&jj342dj\n15&;\n1&sdfsdf\n7&(\n1&jwkeihss\n19&:=\n2&3434\n15&;\n8&)\n1&sdfsdf\n7&(\n19&:=\n1&sdf\n15&;\n2&4423\n19&:=\n1&sdfsdf\n15&;\n1&sdf\n9&+\n2&343\n9&+\n1&sdf\n10&-\n2&234\n11&*\n8&)\n1&_lk_df9\n15&;\n1&sdfbel\n11&*\n1&llk3j\n12&%\n2&4334\n19&:=\n15&;\n9&+\n";
		StringReader in = new StringReader(input);
		StringWriter out = new StringWriter();
		TTokenizer.tokenizeStream(in, out);
		in.close();
		Assert.assertEquals("Tokenization does not match", expected, out.toString());
	}
}