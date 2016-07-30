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
	    return compileWait(words);
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

    private String compileWait(List<String> words) throws ScriptException {
	final ScriptException waitCommandException =
	    new ScriptException("Invalid wait command."
				+ " Must be: wait __ seconds");

	if(words.size() != 3)
	    throw waitCommandException;
	if(!words.get(2).equals("seconds"))
	    throw waitCommandException;
	float seconds;
	try {
	    seconds = Float.parseFloat(words.get(1));
	} catch (NumberFormatException e) {
	    throw waitCommandException;
	}
	int millis = (int)(seconds * 1000);
	if(millis > 99999)
	    throw new ScriptException("Wait times must be below 100 seconds");
	return "w" + String.format("%05d", millis);
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
