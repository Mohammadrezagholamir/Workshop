package models.enums;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/*
Explanation:
- we have commands in our dashboard and this commands need regexes to be checked.
- put those regexes here and use them in your code.
- this regexes need some functions, put those functions in here.
 */
public enum DashboardCommands implements Command {
    DASHBORD("\\s*create-group\\s+-n\\s+(?<name>.+)\\s+-t\\s+(?<type>\\S+)\\s*"),
    SHOWMYGROUPS("\\s*show\\s+my\\s+groups\\s*"),
    ADDTOGROUP("\\s*add-user\\s+-u\\s+(?<username>\\S+)\\s+-e\\s+(?<email>\\S+)\\s+-g\\s+(?<groupid>\\S+)\\s*"),
    ADDEXPENCE("\\s*add-expense\\s+-g\\s+(?<groupId>\\S+)\\s+-s\\s+(?<typeofeq>unequally|equally)\\s+-t\\s+(?<totaleXpence>\\S+)\\s+-n\\s+(?<numberofusers>\\S+)\\s*"),
    UNEQUALLY("\\s*(?<username>\\S+)\\s+(?<expense>\\S+)\\s*"),
    SHOWBALANCE("\\s*show\\s+balance\\s+-u\\s+(?<username>\\S+)\\s*"),
    SETTLEUP("\\s*settle-up\\s+-u\\s+(?<username>\\S+)\\s+-m\\s+(?<inputmoney>\\S+)\\s*"),
    GOTOPROFILE("\\s*go\\s+to\\s+profile\\s+menu\\s*"),
    EXIT("\\s*exit\\s*"),
    LOGOUT("\\s*logout\\s*");
    private final String pattern;
    DashboardCommands(String pattern){
        this.pattern = pattern;
    }
    @Override
    public Matcher getMatcher(String regex){
        Matcher matcher = Pattern.compile(this.pattern).matcher(regex);
        if(matcher.matches()){
            return matcher;
        }
        return null;
    }
}
