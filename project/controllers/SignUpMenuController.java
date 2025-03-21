package controllers;
/*
Explanation:
- This is a controller class for the sign-up menu Controller.
- This class will be used to implement functions that do sign up menu operations.
- notice that this class should not have any input and output and just use it to implement functionalities.
 */
import models.*;
import models.enums.Currency;
import models.enums.Menu;

import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.List;

public class SignUpMenuController {
    public boolean isValidateUsername(String username){
        if (username == null || username.length() < 4 || username.length() > 10) {
            return false;
        }
    
        
        char firstChar = username.charAt(0);
        if (!Character.isLetter(firstChar)) {
            return false;
        }
    
        
        for (int i = 1; i < username.length(); i++) {
            char c = username.charAt(i);
            if (!(Character.isLetterOrDigit(c) || c == '-' || c == '_' || c == '.')) {
                return false;
            }
        }
    
        return true;
    }
    public boolean isValidatePassword(String password){
        if (password.length() < 6 || password.length() > 12) {
            return false;
        }

        boolean hasLower = false, hasUpper = false, hasDigit = false, hasSpecial = false;
        String specialChars = "!@#$%^&*";

        for (char ch : password.toCharArray()) {
            if (Character.isLowerCase(ch)) hasLower = true;
            else if (Character.isUpperCase(ch)) hasUpper = true;
            else if (Character.isDigit(ch)) hasDigit = true;
            else if (specialChars.indexOf(ch) != -1) hasSpecial = true;
        }

        return hasLower && hasUpper && hasDigit && hasSpecial;
    }
    public static boolean isValidDomain(String domain) {
        if (domain.length() < 3 || domain.length() > 7) {
            return false;
        }
    
        if (!Character.isLowerCase(domain.charAt(0)) || !Character.isLowerCase(domain.charAt(domain.length() - 1))) {
            return false;
        }
    
        boolean hasSpecial = false;
        for (int i = 1; i < domain.length() - 1; i++) {
            char ch = domain.charAt(i);
            if (!(Character.isLowerCase(ch) || ch == '-' || ch == '.')) {
                return false;
            }
            if (ch == '-' || ch == '.') {
                if (hasSpecial) return false; 
                hasSpecial = true;
            }
        }
        return true;
    }
    public static boolean isValidName(String name) {
        if (name.isEmpty()) {
            return false;
        }
    
        if (!Character.isLetter(name.charAt(0)) || !Character.isLetter(name.charAt(name.length() - 1))) {
            return false; 
        }
    
        for (char ch : name.toCharArray()) {
            if (!(Character.isLetter(ch) || ch == '-')) {
                return false; 
            }
        }
        return true;
    }
    public  boolean isValidEmail( String user,String domain , String TDL) {
        

    
        
        if (!isValidateUsername(user)){ 
            
            return false;
        }
        


    
        
        if (!isValidDomain(domain)) {
            
            return false;
    
        }
        String[] special = {"com","edu","org","net"};
        int check =0;
        for(int  i=0 ; i<4 ; i++){
            if(TDL.equals(special[i])){
                check++;
                
            }
        }
        if(check == 0){
            return false;
        }
        return true;
    }
    
    



    public Result SignUp(String Username, String Password, String user, String domain, String TDL, String name) {
  

        if (!isValidateUsername(Username)) {
            return new Result(false, "username format is invalid!");
            
        }
        if (!isValidatePassword(Password)) {
            return new Result(false, "password format is invalid!");
            
        }
        if (!isValidEmail(user, domain, TDL)) {
            return new Result(false,"email format is invalid!");
            
        }
        if (!isValidName(name)) {
            return new Result(false, "format name is invalid!");
        }
        
        for(User u : App.users){
            if(Username.equals(u.getUsername())){
                return new Result(false , "this username is already taken!");
            }
        }



        App.users.add(new User(Username, Password, user +"@"+ domain +"."+ TDL, name , Currency.GTC));
        return new Result(true, "user registered successfully. You are now in the login menu!");
    }
    public Result goToLogin(){
        App.setMenu(Menu.LoginMenu);
        return new Result(true , "you are now in login menu!");
    }
    public Result goToSignUp(){
        return new Result(true, "you are now in signup menu!");
    }
    
}

