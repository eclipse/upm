Contributing a module                         {#contributions}
=====================

Here are the rules of contribution:
- Your module must have an example that builds against your UPM library
- Commits must have a sign-off line by everyone who reviewed them
- Commits must be named <file/module>: Some decent description
- You must license your module under a FOSS license. The recommended license
  is MIT but any permissive license is fine. Please consider that people using
  UPM may want to write proprietary programs with your sensors so we like to
  avoid GPL. (LGPL is fine). If your license is not MIT please include a
  LICENSE file in src/<mymodule>/
- Please test your module builds before contributing and make sure it works on
  the latest version of mraa. If you tested on a specific board/platform please
  tell us what this was in your PR.
- Try not to break master. In any commit.
- Attempt to have some decent API documentation below are the explicit rules on documentation:

Documentation
=============

- Try to have no warnings in doxygen, this is generally fairly easy
- Have the specific sensor manufacturer/model & version that you used, if you
  support multiple versions please list
- Comments do not need full stops
- Stick to <80 chars per line even in comments
- No text is allowed on the same line as the start or end of a comment /** */
- All classes should have a "@brief" and a "@snippet"

The example should have an 'Interesting' section which will be highlighted as a
code sample in doxygen.  Everything in between such tags will show up in the
class documentation when the following is put at the end of a class docstring
as show above.

