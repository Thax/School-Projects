import java.io.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Collections;
import java.util.HashMap;
import java.util.Queue;
import java.util.LinkedList;
import java.util.Random;

public class GrammarParser {

    private ArrayList<String> parseRuleArray, variables, terminals, targetSigmas;
    private BufferedReader inGrammar, inFile;
    private HashMap<String, ArrayList> rules, first, follow, firstMap;
    private HashMap<String, HashMap<String, ArrayList<String>>> parseTable;
    private String startVar;

    public static void main(String[] args) {
        FileReader grammarFile = null;
        FileWriter outputFile = null;
        try {
            grammarFile = new FileReader(args[0]);
            outputFile = new FileWriter(args[1]);
        } catch (Exception e) {
            System.out.println("Error: " + e.getMessage());
        }

        GrammarParser parser = new GrammarParser(grammarFile);
        parser.writeParsingTableToFile(outputFile);
    }

    public GrammarParser(FileReader grammarFile) {

        String line = "";
        this.parseRuleArray = new ArrayList<String>();

        try {
            this.inGrammar = new BufferedReader(grammarFile);
            while (line != null) {
                line = this.inGrammar.readLine();
                if (line != null) {
                    this.parseRuleArray.add(line);
                }
            }
        } catch (Exception e) {
            System.out.println("Error: " + e.getMessage());
        }

        this.first = new HashMap<String, ArrayList>();
        this.follow = new HashMap<String, ArrayList>();
        this.targetSigmas = new ArrayList<String>();
        this.firstMap = new HashMap<String, ArrayList>();
        this.populateStartVar();
        this.populateTokens();
        this.populateRules();
        this.removeLeftRecursion();
        this.removeLeftFactoring();
        this.createFirstSets();
        this.createFollowSets();
        this.constructParsingTable();
    }

    private void initializeFirstSets() {
        String var;
        for (int i = 0; i < this.variables.size(); i++) {
            var = this.variables.get(i);
            this.first.put(var, new ArrayList<String>());
            this.firstMap.put(var, new ArrayList<String>());
        }
        for (int i = 0; i < this.terminals.size(); i++) {
            this.first.put(this.terminals.get(i), new ArrayList<String>());
            this.first.get(this.terminals.get(i)).add(this.terminals.get(i));
        }
    }

    private void createFirstSets() {
        this.initializeFirstSets();
        ArrayList<String> mapping;
        for (int j = 0; j < 5; j++) {
        for (int i = 0; i < this.variables.size(); i++) {
            String var = this.variables.get(i);
            ArrayList<String> firstSet = this.createFirstSet(var);
            this.first.get(var).removeAll(firstSet);
            this.first.get(var).addAll(firstSet);
            mapping = this.getMappedFirst(var);
            this.first.get(var).removeAll(mapping);
            this.first.get(var).addAll(mapping);
        }
        }
System.out.println(this.first);
    }

    private ArrayList<String> getMappedFirst(String variable) {
        ArrayList<String> mappingSource = this.firstMap.get(variable);
        ArrayList<String> mappingResult = new ArrayList<String>();
        for (int i = 0; i < mappingSource.size(); i++) {
            if (this.variables.contains(((String)mappingSource.get(i)))) {
                mappingResult = this.getMappedFirst((String)mappingSource.get(i));
            } else {
                mappingResult.remove((String)mappingSource.get(i));
                mappingResult.add((String)mappingSource.get(i));
            }
        }
        return mappingResult;
    }

