import java.io.*;
import java.util.*;

class ArdroidScriptCompiler {

    public class ScriptException extends Exception {
	public ScriptException(String message) {
	    super(message);
	}
    }
    
    public static void main(String[] args) {
	BufferedReader in =
	    new BufferedReader(new InputStreamReader(System.in));
	List<String> script = new ArrayList<>();
	String line;
	try {
	    while((line = in.readLine()) != null) {
		script.add(line);
	    }
	} catch (IOException e) {
	    e.printStackTrace();
	}
	
	ArdroidScriptCompiler compiler = new ArdroidScriptCompiler();
	try {
	    String result = compiler.compile(script);
	    System.out.println(result);
	} catch (ScriptException e) {
	    System.err.println(e.getMessage());
	}
    }

    public String compile(List<String> script) throws ScriptException {
	StringBuilder compiled = new StringBuilder();
	for(String line : script) {
	    String compiledLine = compileLine(line).trim();
	    if(!compiledLine.isEmpty()) {
		compiled.append(compiledLine);
		compiled.append(" ");
	    }
	}
	return compiled.toString().trim();
    }

    private String compileLine(String line) throws ScriptException {
	List<String> words = splitWords(line.toLowerCase());
	if(words.size() == 0)
	    return "";
	String firstWord = words.get(0);

	if(firstWord.equals("script")) {

	} else if(firstWord.equals("wait")) {

	} else if(firstWord.equals("drive")) {

	} else if(firstWord.equals("turn")) {

	} else if(firstWord.equals("move")) {

	} else if(firstWord.equals("stop")) {

	} else if(firstWord.equals("do")) {

	} else {
	    throw new ScriptException("Unrecognized command: " + firstWord);
	}
	
	return words.toString();
    }
    
    private List<String> splitWords(String line) {
	String[] words = line.trim().split("[ \t]");
	List<String> wordsList = new ArrayList<>();
	for(String w : words) {
	    if(!w.isEmpty())
		wordsList.add(w);
	}
	return wordsList;
    }
}
