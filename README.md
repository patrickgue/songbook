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

`songbook` has following arguments:

`-h` Output HTML

`-l` Output LaTeX

`-s` Output standalone document

`-i` Input file

`-o` Output file

Either `-h` or `-l` have to be provided. If input or output file are omitted, the program reads and writes from stdin and stdout respectively.


## Installation

Build using BSD Make: `bmake` (or simply make on BSD systems) 


## Todo

* Format only supports 7-bit ASCII (with escaping two-byte codes). Support UTF-8
* Support additional leadsheets features

