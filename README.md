# IJCP
It's just copy-paste.

## Installation

### Manual

> git clone https://github.com/DasLanky/IJCP.git

> cd IJCP/

> make

> echo PATH=$PATH:$(pwd)/dist

##  Usage

( Input ) | ijcp -(options) ((filename))

- Input: raw text meant to be copied
- options
  - "-c": copy
  - "-p": paste
  - "-o": output file (follow with filename)
  - "-v": verbose

### Example

> $ echo "I love IJCP!" | ijcp -c

> Copied 13 bytes to clipboard

> $ ijcp -p

> I love IJCP!

> $ ijcp -p -o ./output.txt

> $ cat ./output.txt

> I love IJCP!
