# API Definition Parser

## Overview

A Python application which will parse a YAML RVM-CSI API definition file, validate it against the official schema, then generate either a set of C header files or a set of AsciiDoc documentation files.

## Prerequisites

* A Python installation ($\ge$ v3) 
* Python modules `jsonschema` & `PyYaml` .  Can be installed using `pip install -r requirements.txt`

## Usage Examples

### C Header file generation

Build a set of C header files, placing them in the default directory `./output`

`python3 parser.py ./test_data/simple.rvm-csi.yaml`

### AsciiDoc file generation

Build AsciiDoc documentation, placing the files in default directory `./adoc_output`.

`python3 parser.py --generate-docs ./test_data/simple.rvm-csi.yaml`

The top level is `index.adoc` which links to module specific files in a `/modules` subfolder

The .adoc files can be transformed to HTML using [Asciidoctor](https://asciidoctor.org), for example:

`find . -name *.adoc | xargs asciidoctor`

### Further help

`python parser.py --help` will display the application help files 

## Testing

*NB requires further work, possible refactoring into a different directory*

Top level parser tests:

`python3 -m unittest -v parser_test.py`

Header generation tests:

`python3 -m unittest -v header_gen_test.py`