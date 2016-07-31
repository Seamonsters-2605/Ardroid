javac src/*.java -d bin || { exit 1; }
cd bin
cat ../$1 | java ArdroidScriptCompiler
cd ..
