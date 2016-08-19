# ArdroidScript
When ArdroidScript is finished, it will allow the Android controller app to run scripted actions, with a built in script editor on the app. The scripting language is designed to be discoverable and as simple as possible to use, with no programming experience required. It is inspired by Kodu Game Lab.

This directory has the "compiler" for the language. It is written in Java as an extension to App Inventor. The compiled extension is in the root directory: `ardroid.ArdroidScriptCompilerComponent.aix`

See `Documentation.md` for a description of the syntax, and the intermediate compiled form.

The script editor is a work in progress and is currently hidden from the interface. To access it in the app:
- Tap `Dev Tools` on the main screen
- Choose `Yes` for the warning message
- Tap `Open Screen`
- Type `ScriptScreen` with correct capitalization and no spaces in the box
- Tap `OK`

Currently the editor and compiler is finished but the app cannot run compiled code.
