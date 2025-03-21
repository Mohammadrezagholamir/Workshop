package models.enums;
/*
Explanation:
- we have commands in our profile menu and this commands need regexes to be checked.
- put those regexes here and use them in your code.
- this regexes need some functions, put those functions in here.
 */

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public enum ProfileMenuCommands implements Command {
    SHOWPROFILE("^\\s*show\\s+user\\s+info\\s*$"),
    CHANGECURRENCY("^\\s*change-currency\\s+-n\\s+(\\S+)\\s*$"),
    CHANGENAME("^\\s*change-username\\s+-n\\s+(\\S+)\\s*$"),
    CHANGEPASSWORD("^\\s*change-password\\s+-o\\s+(\\S+)\\s+-n\\s+(\\S+)\\s*$"),
    BACK("^\\s*back\\s*$"),
    EXIT("^\\s*exit\\s*$"),
    LOGOUT("logout");
    
    private final String pattern;
    ProfileMenuCommands(String pattern){
        this.pattern = pattern;
    }
    public Matcher getMatcher(String regex){
        Matcher matcher = Pattern.compile(this.pattern).matcher(regex);
        if(matcher.matches()){
            return matcher;
        }
        return null;
    }
}