    private ArrayList createFirstSet(String token) {
        ArrayList<String> returnSet = new ArrayList<String>();
        if (this.terminals.contains(token)) {
            returnSet.add(token);
            return returnSet;
        }            
        ArrayList<String> rightHandRules = this.rules.get(token);
        ArrayList<String> tempFirstList;
        ArrayList<String> testList = new ArrayList<String>();
        while (testList.isEmpty() || !(testList.equals(returnSet))) {
            testList.clear();
            testList.addAll(returnSet);
            for (int i = 0; i < rightHandRules.size(); i++) {
                int k = 0;
                int length = (((String)(rightHandRules.get(i))).split(" ").length);
                boolean cont = true;
                String tokenToProcess = (String)rightHandRules.get(i);
                String[] tokensToProcess = tokenToProcess.split(" ");
                while ((k < length) && (cont == true)) {
                    tokenToProcess = tokensToProcess[k];
                    this.firstMap.get(token).remove(tokenToProcess);
                    this.firstMap.get(token).add(tokenToProcess);
                    tempFirstList = this.createFirstSet(tokenToProcess);
                    returnSet = this.merge(returnSet, tempFirstList);
                    if (!tempFirstList.contains("SIGMA")) {
                        cont = false;
                    }
                    k++;
                }
                if (cont == true) {
                    this.targetSigmas.remove(token);
                    this.targetSigmas.add(token);
                }
            }
        }
        return returnSet;
    }

    private String getFirstToken(String input) {
        return (String)input.split(" ")[0];
    }

    private ArrayList merge(ArrayList<String> targetArray, ArrayList<String> sourceArray) {
    targetArray = (ArrayList<String>)targetArray.clone();
    targetArray.removeAll(sourceArray);
    targetArray.addAll(sourceArray);
    targetArray.remove("SIGMA");
    return targetArray;
    }

    private void createFollowSets() {
        for (int i = 0; i < this.variables.size(); i++) {
            this.follow.put(this.variables.get(i), new ArrayList<String>());
        }
        this.follow.get(this.startVar).add("$");
            HashMap<String, ArrayList> testList = new HashMap<String, ArrayList>();
               while (testList.isEmpty() || !testList.equals(this.follow)) { // While there are no more changes
                 testList.clear();
                testList.putAll(this.follow);       for (int i = 0; i < this.variables.size(); i++) {
            String leftHand = (this.variables.get(i));
            ArrayList<String> rightHandRules = this.rules.get(leftHand);

                for (int j = 0; j < rightHandRules.size(); j++) { // For each production
            ArrayList<String> singleProduction = new ArrayList<String>(Arrays.asList(((String)rightHandRules.get(j)).split(" ")));
            for (int k = 0; k < singleProduction.size(); k++) {
                String currentRule = (String)singleProduction.get(k);
                if (!(this.terminals.contains(currentRule))) {
                    for (int l = k + 1; l < singleProduction.size(); l++) {
                        this.follow.put(currentRule, this.merge(this.follow.get(currentRule), this.first.get(singleProduction.get(l))));
                        if (this.first.get(singleProduction.get(l)).contains("SIGMA")) {
                            this.follow.put(singleProduction.get(l), this.merge(this.follow.get(singleProduction.get(l)), this.follow.get(leftHand)));
                        }
                    }
                }
            }
        }
            }
        }
    }

    private void populateTokens() {
        String removeModulo = this.parseRuleArray.get(0); // First line of the read file will be token names
        removeModulo = removeModulo.replace("%Tokens ", "");
        String[] tokens = removeModulo.split(" "); // Split the line into individual token names
        this.terminals = new ArrayList<String>(Arrays.asList(tokens));
    }

    private void populateStartVar() {
        this.startVar = this.parseRuleArray.get(2).replace("%Start ", "").trim();
    }

