# brace

A lightweight command-line utility for checking balanced symbols in text files. Detects mismatched brackets, parentheses, braces, and user-defined symbol pairs.

## Installation

### Ubuntu (via PPA)

The easiest way to install brace on Ubuntu:

```bash
sudo add-apt-repository ppa:stefanos-kapa/brace
sudo apt update
sudo apt install brace
```

To uninstall:

```bash
sudo apt remove brace
sudo add-apt-repository --remove ppa:stefanos-kapa/brace 
```

### Build from source

```bash
git clone https://github.com/stefanoskapa/brace.git
cd brace
make
sudo make install
```

This installs the binary to `/usr/local/bin` by default. To install to a different location:

```bash
sudo make install PREFIX=/usr
```

To uninstall:

```bash
sudo make uninstall
```

## Usage

```
brace [OPTION]... [FILE]
```

With no FILE, reads from standard input.

### Options

| Option | Description |
| --- | --- |
| `-s <symbols>` | Define custom pairs of open/close symbols (default: `[]{}()`) |
| `-v, --version` | Show version |
| `-h, --help` | Display help |

## Examples

Check a file with default symbols:

```bash
brace main.c
```

Read from stdin:

```bash
cat main.c | brace
```

Use custom symbol pairs:

```bash
brace -s "()<>" template.html
```

Check multiple symbol types:

```bash
brace -s "()[]<>{}" config.xml
```

## Output

When symbols are balanced, brace exits silently with code 0.

When a mismatch is found, brace reports the location:

```
Closing symbol on 42:17 has no match ---> }
```

```
Opening symbol on 10:5 has no match ---> (
```

## Custom symbols

The `-s` option accepts pairs of characters where each pair consists of an opening symbol followed by its closing symbol. For example:

* `()` - parentheses
* `[]` - square brackets
* `{}` - curly braces
* `<>` - angle brackets

Combine multiple pairs in a single string:

```bash
brace -s "()[]{}<>" file.txt
```

## Use cases

* Validating configuration files
* Checking legacy codebases
* Pre-commit hooks
* CI/CD pipelines
* Quick syntax sanity checks

## Exit codes

| Code | Meaning |
| --- | --- |
| 0 | Balanced/Success |
| 1 | Unbalanced |
| 2 | Error |

## License

MIT License - see [LICENSE](LICENSE) for details.
