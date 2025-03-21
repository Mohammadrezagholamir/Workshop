package controllers;
import models.enums.*;

import java.util.ArrayList;
import java.util.regex.*;
import models.*;
/*
Explanation:
- This is a controller class for the dashboard Controller.
- This class will be used to implement functions that do dashboard operations.
- notice that this class should not have any input and output and just use it to implement functionalities.
 */
public class DashboardController {
    private final LoginMenuController controller = new LoginMenuController();
    public int isCorrectTotal(ArrayList<Integer> values , int total){
        int result = 0;
        for(Integer i : values){
            result += i;
        }
        return result;
    }
    public boolean isCorrectFormatExpense(String totalExpense){
        String regex = "^\\d+$";
        Pattern pattern = Pattern.compile(regex);
        Matcher matcher = pattern.matcher(totalExpense);
        if(matcher.matches()){
            return true;
        }
        return false;
    }
    public ArrayList<String> isRealUsersForGroup(ArrayList<String> usernames , int groupId){
        ArrayList<String> errorsForExitingUsersInGroup = new ArrayList<>();
        Group testGroup = null;
        for(Group g: App.groups){
            if(groupId == g.getGroupId()){
                testGroup = g;
            }
        }
        ArrayList<User> users = new ArrayList<>();
        for(String s: usernames){
            users.add(controller.findUser(s, App.users));
        }
        for(String s : usernames){
            User u = controller.findUser(s , App.users);
            if(u == null){
                errorsForExitingUsersInGroup.add(s + "not in group!"); 
            }
            else{
                if(!testGroup.existsInMe(u)){
                    errorsForExitingUsersInGroup.add(u.getUsername() + "not in group!"); 
                }
        }
        }
        return errorsForExitingUsersInGroup;
    }
    public boolean findEmail(String email, ArrayList<User> users, String username) {
        for (User u : users) {
            
            if (u.getUsername().equals(username) && u.getEmail().equals(email)) {
                
                return true;
            }
        }
        
        return false;
    }
    
