import pathlib
import re
from parser_common import format_c_function_prototype, format_c_function_typedef, format_c_enum_typedef

def heading_marker(level):
    '''Add a heading marker at given level relative to the top level as defined by the
    top-heading-level yaml parameter'''
    s = ''
    for i in range(top_heading_level + level):
        s += '='
    return s + ' '

def format_text_from_array(input_array):
    ''' Concatenates an array of strings into a single return string.'''
    out_str = ""
    
    for item in input_array:
        out_str += item
        out_str += "\n"
    return out_str

def preprocess_descriptive_text(text, linked_sections):
    '''Searches text for matches within linked_sections, and replaces with links. 
Also escapes any asciidoc special characters.'''
    for s in linked_sections:
        text = text.replace(s, '<<' + s + ', ' + s + '>>')
    text = re.sub(r"(\s+)_(.+)_", r"\1\_\2_", text) # Escape use of _ that could be interpreted as italics
    return text

def format_adoc_type_declaration(declaration):
    ''' Builds adoc level 3 & 4 section for supplied type declaration.
        Returns this as a string.
    '''
         
    # values can be "struct", "enum", "int", "unsigned"
    c_type = declaration['type'] 
    
    out_str = "[#" + declaration['name'] + "]\n"
    out_str += heading_marker(3) + c_type + " *" + declaration['name'] + "*\n"
    out_str += declaration['description'] + "\n\n" 
            
    if c_type == "enum":
        out_str += '''
[source, c]
----
'''
        out_str += format_c_enum_typedef(declaration, False)
        out_str += "----\n\n"

        out_str += heading_marker(4) + "Values\n"
        out_str += '''
[cols="6,6",options="header"]
|===
|Value|Description
'''
        for member in declaration['enum-members']:
            out_str += '|' + member['name'] +'|'
            if 'description' in member.keys():
                out_str += member['description'] + "\n"
            else:
                out_str += "\n"
        out_str += '''
|===
'''

    if c_type == "struct":
        out_str += heading_marker(4) + "Members\n"

        for member in declaration['struct-members']:
            member_type = member['type']
            delimiter = " "
            if member_type[-1] == '*': # pointer
                delimiter = ""
            out_str += member_type + delimiter + member['name'] + "\n\n"

    if c_type == "function":
        out_str += '''
[source, c]
----
'''
        out_str += format_c_function_typedef(declaration)
        out_str += "----\n\n"
        out_str += heading_marker(4) + "Parameters\n"
        if 'func-typedef-params' in declaration.keys():
            for param in declaration['func-typedef-params']:
                param_type = param['type']
                param_name = param['name']
                if param_type[-1] == '*': # pointer
                    param_type = param_type.rstrip('* ')
                    param_name = "*" + param_name
                out_str += param_type + " `" + param_name + "` - " + param['description'] + "\n\n"
                
                if 'notes' in param.keys():
                    out_str += format_text_from_array(param['notes'])
                out_str += "\n"
        else:
            out_str += "Function takes no parameters\n\n"

    return out_str

def format_adoc_function(function, linked_sections):
    ''' Builds adoc level 3 & 4 section for supplied function declaration.
        Second parameter is a list of types declared within this module which
        is used to create cross-references.
        Returns function adoc string.
    '''
        
    out_str = heading_marker(3) + function['name'] + '[[' + function['name'] + ']]' "\n"

    out_str += '''
[source, c]
----
'''
    out_str += format_c_function_prototype(function)
    out_str += "----\n\n"

    out_str += preprocess_descriptive_text(function['description'], linked_sections) + "\n\n"
    if 'notes' in function.keys():
        for note in function['notes']:
            out_str += preprocess_descriptive_text(note, linked_sections) + "\n\n"
    
    out_str += heading_marker(4) + "Return\n"
    
    if 'c-return-value' in function.keys():
        out_str += "`" + function['c-return-value']['type'] + "` - " + \
                preprocess_descriptive_text(function['c-return-value']['description'], linked_sections) + "\n\n"
    
    out_str += heading_marker(4) + "Parameters\n"
    
    if 'c-params' in function.keys():
        for param in function['c-params']:
                        
            param_type = param['type']
            param_name = param['name']
            if param_type[-1] == '*': # pointer
                param_type = param_type.rstrip('* ')
                param_name = "*" + param_name
                                        
            out_str += preprocess_descriptive_text(param_type, linked_sections) + " `" + param_name + "` - " + \
                    preprocess_descriptive_text(param['description'],linked_sections) + "\n\n"
            
            if 'notes' in param.keys():
                out_str += format_text_from_array(param['notes'])
            out_str += "\n"
                
    else:
        out_str += "Function takes no parameters\n\n"
    
    return out_str   

def format_adoc_macro(macro, linked_sections):
    ''' Builds adoc level 3 & 4 section for supplied macro declaration.
        Returns function adoc string.
    '''
    out_str = heading_marker(3) + macro['name'] + '[[' + macro['name'] + ']]' "\n"
    out_str += '''
[source, c]
----
'''
    out_str += macro['code']
    out_str += "\n----\n\n"

    out_str += preprocess_descriptive_text(macro['description'], linked_sections) + "\n"

    if 'notes' in macro.keys():
        for note in macro['notes']:
            out_str += note + "\n\n"
    
    out_str += heading_marker(4) + "Return\n"
    
    if 'c-return-value' in macro.keys():
        out_str += "`" + macro['c-return-value']['type'] + "` - " + \
                macro['c-return-value']['description'] + "\n\n"
    
    out_str += heading_marker(4) + "Parameters\n"
    
    if 'c-params' in macro.keys():
        for param in macro['c-params']:
                        
            param_type = param['type']
            param_name = param['name']
            if param_type[-1] == '*': # pointer
                param_type = param_type.rstrip('* ')
                param_name = "*" + param_name
                                        
            out_str += param_type + " `" + param_name + "` - " + param['description'] + "\n\n"
            
            if 'notes' in param.keys():
                out_str += format_text_from_array(param['notes'])
            out_str += "\n"
    else:
        out_str += "Macro takes no parameters\n\n"

    return out_str



