package controllers;
import java.util.ArrayList;

import models.*;
import models.enums.Currency;
/*
Explanation:
- This is a controller class for the profile menu Controller.
- This class will be used to implement functions that do profile menu operations.
- notice that this class should not have any input and output and just use it to implement functionalities.
 */
import models.enums.Menu;

public class ProfileMenuController {
    private final SignUpMenuController controller = new SignUpMenuController();
    public boolean checkcurrencyFormat(String format){
        if(format.equals("GTC") || format.equals("QTR") ||format.equals("SUD")){
            return true;
        }
        return false;
    }
    public void outPutCurrency(String c){

        if(c.equals("GTC")){
            App.getLoggedIn().setCurrency(Currency.GTC);
        }
        else if(c.equals("QTR")){
            App.getLoggedIn().setCurrency(Currency.QTR);
        }
        else if(c.equals("SUD")){
            App.getLoggedIn().setCurrency(Currency.SUD);
        }
        
    }
    public boolean isNewName(String name){
        for(User u : App.users){
            if(u.getName().equals(name)){
                return false;
            }
        }
        return true;
    }
    public String userInfo(User user){
        String username = user.getUsername();
        String password = user.getPassword();
        String email = user.getEmail();
        String currency = user.getCurrency().name();
        String name = user.getName();
        return "username : " + username + "\n"+
                "password : " + password +  "\n" +
                "currency : " + currency + "\n" +
                "email : " + email + "\n" +
                "name : " + name;

    }
    public Result showUserInfo(User user){
        return new Result(true, userInfo(user));
    }
    public Result changeCurrency(String newCurrency){
        if(!checkcurrencyFormat(newCurrency)){
            return new Result(false, "currency format is invalid!");
        }
        outPutCurrency(newCurrency);
        return new Result(true, "your currency changed to" + " " + newCurrency + " " + "successfully!");
        
    }
    public Result changeName(String name){
        if(App.getLoggedIn().getName().equals(name)){
            return new Result(false, "please enter a new username!");
        }
        if(!isNewName(name)){
            return new Result(false, "this username is already taken!");
        }
        //check name format!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        App.getLoggedIn().setName(name);
        return new Result(false , "your username changed to " + name +" successfully!");
    }
    public Result changePassword(String oldp , String newp){
        if(App.getLoggedIn().getPassword() != oldp){
            return new Result(false, "password incorrect!");
        }
        if(oldp.equals(newp)){
            return new Result(false, "please enter a new password!");
        }
        if(!controller.isValidatePassword(newp)){
            return new Result(false, "new password format is invalid!");
        }
        return new Result(true, "your password changed successfully!");
    }
    public Result logout(){
        App.setMenu(Menu.LoginMenu);
        App.setLoggedIn(null);
        return new Result(true, "user logged out successfully.you are now in login menu!");
    }
}
