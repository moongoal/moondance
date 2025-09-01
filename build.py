import shutil

from pathlib import Path
from jinja2 import FileSystemLoader, Environment, select_autoescape


PROJDIR = Path(__file__).parent
BUILDDIR = PROJDIR / "build"
SRCDIR = PROJDIR / "src"
INCLUDEDIR = PROJDIR / "include"
TESTSRCDIR = PROJDIR / "test"


def layout():
    BUILDDIR.mkdir(exist_ok=True)


def configure():
    
layout()
configure()
