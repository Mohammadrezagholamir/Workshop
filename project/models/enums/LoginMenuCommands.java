package models.enums;
import java.util.regex.*;
/*
Explanation:
- we have commands in our login menu and this commands need regexes to be checked.
- put those regexes here and use them in your code.
- this regexes need some functions, put those functions in here.
 */
public enum LoginMenuCommands implements Command{
    LOGIN("\\s*login\\s+-u\\s+(?<username>\\S+)\\s+-p\\s+(?<password>\\S+)\\s*"),
    GOTOSIGNUP("\\s*go\\s+to\\s+signup\\s+menu\\s*"),
    FORGOTPASSWORD("\\s*forget-password\\s+-u\\s+(?<username>\\S+)\\s+-e\\s+(?<email>\\S+)\\s*"),
    EXIT("^\\s*exit\\s*$"),
    GOTOLOGIN("\\s*go\\s+to\\s+login\\s+menu\\s*");

    private final String pattern;
    LoginMenuCommands(String pattern){
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