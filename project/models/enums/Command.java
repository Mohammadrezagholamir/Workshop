package models.enums;
import java.util.regex.*;;
/*
Explanation:
- for handling regexes and commands we need to define an interface.
- we have different menus that each one has different commands(so different regexes).
- but after all they are all commands and we can write some part of their code once here (:
 */

public interface Command {
    abstract Matcher getMatcher(String regex);
}