    private void populateRules() {
        this.rules = new HashMap<String, ArrayList>();
        this.variables = new ArrayList<String>();
        String currentRule, rightSplitRule;
        String[] splitCurrentRule, splitRightHandRule;
        ArrayList<String> rulesArray = new ArrayList<String>();
        List rightHandRules;
        for (int i = 4; i < this.parseRuleArray.size(); i++) {
            currentRule = this.parseRuleArray.get(i);
            splitCurrentRule = currentRule.split(" : ");
            String leftSplitRule = splitCurrentRule[0];
            if (!(this.rules.containsKey(leftSplitRule))) {
                this.variables.add(leftSplitRule);
                this.rules.put(splitCurrentRule[0].trim(), new ArrayList<String>());
            }
            if (this.stringContainsCharacter(splitCurrentRule[1], "|")) {
                splitRightHandRule = splitCurrentRule[1].split("\\|");
                rightHandRules = new ArrayList<String>();
                Collections.addAll(rightHandRules, splitRightHandRule);
                for (int j = 0; j < rightHandRules.size(); j++) {
                    rightSplitRule = ((String)rightHandRules.get(j)).trim();
                    if (!rightSplitRule.contentEquals("|")) {
                        this.rules.get(splitCurrentRule[0]).add(rightSplitRule);
                    }
                }
            } else {
                this.rules.get(splitCurrentRule[0]).add(splitCurrentRule[1].trim());
            }
        }
    }

    private void removeLeftRecursion() {
        while(this.removeSingleRecursion());
    }

    private boolean removeSingleRecursion() {
        int numVariables = this.variables.size();
        for (String i : this.variables) {    
            ArrayList<String> currentRuleList = this.rules.get(i);
            for (int j = 0; j < currentRuleList.size(); j++) {
                String currentRule = (String)currentRuleList.get(j);
                String startOfRule = currentRule.split(" ")[0];
                if (i.contentEquals(startOfRule)) {
                    String ruleTail = currentRule.replaceAll("^" + i, "").trim();
                    String newVar = i.replace(">", "-prime>");
                    if (!this.variables.contains(newVar)) {
                        this.variables.add(newVar);
                        this.rules.put(newVar, new ArrayList<String>());
                    }
                    this.rules.get(newVar).add(ruleTail + " " + newVar);
                    this.rules.get(newVar).remove("SIGMA");
                    this.rules.get(newVar).add("SIGMA");
                    this.rules.get(i).remove(j);
                    for (int k = 0; k < this.rules.get(i).size(); k++) {
                        this.rules.get(i).set(k, this.rules.get(i).get(k) + " " + newVar);
                    }
                    return true;
                }
            }
        }
        return false;
    }

    private void removeLeftFactoring() {
        String currentLeftHandRule, prefixToFactor, newVariable, newRule;
        ArrayList<String> rightHandRules, addBack;
        int variableArraySize = this.variables.size(), numberOfRules;
        for (int i = 0; i < variableArraySize; i++) {
            currentLeftHandRule = (String)(this.variables.get(i));
            rightHandRules = this.rules.get(currentLeftHandRule);
            numberOfRules = rightHandRules.size();
            addBack = new ArrayList<String>();
            if (numberOfRules != 1) {
                prefixToFactor = needsLeftFactoring(rightHandRules);
                if (prefixToFactor != null) {
                    newVariable = currentLeftHandRule.replace(">", "-prime>");
                    this.variables.add(newVariable);
                    this.rules.put(newVariable, new ArrayList<String>());
                    for (int j = 0; j < rightHandRules.size(); j++) {
                        newRule = ((String)(rightHandRules.get(j))).replaceAll("^" + prefixToFactor, "");
                        newRule = newRule.trim();
                        if (newRule.contentEquals("")) {
                            newRule = "SIGMA";
                        }
                        if (newRule.contentEquals(((String)(rightHandRules.get(j))))) {
                            addBack.add((String)(rightHandRules.get(j)));
                        } else {
                            this.rules.get(newVariable).add(newRule);
                        }
                    }
                    this.rules.get(currentLeftHandRule).clear();
                    for (int j = 0; j < addBack.size(); j++) {
                        this.rules.get(currentLeftHandRule).add((String)(addBack.get(j)));
                    }
                    this.rules.get(currentLeftHandRule).add(prefixToFactor + " " + newVariable);
                }
            }
        }
    }

