import textwrap, pathlib
from parser_common import format_c_function_prototype, format_c_function_typedef, indent, format_c_enum_typedef

def format_c_comment_lines(input_string):
    ''' Takes a string as input. 
        Returns input string wrapped to 80 chars with each line pre-prepended 
        with an asterix so they can be used in a C comment. 
    '''
    out_str = ""
    for lines in textwrap.wrap(input_string,80):
        out_str += ("* " + lines).rstrip() + "\n "
    return out_str

def format_c_comment_lines_from_array(input_array):
    ''' Takes an array of strings to be used as comments.  
        Returns a string with the array concatenated into comment line.
        Return string does not have comment terminator (*/).
    '''
    
    out_str = ""
    for item in input_array:
        out_str += format_c_comment_lines(item)
        out_str += "*\n "
    return out_str    

def format_c_include_file(include_file):
    ''' Takes an include file object.
        Returns a string with the file built into a C include line
    '''
    
    out_str = ""
    if (include_file['system-header']):
        out_str += "#include <" + include_file['filename'] + ">\n"   
    else:
        out_str += "#include \"" + include_file['filename'] + "\"\n"
    return out_str    

def format_c_type_prefix_list(prefix_list):
    ''' Takes a list of C prefixes to be applied to a type definition.
        Values can be ["const", "static", "volatile", "inline"].
        Returns a string with these concatenated together in the correct order
        for C compilation.
    '''
    
    out_str = ""
    if "static" in prefix_list:
        out_str += "static "
        
    if "volatile" in prefix_list:
        out_str += "volatile "
        
    if "inline" in prefix_list:
        out_str += "inline "        

    if "const" in prefix_list:
        out_str += "const "
        
    return out_str
    
def format_c_type_declaration(declaration):
    out_str = ""
    if 'description' in declaration.keys():
        out_str += "/*\n "
        out_str += format_c_comment_lines(declaration['description'])
        if 'func-typedef-params' in declaration.keys():
            out_str += "*\n "
            for param in declaration['func-typedef-params']:
                out_str += format_c_comment_lines("@param " + param['name'] + ": " + param['description'])
        out_str += "*/\n"

    # values can be "struct", "enum", "int", "unsigned", "function"
    c_type = declaration['type']
    if c_type == "int":
        out_str += "typedef int " + declaration['name'] + ";\n"
        
    elif c_type == "unsigned":
        out_str += "typedef unsigned int " + declaration['name'] + ";\n"
        
    elif c_type == "enum":
        out_str += format_c_enum_typedef(declaration, True)

    elif c_type == "struct":
        out_str += "typedef struct {\n"
        for member in declaration['struct-members']:
            member_type = member['type']
            delimiter = " "
            if member_type[-1] == '*': # pointer
                delimiter = ""
            
            out_str += indent() + member_type + delimiter + member['name'] + ";\n"
        out_str += "} " + declaration['name'] + ";\n"
    
    elif c_type == "function":
        out_str += format_c_function_typedef(declaration)

    else:
        raise('undefined C type definition')   # Should not be possible to reach here            
    
    if 'type-prefixes' in declaration.keys():
        out_str = format_c_type_prefix_list(declaration['type-prefixes']) + out_str
                     
    return out_str

def format_c_function(function):
    ''' Takes a function object.
        Returns a string containing the function formatted as a C function
        prototype.
    '''
    
    # Start the comment.
    out_str = "/*\n "
        
    out_str += format_c_comment_lines(function['description'])
    out_str += "*\n "
    if 'notes' in function.keys():
        for note in function['notes']:
            out_str += format_c_comment_lines(note)
        out_str += "*\n "

    if 'c-params' in function.keys():
        for param in function['c-params']:
            out_str += format_c_comment_lines("@param " + param['name'] + ": " + param['description'])
    if 'c-return-value' in function.keys():
        out_str += format_c_comment_lines("@return " + ": " + function['c-return-value']['description'])

    # Close comment
    out_str += "*/\n"

    # Write out the function prototype
    out_str += format_c_function_prototype(function)

    return out_str

