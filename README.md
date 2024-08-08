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


With makebook multiple `.song` files can be combined into a single web page or PDF document. The body section can be marked with the string `<$body$>`.

## Usage

`converter` has following arguments:

`-h` Output HTML

`-l` Output LaTeX

`-s` Output standalone document

`-i` Input file

`-o` Output file

Either `-h` or `-l` have to be provided. If input or output file are omitted, the program reads and writes from stdin and stdout respectively.

`makebook` has following arguments:

`-h` Output HTML

`-l` Output LaTeX

`-p` input file: Input directory path. Default is the current working directory

`-o` output file: Output directory path/file. Default is book.tex for latex or the directory book for HTML

`-t` template file


## Installation

Build using GNU Make: `make`


## Todo

* Support additional leadsheets features

## Supported environments

Works on Linux (x86_64, gcc). Compiles and mostly works on FreeBSD (c86_64, clang), and on WIN32 (with mingw32) with some minor bugs. Probably works on a lot of other platforms.
