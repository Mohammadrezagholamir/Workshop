package views;
import java.util.regex.*;
import controllers.*;
import models.App;
import models.enums.Menu;

import java.util.Scanner;
/*
Explanation:
- This is a view class for the SignUpMenu.
- This class should use to check inputs and print outputs for the SignUpMenu.
- notice that : this class should not have any logic and just use it to get inputs and handle it to use correct methods in controller.
 */

public class SignUpMenu implements AppMenu {
    final LoginMenuController co = new LoginMenuController();
    final SignUpMenuController controller = new SignUpMenuController();
    final LoginMenuController controler = new LoginMenuController();
    public void check(Scanner scanner){
        String input = scanner.nextLine();
        input = input.trim();
        Matcher matcher;
        if(models.enums.SignUpMenuCommands.SIGNUP.getMatcher(input)!= null){
            matcher = models.enums.SignUpMenuCommands.SIGNUP.getMatcher(input);
            String Username = matcher.group(1);
            String Password = matcher.group(2);
            String User = matcher.group(3);
            String domain = matcher.group(4);
            String TDL = matcher.group(5);
            String nam = matcher.group(6);
            System.out.println(controller.SignUp(Username , Password , User , domain , TDL , nam));

        }


        
        else if(models.enums.SignUpMenuCommands.GOTOLOGIN.getMatcher(input)!=null){
            matcher = models.enums.SignUpMenuCommands.GOTOLOGIN.getMatcher(input);
            System.out.println(controller.goToLogin());
        }
        else if(models.enums.SignUpMenuCommands.GOTOSIGNUP.getMatcher(input)!= null){
            System.out.println(controller.goToSignUp());
        }
        else if(models.enums.SignUpMenuCommands.LOGIN.getMatcher(input)!=null){
            
            
            input = input.replaceAll("\\s+", " ").trim();
            matcher = models.enums.LoginMenuCommands.LOGIN.getMatcher(input);
            String UsernameForLogin = matcher.group(1);
            String passwordForLogin = matcher.group(2);
            System.out.println(controler.login(UsernameForLogin, passwordForLogin , App.users));
        }
        else if(models.enums.SignUpMenuCommands.FORGOTPASSWORD.getMatcher(input)!=null){
            input = input.replaceAll("\\s+", " ").trim();
            matcher = models.enums.SignUpMenuCommands.FORGOTPASSWORD.getMatcher(input);
            
            String username = matcher.group(1);
            String email = matcher.group(2);
            System.out.println(co.ForotPassword( email, username));

        }
        else if(models.enums.SignUpMenuCommands.EXIT.getMatcher(input)!=null){
            App.setMenu(Menu.ExitMenu);
        }
        else{
            System.out.println("invalid command!");
    }
}
}
