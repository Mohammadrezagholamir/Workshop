package models.enums;

import java.util.regex.*;

/*
Explanation:
- we have commands in our sign-up menu and this commands need regexes to be checked.
- put those regexes here and use them in your code.
- this regexes need some functions, put those functions in here.
 */
public enum SignUpMenuCommands implements Command{
    SIGNUP("^\\s*register\\s+-u\\s+(?<username>\\S+)\\s+-p\\s+(?<password>\\S+)\\s+-e\\s+(?<user>\\S+)@(?<domain>\\S+)\\.(?<TDL>\\S+)\\s+-n\\s+(?<name>\\S+)\\s*$"),
    GOTOLOGIN("\\s*go\\s+to\\s+login\\s+menu\\s*"),
    LOGIN("\\s*login\\s+-u\\s+(?<username>\\S+)\\s+-p\\s+(?<password>\\S+)\\s*"),
    FORGOTPASSWORD("\\s*forget-password\\s+-u\\s+(?<username>\\S+)\\s+-e\\s+(?<email>\\S+)\\s*"),
    GOTOSIGNUP("\\s*go\\s+to\\s+signup\\s+menu\\s*"),
    EXIT("^\\s*exit\\s*$");
    private final String pattern;
    SignUpMenuCommands(String pattern){
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
