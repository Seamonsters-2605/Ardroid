javac src/ardroid/ArdroidScriptCompiler.java -d bin -source 1.5 || { exit 1; }
mkdir bin -p
cd bin
cat ../$1 | java ardroid.ArdroidScriptCompiler
cd ..
