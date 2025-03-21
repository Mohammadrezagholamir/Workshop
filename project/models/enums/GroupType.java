package models.enums;
import java.util.regex.*;;


/*
Explanation:
- In our app, groups have some types that are constants.
- In these cases, we use enums to define them and use them in our code.
- put those types here and use them in your code.
 */
public enum GroupType {
    ZAN_O_BACHE("Zan-o-Bache"),
    HOME("Home"),
    Trip("Trip"),
    OTHER("Other");

    private final String pattern;
    GroupType(String pattern){
        this.pattern = pattern;
    }
    public Matcher getMatcher(String input){
        Matcher matcher = Pattern.compile(this.pattern).matcher(input);
        if(matcher.matches()){
            return matcher;
        }
        return null;
    }

}
