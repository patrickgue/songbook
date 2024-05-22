# songbook

Program to create simple song books for different formats. Currently following formats are supported:

* LaTeX using the [leadsheets](https://ctan.org/pkg/leadsheets) package
* HTML

## Format

The formatting is quite simple, it works the following way:

```
@song=Song title
@artist=Song Artist
@capo=2

@maketitle

[Section]

# C          G              Am F
> Let's sing to some chords
```

See [example.song](example.song)


## Usage

`converter` has following arguments:

`-h` Output HTML

`-l` Output LaTeX

`-s` Output standalone document

`-i` Input file

`-o` Output file

Either `-h` or `-l` have to be provided. If input or output file are omitted, the program reads and writes from stdin and stdout respectively.


## Installation

Build using GNU Make: `make`


## Todo

* Support additional leadsheets features

## Supported environments

Successfully tested on Linux (x86_64, gcc) AND FreeBSD (X86_64, clang), probably works on a lot of other platforms.