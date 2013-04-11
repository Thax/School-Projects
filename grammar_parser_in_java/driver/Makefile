all: TTokenizer.class Driver.class GrammarParser.class

Test: RuleTest ParseTableTest

GrammarParser.class: GrammarParser.java
	javac GrammarParser.java

TTest.class: TTest.java TTokenize.class TTokenizer.class Terminal.class
	javac -cp "./:../junit/junit-4.10.jar" TTest.java

TTokenizer.class: TTokenizer.java TTokenize.class
	javac TTokenizer.java

TTokenize.class: TTokenize.java
	javac TTokenize.java

TTokenize.java: ttokenize.flex
	java -cp ../jflex/lib/JFlex.jar JFlex.Main ttokenize.flex

TToken.class: TToken.java
	javac TToken.java

RuleTest: RuleTest.class
	java -cp "./:../junit/junit-4.10.jar" org.junit.runner.JUnitCore RuleTest

RuleTest.class: RuleTest.java Rule.class
	javac -cp "./:../junit/junit-4.10.jar" RuleTest.java

ParseTableTest: ParseTableTest.class
	java -cp "./:../junit/junit-4.10.jar" org.junit.runner.JUnitCore ParseTableTest

ParseTableTest.class: ParseTableTest.java ParseTable.class ParseFailedException.class
	javac -cp "./:../junit/junit-4.10.jar" ParseTableTest.java

Driver.class: Driver.java ParseTable.class Terminal.class Symbol.class Rule.class ParseFailedException.class
	javac Driver.java

ParseTable.class: ParseTable.java NonTerminal.class Terminal.class Rule.class
	javac ParseTable.java

Rule.class: Rule.java Symbol.class Terminal.class NonTerminal.class
	javac Rule.java

Symbol.class: Symbol.java
	javac Symbol.java

Terminal.class: Terminal.java Symbol.class Token.class
	javac Terminal.java

Token.class: Token.java
	javac Token.java

NonTerminal.class: NonTerminal.java Symbol.class
	javac NonTerminal.java

ParseFailedException.class: ParseFailedException.java
	javac ParseFailedException.java

clean:
	rm *.class

.PHONY: check-syntax

check-syntax:
	javac -Xlint $(CHK_SOURCES)