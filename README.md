# jPod
`jPod` stands for "Jerome's Pod", a Linux shell that I built *from scratch*!

It consists of a lexical analyzer and a syntactic analyzer.

The programs are *super modular*, both at function level and source code level!

`jPod` can handle external and built-in commands, input/output redirection, signals, and errors.


# Install
- Download or clone this project repository
- Open terminal and type `cd /where/you/put/the/repo/Linux-Shell`
- To build everything, type `make all`
- Type `./ish` to start `jPod` and have fun!

# Lexical Analyzer
- Accept an array of characters, and return a DynArray object containing tokens.
- From the user's point of view, a token is a word. More formally, from the user's point of view a token consists of a sequence of non-white-space characters that is separated from other tokens by white-space characters. 
Strings enclosed in double quotes (") form part or all of a single token.
- The lexical analyzer works for lines of *any* length.
In other words, the lexical analyzer allows us to do something like:



# Syntactic Analyzer

# 
