Run this by changing to the driver directory.
Run make
Then run ./RunScript.bash <input-file> <input-grammar>
This will run TTokenizer.java on the input-file which will convert it to tokens
it will ignore improper (e.g. length 12 IDs) tokens
Then it will run GrammarParser.java on input-grammar
this will convert it into a parsing table
Then it will run Driver.java on the resulting files of the last two actions
This will determine if it is a well formed grammar and file


begin, end, print, and read must be in lowercase!
