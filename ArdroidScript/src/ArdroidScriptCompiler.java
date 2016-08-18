import java.io.*;
import java.util.*;

class ArdroidScriptCompiler {

    private static final int DEFAULT_SPEED = 191;
    private static final int MAX_SPEED = 255;
    private static final int VERY_SLOW_SPEED = 63;
    private static final int SLOW_SPEED = 127;
    private static final int FAST_SPEED = 191;
    private static final int VERY_FAST_SPEED = 255;

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
	    } catch(Exception e) {
		throw new ScriptException("Unexpected error while compiling!");
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
	if(millis < 0)
	    throw new ScriptException("Wait times must be positive");
	if(millis > 99999)
	    throw new ScriptException("Wait times must be below 100 seconds");
	return "w" + padInt(millis, 5);
    }

    private String compileDrive(List<String> words) throws ScriptException {
	final ScriptException turnCommandException =
	    new ScriptException("Invalid drive command."
				+ " Must be: drive forward/back/left/right");
	int speed = DEFAULT_SPEED;
	int driveSpeed = DEFAULT_SPEED;
	int turnSpeed = 0;
	String suffix = "";

	boolean foundSomething = true;
	while(foundSomething) {
	    foundSomething = false;
	    if(hasTimeModifier(words)) {
		suffix = " " + getTimeModifierCommand(words) + " d0000 s0000";
		foundSomething = true;
	    }
	    if(hasSpeedModifier(words)) {
		speed = driveSpeed = getSpeedModifierValue(words);
		foundSomething = false;
	    }
	}

	List<String> directions = new ArrayList<>();
	if(words.size() < 2) {
	    throw turnCommandException;
	} else {
	    directions.add(words.get(1));
	    if(words.size() == 2) {
		; // do nothing
	    } else if(words.size() == 4) {
		if(!words.get(2).equals("and"))
		    throw turnCommandException;
		directions.add(words.get(3));
	    } else {
		throw turnCommandException;
	    }
	}
	    
	for(String direction : directions) {
	    if(direction.equals("right")) {
		turnSpeed = speed; // do nothing
	    } else if(direction.equals("left")) {
		turnSpeed = -speed;
	    } else if(direction.equals("forward")) {
		driveSpeed = speed;
	    } else if(direction.equals("back")) {
		driveSpeed = -speed;
	    } else {
		throw turnCommandException;
	    }
	}

	return "d" + padInt(driveSpeed, 4) + "s" + padInt(turnSpeed, 4)
	    + suffix;
    }

    private String compileTurn(List<String> words) throws ScriptException {
	final ScriptException turnCommandException =
	    new ScriptException("Invalid turn command."
				+ " Must be: turn left/right");
	int speed = DEFAULT_SPEED;
	String suffix = "";

	boolean foundSomething = true;
	while(foundSomething) {
	    foundSomething = false;
	    if(hasTimeModifier(words)) {
		suffix = " " + getTimeModifierCommand(words) + " s0000";
		foundSomething = true;
	    }
	    if(hasSpeedModifier(words)) {
		speed = getSpeedModifierValue(words);
		foundSomething = false;
	    }
	}

	if(words.size() != 2)
	    throw turnCommandException;
	String direction = words.get(1);
	if(direction.equals("right")) {
	    ; // do nothing
	} else if(direction.equals("left")) {
	    speed = -speed;
	} else {
	    throw turnCommandException;
	}

	return "s" + padInt(speed, 4) + suffix;
    }

    private String compileMove(List<String> words) throws ScriptException {
	return "";
    }

    private String compileStop(List<String> words) throws ScriptException {
	final ScriptException stopCommandException =
	    new ScriptException("Invalid stop command");
	final ScriptException deviceException =
	    new ScriptException("Invalid device in stop command");

	String suffix = "";
	if(hasTimeModifier(words)) {
	    suffix = " " + getTimeModifierCommand(words);
	}
	

	String secondWord;
	switch(words.size()) {
	case 1:
	    return "x" + suffix;
	case 2:
	    secondWord = words.get(1);
	    if(secondWord.equals("all"))
		return "x" + suffix;
	    else if(secondWord.equals("driving"))
		return "d0000 s0000" + suffix; // drive 0 steer 0
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

	    return "v" + padInt(motorNumber, 2) + "0000" + suffix; 
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

    private boolean hasTimeModifier(List<String> words) throws ScriptException {
	final ScriptException timeModifierException =
	    new ScriptException("Invalid time modifier."
				+ " Must be: for __ seconds");
	int size = words.size();
	if(size < 3)
	    return false;
	if(words.get(size - 3).equals("for")) {
	    if(words.get(size - 1).equals("seconds"))
		return true;
	    else
		throw timeModifierException;
	} else {
	    return false;
	}
    }

    private String getTimeModifierCommand(List<String> words)
	throws ScriptException {
	final ScriptException timeModifierException =
	    new ScriptException("Invalid time modifier."
				+ " Must be: for __ seconds");

	float seconds;
	try {
	    seconds = Float.parseFloat(words.get(words.size() - 2));
	} catch (NumberFormatException e) {
	    throw timeModifierException;
	}
	int millis = (int)(seconds * 1000);
	if(millis < 0)
	    throw new ScriptException("Wait times must be positive");
	if(millis > 99999)
	    throw new ScriptException("Wait times must be below 100 seconds");
	
	for(int i = 0; i < 3; i++)
	    words.remove(words.size() - 1);

	return "w" + padInt(millis, 5);
    }

    private boolean hasSpeedModifier(List<String> words) throws ScriptException{
	String last = words.get(words.size() - 1);
	if(last.endsWith("%")) {
	    if(!words.get(words.size() - 2).equals("at"))
		throw new ScriptException("Invalid speed modifier."
					  + " Must be: at __%");
	    else
		return true;
	}
	return last.equals("slow") || last.equals("fast");
    }

    private int getSpeedModifierValue(List<String> words)throws ScriptException{
	String last = words.get(words.size() - 1);
	words.remove(words.size() - 1);
	String secondToLast = words.get(words.size() - 1);
	boolean hasVery = false;
	if(secondToLast.equals("very")) {
	    words.remove(words.size() - 1);
	    hasVery = true;
	}
	if(secondToLast.equals("at")) {
	    words.remove(words.size() - 1);
	}

	if(last.equals("fast")) {
	    return hasVery ? VERY_FAST_SPEED : FAST_SPEED;
	} else if(last.equals("slow")) {
	    return hasVery ? VERY_SLOW_SPEED : SLOW_SPEED;
	} else if(last.endsWith("%")) {
	    float percent;
	    try {
		percent = Float.parseFloat(last.substring(0, last.length()-1));
	    } catch (NumberFormatException e) {
		throw new ScriptException("Invalid speed modifier."
					  + " Must be: at __%");
	    }
	    if(percent < 0 || percent > 100)
		throw new ScriptException("Speed must be between 0 and 100%");
	    return (int)(percent * (float)MAX_SPEED / 100.0);
	} else {
	    throw new ScriptException("Unexpected error parsing speed modifier."
				      );
	}
    }

    private String padInt(int n, int digits) {
	if(n < 0)
	    return "-" + String.format("%0" + (digits - 1) + "d", -n);
	else
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
