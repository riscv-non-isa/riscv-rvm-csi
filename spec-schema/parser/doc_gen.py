import pathlib

def format_text_from_array(input_array):
    ''' Concatenates an array of strings into a single return string.'''
    out_str = ""
    
    for item in input_array:
        out_str += item
        out_str += "\n"
    return out_str

def format_adoc_type_declaration(declaration):
    ''' Builds adoc level 3 & 4 section for supplied type declaration.
        Returns this as a string.
    '''
         
    # values can be "struct", "enum", "int", "unsigned"
    c_type = declaration['type'] 
    
    out_str = "[#type_" + declaration['name'] + "]\n"
    out_str += "=== " + c_type + " *" + declaration['name'] + "*\n"
    out_str += declaration['description'] + "\n\n" 
            
    if c_type == "enum":
        out_str += "==== Values\n"
        
        for member in declaration['enum-members']:
            out_str += "*" + member['name'] + "*\n\n" 

    if c_type == "struct":
        out_str += "==== Members\n"

        for member in declaration['struct-members']:
            member_type = member['type']
            delimiter = " "
            if member_type[-1] == '*': # pointer
                delimiter = ""
            out_str += member_type + delimiter + member['name'] + "\n\n"
                             
    return out_str

def format_adoc_function(function, module_type_list):
    ''' Builds adoc level 3 & 4 section for supplied function declaration.
        Second parameter is a list of types declared within this module which
        is used to create cross-references.
        Returns function adoc string.
    '''
    
    def format_param_type(type,types_defined_by_module):
        ''' If parameter type is defined within module then render this as a cross-reference'''
        if type in types_defined_by_module:
            return "<<type_" + type + ",`" + type + "`>>" # Adoc cross-reference to type
        else:
            return type
        
    out_str = "=== " + function['name'] + "\n"
    out_str += function['description'] + "\n\n"
        
    out_str += "==== Return\n"
    
    if 'c-return-value' in function.keys():
        out_str += "`" + function['c-return-value']['type'] + "` - " + function['c-return-value']['description'] + "\n\n"
    
    out_str += "==== Parameters\n"
    
    if 'c-params' in function.keys():
        for param in function['c-params']:
                        
            param_type = param['type']
            param_name = param['name']
            if param_type[-1] == '*': # pointer
                param_type = param_type.rstrip('* ')
                param_name = "*" + param_name
                                        
            out_str += format_param_type(param_type, module_type_list) + " `" + param_name + "` - " + param['description'] + "\n\n"
            
            if 'notes' in param.keys():
                out_str += format_text_from_array(param['notes'])
            out_str += "\n"
                
    else:
        out_str += "Function takes no parameters\n\n"
    
    return out_str   

def generate_c_module_adoc(module, out_dir, module_sub_dir):
    ''' Builds adoc file for a module.
        Inputs are the module definition and the output directory & sub directory for the
        adoc file.
    '''
        
    filename = module['c-filename'].lower().replace('.','_') + ".adoc"    
    out_file = pathlib.Path(out_dir, module_sub_dir, filename)
    
    out_str = "[#title]\n"
    out_str += "= " + module['c-filename'] + " - " + module['name'] + "\n"
    out_str += ":toc:\n"
    
    out_str += module['description'] + "\n\n"
    
    out_str += "== Notes\n"
    
    if 'notes' in module.keys():
        out_str += format_text_from_array(module['notes'])
        out_str += "\n"
    
    if 'c-specific-notes' in module.keys():
        out_str += format_text_from_array(module['c-specific-notes'])
        out_str += "\n"
                
    out_str += "== Types\n"
    
    # Add type declarations
    types_provided_by_module = [] # used to create link between function params and types, if possible
    if 'c-type-declarations' in module.keys():
        for type_declaration in module['c-type-declarations']:
            out_str += format_adoc_type_declaration(type_declaration)
            out_str += "\n"
            types_provided_by_module.append(type_declaration['name'])
            
        out_str += "\n"
    
    out_str += "== Functions\n"
        
    # Add function declarations
    if 'functions' in module.keys():
        for function in module['functions']:
            out_str += format_adoc_function(function, types_provided_by_module)
            out_str += "\n"
        out_str += "\n"
        
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

def generate_c_adoc(api_definition, out_dir):
    ''' Top level function which builds a top level index adoc file then 
        iterates through modules defined in the api definition to build module
        documentation.
        Input parameters are the api_definition object and the output directory 
        for the adoc files. 
    '''
        
    out_file = pathlib.Path(out_dir, top_level_filename)
        
    out_str = "[#title]\n"
    out_str += "= " + api_definition['c-documentation-title'] + "\n"
    
    if 'notes' in api_definition.keys():
        out_str += format_text_from_array(api_definition['notes'])
        out_str += "\n"
    
    if 'c-specific-notes' in api_definition.keys():
        out_str += format_text_from_array(api_definition['c-specific-notes'])
        out_str += "\n"
    
    out_str += "== Modules\n"
    
    for module in api_definition['modules']:
        # Generate docs for module - this will be a new file
        # Returns [c filename, module name, adoc filename]
        module_links = generate_c_module_adoc(module, out_dir, module_sub_dir)
    
        # Add link to a table of contents
        module_file_wth_path = pathlib.Path(module_sub_dir, module_links[2])
        out_str += "* xref:" + str(module_file_wth_path) + "#title[" + module_links[0] + "] - " + module_links[1] + "\n"
    
    # Write to the output file
    out_file.parent.mkdir(exist_ok=True, parents=True)
    out_file.write_text(out_str)