    private String needsLeftFactoring(ArrayList<String> rules) {
        ArrayList<String> tokensSeen = new ArrayList<String>();
        String[] tokenizedRules;
        for (int i = 0; i < rules.size(); i++) {
            tokenizedRules = rules.get(i).split(" ");
            for (int j = 0; j < tokensSeen.size(); j++) {
                if (tokenizedRules[0].contentEquals((String)tokensSeen.get(j))) {
                    return (String)tokensSeen.get(j);
                }
            }
            tokensSeen.add((String)(rules.get(i)).split(" ")[0]);
        }
        return null;
    }

    private static boolean stringContainsCharacter(String theString, String theChar) {
        String[] theStringArray = theString.split("");
        List characters = new ArrayList<String>();
        Collections.addAll(characters, theStringArray);
        for (int i = 0; i < characters.size(); i++) {
            if (((String)(characters.get(i))).contentEquals(theChar)) {
                return true;
            }
        }
        return false;
    }

    private void printRules() {
        for (int i = 0; i < this.variables.size(); i++) {
            System.out.print(this.variables.get(i) + " --> ");
            System.out.println(this.rules.get(this.variables.get(i)));
        }
    }

    public void constructParsingTable() {
        this.parseTable = new HashMap<String, HashMap<String, ArrayList<String>>>();
        this.parseTable.put("", new HashMap<String, ArrayList<String>>());
        HashMap<String, ArrayList<String>> column;
        for (int i = 0; i < this.terminals.size() + 1; i++) {
            if (i != this.terminals.size()) {
                this.parseTable.put((String)this.terminals.get(i), new HashMap<String, ArrayList<String>>());
                column = this.parseTable.get((String)this.terminals.get(i));
            } else {
                this.parseTable.put("$", new HashMap<String, ArrayList<String>>());
                column = this.parseTable.get("$");
            }
            for (int j = 0; j < this.variables.size(); j++) {
                String variable = (String)this.variables.get(j);
                column.put(variable, new ArrayList<String>());
            }
        }
        for (int j = 0; j < this.variables.size(); j++) {
            String variable = (String)this.variables.get(j);
            ArrayList<String> rules = this.rules.get(variable);
            for (String m : rules) {
                if (this.first.get(getFirstToken(m)).contains("SIGMA")) {
                    ArrayList<String> followSet = this.follow.get(variable);
                    for (String l : followSet) {
            this.parseTable.get(l).get(variable).add(m);
                    }
                }
            }
            for (int i = 0; i < this.rules.get(variable).size(); i++) {
                String production = (String)this.rules.get(variable).get(i);
                String firstOfProduction = getFirstToken(production);
                ArrayList<String> firstTerminals = this.first.get(firstOfProduction);
                for (String k : firstTerminals) {
                    this.parseTable.get(k).get(variable).add(production);
                }
            }
        }
    }

    public void writeParsingTableToFile(FileWriter outputFile) {
        String outString = "";
        for (String i : this.terminals) {
            if (!(i.contentEquals("SIGMA"))) {
                outString = outString  + i + ",";
            }
        }
        outString = outString + "$\n";
        for (String j : this.variables) {
            for (int i = -1; i < this.terminals.size() + 1; i++) {
                if (i == -1) {
                    outString = outString + j + ",";
                } else if (i == this.terminals.size()) {
                    ArrayList<String> production = this.parseTable.get("$").get(j);
                    if (production.isEmpty()) {
                        outString = outString + "?";
                    } else {
                        outString = outString + (String)production.get(0);
                    }
               } else if ((!((String)this.terminals.get(i)).contains("SIGMA"))) {
                    String terminal = (String)this.terminals.get(i);
                    ArrayList<String> production = this.parseTable.get(terminal).get(j);
                    if (production.isEmpty()) {
                        outString = outString + "?,";
                    } else {
                        outString = outString + (String)production.get(0) + ",";
                    }
                }
            }
            outString = outString + "\n";
        }
        outString = outString.replace(" ", ":");
        try {
            BufferedWriter out = new BufferedWriter(outputFile);
            out.write(outString);
            out.close();
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
    }
}
