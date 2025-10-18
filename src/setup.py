from setuptools import setup, Extension

module = Extension("test_module",
                   sources=['src/test.c', 'src/test_python.c'])

setup(name="test_module", version="1.0", ext_modules=[module])
