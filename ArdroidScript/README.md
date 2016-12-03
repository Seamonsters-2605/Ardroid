# ArdroidScript
ArdroidScript allows the Android controller app to run scripted actions, with a built in script editor on the app. The scripting language is designed to be discoverable and as simple as possible to use, with no programming experience required. It is inspired by Kodu Game Lab. Currently some features have been disabled (all stepper motor commands and the `run script` command).

This directory has the "compiler" for the language. It is written in Java as an extension to App Inventor. The compiled extension is in the root directory: `ardroid.ArdroidScriptCompilerComponent.aix`

See `Documentation.md` for a description of the syntax and the intermediate compiled form.
