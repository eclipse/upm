Contributing a module                         {#contributions}
=====================

Here are the rules of contribution:
- Try not to break master. In any commit.
- Commits must have a sign-off line by everyone who reviewed them
- Commits must be named <file/module>: Some decent description
- You must license your module under an FOSS license. The recommended license
  is MIT but any permissive license is fine. Please consider that people using
  UPM may want to write proprietary programs with your sensors so we like to
  avoid GPL. (LGPL is fine). If your license is not MIT please include a
  LICENSE file in src/<mymodule>/
- Please test your module builds before contributing and make sure it works on
  the latest version of maa. If you tested on a specific board/platform please
  tell us what this was in your PR.

