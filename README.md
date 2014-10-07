# QxtLogger

Logger-related classes extracted from LibQxt, namespaced and modernised for Qt 4.7+.

Includes:

* Logger
* Log stream
* Logger engine interface
* Abstract IO and file logger engine
* Basic STD logger engine
* Basic file logger engine

XML logger engine is not included.

## Porting from LibQxt

This library requires Qt 4.7 or newer, instead of Qt 4.2 which is required by LibQxt. All classes are namespaced in `Qxt` and have `Qxt` prefix stripped from their class names. For example, LibQxt’s `QxtLogger` would correspond to `Qxt::Logger` in this library.

Non-class names (e.g. `qxtLogger`) are global, as in LibQxt. Please report further incompatibilities.

## License

QxtLogger is released under LGPL 2.1 and CPL 1.0 (same with LibQxt).