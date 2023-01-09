import sys, json
import argparse, yaml, jsonschema
import header_gen, doc_gen
import os.path

default_schema_file_path = os.path.join(os.path.dirname(__file__), "..")
default_schema_file_name = "rvm-csi.schema.json"

def parse_arguments(argv):
    ''' Parses command land args using standard Python module.'''
    
    parser = argparse.ArgumentParser(description="Parse RVM CSI API yaml definition to generate language specific header files")
    parser.add_argument("infile", help="Input yaml file defining API")
    parser.add_argument("--out-dir", dest='out_dir', default='output', help="Output directory")
    parser.add_argument("--generate-docs", dest='generate_docs',action='store_true', default=False, help="Generate documentaton")
    parser.add_argument("--doc-out-dir", dest='doc_out_dir', default='adoc_output', help="Documentation output directory")
    parser.add_argument("--target-language", dest='target_language',choices=['C'], default='C', help="Target language")

    return parser.parse_args(argv)

def load_api_definition(file_name):
    ''' Loads yaml deifnition from file.'''
    
    with open(file_name,'r') as yaml_in:
        return yaml.safe_load(yaml_in)

def load_api_schema(file_name):
    ''' Loads json schema for api from file
        In the future this may be loaded from an online source.
    '''
    
    with open(file_name,'r') as json_schema_in:
        return json.load(json_schema_in)
        
def validate_json_schema(api_definition, schema):
    ''' Validates api definition against schema to ensure it is valid.'''
    
    jsonschema.validate(api_definition, schema)
    
def generate_documentation(api_definition, opts):
    ''' Calls language appropriate documentation generation function.'''
    
    target_language = opts.target_language
    if target_language == "C":
        doc_gen.generate_c_adoc(api_definition, opts.doc_out_dir)
    else: 
        raise('Target language implementation undefined')
    
def generate_headers(api_definition, opts):
    ''' Calls language appropriate header generation function.'''
    
    target_language = opts.target_language
    if target_language == "C":
        header_gen.generate_c(api_definition, opts.out_dir)
    else: 
        raise('Target language implementation undefined')
        
def main(argv):
    ''' Parser top level - invoked from command line
        Builds either header files or adoc documentation from
        a validated yaml api description,
    '''
    
    options = parse_arguments(argv)
    api_definition = load_api_definition(options.infile)
    
    schema = load_api_schema(os.path.join(default_schema_file_path, default_schema_file_name))

    validate_json_schema(api_definition, schema)
    
    if (options.generate_docs):
        return generate_documentation(api_definition, options)
    else: 
        return generate_headers(api_definition, options)
            
if __name__ == '__main__':
    main(sys.argv[1:])


