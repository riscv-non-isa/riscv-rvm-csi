def indent():
    '''Insert standard-size indentation for C code'''
    return "    "   # indent 4 spaces

def format_c_function_prototype(function):
    '''Format a function prototype as a string, from fields extracted from yaml'''
    return_type = "void" 
    if 'c-return-value' in function.keys():
        return_type = function['c-return-value']['type']
    
    out_str = return_type + " " + function['name'] + "("
    if 'c-params' in function.keys():
        for param in function['c-params']:
            
            param_type = param['type']
            param_name = param['name']
            if param_type[-1] == '*':
                # pointer types - present with spacing as "int *a" 
                param_type = param_type.rstrip('* ')
                param_name = "*" + param_name
                                                                    
            out_str += param_type + " " + param_name + ", "
        
        out_str = out_str.rstrip(", ") # Get rid of last comma/space
        
    else:
        out_str += "void"       
    
    out_str +=");\n"
    return out_str

def format_c_function_typedef(declaration):
    '''Format a function type declaration as a string'''
    retval = "void"
    params = "void"
    if 'func-typedef-retval' in declaration.keys():
        retval = declaration['func-typedef-retval']
    if 'func-typedef-params' in declaration.keys():
        params = ''
        for param in declaration['func-typedef-params']:
            param_type = param['type']
            param_name = param['name']
            if param_type[-1] == '*':
                # pointer types - present with spacing as "int *a" 
                param_type = param_type.rstrip('* ')
                param_name = "*" + param_name                          
            params += param_type + " " + param_name + ", "
        params = params.rstrip(", ") # Get rid of last comma/space
    out_str = "typedef " + retval + " (" + declaration['name'] + ")(" + params + ");\n"
    return out_str

def format_c_enum_typedef(declaration, include_comments):
    '''Format an enum type declaration as a string'''
    out_str = "typedef enum {\n"
    for member in declaration['enum-members']:
        out_str += indent() + member['name'] 
        if 'value' in member.keys():
            out_str += " = " + str(member['value'])
        out_str += "," # trailing comma should be valid for modern compilers
        if include_comments:
            if 'description' in member.keys():
                out_str += " /* " + member['description'] + " */"
        out_str += "\n"
    out_str += "} " + declaration['name'] + ";\n"
    return out_str
