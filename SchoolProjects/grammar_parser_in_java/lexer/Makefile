all: run

run: TTest.class TTokenize.class TTokenizer.class
	java -cp "../junit/junit-4.10.jar:./" org.junit.runner.JUnitCore TTest

TTest.class: TTest.java TTokenize.class TTokenizer.class
	javac -cp "./:../junit/junit-4.10.jar" TTest.java

TTokenizer.class: TTokenizer.java
	javac TTokenizer.java

TTokenize.class: TTokenize.java
	javac TTokenize.java

TTokenize.java: ttokenize.flex
	java -cp ../jflex/lib/JFlex.jar JFlex.Main ttokenize.flex

TToken.class: TToken.java
	javac TToken.java

clean:
	rm -f TTokenize.java TTokenize.class TToken.class TTest.class TTokenizer.class