def generate_c_module_adoc(module, out_dir, module_sub_dir, adoc_optimization, linked_sections):
    ''' Builds adoc file for a module.
        Inputs are the module definition and the output directory & sub directory for the
        adoc file.
    '''
        
    filename = module['c-filename'].lower().replace('.','_') + ".adoc"    
    out_file = pathlib.Path(out_dir, module_sub_dir, filename)
    
    out_str = "indexterm:[" + module['c-filename'] + "]\n\n"
    if adoc_optimization == 'html':
        out_str += "[#title]\n"
    out_str += heading_marker(1) + module['c-filename'] + " - " + module['name'] + "\n"
    out_str += ":toc:\n"
    
    out_str += preprocess_descriptive_text(module['description'], linked_sections) + "\n\n"
    
    if 'notes' in module.keys() or 'c-specific-notes' in module.keys():
        out_str += heading_marker(2) + "Notes\n"
    
    if 'notes' in module.keys():
        for note in module['notes']:
            out_str += preprocess_descriptive_text(note, linked_sections)
            out_str += "\n\n"
    
    if 'c-specific-notes' in module.keys():
        out_str += preprocess_descriptive_text(format_text_from_array(module['c-specific-notes']), linked_sections)
        out_str += "\n"
    
    # Add type declarations
    if 'c-type-declarations' in module.keys():
        out_str += heading_marker(2) + "Types\n"
        for type_declaration in module['c-type-declarations']:
            out_str += format_adoc_type_declaration(type_declaration)
            out_str += "\n"
        out_str += "\n"
    
    # Add function declarations
    if 'functions' in module.keys():
        out_str += heading_marker(2) + "Functions\n"
        for function in module['functions']:
            out_str += format_adoc_function(function, linked_sections)
            out_str += "\n"
        out_str += "\n"

    if 'macros' in module.keys():
        out_str += heading_marker(2) + "Macros\n"
        for macro in module['macros']:
            out_str += format_adoc_macro(macro, linked_sections)
            out_str += "\n"
        out_str += "\n"

    if 'c-definitions' in module.keys():
        out_str += heading_marker(2) + "Definitions\n"
        for fragment in  module['c-definitions']:
            out_str += fragment['comment'] + ':\n'
            out_str += '''
[source, c]
----
'''
            out_str += fragment['fragment'] + '----\n'
    
    if adoc_optimization == 'html':
        # Add link back to top level          
        out_str += "'''\n"
        top_level_filename_with_path = pathlib.Path("..", top_level_filename)
        out_str += "xref:" + str(top_level_filename_with_path) + "#title[Back to top level]\n"
    
    # Write to the output file
    out_file.parent.mkdir(exist_ok=True, parents=True)
    out_file.write_text(out_str)
    
    return module['c-filename'],module['name'],filename

# Top level file is always "index.adoc"
top_level_filename =  "index.adoc"

# Module sub-directory definition
module_sub_dir = "modules"

top_heading_level = 0 #default

def generate_c_adoc(api_definition, module_definitions, out_dir):
    ''' Top level function which builds a top level index adoc file then 
        iterates through modules defined in the api definition to build module
        documentation.
        Input parameters are the api_definition object and the output directory 
        for the adoc files. 
    '''
        
    out_file = pathlib.Path(out_dir, top_level_filename)
    
    global top_heading_level
    top_heading_level = api_definition['top-heading-level']

    # Get a set of function and macro names, we will create links where these are
    # referred to.
    linked_sections = []
    for m in module_definitions:
        module = m['module']
        if 'functions' in module.keys():
            for function in module['functions']:
                linked_sections.append(function['name'])
        if 'c-type-declarations' in module.keys():
            for dec in module['c-type-declarations']:
                linked_sections.append(dec['name'])

    # Create document as a string
    out_str = "[#title]\n"
    out_str += heading_marker(1) + api_definition['c-documentation-title'] + "\n"
    
    if 'notes' in api_definition.keys():
        out_str += format_text_from_array(api_definition['notes'])
        out_str += "\n"
    
    if 'c-specific-notes' in api_definition.keys():
        out_str += format_text_from_array(api_definition['c-specific-notes'])
        out_str += "\n"
    
    out_str += heading_marker(2) + "Modules\n"
    
    for m in module_definitions:
        module = m['module']
        # Generate docs for module - this will be a new file
        # Returns [c filename, module name, adoc filename]
        module_links = generate_c_module_adoc(module, out_dir, module_sub_dir, api_definition['adoc-optimization'], linked_sections)
    
        # Add link to a table of contents
        module_file_wth_path = pathlib.Path(module_sub_dir, module_links[2])
        out_str += "* xref:" + str(module_file_wth_path) + "#title[" + module_links[0] + "] - " + module_links[1] + "\n"
    
    # Write to the output file
    out_file.parent.mkdir(exist_ok=True, parents=True)
    out_file.write_text(out_str)