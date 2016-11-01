# IniRW

##  Feature & Summary

IniRW ( Ini Reader + Writter) is a ini file manager that can be read and writte. 
It's written in c++11.

## Example

```c++
Ini::Reader reader;
if (reader.readIniFile("config.ini"))
reader.get<std::string>("section","key","defaultValue");
```

## How to use it?
Copy `src/Ini.*` to your project.

## LICENSE
`IniRW` is released under the MIT.