    public boolean isValidId(int id , ArrayList<Group> groups){
        for(Group g : groups){
            if(id == g.getGroupId()){
                return true;
            }
        }
        return false;
    }
    public boolean isValideNameGroup(String name){
        if (name.length() < 4 || name.length() > 30) {
            return false;
        }

        
        for (char ch : name.toCharArray()) {
            if (!(Character.isLetterOrDigit(ch) || " !@#$%^&*".indexOf(ch) != -1)) {
                return false;
            }
        }

        return true;
    }
    public boolean isValidtypeGroup(String type){
        if(models.enums.GroupType.HOME.getMatcher(type)!=null){
            return true;
        }
        if(models.enums.GroupType.ZAN_O_BACHE.getMatcher(type)!=null){
            return true;
        }
        if(models.enums.GroupType.OTHER.getMatcher(type)!=null){
            return true;
        }
        if(models.enums.GroupType.Trip.getMatcher(type)!=null){
            return true;
        }
        return false;
    }
    public Result createGroup(String nameOfGroup , String typeOfGroup ){
        if(!isValideNameGroup(nameOfGroup)){
            return new Result(false, "group name format is invalid!");
        }
        if(!isValidtypeGroup(typeOfGroup)){
            return new Result(false, "group type format is invalid!");

        }
        Group g = new Group(nameOfGroup, typeOfGroup, App.getLoggedIn());
        
        App.groups.add(g);
        // App.getLoggedIn().addToGroup(g);
        
        return new Result(true, "group created successfully!");
    }
    public void ShowGroups(User loggedInUser){
        loggedInUser.showMyGroups();
    }
    public Result AddToGroup(String username , String email , int groupId){
        Group testGroup = null;
        
        for(Group group : App.groups){
            
            if(group.getGroupId() == groupId){
                testGroup = group;
            }
        }
        if(controller.findUser(username , App.users) == null){
            return new Result(false, "user not found!");
            
        }
        if(!isValidId(groupId, App.groups)){
            return new Result(false, "group not found!");
            
        }
        if( testGroup != null && testGroup.existsInMe(controller.findUser(username, App.users))){
            return new Result(false, "user already in the group!");
           

        }
        if(!findEmail(email, App.users , username)){
            return new Result(false, "the email provided does not match the username!" + " " + email);
            
        }

        if(App.getLoggedIn() != testGroup.getCreator()){
            return new Result(false, "only the group creator can add users!");
            
        }

        controller.findUser(username, App.users).addToGroup(testGroup);
        for(Group g : App.groups){
            if(groupId == g.getGroupId()){
                g.addUsers(controller.findUser(username, App.users));
            }
        }
        return new Result(true , "user added to the group successfully!");

    }
    public Result addExpenseEqually(int groupId , int totalExpense , int number , ArrayList<String> usernames  , String stringOfTotalExpense){
        if(!isValidId(groupId, App.groups)){
            return new Result(false, "group not found!");
        }
        ArrayList<String> temp = isRealUsersForGroup(usernames, groupId);
        if(!temp.isEmpty()){
            return new Result(false, String.join("\n" , temp));
        }
        if(!isCorrectFormatExpense(stringOfTotalExpense)){
            return new Result(false, "expense format is invalid!");
        }

        for(String s : usernames){
            Expense expence = new Expense(totalExpense / number, App.getLoggedIn() ,controller.findUser(s, App.users) ,  groupId) ;
            int sum = totalExpense / number;
            //System.out.println(sum);
            //controller.findUser(s,App.users).addExpense(expence , App.getLoggedIn().getCurrency() ,number);
            App.getLoggedIn().addExpense(expence, App.getLoggedIn().getCurrency() , number);
        }
        return new Result(false, "expense added successfully!");
    }
    public Result addExpenseUnequally(int groupId , int totalExpense , ArrayList<String> usernames , ArrayList<Integer>valusOfUsers , String stringOfTotalExpense){
        if(!isValidId(groupId, App.groups)){
            return new Result(false, "group not found!");
        }
        ArrayList<String> temp = isRealUsersForGroup(usernames, groupId);
        if(!temp.isEmpty()){
            return new Result(false, String.join("\n" , temp));
        }
        if(totalExpense != isCorrectTotal(valusOfUsers, totalExpense)){
            return new Result(false, "the sum of individual costs does not equal the total cost!");
        }
        for(int i=0 ; i<usernames.size() ; i++){
           Expense expence = new Expense(valusOfUsers.get(i), App.getLoggedIn(), controller.findUser(usernames.get(i), App.users) , groupId);
           controller.findUser(usernames.get(i), App.users).addExpense(expence , App.getLoggedIn().getCurrency() , usernames.size() ); 
        }
        return new Result(true , "expense added successfully!");
    }
    public Result settleUp(String username  , int inputMoney , String stringOfMoney){
        if(controller.findUser(username, App.users) == null){
            return new Result(false,"user not found!");
        }
        if(!isCorrectFormatExpense(stringOfMoney)){
            return new Result(false, "input money format is invalid!");
        }
        return new Result(false,App.getLoggedIn().settleUpUser(controller.findUser(username, App.users) , inputMoney)) ;
    }   
    public Result showBalance(String username){
        if(controller.findUser(username, App.users) == null){
            return new Result(false, "user not found!");
        }
        return new Result(true , App.getLoggedIn().showBalance(controller.findUser(username, App.users)));
    }
    public Result goToProfile(){
        App.setMenu(Menu.ProfileMenu);
        return new Result(true, "you are now in profile menu!");
    }
    public Result logout(){
        App.setMenu(Menu.LoginMenu);
        App.setLoggedIn(null);
        return new Result(true, "user logged out successfully.you are now in login menu!");
    }
}