
package ardroid;

import java.util.*;

import com.google.appinventor.components.runtime.*;
import com.google.appinventor.components.annotations.DesignerComponent;
import com.google.appinventor.components.annotations.DesignerProperty;
import com.google.appinventor.components.annotations.PropertyCategory;
import com.google.appinventor.components.annotations.SimpleEvent;
import com.google.appinventor.components.annotations.SimpleFunction;
import com.google.appinventor.components.annotations.SimpleObject;
import com.google.appinventor.components.annotations.SimpleProperty;
import com.google.appinventor.components.common.ComponentCategory;
import com.google.appinventor.components.common.PropertyTypeConstants;
import com.google.appinventor.components.common.YaVersion;

import ardroid.ArdroidScriptCompiler;
import ardroid.ArdroidScriptCompiler.ScriptException;


@DesignerComponent(version = YaVersion.CLOCK_COMPONENT_VERSION,
		   description = "Compiler for ArdroidScript",
		   category = ComponentCategory.EXTENSION,
		   nonVisible = true,
		   iconName = "images/extension.png")
@SimpleObject(external = true)
public final class ArdroidScriptCompilerComponent 
    extends AndroidNonvisibleComponent implements Component, Deleteable {
    
    private ArdroidScriptCompiler compiler;
    private String error = "";
    private String compiledScript = "";

    /**
     * Creates a new component.
     *
     * @param container ignored (because this is a non-visible component)
     */
    public ArdroidScriptCompilerComponent(ComponentContainer container) {
	super(container.$form());

	compiler = new ArdroidScriptCompiler();
    }

    // Only the above constructor should be used in practice.
    public ArdroidScriptCompilerComponent() {
	super(null);
    }
  
    @SimpleProperty(category = PropertyCategory.BEHAVIOR, description ="")
    public String GetCompiledScript() {
	return compiledScript;
    }

    @SimpleProperty(category = PropertyCategory.BEHAVIOR, description ="")
    public String GetError() {
	return error;
    }

    @SimpleProperty(category = PropertyCategory.BEHAVIOR, description ="")
    public String GetScriptName() {
	return compiler.getScriptName();
    }

    @SimpleFunction (description = "")
    public boolean Compile(List<String> script) {
	try {
	    compiledScript = compiler.compile(script);
	    error = "";
	    return true;
	} catch (ScriptException e) {
	    error = e.getMessage();
	    return false;
	}
    }

    public void onDelete() { }
}