def format_c_macro(macro):
    ''' Takes a macro object.
        Returns a string containing the associated header file content.
    '''
    
    # Start the comment.
    out_str = "/*\n "
        
    out_str += format_c_comment_lines(macro['description'])
    out_str += "*\n "
    if 'notes' in macro.keys():
        for note in macro['notes']:
            out_str += format_c_comment_lines(note)
        out_str += "*\n "

    if 'c-params' in macro.keys():
        for param in macro['c-params']:
            out_str += format_c_comment_lines("@param " + param['name'] + ": " + param['description'])
    if 'c-return-value' in macro.keys():
        out_str += format_c_comment_lines("@return " + ": " + macro['c-return-value']['description'])

    # Close comment
    out_str += "*/\n"

    # Write out the macro code
    out_str += macro['code']
    out_str += "\n"

    return out_str


def generate_c(api_definition, module_definitions, out_dir):
    ''' Top level function which iterates through each of the modules in the api definition 
        to build C header content and write it an appropriate file.
        Input parameters are the api_definition object and the output directory for the header files 
    '''
    
    for m in module_definitions:
        module = m['module']
        out_file = pathlib.Path(out_dir, module['c-filename'])
        
        # Start the comment.
        out_str = "/*\n "
        
        # Insert the module name,  description & boilerplate strings line per line
        out_str += format_c_comment_lines(module['name'])
        out_str += "*\n "
        
        out_str += format_c_comment_lines(module['description'])
        out_str += "*\n "

        if 'notes' in module.keys():
            for note in module['notes']:
                out_str += format_c_comment_lines(note)
                out_str += "*\n "
        
        out_str += format_c_comment_lines(api_definition['boilerplate'])

        # Close comment
        out_str += "*/\n\n"

        # Guard against multiple inclusion with define based on filename, e.g. csi_defs.h => CSI_DEFS_H 
        def_file_name = pathlib.Path(out_file).name.upper().replace('.','_')
        out_str += "#ifndef " + def_file_name + "\n"
        out_str += "#define " + def_file_name + "\n"
        
        out_str += "\n"
        
        # Add include files        
        if 'c-include-files' in module.keys():
            for include_file in module['c-include-files']:
                out_str += format_c_include_file(include_file)
            out_str += "\n"
        
        # Protection against inclusion in assembler code if required
        if 'no-assembler' in module.keys():
            if module['no-assembler']:
                out_str += "#ifndef __ASSEMBLER__\n\n"
        
        # Add type declarations
        if 'c-type-declarations' in module.keys():
            for type_declaration in module['c-type-declarations']:
                out_str += format_c_type_declaration(type_declaration)
                out_str += "\n"
            out_str += "\n"
            
        # Add code fragments
        if 'c-definitions' in module.keys():
            for fragment in module['c-definitions']:
                out_str +=  "/*\n " + format_c_comment_lines(fragment['comment']) + "*/\n"
                out_str += fragment['fragment'] + '\n'
            out_str += "\n"

        # Add macros
        if 'macros' in module.keys():
            for macro in module['macros']:
                out_str += format_c_macro(macro)
            out_str += "\n"

        # Add function declarations
        if 'functions' in module.keys():
            for function in module['functions']:
                out_str += format_c_function(function)
                out_str += "\n"
            out_str += "\n"        
         
        # Protection against inclusion in assembler code if required
        if 'no-assembler' in module.keys():
            if module['no-assembler']:
                out_str += "#endif // __ASSEMBLER__\n\n"

        # Close guard against multiple inclusion
        out_str += "#endif /* " + def_file_name + " */ \n"
        
        # Write to the output file
        out_file.parent.mkdir(exist_ok=True, parents=True)
        out_file.write_text(out_str)
