# IniRW
## SIMPLE USAGE
```c++
Ini::Reader reader;
if (reader.readIniFile("config.ini"))
    reader.get<std::string>("section","key","defaultValue");
```
