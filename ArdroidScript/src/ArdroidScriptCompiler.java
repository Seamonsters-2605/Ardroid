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
	    System.out.println("Name: " + compiler.getScriptName());
	    System.out.println(result);
	} catch (ScriptException e) {
	    System.err.println(e.getMessage());
	}
    }

    
    private String scriptName = null;


    public String compile(List<String> script) throws ScriptException {
	scriptName = null;
	StringBuilder compiled = new StringBuilder();
	for(String line : script) {
	    try {
		String compiledLine = compileLine(line).trim();
		if(!compiledLine.isEmpty()) {
		    compiled.append(compiledLine);
		    compiled.append(" ");
		}
	    } catch(ScriptException e) {
		throw new ScriptException("Error with line: " + line + "\n"
					  + e.getMessage());
	    }
	}

	if(scriptName == null)
	    throw new ScriptException("Please specify the script name with:"
				      + " script name: __");

	return compiled.toString().trim();
    }

    public String getScriptName() {
	return scriptName;
    }


    private String compileLine(String line) throws ScriptException {
	List<String> words = splitWords(line.toLowerCase());
	if(words.size() == 0)
	    return "";
	String firstWord = words.get(0);

	if(firstWord.equals("script")) {
	    return compileScriptName(words);
	} else if(firstWord.equals("wait")) {
	    return compileWait(words);
	} else if(firstWord.equals("drive")) {
	    return compileDrive(words);
	} else if(firstWord.equals("turn")) {
	    return compileTurn(words);
	} else if(firstWord.equals("move")) {
	    return compileMove(words);
	} else if(firstWord.equals("stop")) {
	    return compileStop(words);
	} else if(firstWord.equals("do")) {
	    return compileDo(words);
	} else {
	    throw new ScriptException("Unrecognized command: " + firstWord);
	}
    }
    
    
    private String compileScriptName(List<String> words) throws ScriptException{
	final ScriptException scriptNameException =
	    new ScriptException("Specify the script name with: script name: __."
				+ " Names must be a single word.");

	if(words.size() != 3)
	    throw scriptNameException;
	if(!words.get(1).equals("name:"))
	    throw scriptNameException;
	scriptName = words.get(2);

	return "";
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
	return "w" + padInt(millis, 5);
    }

    private String compileDrive(List<String> words) throws ScriptException {
	return "";
    }

    private String compileTurn(List<String> words) throws ScriptException {
	return "";
    }

    private String compileMove(List<String> words) throws ScriptException {
	return "";
    }

    private String compileStop(List<String> words) throws ScriptException {
	final ScriptException stopCommandException =
	    new ScriptException("Invalid stop command");
	final ScriptException deviceException =
	    new ScriptException("Invalid device in stop command");
	

	String secondWord;
	switch(words.size()) {
	case 1:
	    return "x";
	case 2:
	    secondWord = words.get(1);
	    if(secondWord.equals("all"))
		return "x";
	    else if(secondWord.equals("driving"))
		return "d0000 s0000"; // drive 0 steer 0
	    else
		throw stopCommandException;
	case 4:
	    if(!words.get(2).equals("motor"))
		throw deviceException;

	    int motorNumber;
	    try {
		motorNumber = Integer.parseInt(words.get(3));
	    } catch (NumberFormatException e) {
		throw deviceException;
	    }

	    secondWord = words.get(1);
	    if(secondWord.equals("dc"))
		motorNumber = getDCNumber(motorNumber);
	    else if(secondWord.equals("stepper"))
		motorNumber = getStepperNumber(motorNumber);
	    else
		throw deviceException;

	    return "v" + padInt(motorNumber, 2) + "0000"; 
	default:
	    throw stopCommandException;
	}
    }

    private String compileDo(List<String> words) throws ScriptException {
	final ScriptException scriptCommandException =
	    new ScriptException("Invalid do command."
				+ " Must be: do script __");
	
	if(words.size() < 3)
	    throw scriptCommandException;
	if(!words.get(1).equals("script"))
	    throw scriptCommandException;
	
	String singleCommand = "r" + words.get(2);
	if(words.size() > 3) {
	    final ScriptException repeatedScriptCommandException =
		new ScriptException("Invalid do command."
				    + " Must be: do script __ __ times");
	    if(words.size() != 5)
		throw repeatedScriptCommandException;
	    if(!words.get(4).equals("times"))
		throw repeatedScriptCommandException;
	    int numTimes;
	    try {
		numTimes = Integer.parseInt(words.get(3));
	    } catch (NumberFormatException e) {
		throw repeatedScriptCommandException;
	    }
	    StringBuilder command = new StringBuilder();
	    
	    for(int i = 0; i < numTimes; i++) {
		command.append(singleCommand);
		command.append(" ");
	    }

	    return command.toString();
	} else {
	    return singleCommand;
	}
    }

    private int getDCNumber(int port) {
	return port;
    }

    private int getStepperNumber(int port) {
	return port + 50;
    }

    private String padInt(int n, int digits) {
	return String.format("%0" + digits + "d", n);
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
