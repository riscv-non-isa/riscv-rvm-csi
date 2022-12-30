import unittest, jsonschema, os
import parser

test_data_file_path = "./test_data/"

class Test_Parser_Top_Level(unittest.TestCase):
    def test_should_provide_command_line_help(self):
        try:
            parser.main(["--help"])
            self.assertEqual("Should not reach this statement",None)
        except SystemExit as e:
            self.assertEqual(e.code, 0)
        
    def test_should_throw_error_if_input_file_is_not_found(self):
        try:
            parser.main(["foo.yaml"])
            self.assertEqual("Should not reach this statement",None)
        except FileNotFoundError as e:
            self.assertEqual(e.strerror, "No such file or directory")
            
    def test_should_throw_error_if_input_yaml_does_not_match_schema(self):
        try:
            test_file = test_data_file_path + "invalid.rvm-csi.yaml"
            output = parser.main([test_file])
            self.assertEqual("Should not reach this statement",None)
        except jsonschema.exceptions.ValidationError as e:
            self.assertEqual(e.message, "Additional properties are not allowed ('another-duff-property' was unexpected)")            

    def test_should_build_header_files_for_c_language(self):
        test_file = test_data_file_path + "simple.rvm-csi.yaml"
        parser.main([test_file])
        self.assertEqual(os.path.isfile("./output/csi.h"), True)
        self.assertEqual(os.path.isfile("./output/csi_interrupts.h"), True)
        self.assertEqual(os.path.isfile("./output/csi_discovery.h"), True)
        self.assertEqual(os.path.isfile("./output/csi_defs.h"), True)        

