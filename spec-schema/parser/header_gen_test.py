import unittest, header_gen

class Test_Parser_Units(unittest.TestCase):
    def test_should_format_include_statements(self):
        include_file = {'filename':'stdio.h', 'system-header':True}
        self.assertEqual(header_gen.format_c_include_file(include_file), "#include <stdio.h>\n")
        include_file = {'filename':'csi.h', 'system-header':False}
        self.assertEqual(header_gen.format_c_include_file(include_file), "#include \"csi.h\"\n")

if __name__ == '__main__':
    unittest.main()