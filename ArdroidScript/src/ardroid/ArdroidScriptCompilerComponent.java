
package ardroid;

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


@DesignerComponent(version = YaVersion.CLOCK_COMPONENT_VERSION,
		   description = "Compiler for ArdroidScript",
		   category = ComponentCategory.EXTENSION,
		   nonVisible = true,
		   iconName = "images/ardroidScript.png")
@SimpleObject(external = true)
public final class ArdroidScriptCompilerComponent 
    extends AndroidNonvisibleComponent
    implements Component, OnStopListener, OnResumeListener, OnDestroyListener,
	       Deleteable {
    
    private boolean onScreen = false;
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
	// Set up listeners
	form.registerForOnResume(this);
	form.registerForOnStop(this);
	form.registerForOnDestroy(this);

	if (form instanceof ReplForm) {
	    // In REPL, if this component was added to the project after the
	    // onResume call occurred, then onScreen would be false, but the
	    // REPL app is, in fact, on screen.
	    onScreen = true;
	}

	compiler = new ArdroidScriptCompiler();
    }

    // Only the above constructor should be used in practice.
    public ArdroidScriptCompilerComponent() {
	super(null);
    }
  
    @SimpleProperty(category = PropertyCategory.BEHAVIOR, description ="")
    public int GetCompiledScript() {
	return compiledScript;
    }

    @SimpleProperty(category = PropertyCategory.BEHAVIOR, description ="")
    public int GetError() {
	return error;
    }

    @SimpleProperty(category = PropertyCategory.BEHAVIOR, description ="")
    public int GetScriptName() {
	return compiler.getScriptName();
    }

    @SimpleFunction (description = "")
    public static boolean Compile(List<String> script) {
	try {
	    compiledScript = compiler.compile(script);
	    error = "";
	} catch (ScriptException e) {
	    error = e.getMessage();
	}
    }

    @Override
    public void onStop() {
	onScreen = false;
    }

    @Override
    public void onResume() {
	onScreen = true;
    }

    @Override
    public void onDestroy() {
	timerInternal.Enabled(false);
    }

    @Override
    public void onDelete() {
	timerInternal.Enabled(false);
    }
}
